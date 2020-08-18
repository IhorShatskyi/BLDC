#include "init.h"
#include "stm32f446xx.h"
#include "define.h"

// CLOCK
#define APB1_CLK CPU_CLK/4
#define APB2_CLK CPU_CLK/2

// UART
#define BR_UART1 2048000 // Debug
#define BR_UART3 9600 // Bluetooth

// Master Functions
static void Gpio(void);
static void Systick(void);
static void Tim1(void);
static void Uart(void);
static void Dma(void);
static void FlashMemory(void);
static void Rcc(void);
static void Exti(void);
static void Irq(void);

// Master struct
const struct initFunctions Init = {
	Gpio,
	Systick,
	Tim1,
	Uart,
	Dma,
	FlashMemory,
	Rcc,
	Exti,
	Irq
};

static uint32_t pwmFactor = 64;

static void Exti(void) {
	
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	
	// Hall 1
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PA; // PA0
	EXTI->RTSR |= EXTI_RTSR_TR0; // Rising trigger
	EXTI->FTSR |= EXTI_FTSR_TR0; // Falling trigger
	EXTI->PR = EXTI_PR_PR0; // clear flag
	EXTI->IMR |= EXTI_IMR_MR0; // interrupt
	
	// Hall 2
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI1_PA; // PA1
	EXTI->RTSR |= EXTI_RTSR_TR1; // Rising trigger
	EXTI->FTSR |= EXTI_FTSR_TR1; // Falling trigger
	EXTI->PR = EXTI_PR_PR1; // clear flag	
	EXTI->IMR |= EXTI_IMR_MR1; // interrupt
	
	// Hall 3
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI2_PA; // PA2
	EXTI->RTSR |= EXTI_RTSR_TR2; // Rising trigger
	EXTI->FTSR |= EXTI_FTSR_TR2; // Falling trigger
	EXTI->PR = EXTI_PR_PR2; // clear flag		
	EXTI->IMR |= EXTI_IMR_MR2; // interrupt
	
}

static void Tim1(void){
	
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	
	
	TIM1->PSC = 200 - 1; // CPU_CLK / 200 = 102 4000 Hz
	TIM1->ARR = 64 - 1; // 102 4000 Hz / 64 = 16 000 Hz
	
	// 1
	TIM1->CCR1 = pwmFactor; // skvagnost
	TIM1->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1;  // mode pwm 1
	TIM1->CCMR1 |= TIM_CCMR1_OC1PE; // preload enable
	TIM1->CCER |= TIM_CCER_CC1NP| TIM_CCER_CC1P; // output polarity complementary - low, output polarity - low
	

	// 2
	TIM1->CCR2 = pwmFactor; // skvagnost
	TIM1->CCMR1 |= TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1;  // mode pwm 1
	TIM1->CCMR1 |= TIM_CCMR1_OC2PE; // preload enable
	TIM1->CCER |= TIM_CCER_CC2NP|TIM_CCER_CC2P; // output polarity complementary - low, output polarity - low
	
	// 3
	TIM1->CCR3 = pwmFactor; // skvagnost
	TIM1->CCMR2 |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1;  // mode pwm 1
	TIM1->CCMR2 |= TIM_CCMR2_OC3PE; // preload enable
	TIM1->CCER |= TIM_CCER_CC3NP|TIM_CCER_CC3P; // output polarity complementary - low, output polarity - low
	
	
	//TIM1->BDTR |= 255; // Dead-time generator setup
	TIM1->BDTR |= TIM_BDTR_MOE; // main output enable
	
	TIM1->CR1 |= TIM_CR1_CEN;
	
	// SYSTIMER
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
  TIM2->PSC = 10240 - 1 ;
	TIM2->ARR = (uint32_t) 0xf;
	TIM2->CR1 |= TIM_CR1_CEN;
	
	
	
}


// Describe master functions
static void Gpio(void) {

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // Port A
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; // Port B
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; // Port C

	// Hall sensor A PA0
	GPIOA->MODER |= 0<< GPIO_MODER_MODER0_Pos; // input mode
	GPIOA->PUPDR |= 0x2 << GPIO_PUPDR_PUPD0_Pos; // Pull-down

	// Hall sensor B PA1
	GPIOA->MODER |= 0<< GPIO_MODER_MODER1_Pos; // input mode
	GPIOA->PUPDR |= 0x2 << GPIO_PUPDR_PUPD1_Pos; // Pull-down

	// Hall sensor C PA2
	GPIOA->MODER |= 0<< GPIO_MODER_MODER2_Pos; // input mode
	GPIOA->PUPDR |= 0x2 << GPIO_PUPDR_PUPD2_Pos; // Pull-down

	// HS1 PA8
	GPIOA->MODER |= 0x2U << GPIO_MODER_MODER8_Pos; // Mode - alternative function
	GPIOA->OTYPER |= 0x0U << 8; // Type Push-pull
	GPIOA->OSPEEDR |= 0x3U << GPIO_OSPEEDR_OSPEED8_Pos; // Speed - very high
	GPIOA->PUPDR |= 0x0U << GPIO_PUPDR_PUPD8_Pos; // No pull-up, no pull-down
	GPIOA->AFR[1] |= 0x1U << GPIO_AFRH_AFSEL8_Pos; // Alternate function - TIM1_CH1

	// HS2 PA9
	GPIOA->MODER |= 0x2U << GPIO_MODER_MODER9_Pos; // Mode - alternative function
	GPIOA->OTYPER |= 0x0U << 9; // Type Push-pull
	GPIOA->OSPEEDR |= 0x3U << GPIO_OSPEEDR_OSPEED9_Pos; // Speed - very high
	GPIOA->PUPDR |= 0x0U << GPIO_PUPDR_PUPD9_Pos; // No pull-up, no pull-down
	GPIOA->AFR[1] |= 0x1U << GPIO_AFRH_AFSEL9_Pos; // Alternate function - TIM1_CH2

	// HS3 PA10
	GPIOA->MODER |= 0x2U << GPIO_MODER_MODER10_Pos; // Mode - alternative function
	GPIOA->OTYPER |= 0x0U << 10; // Type Push-pull
	GPIOA->OSPEEDR |= 0x3U << GPIO_OSPEEDR_OSPEED10_Pos; // Speed - very high
	GPIOA->PUPDR |= 0x0U << GPIO_PUPDR_PUPD10_Pos; // No pull-up, no pull-down
	GPIOA->AFR[1] |= 0x1U << GPIO_AFRH_AFSEL10_Pos; // Alternate function - TIM1_CH3

	// LS1 PB13
	GPIOB->MODER |= 0x2U << GPIO_MODER_MODER13_Pos; // Mode - alternative function
	GPIOB->OTYPER |= 0x0U << 13; // Type Push-pull
	GPIOB->OSPEEDR |= 0x3U << GPIO_OSPEEDR_OSPEED13_Pos; // Speed - very high
	GPIOB->PUPDR |= 0x0U << GPIO_PUPDR_PUPD13_Pos; // No pull-up, no pull-down
	GPIOB->AFR[1] |= 0x1U << GPIO_AFRH_AFSEL13_Pos; // Alternate function - TIM1_CH1N
	
	// LS2 PB14
	GPIOB->MODER |= 0x2U << GPIO_MODER_MODER14_Pos; // Mode - alternative function
	GPIOB->OTYPER |= 0x0U << 14; // Type Push-pull
	GPIOB->OSPEEDR |= 0x3U << GPIO_OSPEEDR_OSPEED14_Pos; // Speed - very high
	GPIOB->PUPDR |= 0x0U << GPIO_PUPDR_PUPD14_Pos; // No pull-up, no pull-down
	GPIOB->AFR[1] |= 0x1U << GPIO_AFRH_AFSEL14_Pos; // Alternate function - TIM1_CH2N
	
	// LS3 PB15
	GPIOB->MODER |= 0x2U << GPIO_MODER_MODER15_Pos; // Mode - alternative function
	GPIOB->OTYPER |= 0x0U << 15; // Type Push-pul
	GPIOB->OSPEEDR |= 0x3U << GPIO_OSPEEDR_OSPEED15_Pos; // Speed - very high
	GPIOB->PUPDR |= 0x0U << GPIO_PUPDR_PUPD15_Pos; // No pull-up, no pull-down
	GPIOB->AFR[1] |= 0x1U << GPIO_AFRH_AFSEL15_Pos; // Alternate function - TIM1_CH3N
	
	// USART1 - Debug
	// PB6 - TX	
	GPIOB->MODER |= 0x2 << GPIO_MODER_MODE6_Pos; // Alternate function mode
	GPIOB->OTYPER |= 0x0 << GPIO_OTYPER_OT6_Pos; // Push-pul
	GPIOB->OSPEEDR |= 0x3 << GPIO_OSPEEDR_OSPEED6_Pos; // Very high speed
	GPIOB->PUPDR |= 0x2 << GPIO_PUPDR_PUPD6_Pos; // Pull-down
	GPIOB->AFR[0] |= 0x7 << GPIO_AFRL_AFSEL6_Pos; // AF7
	// PB7 - RX	
	GPIOB->MODER |= 0x2 << GPIO_MODER_MODE7_Pos; // Alternate function mode
	GPIOB->OTYPER |= 0x0 << GPIO_OTYPER_OT7_Pos; // Push-pul
	GPIOB->OSPEEDR |= 0x3 << GPIO_OSPEEDR_OSPEED7_Pos; // Very high speed
	GPIOB->PUPDR |= 0x2 << GPIO_PUPDR_PUPD7_Pos; // Pull-down
	GPIOB->AFR[0]  |= 0x7 << GPIO_AFRL_AFSEL7_Pos; // AF7

	// USART3 - Bluetooth
	// PC10 - TX	
	GPIOC->MODER |= 0x2 << GPIO_MODER_MODE10_Pos; // Alternate function mode
	GPIOC->OTYPER |= 0x0 << GPIO_OTYPER_OT10_Pos; // Push-pul
	GPIOC->OSPEEDR |= 0x3 << GPIO_OSPEEDR_OSPEED10_Pos; // Very high speed
	GPIOC->PUPDR |= 0x2 << GPIO_PUPDR_PUPD10_Pos; // Pull-down
	GPIOC->AFR[1] |= 0x7 << GPIO_AFRH_AFSEL10_Pos; // AF7
	// PC11 - RX	
	GPIOC->MODER |= 0x2 << GPIO_MODER_MODE11_Pos; // Alternate function mode
	GPIOC->OTYPER |= 0x0 << GPIO_OTYPER_OT11_Pos; // Push-pul
	GPIOC->OSPEEDR |= 0x3 << GPIO_OSPEEDR_OSPEED11_Pos; // Very high speed
	GPIOC->PUPDR |= 0x2 << GPIO_PUPDR_PUPD11_Pos; // Pull-down
	GPIOC->AFR[1]  |= 0x7 << GPIO_AFRH_AFSEL11_Pos; // AF7


	// LED PC13
	GPIOC->MODER |= 0x1U << GPIO_MODER_MODE13_Pos; // Output mode
	GPIOC->OTYPER |= 0x0U << GPIO_OTYPER_OT13_Pos; // Push-pul
	GPIOC->OSPEEDR |= 0x3U << GPIO_OSPEEDR_OSPEED13_Pos; // Very high speed
	GPIOC->PUPDR |= 0x2U << GPIO_PUPDR_PUPD13_Pos; // Pull-down

}

static void Systick(void){
	SysTick->LOAD = 0xFFFFFFFFU;
	SysTick->VAL = 0xFFFFFFFFU;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;  
}

static void FlashMemory(void){
	FLASH->ACR |= FLASH_ACR_LATENCY_5WS;
	FLASH->ACR |= FLASH_ACR_PRFTEN;	// Prefetch enable
	FLASH->ACR |= FLASH_ACR_ICEN; // Instruction cache enable
	FLASH->ACR |= FLASH_ACR_DCEN;	// Data cache enable
}

static void Rcc(void){
	RCC->CR |= RCC_CR_HSEON; // HSE clock enable
	while(!(RCC->CR & RCC_CR_HSERDY)); // HSE clock ready flag
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLSRC; // Main PLL(PLL) and audio PLL (PLLI2S) entry clock source
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLM; // Division factor for the main PLL (PLL) and audio PLL (PLLI2S) input clock
	RCC->PLLCFGR |= 9 << RCC_PLLCFGR_PLLM_Pos;
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLN; // Main PLL (PLL) multiplication factor for VCO
	RCC->PLLCFGR |= 200 << RCC_PLLCFGR_PLLN_Pos;//200-2048000
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLP; // Main PLL (PLL) division factor for main system clock 
	RCC->PLLCFGR |= 0x0 << RCC_PLLCFGR_PLLP_Pos;	
	RCC->CR |= RCC_CR_PLLON; // Main PLL (PLL) enable
	while(!(RCC->CR & RCC_CR_PLLRDY)); // Main PLL (PLL) clock ready flag
	RCC->CFGR &= ~RCC_CFGR_PPRE1; // APB Low speed prescaler (APB1)
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;
	RCC->CFGR &= ~RCC_CFGR_PPRE2; // APB high-speed prescaler (APB2)
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;
	RCC->CFGR &= ~RCC_CFGR_SW; // System clock switch
	RCC->CFGR |= 0x2 << RCC_CFGR_SW_Pos;
}

static void Uart(){
	
	// USART 1 - Debug
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN; // Clock	
	USART1->BRR = (uint32_t) (APB2_CLK/BR_UART1); // Baud rate
	USART1->CR1 |= USART_CR1_TE; // Transmitter enable
	//USART1->CR1 |= USART_CR1_RE; // Receiver enable
	//USART1->CR1 |= USART_CR1_RXNEIE; // RXNE interrupt enable
	USART1->CR3 |= USART_CR3_DMAT; // DMA enable transmitter
	//USART1->CR3 |= USART_CR3_CTSE; // CTS mode enabled
	//USART1->CR3 |= USART_CR3_RTSE; // RTS output enabled
	USART1->CR1 |= USART_CR1_UE; // USART enable 	

	// USART 3 - Bluetooth
	RCC->APB1ENR |= RCC_APB1ENR_USART3EN; // Clock
	USART3->BRR = (uint32_t) (APB1_CLK/BR_UART3); // Baud rate
	USART3->CR1 |= USART_CR1_TE; // Transmitter enable
	USART3->CR1 |= USART_CR1_RE; // Receiver enable
	USART3->CR1 |= USART_CR1_RXNEIE; // RXNE interrupt enable
	USART3->CR3 |= USART_CR3_DMAT; // DMA enable transmitter
	//USART3->CR3 |= USART_CR3_CTSE; // CTS mode enabled
	//USART3->CR3 |= USART_CR3_RTSE; // RTS output enabled
	USART3->CR1 |= USART_CR1_UE; // USART enable
}

static void Dma(void){
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;	

	DMA1_Stream3->PAR = (int)&(USART3->DR);
	DMA1_Stream3->CR |= 1 << DMA_SxCR_MINC_Pos;
	DMA1_Stream3->CR |= 1 << DMA_SxCR_DIR_Pos;
	DMA1_Stream3->CR |= 4 << DMA_SxCR_CHSEL_Pos;
	
	DMA2_Stream7->PAR = (int)&(USART1->DR);
	DMA2_Stream7->CR |=  1 << DMA_SxCR_MINC_Pos;
	DMA2_Stream7->CR |=  1 << DMA_SxCR_DIR_Pos ;
	DMA2_Stream7->CR |=  4 << DMA_SxCR_CHSEL_Pos;
	
	
	
	
	
}

static void Irq(void){
	
	 NVIC_EnableIRQ(EXTI0_IRQn);
	 NVIC_EnableIRQ(EXTI1_IRQn);
	 NVIC_EnableIRQ(EXTI2_IRQn);
	 NVIC_EnableIRQ(USART3_IRQn);
	
}

