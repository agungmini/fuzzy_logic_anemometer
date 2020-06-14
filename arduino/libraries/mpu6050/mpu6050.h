#ifndef MPU6050_H
#define MPU6050_H

#include "Arduino.h"
#include "i2c.h"
#include "stdint.h"
#include "inttypes.h"

#ifdef __cplusplus
extern "C" {
#endif

//device address
#define MPU6050WR (0x68<<1|0x0)
#define MPU6050RD (0x68<<1|0x1)

//accelero
#define AXH 0x3B
#define AXL 0X3C
#define AYH 0X3D
#define AYL 0X3E
#define AZH 0X3F
#define AZL 0X40

//gyroscope
#define GXH 0X43
#define GXL 0X44
#define GYH 0X45
#define GYL 0X46
#define GZH 0X47
#define GZL 0X48


void mpu6050_init();
uint8_t get_register_val(uint8_t address_word);

#ifdef __cplusplus
} // extern "C"
#endif

#endif //MPU6050_H
