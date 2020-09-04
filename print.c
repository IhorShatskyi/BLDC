#include "print.h"

// list
static uint8_t print[SIZE_PRINT];
list listPrint = {"Print", print, SIZE_PRINT, 0, 0};

// common functions
static void printChar(char);
static char *toStringUint(uint32_t);
static char *toStringInt(int32_t);
static char *toStringFloat(float);

// print functions
static void printUint(uint32_t);
static void printInt(int32_t);
static void printUshort(uint16_t);
static void printShort(int16_t);
static void printUbyte(uint8_t);
static void printByte(int8_t);
static void printFloat(float);
static void printComplex(complex);
static void printString(char *);
static void printConstString(const char *);

// printLn functions
static void printLnUint(uint32_t);
static void printLnInt(int32_t);
static void printLnUshort(uint16_t);
static void printLnShort(int16_t);
static void printLnUbyte(uint8_t);
static void printLnByte(int8_t);
static void printLnFloat(float);
static void printLnComplex(complex);
static void printLnString(char *);
static void printLnConstString(const char *);

// description common functions
static void printChar(char src){	
	List.Write.Ubyte(&listPrint, src);
}
static char *toStringUint(uint32_t src){
	static char string[16] = {[15] = 0};
	int32_t i = 15;
	do{
		string[--i] = '0' + src % 10;
		src /= 10;
	}	while(src);
	return &string[i];
}
static char *toStringInt(int32_t src){
	static char string[16] = {[15] = 0};
	int32_t i = 15;
	int32_t sign = src;
	do{
		if(sign < 0) string[--i] = '0' - src % 10;
		else string[--i] = '0' + src % 10;
		src /= 10;
	} while(src);
	if(sign < 0) string[--i] = '-';
	return &string[i];
}
static char *toStringFloat(float src){
	static char string[32] = {[15] = '.'};

	int32_t integer = (int32_t)src;
	float fraction = src - (float)integer;
	
	int32_t i = 15;
	if(fraction < 0) fraction *= -1.f;
	do{
		fraction *= 10.f;
		string[++i] = (uint8_t)fraction + '0';
		fraction -= (float)((uint8_t)fraction);
	}	while(i != 30 && fraction != 0.f);
	string[++i] = 0;
	
	i = 15;
	do{
		if(src < 0.f) string[--i] = '0' - integer % 10;
		else string[--i] = '0' + integer % 10;
		integer /= 10;
	} while(i != 1 && integer != 0 );
	
	if(src < 0.f) string[--i] = '-';

	return &string[i];
}
// description print functions
static void printUint(uint32_t src){
	printString(toStringUint(src));
}
static void printInt(int32_t src){
	printString(toStringInt(src));
}
static void printUshort(uint16_t src){
	printString(toStringUint(src));
}
static void printShort(int16_t src){
	printString(toStringInt(src));
}
static void printUbyte(uint8_t src){
	printString(toStringUint(src));
}
static void printByte(int8_t src){
	printString(toStringInt(src));
}
static void printFloat(float src){
	printString(toStringFloat(src));
}
static void printComplex(complex src){
	printString(toStringFloat(src.re));
	printChar(' ');
	printString(toStringFloat(src.im));
}
static void printString(char *string){
	while(*string)
		printChar(*string++);
}
static void printConstString(const char *string){
	while(*string)
		printChar(*string++);
}

// description print functions
void PrintStringFinish(void){
	uint32_t size = 60 - (listPrint.countWrite % 60);
	while(size--)
		printChar(' ');
}

static void printLnUint(uint32_t src){
	printUint(src);
	//PrintStringFinish();
	printString("\n\r");
}
static void printLnInt(int32_t src){
	printInt(src);
	//PrintStringFinish();
	printString("\n\r");
}
static void printLnUshort(uint16_t src){
	printUshort(src);
	//PrintStringFinish();
	printString("\n\r");
}
static void printLnShort(int16_t src){
	printShort(src);
	//PrintStringFinish();
	printString("\n\r");
}
static void printLnUbyte(uint8_t src){
	printUbyte(src);
	//PrintStringFinish();
	printString("\n\r");
}
static void printLnByte(int8_t src){
	printByte(src);
	//PrintStringFinish();
	printString("\n\r");
}
static void printLnFloat(float src){
	printFloat(src);
	//PrintStringFinish();
	printString("\n\r");
}
static void printLnComplex(complex src){
	printComplex(src);
	//PrintStringFinish();
	printString("\n\r");
}
static void printLnString(char *src){
	printString(src);
	//PrintStringFinish();
	printString("\n\r");
}
static void printLnConstString(const char *src){
	printConstString(src);
	//PrintStringFinish();
	printString("\n\r");
}

// description PrintTimer functions
void PrintTimer(uint32_t ms){
	uint32_t seconds = ms / 1000;
	uint32_t minutes = seconds / 60;
	Print(" ");
	Print(minutes % 60); Print(":");
	Print(seconds % 60);
}

// structs
const struct printFunctions Print = {
	printUint,
	printInt,
	printUshort,
	printShort,
	printUbyte,
	printByte,
	printFloat,
	printComplex,
	printString,
	printConstString,
};
const struct printLnFunctions PrintLn = {
	printLnUint,
	printLnInt,
	printLnUshort,
	printLnShort,
	printLnUbyte,
	printLnByte,
	printLnFloat,
	printLnComplex,
	printLnString,
	printLnConstString
};
