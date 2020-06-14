#ifndef I2C_H
#define I2C_H

#include "Arduino.h"
#include "stdint.h"
#include "inttypes.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CPUCLK 16000
#define ACK 1
#define NACK 0

void i2c_init(int clkspeed);
void i2c_address(uint8_t device_address);
void i2c_send(uint8_t data);
int i2c_getval(uint8_t ack);
void i2c_stop();

#ifdef __cplusplus
} // extern "C"
#endif

#endif //I2C_H
