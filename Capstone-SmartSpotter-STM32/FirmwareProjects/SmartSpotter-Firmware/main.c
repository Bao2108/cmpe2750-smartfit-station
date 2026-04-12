/*********************************************************************
File    : main.c
Program : LCD + MPU6050 raw values
Purpose : Show ax, ay, az, gx, gy, gz on LCD1602
*********************************************************************/

#include <stdint.h>
#include <stdio.h>
#include "stm32g0b1xx.h"
#include "clock.h"
#include "gpio.h"
#include "uart.h"
#include "i2c.h"
#include "mpu6050.h"

#define LCD_ADDR  0x27      /* change to 0x3F if needed */
#define LCD_RS    0x01
#define LCD_EN    0x04
#define LCD_BL    0x08

volatile uint32_t msNow = 0;
static uint8_t lcdPage = 0;
static uint32_t lcdPageTick = 0;

static void HAL_Init_Custom(void);
static void wait_ms(uint32_t ms);
static void delay_loop(volatile uint32_t n);
static void LCD_PrintInt(int16_t v);
static void LCD_WriteRaw(uint8_t data);
static void LCD_PulseEnable(uint8_t data);
static void LCD_Write4Bits(uint8_t nibble, uint8_t rs);
static void LCD_Cmd(uint8_t cmd);
static void LCD_Data(uint8_t data);
static void LCD_Init1602(void);
static void LCD_Clear(void);
static void LCD_SetCursor(uint8_t col, uint8_t row);
static void LCD_Print(const char* str);
static void LCD_PrintFixed16(const char* str);
static void LCD_ShowMPUPage(int16_t ax, int16_t ay, int16_t az,
                            int16_t gx, int16_t gy, int16_t gz);

int main(void)
{
  int16_t ax, ay, az, gx, gy, gz;
  uint8_t who = 0;
  char msg[32];

  HAL_Init_Custom();
  Clock_InitPll(PLL_40MHZ);

  /* UART2 PA2/PA3 AF1 */
  GPIO_InitAlternateF(GPIOA, 2, 1);
  GPIO_InitAlternateF(GPIOA, 3, 1);
  UART_Init(USART2, 115200, 0);

  SysTick_Config(SystemCoreClock / 1000);

  GPIO_InitOutput(GPIOC, 6);

  /* I2C1 PB8/PB9 AF6 */
  GPIO_InitAlternateF(GPIOB, 8, 6);
  GPIO_InitAlternateF(GPIOB, 9, 6);
  GPIO_I2C_Config(GPIOB, 8);
  GPIO_I2C_Config(GPIOB, 9);
  I2C_Init(I2C1, I2C_Standard);

  wait_ms(50);

  LCD_Init1602();
  LCD_Clear();
  LCD_SetCursor(0, 0);
  LCD_PrintFixed16("MPU6050 + LCD");
  LCD_SetCursor(0, 1);
  LCD_PrintFixed16("Starting...");
  wait_ms(800);

  if (MPU6050_ReadWhoAmI(I2C1, &who))
  {
    snprintf(msg, sizeof(msg), "WHO_AM_I=0x%02X\r\n", who);
    UART_TxStr(USART2, msg);
  }
  else
  {
    UART_TxStr(USART2, "WHO_AM_I read fail\r\n");
  }

  if (!MPU6050_Init(I2C1))
  {
    LCD_Clear();
    LCD_SetCursor(0, 0);
    LCD_PrintFixed16("MPU init FAIL");
    while (1) { }
  }

  LCD_Clear();
  lcdPage = 0;
  lcdPageTick = msNow;

  while (1)
  {
    if (MPU6050_ReadRaw(I2C1, &ax, &ay, &az, &gx, &gy, &gz))
    {
      if ((msNow - lcdPageTick) >= 2000u)
      {
        lcdPageTick = msNow;
        lcdPage ^= 1u;
      }

      LCD_ShowMPUPage(ax, ay, az, gx, gy, gz);
    }
    else
    {
      LCD_SetCursor(0, 0);
      LCD_PrintFixed16("MPU read FAIL");
      LCD_SetCursor(0, 1);
      LCD_PrintFixed16("Check wiring");
    }

    GPIO_Toggle(GPIOC, 6);
    wait_ms(150);
  }
}

static void HAL_Init_Custom(void)
{
  RCC->APBENR1 |= RCC_APBENR1_PWREN_Msk;
  FLASH->ACR |= FLASH_ACR_PRFTEN_Msk;
  FLASH->ACR |= FLASH_ACR_LATENCY_1;

  RCC->IOPENR  |= RCC_IOPENR_GPIOAEN_Msk;
  RCC->IOPENR  |= RCC_IOPENR_GPIOBEN_Msk;
  RCC->IOPENR  |= RCC_IOPENR_GPIOCEN_Msk;

  RCC->APBENR1 |= RCC_APBENR1_USART2EN_Msk;
  RCC->APBENR1 |= RCC_APBENR1_I2C1EN_Msk;
}

void SysTick_Handler(void)
{
  msNow++;
}

static void wait_ms(uint32_t ms)
{
  uint32_t start = msNow;
  while ((msNow - start) < ms) { }
}

static void delay_loop(volatile uint32_t n)
{
  while (n--) { __NOP(); }
}

static void LCD_WriteRaw(uint8_t data)
{
  I2C_Transmit(I2C1, LCD_ADDR, &data, 1);
}

static void LCD_PulseEnable(uint8_t data)
{
  LCD_WriteRaw(data | LCD_EN);
  delay_loop(3000);
  LCD_WriteRaw((uint8_t)(data & ~LCD_EN));
  delay_loop(3000);
}

static void LCD_Write4Bits(uint8_t nibble, uint8_t rs)
{
  uint8_t data = LCD_BL;
  if (rs) data |= LCD_RS;
  data |= (uint8_t)((nibble & 0x0F) << 4);
  LCD_WriteRaw(data);
  LCD_PulseEnable(data);
}

static void LCD_Cmd(uint8_t cmd)
{
  LCD_Write4Bits((uint8_t)((cmd >> 4) & 0x0F), 0);
  LCD_Write4Bits((uint8_t)(cmd & 0x0F), 0);
  wait_ms(2);
}

static void LCD_Data(uint8_t data)
{
  LCD_Write4Bits((uint8_t)((data >> 4) & 0x0F), 1);
  LCD_Write4Bits((uint8_t)(data & 0x0F), 1);
  wait_ms(1);
}

static void LCD_Init1602(void)
{
  wait_ms(50);

  LCD_Write4Bits(0x03, 0);
  wait_ms(5);
  LCD_Write4Bits(0x03, 0);
  wait_ms(5);
  LCD_Write4Bits(0x03, 0);
  wait_ms(5);
  LCD_Write4Bits(0x02, 0);
  wait_ms(5);

  LCD_Cmd(0x28);
  LCD_Cmd(0x0C);
  LCD_Cmd(0x06);
  LCD_Cmd(0x01);
  wait_ms(5);
}

static void LCD_Clear(void)
{
  LCD_Cmd(0x01);
  wait_ms(2);
}

static void LCD_SetCursor(uint8_t col, uint8_t row)
{
  const uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
  LCD_Cmd((uint8_t)(0x80 | (col + row_offsets[row])));
}

static void LCD_Print(const char* str)
{
  while (*str)
  {
    LCD_Data((uint8_t)*str++);
  }
}

static void LCD_PrintFixed16(const char* str)
{
  uint8_t i = 0;

  while (*str && i < 16)
  {
    LCD_Data((uint8_t)*str++);
    i++;
  }

  while (i < 16)
  {
    LCD_Data(' ');
    i++;
  }
}

static void LCD_PrintInt(int16_t v)
{
  char buf[8];
  int i = 0;
  uint16_t mag;

  if (v < 0)
  {
    LCD_Data('-');
    mag = (uint16_t)(-v);
  }
  else
  {
    mag = (uint16_t)v;
  }

  if (mag == 0)
  {
    LCD_Data('0');
    return;
  }

  while (mag > 0 && i < 5)
  {
    buf[i++] = (char)('0' + (mag % 10));
    mag /= 10;
  }

  while (i > 0)
  {
    LCD_Data((uint8_t)buf[--i]);
  }
}

static void LCD_ShowMPUPage(int16_t ax, int16_t ay, int16_t az,
                            int16_t gx, int16_t gy, int16_t gz)
{
  if (lcdPage == 0)
  {
    LCD_SetCursor(0, 0);
    LCD_PrintFixed16("AX:      AY:    ");
    LCD_SetCursor(3, 0);
    LCD_PrintInt(ax);
    LCD_SetCursor(11, 0);
    LCD_PrintInt(ay);

    LCD_SetCursor(0, 1);
    LCD_PrintFixed16("AZ:            ");
    LCD_SetCursor(3, 1);
    LCD_PrintInt(az);
  }
  else
  {
    LCD_SetCursor(0, 0);
    LCD_PrintFixed16("GX:      GY:    ");
    LCD_SetCursor(3, 0);
    LCD_PrintInt(gx);
    LCD_SetCursor(11, 0);
    LCD_PrintInt(gy);

    LCD_SetCursor(0, 1);
    LCD_PrintFixed16("GZ:            ");
    LCD_SetCursor(3, 1);
    LCD_PrintInt(gz);
  }
}