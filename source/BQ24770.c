/*
 * BQ24770.c
 *
 *  Created on: Oct 18, 2020
 *      Author: kbisland
 */

void i2c_send(uint8_t reg, uint8_t* packet, uint8_t len){
	I2C_MasterStart(I2C3_PERIPHERA,0x12, kI2C_Write);
	I2C_MasterWriteBlocking(I2C3_PERIPHERAL, reg, 1, kI2C_TransferDefaultFlag);
	I2C_MasterWriteBlocking(I2C3_PERIPHERAL, packet, len, kI2C_TransferDefaultFlag);
	I2C_MasterStop(I2C3_PERIPHERAL);
}

void set_chg_current(){

	uint16_t current = 1<<9;
	i2c_send( 0x14,(uint8_t*)&current, 2);
}

void set_chg_voltage(){

	uint16_t voltage = 1<<14|1<<8|1<<7;

	i2c_send( 0x15,(uint8_t*)&voltage, 2);
}

void disable(){
	uint16_t current = 1<<11|1<<10;

		i2c_send( 0x3F,(uint8_t*)&current, 2);
}

void disable_chg_current(){

	uint16_t current = 0;
	i2c_send( 0x14,(uint8_t*)&current, 2);
}

void disable_chg_voltage(){

	uint16_t voltage = 0;

	i2c_send( 0x15,(uint8_t*)&voltage, 2);
}

void set_input_current(){
	uint16_t current = 0;

		i2c_send( 0x3F,(uint8_t*)&current, 2);
}

void start_charger(){
	set_input_current();
	set_chg_current();
	set_chg_voltage();

}

void stop_charger(){
	disable_input_current();
	disable_chg_current();
	disable_chg_voltage();

}
