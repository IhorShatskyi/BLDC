#ifndef INIT_H
#define INIT_H

extern const struct initFunctions {
	void (*const Gpio)(void);
	void (*const Systick)(void);
	void (*const Tim)(void);
	void (*const Uart)(void);
	void (*const Dma)(void);
	void (*const FlashMemory)(void);
	void (*const Rcc)(void);
	void (*const Exti)(void);
	void (*const Irq)(void);
	void (*const Adc)(void);
} Init;

#endif
