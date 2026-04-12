

#include "st7735.h"
#include "spi.h"
#include "gpio.h"

/*
Pin‑by‑Pin Meaning (ST7735S SPI Module)
GND: Ground
VCC: Power Supply (3.3V)
SCL: Serial Clock (SPI CLK)
SDA: Serial Data (SPI MOSI)
RES: Reset (Active Low)
DC: Data/Command Control (High for Data, Low for Command)   
CS: Chip Select (Active Low)
BLK: Backlight Control (Optional - 3.3V PWM for brightness control)

*/


void LCD_Init(LCD_Driver_t* lcd, GPIO_TypeDef* dc_port, uint16_t dc_pin,
              GPIO_TypeDef* cs_port, uint16_t cs_pin,
              GPIO_TypeDef* rst_port, uint16_t rst_pin,
              SPI_TypeDef* spi) 
{
    lcd->dc_port = dc_port;
    lcd->dc_pin = dc_pin;
    lcd->cs_port = cs_port;
    lcd->cs_pin = cs_pin;
    lcd->spi = spi;
    // Initialize GPIO pins for DC and CS
    GPIO_InitOutput(lcd->dc_port, lcd->dc_pin);
    GPIO_InitOutput(lcd->cs_port, lcd->cs_pin);
}

void LCD_Cmd(LCD_Driver_t* lcd, uint8_t c) 
{
    //DC_LOW
    GPIO_Clear(lcd->dc_port, lcd->dc_pin);
    //CS_LOW
    GPIO_Clear(lcd->cs_port, lcd->cs_pin); 
    SPI_TxRxByte(lcd->spi, c);  
    //CS_HIGH
    GPIO_Set(lcd->cs_port, lcd->cs_pin);
}

void LCD_Data(LCD_Driver_t* lcd, uint8_t d) 
{
    //DC_HIGH
    GPIO_Set(lcd->dc_port, lcd->dc_pin);
    //CS_LOW
    SPI_TxRxByte(lcd->spi, d);
    //CS_HIGH
    GPIO_Set(lcd->cs_port, lcd->cs_pin);
}

void LCD_Data_Buf(LCD_Driver_t* lcd, const uint8_t *buf, uint32_t len)
{
    // DC_HIGH
    GPIO_Set(lcd->dc_port, lcd->dc_pin);
    //CS_LOW
    GPIO_Clear(lcd->cs_port, lcd->cs_pin);
    SPI_TransmitReceive(lcd->spi, buf, len);
    //CS_HIGH
    GPIO_Set(lcd->cs_port, lcd->cs_pin);
}