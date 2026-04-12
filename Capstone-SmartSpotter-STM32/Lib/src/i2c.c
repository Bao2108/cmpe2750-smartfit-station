//******************
//I2C Library
//
// CREATED: 01/28/2025, by Carlos Estay
//
// FILE: i2c.c
//

#include "i2c.h"
#include <stdio.h>

void I2C_Init(I2C_TypeDef* i2c, I2C_Speed speed)
{//Defaulted to 7-bit address
  i2c->CR1 &=  ~I2C_CR1_PE;             //Disable peripheral, to configure it
  i2c->TIMINGR = speed;                //Set speed mode
  i2c->OAR1 &= ~I2C_OAR1_OA1EN; 
  //Must diable own address before any setting can be added, add settings here
  i2c->OAR1 |= I2C_OAR1_OA1EN;          //Receiver address is ACKed, Addr 0
  i2c->CR2 &= ~I2C_CR2_ADD10;           //7-bit address
  i2c->CR2 |= I2C_CR2_AUTOEND | I2C_CR2_NACK; //Autoend mode (STOP condition), enable NACK(disable in salve mode)
  i2c->CR1 |= I2C_CR1_PE;               //Enable I2C
}
//---------------------------------------------------------------
void I2C_Reset(I2C_TypeDef* i2c)
{
  i2c->CR1 &=  ~I2C_CR1_PE;
}
//---------------------------------------------------------------
void  I2C_ScanFirstAddr(I2C_TypeDef* i2c)
{
  uint8_t exit; 
  for (uint8_t address = 0x0; address < 0x7F; address++) 
  {
    exit = 0;
    uint16_t delay = 10000;
    while(--delay);
    //Set address with autoend and Tx 1 dummy byte
    i2c->CR2 = (address << 1) | I2C_CR2_AUTOEND | (0 << I2C_CR2_NBYTES_Pos);
    // Generate START condition
    i2c->CR2 |= I2C_CR2_START;
    //Dummy Byte
    //i2c->TXDR = 0x0F;
    while (!exit) // Wait for address to be sent
    {
    // Check for stop condition
      if(i2c->ISR & I2C_ISR_STOPF)
      {
        exit = 1;
        if (i2c->ISR & I2C_ISR_NACKF)
        {
          i2c->ICR |= I2C_ICR_NACKCF;
        }
        else
        {
          printf("I2C Device found at address: 0x%x\r\n", address);
        } 
        i2c->ICR |= I2C_ICR_STOPCF;
      }
    }
  }
}
//---------------------------------------------------------------
int I2C_Transmit(I2C_TypeDef* i2c, uint8_t addr, uint8_t* pData, uint8_t size)
{
  while (I2C_IsBusy(i2c)) { }

  addr &= 0x7F;

  i2c->ICR = I2C_ICR_STOPCF | I2C_ICR_NACKCF;

  i2c->CR2 = 0;
  i2c->CR2 |= (addr << 1);
  i2c->CR2 |= ((uint32_t)size << I2C_CR2_NBYTES_Pos);
  i2c->CR2 |= I2C_CR2_AUTOEND;
  i2c->CR2 |= I2C_CR2_START;

  for (uint8_t i = 0; i < size; i++)
  {
    while (!(i2c->ISR & I2C_ISR_TXIS))
    {
      if (i2c->ISR & I2C_ISR_NACKF)
      {
        i2c->ICR |= I2C_ICR_NACKCF;
        return 0;
      }
    }

    i2c->TXDR = pData[i];
  }

  while (!(i2c->ISR & I2C_ISR_STOPF))
  {
    if (i2c->ISR & I2C_ISR_NACKF)
    {
      i2c->ICR |= I2C_ICR_NACKCF;
      return 0;
    }
  }

  i2c->ICR |= I2C_ICR_STOPCF;
  return 1;
}
//---------------------------------------------------------------
int I2C_Writereg(I2C_TypeDef* i2c, uint8_t addr, uint8_t reg, uint8_t* pData, uint8_t size)
{
  // Accept both 7-bit (0x68) and 8-bit (0xD0/0xD1) address forms
  if (addr > 0x7F) addr >>= 1;
  addr &= 0x7F;

  // Clear stale flags
  i2c->ICR = I2C_ICR_STOPCF | I2C_ICR_NACKCF;

  // Wait until bus not busy
  while (I2C_IsBusy(i2c)) {}

  // Build CR2 cleanly
  i2c->CR2 = 0;
  i2c->CR2 |= (addr << 1);                          // SADD
  i2c->CR2 |= ((uint32_t)(size + 1U) << I2C_CR2_NBYTES_Pos); // reg + data
  i2c->CR2 &= ~I2C_CR2_RD_WRN;                      // write
  i2c->CR2 |= I2C_CR2_AUTOEND;                      // auto STOP
  i2c->CR2 |= I2C_CR2_START;

  // Send REG byte
  while (!(i2c->ISR & I2C_ISR_TXIS))
  {
    if (i2c->ISR & I2C_ISR_NACKF) { i2c->ICR |= I2C_ICR_NACKCF; return 0; }
  }
  i2c->TXDR = reg;

  // Send DATA bytes
  for (uint8_t i = 0; i < size; i++)
  {
    while (!(i2c->ISR & I2C_ISR_TXIS))
    {
      if (i2c->ISR & I2C_ISR_NACKF) { i2c->ICR |= I2C_ICR_NACKCF; return 0; }
    }
    i2c->TXDR = pData[i];
  }

  // Wait for STOP (AUTOEND)
  while (!(i2c->ISR & I2C_ISR_STOPF))
  {
    if (i2c->ISR & I2C_ISR_NACKF) { i2c->ICR |= I2C_ICR_NACKCF; return 0; }
  }
  i2c->ICR |= I2C_ICR_STOPCF;

  return 1;
}
//---------------------------------------------------------------
int I2C_IsBusy(I2C_TypeDef* i2c)
{
  return (i2c->ISR & I2C_ISR_BUSY);
}
//---------------------------------------------------------------


int I2C_ReadReg(I2C_TypeDef* i2c, uint8_t addr, uint8_t reg, uint8_t* pData, uint8_t size)
{
  while (I2C_IsBusy(i2c));

  addr &= 0x7F;

  // ---- Write register address (no AUTOEND, because we want repeated START)
  i2c->CR2 = 0;
  i2c->CR2 |= (addr << 1);                         // SADD
  i2c->CR2 |= (1 << I2C_CR2_NBYTES_Pos);           // 1 byte: reg
  i2c->CR2 &= ~I2C_CR2_RD_WRN;                     // write
  i2c->CR2 |= I2C_CR2_START;

  while (!(i2c->ISR & I2C_ISR_TXIS))
  {
    if (i2c->ISR & I2C_ISR_NACKF) { i2c->ICR |= I2C_ICR_NACKCF; return 0; }
  }

  i2c->TXDR = reg;

  while (!(i2c->ISR & I2C_ISR_TC))                 // transfer complete (no stop)
  {
    if (i2c->ISR & I2C_ISR_NACKF) { i2c->ICR |= I2C_ICR_NACKCF; return 0; }
  }

  // ---- Repeated START: read bytes
  i2c->CR2 = 0;
  i2c->CR2 |= (addr << 1);
  i2c->CR2 |= (size << I2C_CR2_NBYTES_Pos);
  i2c->CR2 |= I2C_CR2_RD_WRN;                      // read
  i2c->CR2 |= I2C_CR2_AUTOEND;                     // auto STOP after read
  i2c->CR2 |= I2C_CR2_START;

  for (uint8_t i = 0; i < size; i++)
  {
    while (!(i2c->ISR & I2C_ISR_RXNE))
    {
      if (i2c->ISR & I2C_ISR_NACKF) { i2c->ICR |= I2C_ICR_NACKCF; return 0; }
    }
    pData[i] = (uint8_t)i2c->RXDR;
  }

  while (!(i2c->ISR & I2C_ISR_STOPF)) { }
  i2c->ICR |= I2C_ICR_STOPCF;

  return 1;
}
