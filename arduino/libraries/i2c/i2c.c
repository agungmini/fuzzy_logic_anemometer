#include "i2c.h"

void i2c_init(int clkspeed){
	int temp;
	temp= (CPUCLK/clkspeed)-16;
	temp= temp/2;
	TWBR=  temp;
}

void i2c_address(uint8_t device_address){
  	TWCR = (1<<TWSTA)|(1<<TWEN)|(1<<TWINT);
    	while(!(TWCR & (1<<TWINT)));
    	TWDR= device_address;
    	TWCR = (1<< TWINT)|(1<<TWEN);
    	while(!(TWCR & (1<<TWINT)));
}

void i2c_send(uint8_t data){
  	TWDR = data;
  	TWCR = (1<< TWINT)|(1<<TWEN);
  	while(!(TWCR & (1<<TWINT)));
}
int i2c_getval(uint8_t ack){
  	TWCR = (1<< TWINT)|(1<<TWEN)|(ack<<TWEA);
  	while(!(TWCR & (1<<TWINT)));
  	
return TWDR;
}

void i2c_stop(){
  	TWCR =(1<<TWEN)|(1<<TWINT)|(1<<TWSTO);
}
