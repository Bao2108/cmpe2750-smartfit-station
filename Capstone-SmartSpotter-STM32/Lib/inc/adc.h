//******************
// File: adc.h
//
// DESCRIPTION:
// Driver for handling analog inputs
//
// CREATED: Oct/17/2024, by Carlos Estay
//
//


#ifndef ADC_H
#define ADC_H
#include "stm32g0b1xx.h"


typedef enum ADC_Presc_Typedef__
{
    ADC_Presc_1, 
    ADC_Presc_2,
    ADC_Presc_4,
    ADC_Presc_6,
    ADC_Presc_8,
    ADC_Presc_10,
    ADC_Presc_12,
    ADC_Presc_16,
    ADC_Presc_32,
    ADC_Presc_64,
    ADC_Presc_128,
    ADC_Presc_256    
}ADC_Presc;

typedef enum ADC_Channel_Typedef__
{
    ADC_Channel_0 = 1<<0, 
    ADC_Channel_1 = 1<<1, 
    ADC_Channel_2 = 1<<2, 
    ADC_Channel_3 = 1<<3, 
    ADC_Channel_4 = 1<<4, 
    ADC_Channel_5 = 1<<5, 
    ADC_Channel_6 = 1<<6, 
    ADC_Channel_7 = 1<<7, 
    ADC_Channel_8 = 1<<8, 
    ADC_Channel_9 = 1<<9, 
    ADC_Channel_10 = 1<<10, 
    ADC_Channel_11 = 1<<11, 
    ADC_Channel_12 = 1<<12, 
    ADC_Channel_13 = 1<<13, 
    ADC_Channel_14 = 1<<14, 
    ADC_Channel_15 = 1<<15, 
    ADC_Channel_16 = 1<<16, 
    ADC_Channel_17 = 1<<17,   
    ADC_Channel_18 = 1<<18, 
}ADC_Channel;

void ADC_Init(ADC_TypeDef* adc, ADC_Common_TypeDef* adcCommon, ADC_Presc psc, uint32_t chEn);
void ADC_InterruptEn(ADC_TypeDef* adc, int IRQn);
uint32_t ADC_Read(ADC_TypeDef* adc);
void ADC_TriggerConv(ADC_TypeDef* adc);
uint32_t ADC_BlockingRead(ADC_TypeDef* adc);

#endif /* ADC_H */