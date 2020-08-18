#ifndef DMA_TRANSFER_H
#define DMA_TRANSFER_H

#include "list.h"

extern const struct dmaTransferFunctions {
	void (*const Uart1)(list *);
	void (*const Uart3)(list *);
} DmaTransfer;

#endif
