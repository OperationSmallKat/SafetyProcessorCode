/*
 * BQ76925.c
 *
 *  Created on: Oct 18, 2020
 *      Author: kbisland
 */



// Select SENSEN measurement (I_AMP_CAL = 0) and gain of 4 (I_GAIN = 0)
    if (i2c_write_and_verify(CONFIG_1, 0x00, &i2c_write_struct)) error_trap();
    // Set VREF_NOM = 3.0 V and GVCOUT = 0.6 (REF_SEL = 1)
    if (i2c_write_and_verify(CONFIG_2, 0x01, &i2c_write_struct)) error_trap();
    // Enable VREF, VCOUT and VIOUT.
    if (i2c_write_and_verify(POWER_CTL, 0x0D, &i2c_write_struct)) error_trap();
