//******************
//SPI Library
//
// CREATED: 10/04/2024, by Carlos Estay
//
// FILE: spi.h
//
#ifndef SPI_H
#define SPI_H

#include "stm32g0b1xx.h"
#include "gpio.h"


typedef enum SPIBusPrescalerTpedef__
{
DIV_2 = 0<<3,
DIV_4 = 1<<3,
DIV_8 = 2<<3,
DIV_16 = 3<<3,
DIV_32 = 4<<3,
DIV_64 = 5<<3,
DIV_128 = 6<<3,
DIV_256 = 7<<3
}SPIBusPrescaler;


void SPI_Init(SPI_TypeDef* spi, SPIBusPrescaler div);
void SPI_TxByte(SPI_TypeDef* spi, uint8_t data);
uint8_t SPI_RxByte(SPI_TypeDef* spi);
uint8_t SPI_TxRxByte(SPI_TypeDef* spi, uint8_t txData);
void SPI_TransmitReceive(SPI_TypeDef* spi, uint8_t* src, uint8_t* dest, uint16_t size);


#endif /* SPI_H */