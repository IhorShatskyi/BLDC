#include "dmaTransfer.h"
#include "stm32f446xx.h"

// functions
static void Uart1(list *);
static void Uart3(list *);

// struct
const struct dmaTransferFunctions DmaTransfer = {
	Uart1,
	Uart3
};

// description
static void Uart1(list *src){

	// get num
	uint32_t num = List.Count(src);
	if(!num) return;	
	
	// check stream DMA
	if(DMA2_Stream7->NDTR) return;	
	
	// check send last byte
	if(!(USART1->SR & USART_SR_TXE)) return;	

	// correct num
	uint32_t writePointer = List.Index.Write(src);
	if(num > writePointer)
		num -= writePointer;
	
	// get address read pointer
	uint32_t readPointer = List.Index.Read(src);	
	uint32_t addressReadPointer = (uint32_t)&(src->array[readPointer]);

	// start stream
	DMA2_Stream7->NDTR = (uint32_t) num;
	DMA2_Stream7->PAR  = (uint32_t)&(USART1->DR);    
	DMA2_Stream7->M0AR = addressReadPointer;
	DMA2->HIFCR |= DMA_HIFCR_CTCIF7;
	DMA2_Stream7->CR |= DMA_SxCR_EN;     

	// modify read pointer
	src->countRead += num;
}

static void Uart3(list *src){

	// get num
	uint32_t num = List.Count(src);
	if(!num) return;	
	
	// check stream DMA
	if(DMA1_Stream3->NDTR) return;

	// check send last byte
	if(!(USART3->SR & USART_SR_TXE)) return;	

	// correct num
	uint32_t writePointer = List.Index.Write(src);
	if(num > writePointer)
		num -= writePointer;	
	
	// get address read pointer
	uint32_t readPointer = List.Index.Read(src);
	uint32_t addressReadPointer = (uint32_t)&(src->array[readPointer]);

	// start stream
	DMA1_Stream3->NDTR = (uint32_t) num;
	DMA1_Stream3->PAR  = (uint32_t)&(USART3->DR);    
	DMA1_Stream3->M0AR = addressReadPointer;
	DMA1->LIFCR |= DMA_LIFCR_CTCIF3;
	DMA1_Stream3->CR |= DMA_SxCR_EN;     

	// modify read pointer
	src->countRead += num;
}
