//******************
// FILE: adc.c
//
// DESCRIPTION:
//  Driver for handling analog inputs
//
// CREATED: Oct/17/2024, by Carlos Estay
//

#include "adc.h"


void ADC_Init(ADC_TypeDef* adc, ADC_Common_TypeDef* adcCommon, ADC_Presc psc, uint32_t chEn)
{
  //-------------------------ADC1 Auto Calibration------------------------------
  if( adc->CR & ADC_CR_ADEN)   // If adc is enabled
  {
    adc->CR |= ADC_CR_ADDIS;   // Disable it
    while(adc->CR & ADC_CR_ADEN); // Wait until it is disabled
  }
  adc->CHSELR = chEn; 
  //Write prescale value
  adcCommon->CCR &= ~ADC_CCR_PRESC_Msk;//clear setting
  adcCommon->CCR |= psc << ADC_CCR_PRESC_Pos;

  //Internal reference enable
  adcCommon->CCR |= ADC_CCR_VREFEN;

  //adc->CR &= ~ADC_CR_ADCALDIF;         // Single ended channels calibration
  adc->CR |= ADC_CR_ADCAL;             // Start calibration
  while(adc->CR & ADC_CR_ADCAL);      // Wait until it is done

  /*get the calibration factor, to add code*/
  //adc->CALFACT

  adc->CR |= ADC_CR_ADEN; // adc on
  while(!(ADC1->ISR & ADC_ISR_ADRDY));  //Wait till ready

  //Enter sequence of channels in
}
void ADC_InterruptEn(ADC_TypeDef* adc, int IRQn)
{
    adc->IER |= ADC_IER_EOCIE;
    NVIC_EnableIRQ(IRQn);
}

uint32_t ADC_Read(ADC_TypeDef* adc)
{
    return adc->DR;
}

void ADC_TriggerConv(ADC_TypeDef* adc)
{
    adc->CR |= ADC_CR_ADSTART_Msk;
}

uint32_t ADC_BlockingRead(ADC_TypeDef* adc)
{
    ADC_TriggerConv(adc);
    while(!(adc->ISR & ADC_ISR_EOC_Msk));   //Wait for end of conversion flag
    uint32_t data = ADC_Read(adc);
    return data;
}