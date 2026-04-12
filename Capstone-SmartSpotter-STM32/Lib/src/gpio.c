//******************
//GPIO Library
//
// CREATED: 10/27/2023, by Carlos Estay
//
// FILE: gpio.c
//

#include "gpio.h"

/*
  16-pin port
  MODER REGISTER (32-bits, 2 bits per pin)
  -00: Input mode
  -01: General purpose output mode
  -10: Alternate function mode
  -11: Analog mode (reset state)



  OTYPER REGISTER 16-LSBits, 1 per pin)
  The output type will be configured to push-pull by default.
  If open drain is desired, change OTYPER register
  
  PUPDR REGISTER (32-bits, 2 per pin)
  -00: No pull-up, pull-down
  -01: Pull-up
  -10: Pull-down
  -11: Reserved

*/
void GPIO_InitInput(GPIO_TypeDef* port, uint16_t pin)
{
  port->MODER &= ~(0b11 << (pin<<1)); //Clear moder, configure as input 
}
//------------------------------------------------------------------------------
void GPIO_InitOutput(GPIO_TypeDef* port, uint16_t pin)
{

  port->MODER &= ~(0b11 << (pin<<1)); //Clear moder
  port->MODER |= (0b01 << (pin<<1)); //Configure as output
}
//------------------------------------------------------------------------------
void GPIO_SetPullMode(GPIO_TypeDef* port, uint16_t pin, IO_PullMode pullMode)
{
  port->PUPDR &= ~((uint16_t)PullMode_Mask << (pin<<1));
  port->PUPDR |= (uint16_t)pullMode << (pin<<1);
}
//------------------------------------------------------------------------------
void GPIO_SetType(GPIO_TypeDef* port, uint16_t pin, IO_Type type)
{
  port->OTYPER &= ~(1<<pin);
  port->OTYPER = type? port->OTYPER |= 1<<pin : port->OTYPER; 
}
//------------------------------------------------------------------------------
void GPIO_SetSpeed(GPIO_TypeDef* port, uint16_t pin, IO_Speed speed)
{
  port->OSPEEDR &= ~((uint16_t)Speed_VeryHigh << (pin<<1)); //Clear setting
  port->OSPEEDR |= (uint16_t)speed << (pin<<1);//set speed
}
//------------------------------------------------------------------------------
  void GPIO_I2C_Config(GPIO_TypeDef* port, uint16_t pin)
  {
    GPIO_SetPullMode(port, pin, PullMode_PullUp);
    GPIO_SetType(port, pin, Type_OpenDrain);
    GPIO_SetSpeed(port, pin, Speed_VeryHigh);
  }  
//------------------------------------------------------------------------------
  /*
    ALTERNATE FUNCTIONS (AF) see datasheet Tables 17 and 18
  - They go from AF0 to AF15 and they use 4 bits
  - If the port BIT is 7 or less AF goes in AFR[0]
  - If the port BIT is 8 or more (up to 15) AF goes in AFR[1]
  */
void GPIO_InitAlternateF(GPIO_TypeDef* port, uint16_t bit, uint16_t function)
{
    port->MODER &= ~(0b11 <<(2*bit)); //Clear moder
    port->MODER |= (0b10 << (2*bit)); //Configure as Alternate Function
    if(bit < 8)
    {
      port->AFR[0] &= ~(0xF << (bit*4));     //Clear AF setting
      port->AFR[0] |= function << (bit*4);   //Write AF
    }
    else if(bit < 16)
    {
      port->AFR[1] &= ~(0xF << ((bit-8)*4));     //Clear AF setting
      port->AFR[1] |= function << ((bit-8)*4);   //Write AF      
    }
}
//------------------------------------------------------------------------------
void GPIO_Set(GPIO_TypeDef* port, uint16_t bit)
{
  port->ODR |= (1 << bit);
}
//------------------------------------------------------------------------------
void GPIO_Clear(GPIO_TypeDef* port, uint16_t bit)
{
  port->ODR &= ~(1 << bit);
}
//------------------------------------------------------------------------------
void GPIO_Toggle(GPIO_TypeDef* port, uint16_t bit)
{
  port->ODR ^= (1 << bit);
}
//------------------------------------------------------------------------------
int GPIO_Read(GPIO_TypeDef* port, uint16_t bit)
{
  return(port->IDR & (1 << bit));
}