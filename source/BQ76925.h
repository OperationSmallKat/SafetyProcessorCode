/*
 * BQ76925.h
 *
 *  Created on: Oct 18, 2020
 *      Author: kbisland
 */

#ifndef BQ76925_H_
#define BQ76925_H_

#define I2C_GROUP_ADDR  0x4
#define STATUS          (I2C_GROUP_ADDR << 3) + 0x00    // Read & Write
#define CELL_CTL        (I2C_GROUP_ADDR << 3) + 0x01    // Read & Write
#define BAL_CTL         (I2C_GROUP_ADDR << 3) + 0x02    // Read & Write
#define CONFIG_1        (I2C_GROUP_ADDR << 3) + 0x03    // Read & Write
#define CONFIG_2        (I2C_GROUP_ADDR << 3) + 0x04    // Read & Write
#define POWER_CTL       (I2C_GROUP_ADDR << 3) + 0x05    // Read & Write
#define CHIP_ID         (I2C_GROUP_ADDR << 3) + 0x07    // Read Only
#define VREF_CAL        (I2C_GROUP_ADDR << 3) + 0x10    // Read Only
#define VC1_CAL         (I2C_GROUP_ADDR << 3) + 0x11    // Read Only
#define VC2_CAL         (I2C_GROUP_ADDR << 3) + 0x12    // Read Only
#define VC3_CAL         (I2C_GROUP_ADDR << 3) + 0x13    // Read Only
#define VC4_CAL         (I2C_GROUP_ADDR << 3) + 0x14    // Read Only
#define VC5_CAL         (I2C_GROUP_ADDR << 3) + 0x15    // Read Only
#define VC6_CAL         (I2C_GROUP_ADDR << 3) + 0x16    // Read Only
#define VC_CAL_EXT_1    (I2C_GROUP_ADDR << 3) + 0x17    // Read Only
#define VC_CAL_EXT_2    (I2C_GROUP_ADDR << 3) + 0x18    // Read Only
#define VREF_CAL_EXT    (I2C_GROUP_ADDR << 3) + 0x1B    // Read Only




#endif /* BQ76925_H_ */
