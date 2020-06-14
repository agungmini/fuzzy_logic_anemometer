#include "i2c_lcd.h"

void i2c_lcdEnable(uint8_t data){
	i2c_address(LCDWR);
	i2c_send(data|ENABLE);
	i2c_stop();
	delayMicroseconds(2);
	i2c_address(LCDWR);
	i2c_send(data & ~ENABLE);
	i2c_stop();
	delayMicroseconds(2);
}

void i2c_lcdWrite8bit(uint8_t data,uint8_t mode){
	i2c_address(LCDWR);
	i2c_send(data|mode|BACKLIGHT);
	i2c_stop();
	i2c_lcdEnable(data|mode|BACKLIGHT);
}

void i2c_lcdWrite4bit(uint8_t data,uint8_t mode){
	int hi_nible,lo_nible;
	hi_nible= data & 0xF0;
	lo_nible= (data & 0x0F)<< 4;
	
	i2c_lcdWrite8bit(hi_nible,mode);
	i2c_lcdWrite8bit(lo_nible,mode);
}

void i2c_lcdCommand(uint8_t command){
	i2c_lcdWrite4bit(command,0);
	delayMicroseconds(48);
}

void i2c_lcdClear(){
	i2c_lcdCommand(CLRDSP);
	delayMicroseconds(1600);
}

void i2c_lcdHome(){
	i2c_lcdCommand(HOME);
	delayMicroseconds(1600);
}

void i2c_lcdWriteChar(const char karakter){
	i2c_lcdWrite4bit(karakter,RS);
	delayMicroseconds(48);
}

void i2c_lcdWriteStr(const char *karakter){
	while(*karakter){
		i2c_lcdWriteChar(*karakter++);
	}
}

void i2c_lcdSetCursor(uint8_t row, uint8_t col){
	switch(row){
		case 0: i2c_lcdCommand(DDRAM|0x00|col);
		break;
		case 1: i2c_lcdCommand(DDRAM|0x40|col);
		break;
		case 2: i2c_lcdCommand(DDRAM|0x14|col);
		break;	
		case 3: i2c_lcdCommand(DDRAM|0x54|col);
		break;
	}	
}

void i2c_lcdInit(void){		
	delayMicroseconds(15005);
	i2c_lcdWrite8bit(0x30,0);
	delayMicroseconds(4105);
	i2c_lcdWrite8bit(0x30,0);
	delayMicroseconds(105);
	i2c_lcdWrite8bit(0x30,0);
	
	i2c_lcdWrite8bit(0x20,0);
	
	i2c_lcdCommand(MODE4BIT|MODE2LINE|MODE5DOTS);
	i2c_lcdCommand(DPYOFF);
	i2c_lcdClear();
	i2c_lcdCommand(INCRMNT);
	i2c_lcdCommand(DPYON);
}
