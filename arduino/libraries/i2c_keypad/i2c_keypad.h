#ifndef I2C_KEYPAD_H
#define I2C_KEYPAD_H

#include "i2c.h"
#include "Arduino.h"
#include "stdint.h"
#include "inttypes.h"

#ifdef __cplusplus
extern "C" {
#endif

//device address
#define KEYPADWR 0x7C //io expander write
#define KEYPADRD 0x7D //io expander read

int keypad();

#ifdef __cplusplus
} // extern "C"
#endif

#endif //I2C_KEYPAD_H
