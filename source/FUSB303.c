#include <FUSB303.h>
#include <ch.h>
#include <hal.h>

#include <pd.h>


/*
 * Read a single byte from the FUSB302B
 *
 * cfg: The FUSB302B to communicate with
 * addr: The memory address from which to read
 *
 * Returns the value read from addr.
 */
static uint8_t fusb_read_byte(struct pdb_fusb_config *cfg, uint8_t addr)
{
    uint8_t buf;
    i2cMasterTransmit(cfg->i2cp, cfg->addr, &addr, 1, &buf, 1);
    return buf;
}

/*
 * Read multiple bytes from the FUSB302B
 *
 * cfg: The FUSB302B to communicate with
 * addr: The memory address from which to read
 * size: The number of bytes to read
 * buf: The buffer into which data will be read
 */
static void fusb_read_buf(struct pdb_fusb_config *cfg, uint8_t addr,
        uint8_t size, uint8_t *buf)
{
    i2cMasterTransmit(cfg->i2cp, cfg->addr, &addr, 1, buf, size);
}

/*
 * Write a single byte to the FUSB302B
 *
 * cfg: The FUSB302B to communicate with
 * addr: The memory address to which we will write
 * byte: The value to write
 */
static void fusb_write_byte(struct pdb_fusb_config *cfg, uint8_t addr,
        uint8_t byte)
{
    uint8_t buf[2] = {addr, byte};
    i2cMasterTransmit(cfg->i2cp, cfg->addr, buf, 2, NULL, 0);
}

/*
 * Write multiple bytes to the FUSB302B
 *
 * cfg: The FUSB302B to communicate with
 * addr: The memory address to which we will write
 * size: The number of bytes to write
 * buf: The buffer to write
 */
static void fusb_write_buf(struct pdb_fusb_config *cfg, uint8_t addr,
        uint8_t size, const uint8_t *buf)
{
    uint8_t txbuf[size + 1];

    /* Prepare the transmit buffer */
    txbuf[0] = addr;
    for (int i = 0; i < size; i++) {
        txbuf[i + 1] = buf[i];
    }

    i2cMasterTransmit(cfg->i2cp, cfg->addr, txbuf, size + 1, NULL, 0);
}

void fusb_send_message(struct pdb_fusb_config *cfg, const union pd_msg *msg)
{
    /* Token sequences for the FUSB302B */
    static uint8_t sop_seq[5] = {
        FUSB_FIFO_TX_SOP1,
        FUSB_FIFO_TX_SOP1,
        FUSB_FIFO_TX_SOP1,
        FUSB_FIFO_TX_SOP2,
        FUSB_FIFO_TX_PACKSYM
    };
    static uint8_t eop_seq[4] = {
        FUSB_FIFO_TX_JAM_CRC,
        FUSB_FIFO_TX_EOP,
        FUSB_FIFO_TX_TXOFF,
        FUSB_FIFO_TX_TXON
    };

    /* Take the I2C2 mutex now so there can't be a race condition on sop_seq */
    i2cAcquireBus(cfg->i2cp);

    /* Get the length of the message: a two-octet header plus NUMOBJ four-octet
     * data objects */
    uint8_t msg_len = 2 + 4 * PD_NUMOBJ_GET(msg);

    /* Set the number of bytes to be transmitted in the packet */
    sop_seq[4] = FUSB_FIFO_TX_PACKSYM | msg_len;

    /* Write all three parts of the message to the TX FIFO */
    fusb_write_buf(cfg, FUSB_FIFOS, 5, sop_seq);
    fusb_write_buf(cfg, FUSB_FIFOS, msg_len, msg->bytes);
    fusb_write_buf(cfg, FUSB_FIFOS, 4, eop_seq);

    i2cReleaseBus(cfg->i2cp);
}

uint8_t fusb_read_message(struct pdb_fusb_config *cfg, union pd_msg *msg)
{
    uint8_t garbage[4];
    uint8_t numobj;

    i2cAcquireBus(cfg->i2cp);

    /* If this isn't an SOP message, return error.
     * Because of our configuration, we should be able to assume this means the
     * buffer is empty, and not try to read past a non-SOP message. */
    if ((fusb_read_byte(cfg, FUSB_FIFOS) & FUSB_FIFO_RX_TOKEN_BITS)
            != FUSB_FIFO_RX_SOP) {
        i2cReleaseBus(cfg->i2cp);
        return 1;
    }
    /* Read the message header into msg */
    fusb_read_buf(cfg, FUSB_FIFOS, 2, msg->bytes);
    /* Get the number of data objects */
    numobj = PD_NUMOBJ_GET(msg);
    /* If there is at least one data object, read the data objects */
    if (numobj > 0) {
        fusb_read_buf(cfg, FUSB_FIFOS, numobj * 4, msg->bytes + 2);
    }
    /* Throw the CRC32 in the garbage, since the PHY already checked it. */
    fusb_read_buf(cfg, FUSB_FIFOS, 4, garbage);

    i2cReleaseBus(cfg->i2cp);
    return 0;
}

void fusb_send_hardrst(struct pdb_fusb_config *cfg)
{
    i2cAcquireBus(cfg->i2cp);

    /* Send a hard reset */
    fusb_write_byte(cfg, FUSB_CONTROL3, 0x07 | FUSB_CONTROL3_SEND_HARD_RESET);

    i2cReleaseBus(cfg->i2cp);
}

void fusb_setup(struct pdb_fusb_config *cfg)
{
    i2cAcquireBus(cfg->i2cp);

    /* Fully reset the FUSB302B */
    fusb_write_byte(cfg, FUSB_RESET, FUSB_RESET_SW_RES);

    /* Turn on all power */
    fusb_write_byte(cfg, FUSB_POWER, 0x0F);

    /* Set interrupt masks */
    fusb_write_byte(cfg, FUSB_MASK1, 0x00);
    fusb_write_byte(cfg, FUSB_MASKA, 0x00);
    fusb_write_byte(cfg, FUSB_MASKB, 0x00);
    fusb_write_byte(cfg, FUSB_CONTROL0, 0x04);

    /* Enable automatic retransmission */
    fusb_write_byte(cfg, FUSB_CONTROL3, 0x07);

    /* Flush the RX buffer */
    fusb_write_byte(cfg, FUSB_CONTROL1, FUSB_CONTROL1_RX_FLUSH);

    /* Measure CC1 */
    fusb_write_byte(cfg, FUSB_SWITCHES0, 0x07);
    chThdSleepMicroseconds(250);
    uint8_t cc1 = fusb_read_byte(cfg, FUSB_STATUS0) & FUSB_STATUS0_BC_LVL;

    /* Measure CC2 */
    fusb_write_byte(cfg, FUSB_SWITCHES0, 0x0B);
    chThdSleepMicroseconds(250);
    uint8_t cc2 = fusb_read_byte(cfg, FUSB_STATUS0) & FUSB_STATUS0_BC_LVL;

    /* Select the correct CC line for BMC signaling; also enable AUTO_CRC */
    if (cc1 > cc2) {
        fusb_write_byte(cfg, FUSB_SWITCHES1, 0x25);
        fusb_write_byte(cfg, FUSB_SWITCHES0, 0x07);
    } else {
        fusb_write_byte(cfg, FUSB_SWITCHES1, 0x26);
        fusb_write_byte(cfg, FUSB_SWITCHES0, 0x0B);
    }

    /* Reset the PD logic */
    fusb_write_byte(cfg, FUSB_RESET, FUSB_RESET_PD_RESET);

    i2cReleaseBus(cfg->i2cp);
}

void fusb_get_status(struct pdb_fusb_config *cfg, union fusb_status *status)
{
    i2cAcquireBus(cfg->i2cp);

    /* Read the interrupt and status flags into status */
    fusb_read_buf(cfg, FUSB_STATUS0A, 7, status->bytes);

    i2cReleaseBus(cfg->i2cp);
}

enum fusb_typec_current fusb_get_typec_current(struct pdb_fusb_config *cfg)
{
    i2cAcquireBus(cfg->i2cp);

    /* Read the BC_LVL into a variable */
    enum fusb_typec_current bc_lvl = fusb_read_byte(cfg, FUSB_STATUS0)
        & FUSB_STATUS0_BC_LVL;

    i2cReleaseBus(cfg->i2cp);

    return bc_lvl;
}

void fusb_reset(struct pdb_fusb_config *cfg)
{
    i2cAcquireBus(cfg->i2cp);

    /* Flush the TX buffer */
    fusb_write_byte(cfg, FUSB_CONTROL0, 0x44);
    /* Flush the RX buffer */
    fusb_write_byte(cfg, FUSB_CONTROL1, FUSB_CONTROL1_RX_FLUSH);
    /* Reset the PD logic */
    fusb_write_byte(cfg, FUSB_RESET, FUSB_RESET_PD_RESET);

    i2cReleaseBus(cfg->i2cp);
}
