#pragma once
#include <stdint.h>
#include "stm32g0b1xx.h"

#define MPU6050_ADDR        0x68

// Registers
#define MPU6050_WHO_AM_I    0x75
#define MPU6050_PWR_MGMT_1  0x6B
#define MPU6050_SMPLRT_DIV  0x19
#define MPU6050_CONFIG      0x1A
#define MPU6050_GYRO_CONFIG 0x1B
#define MPU6050_ACCEL_CONFIG 0x1C
#define MPU6050_ACCEL_XOUT_H 0x3B

int MPU6050_Init(I2C_TypeDef* i2c);
int MPU6050_ReadWhoAmI(I2C_TypeDef* i2c, uint8_t* who);
int MPU6050_ReadRaw(I2C_TypeDef* i2c,
                    int16_t* ax, int16_t* ay, int16_t* az,
                    int16_t* gx, int16_t* gy, int16_t* gz);
