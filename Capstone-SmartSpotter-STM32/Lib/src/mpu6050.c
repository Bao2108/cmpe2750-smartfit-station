#include "mpu6050.h"
#include "i2c.h"

static int write8(I2C_TypeDef* i2c, uint8_t reg, uint8_t val)
{
  return I2C_Writereg(i2c, MPU6050_ADDR, reg, &val, 1);
}

int MPU6050_ReadWhoAmI(I2C_TypeDef* i2c, uint8_t* who)
{
  return I2C_ReadReg(i2c, MPU6050_ADDR, MPU6050_WHO_AM_I, who, 1);
}

int MPU6050_Init(I2C_TypeDef* i2c)
{
  // Wake up: clear sleep bit
  if (!write8(i2c, MPU6050_PWR_MGMT_1, 0x00)) return 0;

  // Basic stable defaults (good for bring-up)
  // Sample rate divider (optional)
  write8(i2c, MPU6050_SMPLRT_DIV, 0x09); // ~100 Hz if DLPF enabled (rough bring-up)
  write8(i2c, MPU6050_CONFIG, 0x03);     // DLPF = 3 (helps noise)
  write8(i2c, MPU6050_GYRO_CONFIG, 0x00);  // ±250 dps
  write8(i2c, MPU6050_ACCEL_CONFIG, 0x00); // ±2g

  return 1;
}

int MPU6050_ReadRaw(I2C_TypeDef* i2c,
                    int16_t* ax, int16_t* ay, int16_t* az,
                    int16_t* gx, int16_t* gy, int16_t* gz)
{
  uint8_t buf[14];

  // Read accel(6) + temp(2) + gyro(6) starting at ACCEL_XOUT_H
  if (!I2C_ReadReg(i2c, MPU6050_ADDR, MPU6050_ACCEL_XOUT_H, buf, 14)) return 0;

  *ax = (int16_t)((buf[0] << 8) | buf[1]);
  *ay = (int16_t)((buf[2] << 8) | buf[3]);
  *az = (int16_t)((buf[4] << 8) | buf[5]);

  // buf[6..7] temp (ignore for now)

  *gx = (int16_t)((buf[8]  << 8) | buf[9]);
  *gy = (int16_t)((buf[10] << 8) | buf[11]);
  *gz = (int16_t)((buf[12] << 8) | buf[13]);

  return 1;
}
