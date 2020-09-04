#ifndef DEFINE_H
#define DEFINE_H

#define TEST1_ON GPIOC->BSRR = GPIO_BSRR_BS_14
#define TEST1_OFF GPIOC->BSRR = GPIO_BSRR_BR_14
#define TEST1_TOOGLE GPIOC->ODR ^= GPIO_ODR_ODR_14

#define TEST2_ON GPIOC->BSRR = GPIO_BSRR_BS_15
#define TEST2_OFF GPIOC->BSRR = GPIO_BSRR_BR_15
#define TEST2_TOOGLE GPIOC->ODR ^= GPIO_ODR_ODR_15

// Clock
#define CPU_CLK 204800000

// Systimer
#define SYSTIMER (TIM2->CNT * 10) // current time in microsecond

#endif
