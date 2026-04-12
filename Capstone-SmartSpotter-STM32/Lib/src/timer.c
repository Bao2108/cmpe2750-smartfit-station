//******************
//PWM Library
//
// CREATED: 10/04/2024, by Carlos Estay
//
// FILE: timer.c
//
#include "timer.h"

void Timer_Setup(TIM_TypeDef * timer, uint16_t psc, uint16_t period)
{
    timer->CR1 &= ~TIM_CR1_CEN;              //Stop Timer
    timer->PSC = psc-1;                      //Set prescale
    timer->ARR = period-1;
}

void Timer_SetupChannel(TIM_TypeDef * timer, CCR_Typedef ccr, ChannelMode_Typedef chMode)
{
    /*
    We first discriminate, which channel we want to configure
    It only supports up to 4 channels, check timer specs in reference manual
    to find out how many channels it has.
    */
    switch (ccr)
    {
    case TimCCR1:
        timer->CCMR1 &= ~TIM_CCMR1_CC1S_Msk;    //Clear capture-compare
        timer->CCMR1 &= ~TIM_CCMR1_OC1M_Msk;    //Clear mode
        if(chMode == InputCapture)
        {
            timer->CCMR1 |= TIM_CCMR1_CC1S_0;   

        }//Output compare or PWM keeps setting (00b)
        else if(chMode == OutputCompareToggle)
        {        
            timer->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_0;    //Toggle mode
        }
        else if(chMode == Pwm1)
        {
            timer->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1;    //PWM1 mode
        }
        else if(chMode == Pwm2)
        {
            timer->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_0;    //PWM2 mode
        } 
        timer->CCER |= TIM_CCER_CC1E;    //Enable channel
        break;
    case TimCCR2:
        timer->CCMR1 &= ~TIM_CCMR1_CC2S_Msk;    //Clear capture-compare
        timer->CCMR1 &= ~TIM_CCMR1_OC2M_Msk;    //Clear mode
        if(chMode == InputCapture)
        {
            timer->CCMR1 |= TIM_CCMR1_CC2S_0;       
        }//Output compare or PWM keeps setting (00b)
        else if(chMode == OutputCompareToggle)
        {        
            timer->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_0;    //Toggle mode
        }
        else if(chMode == Pwm1)
        {
            timer->CCMR1 |= TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1;    //PWM1 mode
        }
        else if(chMode == Pwm2)
        {
            timer->CCMR1 |= TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_0;    //PWM2 mode
        } 
        timer->CCER |= TIM_CCER_CC2E;    //Enable channel
        break;

    case TimCCR3:
        timer->CCMR2 &= ~TIM_CCMR2_CC3S_Msk;    //Clear capture-compare
        timer->CCMR2 &= ~TIM_CCMR2_OC3M_Msk;    //Clear mode
        if(chMode == InputCapture)
        {
            timer->CCMR2 |= TIM_CCMR2_CC3S_0;       
        }//Output compare or PWM keeps setting (00b)
        else if(chMode == OutputCompareToggle)
        {        
            timer->CCMR2 |= TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_0;    //Toggle mode
        }
        else if(chMode == Pwm1)
        {
            timer->CCMR2 |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1;    //PWM1 mode
        }
        else if(chMode == Pwm2)
        {
            timer->CCMR2 |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_0;    //PWM2 mode
        } 
        timer->CCER |= TIM_CCER_CC3E;    //Enable channel
        break;

    case TimCCR4:
        timer->CCMR2 &= ~TIM_CCMR2_CC4S_Msk;   //Clear capture-compare
        timer->CCMR2 &= ~TIM_CCMR2_OC4M_Msk;    //Clear mode
        if(chMode == InputCapture)
        {
            timer->CCMR2 |= TIM_CCMR2_CC4S_0;       
        }//Output compare or PWM keeps setting (00b)
        else if(chMode == OutputCompareToggle)
        {        
            timer->CCMR2 |= TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_0;    //Toggle mode
        }
        else if(chMode == Pwm1)
        {
            timer->CCMR2 |= TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1;    //PWM1 mode
        }
        else if(chMode == Pwm2)
        {
            timer->CCMR2 |= TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_0;    //PWM2 mode
        }         
        timer->CCER |= TIM_CCER_CC4E;    //Enable channel
        break;

    default:
        break;
    }
    if(chMode != InputCapture)
    {
        timer->BDTR |= TIM_BDTR_MOE; //enable master output (for OC and PWM)
    }
}

void Timer_WriteCCR(TIM_TypeDef * timer, CCR_Typedef ccr, uint32_t ccrTicks)
{
    *(&timer->CCR1 + ccr) = ccrTicks;
}


void Timer_EnableInterrupt(TIM_TypeDef * timer, IRQn_Type timerIRQn, Timer_IE interruptMask)
{
    timer->DIER |= interruptMask;
    NVIC_EnableIRQ(timerIRQn);
}

void Timer_SetEnable(TIM_TypeDef * timer, uint16_t en)
{
    if(en)
    {
        timer->CR1 |= TIM_CR1_CEN; 
    }
    else
    {
        timer->CR1 &= ~TIM_CR1_CEN; 
    }
}

void Timer_SetDelay_us(TIM_TypeDef * timer)
{
    uint16_t psc = SystemCoreClock / 1000000;
    Timer_Setup(timer, psc, 0xFFFF);
    timer->DIER &= ~TIM_DIER_UIE_Msk;   //Disable update interrupt
    timer->CR1 |= TIM_CR1_URS_Msk; //Only counter overflow generates Update event
}

void Timer_Delay_us(TIM_TypeDef * timer, uint16_t us)
{
    timer->CR1 &= ~TIM_CR1_CEN;             //Disable counter
    timer->ARR = us;
    timer->EGR |= TIM_EGR_UG;       //Restart counter
    timer->SR &= ~TIM_SR_UIF;     //clear flag
    timer->CR1 |= TIM_CR1_CEN;      //Start timer
    while(!(timer->SR & TIM_SR_UIF));
    
}