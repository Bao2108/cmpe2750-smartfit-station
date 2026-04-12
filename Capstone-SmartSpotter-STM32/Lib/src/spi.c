//******************
//SPI Library
//
// CREATED: 10/04/2024, by Carlos Estay
//
// FILE: spi.c
//
#include "spi.h"


void SPI_Init(SPI_TypeDef* spi, SPIBusPrescaler div)
{
  spi->CR1 = div;
  spi->CR1 |= SPI_CR1_CPOL | SPI_CR1_CPHA;    //Set mode to (1,1) (pol. pha)
  spi->CR1 |= SPI_CR1_MSTR;                   //Master Configuration
  spi->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;      //Soft Slave mngment enabled     
  
  //8-bit data  size
  spi->CR2 |= SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0;   
  spi->CR2 |= SPI_CR2_FRXTH;                  //8-bit data  event
  spi->CR1 |= SPI_CR1_SPE;                    //Enable SPI
}
//------------------------------------------------------------------------------
void SPI_TxByte(SPI_TypeDef* spi, uint8_t data)
{
  while(!(spi->SR & SPI_SR_TXE));
  spi->DR = data;
}
//------------------------------------------------------------------------------
uint8_t SPI_RxByte(SPI_TypeDef* spi)
{
  uint8_t data = 0;
  
  while(!(spi->SR & SPI_SR_RXNE));
  data = spi->DR;
  return data;
}
//------------------------------------------------------------------------------
uint8_t SPI_TxRxByte(SPI_TypeDef* spi, uint8_t txData)
{
  uint8_t rxData = 0;

  while(!(spi->SR & SPI_SR_TXE)); //Wait until TX register is empty
  spi->DR= txData;
  while(!(spi->SR & SPI_SR_RXNE)); //Wait until RX register is not empty
  if(spi->SR & SPI_SR_RXNE)
  {
      rxData = spi->DR;  
  }
  return rxData;
}
//------------------------------------------------------------------------------
void SPI_TransmitReceive(SPI_TypeDef* spi, uint8_t* src, uint8_t* dest, uint16_t size)
{
  ///m_Cs->Clear();
  for(int i = 0; i < size; i++)
  {
    dest[i] = SPI_TxRxByte(spi, src[i]);
  }
  //m_Cs->Set();
}