//******************
//UART Library
//
// CREATED: 11/05/2023, by Carlos Estay
//
// FILE: uart.c
//

#include "uart.h"

void UART_Init(USART_TypeDef* uart, uint32_t br, char iEn)
{
  //Disable USART/UART to configure it
  uart->CR1 &= ~(USART_CR1_UE | USART_CR1_TE | USART_CR1_RE);
  uart->BRR = SystemCoreClock / br;      //Set Baud Rate
  uart->CR3 |= USART_CR3_OVRDIS;        //Disable Overrun (40.8.3)
  //Enable USART/UART, Tx and Rx
  uart->CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;

  if(iEn)
  {//Enable interrupts for receiving
    uart->CR1 |= USART_CR1_RXNEIE_RXFNEIE_Msk;
  }
  else
  {//Disable interrupts for receiving
    uart->CR1 &= ~USART_CR1_RXNEIE_RXFNEIE_Msk;
  }
  uart->RQR |= USART_RQR_RXFRQ;//clear receiving flag, just in case
}
//------------------------------------------------------------------------------
void UART_TxByte(USART_TypeDef* uart, uint8_t data)
{
  while(!(uart->ISR & USART_ISR_TXE_TXFNF_Msk));
  uart->TDR = data;  
}
//------------------------------------------------------------------------------
void UART_TxStr(USART_TypeDef* uart,const char* str)
{
    while(*str != 0)
    {
        UART_TxByte(uart, *str++);
    }
}
//------------------------------------------------------------------------------
void UART_TxBuffer(USART_TypeDef* uart, uint8_t* str)
{
    UART_TxStr(uart, (const char*)str);
}
//------------------------------------------------------------------------------
uint8_t UART_RxByte(USART_TypeDef* uart, uint8_t* pData)
{
    if(uart->ISR & USART_ISR_RXNE_RXFNE_Msk)
    {
        *pData = uart->RDR;
        return 1;
    }
    return 0;
}