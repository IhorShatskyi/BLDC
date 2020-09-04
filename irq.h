#ifndef IRQ_H
#define IRQ_H

#include "stm32f446xx.h"

extern float pwmFactor;

void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void EXTI2_IRQHandler(void);
void USART3_IRQHandler(void);
void TIM4_IRQHandler(void);
void TIM3_IRQHandler(void);

#endif
