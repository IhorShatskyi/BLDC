#ifndef MOTOR_H
#define MOTOR_H

#include "stm32f446xx.h"

extern volatile uint8_t sinTableIndex;
extern int motorStop;


extern const struct motorFunctions {
	
	struct {
		
		void (*const Set)(float);
		float (*const Get)(void);
		
	} Speed;

	
	struct {
		
		void (*const Update)(void);
		
	} Coils;
	
	float (*const GetRealSpeed)(void);
	void (*const UpdateRealSpeed)(float);
	
} Motor;

#endif
