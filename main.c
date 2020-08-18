#include "init.h"
#include "print.h"
#include "dmaTransfer.h"
#include "led.h"
#include "delay.h"
#include "irq.h"
#include "phase.h"
#include "motor.h"
#include "bluetooth.h"

int main(void){

	Init.Gpio();
	Init.Systick();
	Init.Tim1();
	Init.Uart();
	Init.Dma();
	Init.FlashMemory();
	Init.Rcc();
	Init.Exti();
	Init.Irq();
	
	LED_ON;
	
	
	// Reload systimer
  TIM2->ARR = 0xffffffff;
	
//	static float pwm = 640.f;
//	static uint32_t pwmInt = 640;
	
	Motor.Speed.Set(8.2f);
	
	while(1){
		
		LED_TOOGLE;
		
		Print("Speed: ");
		Print((int)(Motor.Speed.Get()));
		Print(" rps (");
		Print((int)(Motor.Speed.Get() * 60.f));
		//Print(" rpm)  \r");
		PrintLn(" rpm)");		


		Bluetooth.Send.Srting("Speed ");
		Bluetooth.Send.Data((int) (Motor.Speed.Get()));
		Bluetooth.Send.Srting("\r\n");
		
		
		Delay.MilliSecond(1000.f);		

		// Print out
		DmaTransfer.Uart1(&listPrint);
		
		// Bluetooth out
		DmaTransfer.Uart3(&listBluetooth);
		
	}

}
