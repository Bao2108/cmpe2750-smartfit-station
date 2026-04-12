//******************
//Clock Library
//
// CREATED: Sept/24/2024, by Carlos Estay
//
// FILE: clock.h
//
//
#include "stm32g0b1xx.h"

#ifndef CLOCK_H
#define CLOCK_H

#define RCC_CFGR_SW_PLL RCC_CFGR_SW_1

/*
  PLL_CLK = PLL_IN x (N / M) / R 
  Minumum R = 2

  R and M factor must subtract 1 as 0 means setting 1
*/
typedef enum PllRangeTypedef__
{                                                                                             // PLL_IN   N   M   R
    PLL_16MHZ = (8-1)<<RCC_PLLCFGR_PLLR_Pos |  8<<RCC_PLLCFGR_PLLN_Pos | (1-1)<<RCC_PLLCFGR_PLLM_Pos, // 16MHz x  8 / 1 / 8 = 16MHz
    PLL_20MHZ = (8-1)<<RCC_PLLCFGR_PLLR_Pos | 10<<RCC_PLLCFGR_PLLN_Pos | (1-1)<<RCC_PLLCFGR_PLLM_Pos, // 16MHz x 10 / 1 / 8 = 20MHz
    PLL_24MHZ = (8-1)<<RCC_PLLCFGR_PLLR_Pos | 12<<RCC_PLLCFGR_PLLN_Pos | (1-1)<<RCC_PLLCFGR_PLLM_Pos, // 16MHz x 12 / 1 / 8 = 24MHz
    PLL_32MHZ = (4-1)<<RCC_PLLCFGR_PLLR_Pos | 8<<RCC_PLLCFGR_PLLN_Pos | (1-1)<<RCC_PLLCFGR_PLLM_Pos,  // 16MHz x  8 / 1 / 4 = 32MHz
    PLL_40MHZ = (4-1)<<RCC_PLLCFGR_PLLR_Pos | 10<<RCC_PLLCFGR_PLLN_Pos | (1-1)<<RCC_PLLCFGR_PLLM_Pos, // 16MHz x 10 / 1 / 2 = 40MHz
    PLL_48MHZ = (4-1)<<RCC_PLLCFGR_PLLR_Pos | 12<<RCC_PLLCFGR_PLLN_Pos | (1-1)<<RCC_PLLCFGR_PLLM_Pos, // 16MHz x 12 / 1 / 4 = 48MHz
    PLL_50MHZ = (4-1)<<RCC_PLLCFGR_PLLR_Pos | 25<<RCC_PLLCFGR_PLLN_Pos | (2-1)<<RCC_PLLCFGR_PLLM_Pos, // 16MHz x 25 / 2 / 4 = 50MHz  
    PLL_60MHz = (2-1)<<RCC_PLLCFGR_PLLR_Pos | 15<<RCC_PLLCFGR_PLLN_Pos | (2-1)<<RCC_PLLCFGR_PLLM_Pos, // 16MHz x 15 / 2 / 2 = 60MHz
    PLL_64MHZ = (2-1)<<RCC_PLLCFGR_PLLR_Pos | 8 <<RCC_PLLCFGR_PLLN_Pos | (1-1)<<RCC_PLLCFGR_PLLM_Pos, // 16MHz x 8 / 1 / 2 = 64MHz

}PllRange;  


typedef enum MCO_DivTpedef__
{
    MCO_Div1 = 0 << 28,
    MCO_Div2 = 1 << 28,
    MCO_Div4 = 2 << 28,
    MCO_Div8 = 3 << 28,
    MCO_Div16 = 4 << 28
}MCO_Div;

typedef enum MCO_SelectTpedef__
{
    MCO_Sel_None= 0 << 24,
    MCO_Sel_SYSCLK = 1 << 24,
    MCO_Sel_HSI16 = 2 << 24,
    MCO_Sel_MSI = 3 << 24,
    MCO_Sel_HSE = 4 << 24,
    MCO_Sel_PLL = 5 << 24,
    MCO_Sel_LSI = 6 << 24,
    MCO_Sel_LSE = 7 << 24
}MCO_Select;


void Clock_InitPll(PllRange);
void Clock_EnableOutput(MCO_Select, MCO_Div);
  
#endif /* CLOCK_H */