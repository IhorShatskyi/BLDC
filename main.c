#include "init.h"
#include "print.h"
#include "dmaTransfer.h"
#include "led.h"
#include "delay.h"
#include "irq.h"
#include "phase.h"
#include "motor.h"
#include "bluetooth.h"
#include "pwmSinTable.h"
#include "define.h"

static float avg[100];
static float acummAvg;
static uint32_t indexAvg;


int main(void){

	static float batttery = 0.f;
	static float distance = 0.f;
	static uint32_t acselerationTimer = 0;
	static uint32_t outDataTimer = 0;
	
	
	for(int i = 0; i < 100; i++)
		avg[i] = 0.f;

	acummAvg = 0.f;	
	indexAvg = 0;

	
	Init.Gpio();
	Init.Systick();
	Init.Tim();
	Init.Uart();
	Init.Adc();
	Init.Dma();
	Init.FlashMemory();
	Init.Rcc();
	Init.Exti();
	Init.Irq();
		
	LED_ON;
	
	// Reload systimer
  TIM2->ARR = 0xffffffff;
	
	//Motor.Speed.Set(10.0f);
	
	//TIM4->ARR = 500;
	
//	A_HI_ON;
//	B_HI_ON;
//	C_HI_ON;
//	A_LO_ON;
//	B_LO_ON;
//	C_LO_ON;
	
	
	
	float pwm = (float)(TIM4->ARR);
	pwm *= 10.f;
	
	//Print("Fuck. I am reset");
	
	
	while(1){

		
		if(SYSTIMER - acselerationTimer > 10000){
			acselerationTimer = SYSTIMER;
			float needPwmFactor = 6400.f * Motor.Speed.Get() / 9.f;
			
			if(needPwmFactor - pwmFactor > 100.f ){
				pwmFactor *= 1.1f;
			} 
			if(pwmFactor - needPwmFactor > 100.f ){
				pwmFactor *= 0.9f;
			}
			
		}
			
		if(motorStop){
			
			if(Motor.GetRealSpeed() * 1.176f > 5.f){
					motorStop = 0;
					pwmFactor = 640.f;
					NVIC_EnableIRQ(TIM3_IRQn);			
					NVIC_EnableIRQ(TIM4_IRQn);
					Motor.Speed.Set(2.f);
					TIM1->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E | TIM_CCER_CC1NE | TIM_CCER_CC2NE | TIM_CCER_CC3NE;
				}
		}
		
			
		if(SYSTIMER - outDataTimer > 10000){
			outDataTimer = SYSTIMER;
			float speed = Motor.GetRealSpeed();
			//batttery = (float)(ADC1->DR) * 30.f / 2605.f;
			batttery = (float)(ADC1->DR);
			
			
			
			acummAvg += batttery - avg[indexAvg];
			avg[indexAvg] = batttery;
			indexAvg++;
			indexAvg %= 100;
			batttery = acummAvg / 100.f;
			
			
			distance += speed * 0.326f * 1.0f;
		



			Print("Speed: ");
			Print((int)speed);
			Print(" rps ");
			Print((int)(speed * 60.f));
			Print(" rpm ");
			Print((int)(speed * 1.176f));
			Print(" km/h \tBattery: ");
			Print((uint32_t)batttery);
			Print(" V \tDistance: ");
			Print((uint32_t)(distance/1000.f));
			Print(" km ");
			Print((uint32_t)(distance) % 1000);
			Print(" m          \r");
			//PrintLn("");


//			Bluetooth.Send.Srting("Speed ");
//			Bluetooth.Send.Data((int)(speed * 1.176f));
//			Bluetooth.Send.Srting(" Volt ");
//			Bluetooth.Send.Data((int)batttery);
//			Bluetooth.Send.Srting(" Dist ");
//			Bluetooth.Send.Data((int)distance);
//			Bluetooth.Send.Srting("\r\n");


			ADC1->CR2 |= ADC_CR2_SWSTART;

			//Delay.MilliSecond(1000.f);

			// Print out
			DmaTransfer.Uart1(&listPrint);

			// Bluetooth out
			DmaTransfer.Uart3(&listBluetooth);
			

			
		}
	}

}
