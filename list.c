#include "list.h"
#include "print.h"

// common functions
static uint32_t listCount(list *);
static uint32_t listIndexWrite(list *);
static uint32_t listIndexRead(list *);
static void listCopy(list *, list *);
static void listClear(list *);

// write functions
static void listWriteUbyte(list *, uint8_t);
static void listWriteByte(list *, int8_t);
static void listWriteUshort(list *, uint16_t);
static void listWriteShort(list *, int16_t);
static void listWriteUint(list *, uint32_t);
static void listWriteInt(list *, int32_t);
static void listWriteFloat(list *, float);
static void listWriteComplex(list *, complex);
static void listWriteString(list *, char *);

// read functions
static uint8_t listReadUbyte(list *);
static int8_t listReadByte(list *);
static uint16_t listReadUshort(list *);
static int16_t listReadShort(list *);
static uint32_t listReadUint(list *);
static int32_t listReadInt(list *);
static float listReadFloat(list *);
static complex listReadComplex(list *);

// description common functions
static uint32_t listCount(list *list){
	return list->countWrite - list->countRead;
}
static uint32_t listIndexWrite(list *list){
	return list->countWrite % list->size;
}
static uint32_t listIndexRead(list *list){
	return list->countRead % list->size;
}
static void listCopy(list *srcList, list *dstList){
	uint32_t index = srcList->countRead;
	uint32_t count = List.Count(srcList);
	while(count--)
		List.Write.Ubyte(dstList, List.Read.Ubyte(srcList));
	srcList->countRead = index;
}
static void listClear(list *list){
	list->countRead = list->countWrite;
}

// description write functions
static void listWriteUbyte(list *list, uint8_t src){
	uint32_t index = list->countWrite++;
	if(index - list->countRead >= list->size){
		list->countRead = index;
		Print("ERROR: write "); PrintLn(list->name);
	}
	list->array[index % list->size] = src;
}
static void listWriteByte(list *list, int8_t src){
	uint8_t *pointer = (uint8_t *)&src;
	listWriteUbyte(list, *pointer);
}
static void listWriteUshort(list *list, uint16_t src){
	uint8_t *pointer = (uint8_t *)&src;
	listWriteUbyte(list, *pointer++);
	listWriteUbyte(list, *pointer);
}
static void listWriteShort(list *list, int16_t src){
	uint8_t *pointer = (uint8_t *)&src;
	listWriteUbyte(list, *pointer++);
	listWriteUbyte(list, *pointer);
}
static void listWriteUint(list *list, uint32_t src){
	uint8_t *pointer = (uint8_t *)&src;
	listWriteUbyte(list, *pointer++);
	listWriteUbyte(list, *pointer++);
	listWriteUbyte(list, *pointer++);
	listWriteUbyte(list, *pointer);
}
static void listWriteInt(list *list, int32_t src){
	uint8_t *pointer = (uint8_t *)&src;
	listWriteUbyte(list, *pointer++);
	listWriteUbyte(list, *pointer++);
	listWriteUbyte(list, *pointer++);
	listWriteUbyte(list, *pointer);
}
static void listWriteFloat(list *list, float src){
	uint8_t *pointer = (uint8_t *)&src;
	listWriteUbyte(list, *pointer++);
	listWriteUbyte(list, *pointer++);
	listWriteUbyte(list, *pointer++);
	listWriteUbyte(list, *pointer);
}
static void listWriteComplex(list *list, complex src){
	uint8_t *pointer = (uint8_t *)&src;
	listWriteUbyte(list, *pointer++);
	listWriteUbyte(list, *pointer++);
	listWriteUbyte(list, *pointer++);
	listWriteUbyte(list, *pointer++);
	listWriteUbyte(list, *pointer++);
	listWriteUbyte(list, *pointer++);
	listWriteUbyte(list, *pointer++);
	listWriteUbyte(list, *pointer);
}
static void listWriteString(list *list, char *string){
	while(*string)
		listWriteUbyte(list, *string++);
}
// description read functions
static uint8_t listReadUbyte(list *list){
	uint32_t index = list->countRead++;
	if(list->countWrite - index == 0){
		list->countWrite += list->size;		
		Print("ERROR: read "); PrintLn(list->name);	
	}
	return list->array[index % list->size];
}
static int8_t listReadByte(list *list){
	static int8_t result;
	uint8_t *pointer = (uint8_t *)&result;
	*pointer = listReadUbyte(list);
	return result;
}
static uint16_t listReadUshort(list *list){
	static uint16_t result;
	uint8_t *pointer = (uint8_t *)&result;
	*pointer++ = listReadUbyte(list);
	*pointer = listReadUbyte(list);
	return result;
}
static int16_t listReadShort(list *list){
	static int16_t result;
	uint8_t *pointer = (uint8_t *)&result;
	*pointer++ = listReadUbyte(list);
	*pointer = listReadUbyte(list);
	return result;
}
static uint32_t listReadUint(list *list){
	static uint32_t result;
	uint8_t *pointer = (uint8_t *)&result;
	*pointer++ = listReadUbyte(list);
	*pointer++ = listReadUbyte(list);
	*pointer++ = listReadUbyte(list);
	*pointer = listReadUbyte(list);
	return result;
}
static int32_t listReadInt(list *list){
	static int32_t result;
	uint8_t *pointer = (uint8_t *)&result;
	*pointer++ = listReadUbyte(list);
	*pointer++ = listReadUbyte(list);
	*pointer++ = listReadUbyte(list);
	*pointer = listReadUbyte(list);
	return result;
}
static float listReadFloat(list *list){
	static float result;
	uint8_t *pointer = (uint8_t *)&result;
	*pointer++ = listReadUbyte(list);
	*pointer++ = listReadUbyte(list);
	*pointer++ = listReadUbyte(list);
	*pointer = listReadUbyte(list);
	return result;
}
static complex listReadComplex(list *list){
	static complex result;
	uint8_t *pointer = (uint8_t *)&result;
	*pointer++ = listReadUbyte(list);
	*pointer++ = listReadUbyte(list);
	*pointer++ = listReadUbyte(list);
	*pointer++ = listReadUbyte(list);
	*pointer++ = listReadUbyte(list);
	*pointer++ = listReadUbyte(list);
	*pointer++ = listReadUbyte(list);
	*pointer = listReadUbyte(list);	
	return result;
}

// struct
const struct listFunctions List = {

	listCount,
	listCopy,
	listClear,

	{
		listIndexWrite,
		listIndexRead
	},{
		listWriteUbyte,
		listWriteByte,
		listWriteUshort,		
		listWriteShort,
		listWriteUint,
		listWriteInt,
		listWriteFloat,
		listWriteComplex,
		listWriteString
	},{
		listReadUbyte,		
		listReadByte,
		listReadUshort,		
		listReadShort,
		listReadUint,		
		listReadInt,
		listReadFloat,
		listReadComplex
	}
	
};
