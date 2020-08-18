#ifndef LIST_H
#define LIST_H

#include "stm32f446xx.h"
#include "complex.h"

typedef struct {
	const char *const name;	
	uint8_t *const array;
	const uint32_t size;
	uint32_t countWrite;
	uint32_t countRead;
} list;

extern const struct listFunctions {

	uint32_t (*const Count)(list *);
	void (*const Copy)(list *, list *);
	void (*const Clear)(list *);
	
	struct {
		uint32_t (*const Write)(list *);
		uint32_t (*const Read)(list *);
	} Index;

	struct {
		void (*const Ubyte)(list *, uint8_t);	
		void (*const Byte)(list *, int8_t);
		void (*const Ushort)(list *, uint16_t);		
		void (*const Short)(list *, int16_t);
		void (*const Uint)(list *, uint32_t);
		void (*const Int)(list *, int32_t);
		void (*const Float)(list *, float);
		void (*const Complex)(list *, complex);
		void (*const String)(list *, char *);
	} Write;

	struct {
		uint8_t (*const Ubyte)(list *);
		int8_t (*const Byte)(list *);
		uint16_t (*const Uint16)(list *);		
		int16_t (*const Int16)(list *);
		uint32_t (*const Uint)(list *);		
		int32_t (*const Int)(list *);
		float (*const Float)(list *);
		complex (*const Complex)(list *);
	} Read;

} List;

#endif
