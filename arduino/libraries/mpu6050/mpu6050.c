#include "mpu6050.h"

void mpu6050_init(){
	i2c_address(MPU6050WR);
	i2c_send(0x6B);
	i2c_send(0x00);
	i2c_stop();
}

uint8_t get_register_val(uint8_t address_word){
	uint8_t temp;	
	i2c_address(MPU6050WR);
	i2c_send(address_word);
	i2c_address(MPU6050RD);
	temp= i2c_getval(NACK);
	i2c_stop();
return temp;
}
