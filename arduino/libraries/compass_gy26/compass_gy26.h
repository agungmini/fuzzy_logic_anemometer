#ifndef COMPASS_GY26_H
#define COMPASS_GY26_H

#include "i2c.h"
#include "Arduino.h"
#include "stdint.h"
#include "inttypes.h"
#include "stdlib.h"
#include "stdio.h"

#ifdef __cplusplus
extern "C" {
#endif

//device address
#define GY26_WR 0xE0
#define GY26_RD 0xE1

//command
#define PRELIMINARY 0x00
#define MEASURE_ANGLE 0x31
#define START_CALIBRATION 0xC0
#define END_CALIBRATION 0xC1
#define RETURN_FACTORY0 0XA0
#define RETURN_FACTORY1 0XAA
#define RETURN_FACTORY2 0XA5
#define RETURN_FACTORY3 0XC5
#define CHANGE_ADDRESS0 0XC1
#define CHANGE_ADDRESS1 0XA0
#define CHANGE_ADDRESS2 0XAA
#define DECL_HIGH 0X03
#define DECL_LOW 0X04

void reset_gy26(void);
long int get_angle_gy26();

#ifdef __cplusplus
} //extern "C"
#endif

#endif //COMPASS_GY26_H
