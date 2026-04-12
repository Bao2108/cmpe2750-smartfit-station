//******************
//Clock Library
//
// CREATED: Sept/24/2024, by Carlos Estay
//
// FILE: clock.c
//


//Max MCU Frequency: 32MHz

#include "gpio.h"
#include "clock.h"

/*
  The HSISYS is used as system clock source after startup from reset, with the division by 1 (producing HSI16 frequency).
  The HCLK clock and PCLK clock are used for clocking the AHB and the APB domains,
  respectively. Their maximum allowed frequency is 64MHz. (RM 5.2)
*/

void Clock_InitPll(PllRange pllRange)
{
  
  RCC->CR &= ~RCC_CR_PLLON;             //Disable Pll
  while(RCC->CR  & RCC_CR_PLLRDY);     //Wait until Pll is fully stopped
  /*
  For Pll Configuration check (5.2.4) in the Reference Manual
  Also, the Clock Configuration tab in STMCubeMx.

  PLL_CLK = PLL_IN x (N / M) / R   

  he PLL PLL_CLK frequency must not exceed 64 MHz

  The following configurations get done in the RCC_PLLCFGR register:

  (1) PLL_IN, depends on PLLSRC BITS[1:0]
        - PLLSRC = 0 (HSI16 Clock)  //16MHZ
        - PLLSRC = 1 (HSE Clock)    //Given by external crystal

  (2) N depends on PLLN BITS[14:8]

  (3) M depends on PLLM BITS[6:4]

  (4) R depends on PLLR BITS[31:29]

  */
  RCC->PLLCFGR = pllRange | RCC_PLLCFGR_PLLSRC_HSI; //Add tange and select HSI16 as source

  RCC->CR |= RCC_CR_PLLON;              //Enable Pll again
  while(!(RCC->CR & RCC_CR_PLLRDY));    //Wait until PLL is locked
  RCC->PLLCFGR |= RCC_PLLCFGR_PLLPEN | RCC_PLLCFGR_PLLREN;   //Enable PLL/PLLR output
  RCC->CFGR |= RCC_CFGR_SW_PLL;         //Set Pll as System Clock

  SystemCoreClockUpdate();  //This is a CMSIS function
  //https://arm-software.github.io/CMSIS_5/latest/Core/html/group__system__init__gr.html
}

void Clock_EnableOutput(MCO_Select sel, MCO_Div div)
{
  GPIO_InitAlternateF(GPIOA, 8, 0);
  //Clear current DIV and source
  RCC->CFGR &= ~(RCC_CFGR_MCOPRE_Msk| RCC_CFGR_MCOSEL_Msk);
  //RCC->CFGR |= div | sel;
}