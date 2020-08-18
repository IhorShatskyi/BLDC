#ifndef PRINT_H
#define PRINT_H

#include "stm32f446xx.h"
#include "complex.h"
#include "list.h"

#define SIZE_PRINT 8192

extern list listPrint;

extern const struct printFunctions {
	void (*const Uint)(uint32_t);
	void (*const Int)(int32_t);
	void (*const Ushort)(uint16_t);
	void (*const Short)(int16_t);
	void (*const Ubyte)(uint8_t);
	void (*const Byte)(int8_t);
	void (*const Float)(float);
	void (*const Complex)(complex);
	void (*const Srting)(char *);
	void (*const ConstSrting)(const char *);
} Print;

#define Print(X) \
	_Generic((X), \
	uint32_t: Print.Uint, \
	int32_t: Print.Int, \
	uint16_t: Print.Ushort, \
	int16_t: Print.Short, \
	uint8_t: Print.Ubyte, \
	int8_t: Print.Byte, \
	float: Print.Float, \
	complex: Print.Complex, \
	char *: Print.Srting, \
	const char *: Print.ConstSrting \
)(X)

extern const struct printLnFunctions {
	void (*const Uint)(uint32_t);
	void (*const Int)(int32_t);
	void (*const Ushort)(uint16_t);
	void (*const Short)(int16_t);
	void (*const Ubyte)(uint8_t);
	void (*const Byte)(int8_t);
	void (*const Float)(float);
	void (*const Complex)(complex);
	void (*const Srting)(char *);
	void (*const ConstSrting)(const char *);
} PrintLn;

#define PrintLn(X) \
	_Generic((X), \
	uint32_t: PrintLn.Uint, \
	int32_t: PrintLn.Int, \
	uint16_t: PrintLn.Ushort, \
	int16_t: PrintLn.Short, \
	uint8_t: PrintLn.Ubyte, \
	int8_t: PrintLn.Byte, \
	float: PrintLn.Float, \
	complex: PrintLn.Complex, \
	char *: PrintLn.Srting, \
	const char *: PrintLn.ConstSrting \
)(X)

void PrintTimer(uint32_t);
void PrintStringFinish(void);

#endif
