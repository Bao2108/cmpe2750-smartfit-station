
#include "stm32g0b1xx.h"




typedef struct LCD_Driver__ 
{
    SPI_TypeDef* spi;
    GPIO_TypeDef* dc_port;
    uint16_t dc_pin;
    GPIO_TypeDef* cs_port;
    uint16_t cs_pin;
} LCD_Driver_t;


void LCD_Init(LCD_Driver_t* lcd, GPIO_TypeDef* dc_port; uint16_t dc_pin,
              GPIO_TypeDef* cs_port; uint16_t cs_pin,
              SPI_TypeDef* spi) 

void LCD_Cmd(uint8_t c);
void LCD_Data(uint8_t d);
void LCD_Data_Buf(const uint8_t *buf, uint32_t len);