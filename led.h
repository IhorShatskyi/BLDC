#ifndef LED_H
#define LED_H

#include "stm32f446xx.h"

#define LED_ON GPIOC->BSRR = GPIO_BSRR_BS_13
#define LED_OFF GPIOC->BSRR = GPIO_BSRR_BR_13
#define LED_TOOGLE GPIOC->ODR ^= GPIO_ODR_ODR_13

#endif
