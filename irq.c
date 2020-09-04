#include "irq.h"

#include "stm32f446xx.h"
#include "motor.h"
#include "print.h"
#include "bluetooth.h"
#include "led.h"
#include "pwmSinTable.h"
#include "define.h"

float pwmFactor = 0.f;

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
	
	if(data < '0') {
		Print("Speed error");
		return;
	}
	
	if(data > '9') {
		Print("Speed error");
		return;
	}	
	
	data -= '0';
	//Print("Speed set "); PrintLn(data);
	//Bluetooth.Send.Srting("Speed set "); Bluetooth.Send.Data((int)data); Bluetooth.Send.Srting("\r\n");
	Motor.Speed.Set((float) data);
}

void TIM4_IRQHandler(void){
	TIM4->SR = 0;	
	TIM1->CCR3 = (uint32_t)( ( pwmFactor * (float)(pwmSinTable[sinTableIndex][0])) / 255.f );
	TIM1->CCR1 = (uint32_t)( ( pwmFactor * (float)(pwmSinTable[sinTableIndex][1])) / 255.f );
	TIM1->CCR2 = (uint32_t)( ( pwmFactor * (float)(pwmSinTable[sinTableIndex][2])) / 255.f );
	sinTableIndex++;
	sinTableIndex %= 192;
}


void TIM3_IRQHandler(void){ // stop
	TIM3->SR = 0;
	TIM3->SR = 0;
	TIM3->SR = 0;
//	NVIC_DisableIRQ(TIM4_IRQn);
//	NVIC_DisableIRQ(TIM3_IRQn);	
	Motor.UpdateRealSpeed(0.f);
//	Motor.Speed.Set(0.f);
	pwmFactor = 0.f;
	motorStop = 1;
	TIM1->CCER = 0;
}

