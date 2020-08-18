#include "irq.h"

#include "stm32f446xx.h"
#include "motor.h"
#include "print.h"
#include "bluetooth.h"

void EXTI0_IRQHandler(void){
	
	EXTI->PR = EXTI_PR_PR0;
	Motor.Coils.Update();
	
}

void EXTI1_IRQHandler(void){
	
	EXTI->PR = EXTI_PR_PR1;
	Motor.Coils.Update();
	
}

void EXTI2_IRQHandler(void){
	
	EXTI->PR = EXTI_PR_PR2;
	Motor.Coils.Update();
	
}

void USART3_IRQHandler(void){
	
	uint32_t data = USART3->DR;
	
	data -= '0';
	
	Print("Speed set ");
	PrintLn(data);
	
	Motor.Speed.Set((float) data);
	
}
