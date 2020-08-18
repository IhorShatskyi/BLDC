#ifndef MOTOR_H
#define MOTOR_H

#include "stm32f446xx.h"


extern const struct motorFunctions {
	
	struct {
		
		void (*const Set)(float);
		float (*const Get)(void);
		
	} Speed;

	struct {
		
		void (*const Update)(void);
		
	} Coils;
	
	
} Motor;

#endif
