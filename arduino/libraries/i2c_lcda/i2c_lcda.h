#ifndef I2C_LCD_H
#define I2C_LCD_H

#include "i2c.h"
#include "Arduino.h"
#include "stdint.h"
#include "inttypes.h"

#ifdef __cplusplus
extern "C" {
#endif

//backlight
#define BACKLIGHT 0x08

//device address
#define LCDWR 0x7E //io expander write
#define LCDRD 0x7F //io expander read

//untuk LCD
#define ENABLE 0x04
#define RS 0x01

//command LCD
#define FUNCTSET 0x20
#define MODE4BIT (FUNCTSET | 0)
#define MODE8BIT (FUNCTSET | 0x10)
#define MODE1LINE (FUNCTSET | 0)
#define MODE2LINE (FUNCTSET | 0x08)
#define MODE5DOTS (FUNCTSET | 0)
#define MODE11DOTS (FUNCTSET | 0x04)

#define ENTRYMODE 0x04
#define INCRMNT (ENTRYMODE | 0x02)
#define DECRMNT (ENTRYMODE | 0)
#define SHFTR (ENTRYMODE | 0x00) 
#define SHFTL (ENTRYMODE | 0x01)

#define DISPLAY 0x08
#define DPYON (DISPLAY | 0x04)
#define DPYOFF (DISPLAY | 0) 

#define CLRDSP 0x01
#define HOME 0x02
#define DDRAM 0x80

void i2c_lcdEnable(uint8_t data);
void i2c_lcdWrite8bit(uint8_t data,uint8_t mode);
void i2c_lcdWrite4bit(uint8_t data,uint8_t mode);
void i2c_lcdCommand(uint8_t command);
void i2c_lcdClear();
void i2c_lcdHome();
void i2c_lcdWriteChar(const char karakter);
void i2c_lcdWriteStr(const char *karakter);
void i2c_lcdSetCursor(uint8_t row, uint8_t col);
void i2c_lcdInit();

#ifdef __cplusplus
} // extern "C"
#endif

#endif //I2C_LCD_H
