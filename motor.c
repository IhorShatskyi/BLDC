#include "motor.h"
#include "phase.h"
#include "led.h"
#include "define.h"
#include "math.h"

static float setSpeed;
static float realSpeed;

static uint32_t state[8] = {
	0, 				// unused
	A_C,	// 001
	B_A,	// 010
	B_C,	// 011
	C_B,	// 100
	A_B,	// 101
	C_A,	// 110
	0					// unused
};

// master functions
static void speedSet(float);
static float speedGet(void);
static void coilsUpdate(void);

// slave functions
static uint32_t hallSensorsGet(void);
static void rotationControl(void);

// struct functions
const struct motorFunctions Motor = {
	
	{
		speedSet,
		speedGet
	},

	{
		coilsUpdate
	}
	
};

// describe master functions
static void speedSet(float value){setSpeed = value;}
static float speedGet(void){return realSpeed;}
static void coilsUpdate(void){
	static uint32_t sequenceTrue[6] = {5,1,3,2,6,4};
	static uint32_t sequenceCounter = 0;
	static uint32_t oldSensorsValue = 0;
	static uint32_t timer;
	
	uint32_t sensorsValue = hallSensorsGet();
	
	if(state[sensorsValue] == 0) return;
	if(sequenceTrue[sequenceCounter % 6] != sensorsValue) return;
	if(oldSensorsValue == sensorsValue)	return;
	
	sequenceCounter++;
	oldSensorsValue = sensorsValue;
	
	TIM1->CCER = state[sensorsValue];

	
	if(sequenceCounter % 30 == 0)	LED_TOOGLE;
	
	if(sequenceCounter % 6 != 0) return;
	
	realSpeed = 1000.f / ((float)(SYSTIMER - timer) * 10.f);
	
	

	
	rotationControl();
		timer = SYSTIMER;
	
}

// describe slave functions
static uint32_t hallSensorsGet(void){return GPIOA->IDR & 0x7;}
static void rotationControl(void){
			//if(Motor.Speed.Get() > 0.f){
		
	
	if(fabsf(realSpeed - setSpeed) < 0.5f) return;
	
	if(realSpeed < setSpeed){
			
			if(TIM1->CCR1 > 10) {
			
//				pwm -= 0.1f;
//				pwmInt = (uint32_t)pwm;
				TIM1->CCR1--;// = pwmInt;
				TIM1->CCR2--;// = pwmInt;
				TIM1->CCR3--;// = pwmInt;
			}
			
		} else {
			
			if(TIM1->CCR1 < 60) {
			
//				pwm += 0.1f;
//				pwmInt = (uint32_t)pwm;
				
				TIM1->CCR1++;// = pwmInt;
				TIM1->CCR2++;// = pwmInt;
				TIM1->CCR3++;// = pwmInt;
			}
		}
	//}

}

