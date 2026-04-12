//******************
//Switch Library
//
// CREATED: 11/06/2023, by Carlos Estay
//
// FILE: switch.c
//

#include "switch.h"

SwState Sw_Process(SwState* state, GPIO_TypeDef* port, uint16_t pin)
{
    if(port->IDR & (1<<pin))
    {//switch active
        if(*state == Idle)
        {
            *state = Pressed;
        }
        else
        {
            *state = Held;
        }
    }
    else
    {//switch inactive
        if(*state == Released)
        {
            *state = Idle;
        }
        else
        {
            *state = Released;
        }
    }
    return *state;
}