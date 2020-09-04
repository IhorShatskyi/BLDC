#include "motor.h"
#include "phase.h"
#include "led.h"
#include "define.h"
#include "math.h"
#include "print.h"
#include "pwmSinTable.h"

static float speed = 0;
static float realSpeed = 0;
int motorStop = 1;

volatile uint8_t sinTableIndex = 0;
static const uint8_t PMSM_STATE_TABLE_INDEX_FORWARD[8] = {0, 160, 32, 0, 96, 128, 64, 0};

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
static float getRealSpeed(void);

// slave functions
static uint32_t hallSensorsGet(void);
static void updateRealSpeed(float);

// struct functions
const struct motorFunctions Motor = {
	
	{
		speedSet,
		speedGet
	},

	{
		coilsUpdate
	},
	
	getRealSpeed,
	updateRealSpeed
	
};

// describe master functions
static void speedSet(float value){speed = value;}
static float speedGet(void){return speed;}
static void coilsUpdate(void){
	
	//static uint32_t sequenceTrue[6] = {5,1,3,2,6,4};
	static uint32_t sequenceCounter = 0;
	static uint32_t timer = 0;

//	
//	
	uint32_t sensorsValue = hallSensorsGet();	
	//Print(hallSensorsGet());

	if(sensorsValue == 7) return;
	if(sensorsValue == 0) return;
	
  //if(sequenceTrue[sequenceCounter % 6] != sensorsValue) return;
	sequenceCounter++;


	//LED_TOOGLE;
	
	
	//TIM1->CCER = state[sensorsValue];
	
	if(sequenceCounter % 30 == 0)	LED_TOOGLE;
	
	//TIM5->CR1 |= TIM_CR1_CEN;

	if (sequenceCounter % 60 == 0)	{	
		float period = (float)(SYSTIMER - timer);
		timer = SYSTIMER;				
////		TIM4->CNT = 0;
////		TIM4->ARR = (uint32_t)(((period * 1024000.f) / 1000000.f) / 32.f);
		//Print("Time: "); PrintLn(period);
		updateRealSpeed( 1000000.f / period );
	}
		uint32_t PMSM_Speed = TIM3->CNT;
		TIM3->CNT = 0;
		
		TIM4->CNT = 0;
		TIM4->ARR = PMSM_Speed / 32;
		
		//updateRealSpeed( 1024000.f /(float) (PMSM_Speed * 60u) );	
		
		//PrintLn(PMSM_Speed * 60u);
		
		
//			if(Motor.Speed.Get() < 10.f ){
//				
//				if(mySpeed < 100)
//						mySpeed++;
//		} else if (Motor.Speed.Get() > 10.f ){
//			
//							if(mySpeed > 30)
//						mySpeed--;
//		}		
//	}	


		
			
		sinTableIndex = PMSM_STATE_TABLE_INDEX_FORWARD[sensorsValue];
		sinTableIndex += 6;
		sinTableIndex %= 192;
	
		

	
	


	
	//rotationControl();

	//LED_TOOGLE;

}

static float getRealSpeed(void){
	return realSpeed;
}

// describe slave functions
static void updateRealSpeed(float value){
	realSpeed = value;
}


static uint32_t hallSensorsGet(void){return GPIOA->IDR & 0x7;}


