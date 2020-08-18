#include "stm32f446xx.h"
#include "delay.h"
#include "define.h"

static void MicroSecond(float);
static void MilliSecond(float);
static void Second(float);

const struct delayFunctions Delay = {
	MicroSecond,
	MilliSecond,
	Second
};

static void MilliSecond(float ms){
	MicroSecond(1000.f * ms);
}

static void Second(float s){
	MilliSecond(1000.f * s);
}

static void MicroSecond(float  us){
	uint32_t time_start, count;  
	if(us > 0)
	{
		uint32_t tact = (uint32_t) ((float) CPU_CLK * us / 1000000.f);
		tact -= 17;
    count = tact % 0x00FFFFFFU;
    
    time_start =  SysTick->VAL;   
    while(((time_start - SysTick->VAL) & 0x00FFFFFFU) < count);
    count = tact >> 24;
    while(count--)
		{
			time_start =  SysTick->VAL;   
			while(((time_start - SysTick->VAL) & 0x00FFFFFFU)<0x00FFFF28U);   
    }
	}
}
