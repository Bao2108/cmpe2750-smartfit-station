//******************
//Switch Library
//
// CREATED: 11/06/2023, by Carlos Estay
//
// FILE: switch.h
//

#include "stm32g0b1xx.h"


typedef enum SwStateTypedef__
{
  Idle,
  Pressed,
  Held,
  Released,
}SwState;


SwState Sw_Process(SwState*, GPIO_TypeDef*, uint16_t);