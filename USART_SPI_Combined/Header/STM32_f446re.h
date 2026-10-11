/*
 * STM32_f446re.h
 *
 *  Created on: May 9, 2026
 *      Author: User
 */
#include <stdint.h>
#include <stdio.h>

#ifndef STM32_F446RE_H_
#define STM32_F446RE_H_


#define _vo volatile

#define NVIC_ISER0 		((_vo uint32_t*)0xE000E100)
#define NVIC_ISER1 		((_vo uint32_t*)0xE000E104)
#define NVIC_ISER2 		((_vo uint32_t*)0xE000E108)
#define NVIC_ISER3 		((_vo uint32_t*)0xE000E10C)
#define NVIC_ISER4 		((_vo uint32_t*)0xE000E110)
#define NVIC_ISER5 		((_vo uint32_t*)0xE000E114)
#define NVIC_ISER6 		((_vo uint32_t*)0xE000E118)
#define NVIC_ISER7 		((_vo uint32_t*)0xE000E11C)

#define NVIC_ICER0 		((_vo uint32_t*)0XE000E180)
#define NVIC_ICER1 		((_vo uint32_t*)0XE000E184)
#define NVIC_ICER2 		((_vo uint32_t*)0XE000E188)
#define NVIC_ICER3 		((_vo uint32_t*)0XE000E18C)
#define NVIC_ICER4 		((_vo uint32_t*)0XE000E190)
#define NVIC_ICER5 		((_vo uint32_t*)0XE000E194)
#define NVIC_ICER6 		((_vo uint32_t*)0XE000E198)
#define NVIC_ICER7 		((_vo uint32_t*)0XE000E19C)

#define NVIC_IPR_BASE	((_vo uint32_t*)0xE000E400)
#define NO_PR_BITS_IMPLEMENTED 4U


#define PERIPH_BASEADDR 	0x40000000U
#define APB1_BASE 			PERIPH_BASEADDR
#define APB2_BASE 			0x40010000U
#define AHB1_BASE 			0x40020000U
#define AHB2_BASE 			0x50000000U
#define AHB3_BASE 			0x60000000U

//AHB1 Peripherals

#define GPIOA_BASE 	AHB1_BASE
#define GPIOB_BASE 	(AHB1_BASE + 0x0400)
#define GPIOC_BASE 	(AHB1_BASE + 0x0800)
#define GPIOD_BASE 	(AHB1_BASE + 0x0C00)
#define GPIOE_BASE 	(AHB1_BASE + 0x1000)
#define GPIOF_BASE 	(AHB1_BASE + 0x1400)
#define GPIOG_BASE 	(AHB1_BASE + 0x1800)
#define GPIOH_BASE 	(AHB1_BASE + 0x1C00)

#define CRC_BASE 	(AHB1_BASE + 0x3000)
#define RCC_BASE	(AHB1_BASE + 0x3800)


//APB2 Peripherals

#define TIM1_BASE 		APB2_BASE
#define TIM8_BASE		(APB2_BASE + 0x0400)

#define USART1_BASE		(APB2_BASE + 0x1000)
#define USART6_BASE 	(APB2_BASE + 0x1400)

#define SDIO_BASE 		(APB2_BASE + 0x2C00)

#define SPI1_BASE 		(APB2_BASE + 0x3000)
#define SPI4_BASE 		(APB2_BASE + 0x3400)

#define SYSCFG_BASE 	(APB2_BASE + 0x3800)
#define EXTI_BASE 		(APB2_BASE + 0x3C00)

#define TIM9_BASE 		(APB2_BASE + 0x4000)
#define TIM10_BASE		(APB2_BASE + 0x4400)
#define TIM11_BASE		(APB2_BASE + 0x4800)

#define SAI1_BASE		(APB2_BASE + 0x5800)
#define SAI2_BASE		(APB2_BASE + 0x5C00)

//APB1 Peripherals

#define TIM2_BASE 		APB1_BASE
#define TIM3_BASE		(APB1_BASE + 0x0400)
#define TIM4_BASE		(APB1_BASE + 0x0800)
#define TIM5_BASE		(APB1_BASE + 0x0C00)
#define TIM6_BASE		(APB1_BASE + 0x1000)
#define TIM7_BASE 		(APB1_BASE + 0x1400)
#define TIM12_BASE		(APB1_BASE + 0x1800)
#define TIM13_BASE		(APB1_BASE + 0x1C00)
#define TIM14_BASE		(APB1_BASE + 0x2000)

#define WWDG_BASE		(APB1_BASE + 0x2C00)
#define IWDG_BASE		(APB1_BASE + 0x3000)

#define SPI2_BASE 		(APB1_BASE + 0x3800)
#define SPI3_BASE		(APB1_BASE + 0x3C00)

#define I2S2_BASE 		SPI2_BASE
#define I2S3_BASE		SPI3_BASE

#define SPDIFRX_BASE	(APB1_BASE + 0x4000)

#define USART2_BASE		(APB1_BASE + 0x4400)
#define USART3_BASE		(APB1_BASE + 0x4800)
#define UART4_BASE		(APB1_BASE + 0x4C00)
#define UART5_BASE		(APB1_BASE + 0x5000)

#define I2C1_BASE		(APB1_BASE + 0x5400)
#define I2C2_BASE		(APB1_BASE + 0x5800)
#define I2C3_BASE		(APB1_BASE + 0x5C00)

#define FMPI2C1_BASE	(APB1_BASE + 0x6000)

#define CAN1_BASE		(APB1_BASE + 0x6400)
#define CAN2_BASE		(APB1_BASE + 0x6800)


#define HDMI_CEC 		(APB1_BASE + 0x6C00)
#define PWR_BASE		(APB1_BASE + 0x7000)
#define DAC_BASE		(APB1_BASE + 0x7400)


typedef struct
{
	_vo uint32_t MODER;
	_vo uint32_t OTYPER;
	_vo uint32_t OSPEEDR;
	_vo uint32_t PUPDR;
	_vo uint32_t IDR;
	_vo uint32_t ODR;
	_vo uint32_t BSRR;
	_vo uint32_t LCKR;
	_vo uint32_t AFR[2];
}GPIO_RegDef_t;

typedef struct
{
	_vo uint32_t CR;
	_vo uint32_t PLLCFGR;
	_vo uint32_t CFGR;
	_vo uint32_t CIR;
	_vo uint32_t AHB1RSTR;
	_vo uint32_t AHB2RSTR;
	_vo uint32_t AHB3RSTR;
	uint32_t RESERVED0;
	_vo uint32_t APB1RSTR;
	_vo uint32_t APB2RSTR;
	uint32_t RESERVER1[2];
	_vo uint32_t AHB1ENR;
	_vo uint32_t AHB2ENR;
	_vo uint32_t AHB3ENR;
	uint32_t RESERVED2;
	_vo uint32_t APB1ENR;
	_vo uint32_t APB2ENR;
	uint32_t RESERVED3[2];
	_vo uint32_t AHB1LPENR;
	_vo uint32_t AHB2LPENR;
	_vo uint32_t AHB3LPENR;
	uint32_t RESERVED4;
	_vo uint32_t APB1LPENR;
	_vo uint32_t APB2LPENR;
	uint32_t RESERVED5[2];
	_vo uint32_t BDCR;
	_vo uint32_t CSR;
	uint32_t RESERVED6[2];
	_vo uint32_t SSCGR;
	_vo uint32_t PLLI2SCFGR;
	_vo uint32_t PLLSAICFGR;
	_vo uint32_t DCKCFGR;
	_vo uint32_t CKGATENR;
	_vo uint32_t DCKCFGR2;
}RCC_RegDef_t;

typedef struct
{
	_vo uint32_t MEMRMP;
	_vo uint32_t PMC;
	_vo uint32_t EXTICR[4];
	uint32_t 	 RESERVED0[2];
	_vo uint32_t CMPCR;
	uint32_t 	 RESERVED1[2];
	_vo uint32_t CFGR;
}SYSCFG_RegDef_t;

typedef struct
{
	_vo uint32_t IMR;
	_vo uint32_t EMR;
	_vo uint32_t RTSR;
	_vo uint32_t FTSR;
	_vo uint32_t SWIER;
	_vo uint32_t PR;
}EXTI_RegDef_t;

typedef struct
{
	_vo uint32_t CR1;
	_vo uint32_t CR2;
	_vo uint32_t SR;
	_vo uint32_t DR;
	_vo uint32_t CRCPR;
	_vo uint32_t RXCRCR;
	_vo uint32_t TXCRCR;
	_vo uint32_t I2SCFGR;
	_vo uint32_t I2SPR;
}SPI_RegDef_t;

typedef struct
{
	_vo uint32_t SR;
	_vo uint32_t DR;
	_vo uint32_t BRR;
	_vo uint32_t CR1;
	_vo uint32_t CR2;
	_vo uint32_t CR3;
	_vo uint32_t GTPR;
}USART_RegDef_t;

typedef struct
{
    volatile uint32_t CR;
    volatile uint32_t SWTRIGR;
    volatile uint32_t DHR12R1;
    volatile uint32_t DHR12L1;
    volatile uint32_t DHR8R1;
    volatile uint32_t DHR12R2;
    volatile uint32_t DHR12L2;
    volatile uint32_t DHR8R2;
    volatile uint32_t DHR12RD;
    volatile uint32_t DHR12LD;
    volatile uint32_t DHR8RD;
    volatile uint32_t DOR1;
    volatile uint32_t DOR2;
    volatile uint32_t SR;
} DAC_RegDef_t;

#define GPIOA	((GPIO_RegDef_t*)GPIOA_BASE)
#define GPIOB	((GPIO_RegDef_t*)GPIOB_BASE)
#define GPIOC	((GPIO_RegDef_t*)GPIOC_BASE)
#define GPIOD	((GPIO_RegDef_t*)GPIOD_BASE)
#define GPIOE	((GPIO_RegDef_t*)GPIOE_BASE)
#define GPIOF	((GPIO_RegDef_t*)GPIOF_BASE)
#define GPIOG	((GPIO_RegDef_t*)GPIOG_BASE)
#define GPIOH	((GPIO_RegDef_t*)GPIOH_BASE)

#define RCC 	((RCC_RegDef_t*)RCC_BASE)
#define EXTI	((EXTI_RegDef_t*)EXTI_BASE)
#define SYSCFG	((SYSCFG_RegDef_t*)SYSCFG_BASE)

#define SPI1	((SPI_RegDef_t*)SPI1_BASE)
#define SPI2	((SPI_RegDef_t*)SPI2_BASE)
#define SPI3	((SPI_RegDef_t*)SPI3_BASE)
#define SPI4	((SPI_RegDef_t*)SPI4_BASE)

#define USART1 	((USART_RegDef_t*)USART1_BASE)
#define USART2	((USART_RegDef_t*)USART2_BASE)
#define USART3	((USART_RegDef_t*)USART3_BASE)
#define UART4	((USART_RegDef_t*)UART4_BASE)
#define UART5	((USART_RegDef_t*)UART5_BASE)
#define USART6	((USART_RegDef_t*)USART6_BASE)

#define DAC     ((DAC_RegDef_t *)DAC_BASE)
//Macros

#define GPIOA_PCLK_EN()	(RCC->AHB1ENR |= (1<<0))
#define GPIOB_PCLK_EN()	(RCC->AHB1ENR |= (1<<1))
#define GPIOC_PCLK_EN()	(RCC->AHB1ENR |= (1<<2))
#define GPIOD_PCLK_EN()	(RCC->AHB1ENR |= (1<<3))
#define GPIOE_PCLK_EN()	(RCC->AHB1ENR |= (1<<4))
#define GPIOF_PCLK_EN()	(RCC->AHB1ENR |= (1<<5))
#define GPIOG_PCLK_EN()	(RCC->AHB1ENR |= (1<<6))
#define GPIOH_PCLK_EN()	(RCC->AHB1ENR |= (1<<7))

#define GPIOA_PCLK_DI()	(RCC->AHB1ENR &= ~(1<<0))
#define GPIOB_PCLK_DI()	(RCC->AHB1ENR &= ~(1<<1))
#define GPIOC_PCLK_DI()	(RCC->AHB1ENR &= ~(1<<2))
#define GPIOD_PCLK_DI()	(RCC->AHB1ENR &= ~(1<<3))
#define GPIOE_PCLK_DI()	(RCC->AHB1ENR &= ~(1<<4))
#define GPIOF_PCLK_DI()	(RCC->AHB1ENR &= ~(1<<5))
#define GPIOG_PCLK_DI()	(RCC->AHB1ENR &= ~(1<<6))
#define GPIOH_PCLK_DI()	(RCC->AHB1ENR &= ~(1<<7))

#define SPI1_PCLK_EN() 	(RCC->APB2ENR |= (1<<12))
#define SPI2_PCLK_EN()	(RCC->APB1ENR |= (1<<14))
#define SPI3_PCLK_EN()	(RCC->APB1ENR |= (1<<15))
#define SPI4_PCLK_EN()	(RCC->APB2ENR |= (1<<13))

#define SPI1_PCLK_DI() 	(RCC->APB2ENR &= ~(1<<12))
#define SPI2_PCLK_DI()	(RCC->APB1ENR &= ~(1<<14))
#define SPI3_PCLK_DI()	(RCC->APB1ENR &= ~(1<<15))
#define SPI4_PCLK_DI()	(RCC->APB2ENR &= ~(1<<13))

#define USART1_PCLK_EN()	(RCC->APB2ENR |= (1<<4))
#define USART2_PCLK_EN()	(RCC->APB1ENR |= (1<<17))
#define USART3_PCLK_EN()	(RCC->APB1ENR |= (1<<18))
#define UART4_PCLK_EN()		(RCC->APB1ENR |= (1<<19))
#define UART5_PCLK_EN()		(RCC->APB1ENR |= (1<<20))
#define USART6_PCLK_EN()	(RCC->APB2ENR |= (1<<5))

#define USART1_PCLK_DI()	(RCC->APB2ENR &= ~(1 << 4))
#define USART2_PCLK_DI()	(RCC->APB1ENR &= ~(1 << 17))
#define USART3_PCLK_DI()	(RCC->APB1ENR &= ~(1 << 18))
#define UART4_PCLK_DI()		(RCC->APB1ENR &= ~(1 << 19))
#define UART5_PCLK_DI()		(RCC->APB1ENR &= ~(1 << 20))
#define USART6_PCLK_DI()	(RCC->APB2ENR &= ~(1 << 5))

#define DAC_PCLK_EN()		(RCC->APB1ENR |= (1U << 29U))

#define SYSCFG_PCLK_EN()	(RCC->APB2ENR |= (1<<14))

#define GPIOA_RST()		do{ (RCC->AHB1RSTR |= (1<<0)); (RCC->AHB1RSTR &= ~(1<<0)); }while(0)
#define GPIOB_RST()		do{ (RCC->AHB1RSTR |= (1<<1)); (RCC->AHB1RSTR &= ~(1<<1)); }while(0)
#define GPIOC_RST()		do{ (RCC->AHB1RSTR |= (1<<2)); (RCC->AHB1RSTR &= ~(1<<2)); }while(0)
#define GPIOD_RST()		do{ (RCC->AHB1RSTR |= (1<<3)); (RCC->AHB1RSTR &= ~(1<<3)); }while(0)
#define GPIOE_RST()		do{ (RCC->AHB1RSTR |= (1<<4)); (RCC->AHB1RSTR &= ~(1<<4)); }while(0)
#define GPIOF_RST()		do{ (RCC->AHB1RSTR |= (1<<5)); (RCC->AHB1RSTR &= ~(1<<5)); }while(0)
#define GPIOG_RST()		do{ (RCC->AHB1RSTR |= (1<<6)); (RCC->AHB1RSTR &= ~(1<<6)); }while(0)
#define GPIOH_RST()		do{ (RCC->AHB1RSTR |= (1<<7)); (RCC->AHB1RSTR &= ~(1<<7)); }while(0)

#define SPI1_RST()		do{	(RCC->APB2RSTR |= (1<<12)); (RCC->APB2RSTR &= ~(1<<12));}while(0)
#define SPI2_RST() 		do{	(RCC->APB1RSTR |= (1<<14)); (RCC->APB1RSTR &= ~(1<<14));}while(0)
#define SPI3_RST() 		do{	(RCC->APB1RSTR |= (1<<15)); (RCC->APB1RSTR &= ~(1<<15));}while(0)
#define SPI4_RST()		do{	(RCC->APB2RSTR |= (1<<13)); (RCC->APB2RSTR &= ~(1<<13));}while(0)

#define USART1_RST()    do{ (RCC->APB2RSTR |= (1<<4));	(RCC->APB2RSTR &= ~(1<<4)); }while(0)
#define USART2_RST()	do{ (RCC->APB1RSTR |= (1<<17)); (RCC->APB1RSTR &= ~(1<<17));}while(0)
#define USART3_RST()    do{ (RCC->APB1RSTR |= (1<<18)); (RCC->APB1RSTR &= ~(1<<18));}while(0)
#define UART4_RST()		do{ (RCC->APB1RSTR |= (1<<19)); (RCC->APB1RSTR &= ~(1<<19));}while(0)
#define UART5_RST()		do{ (RCC->APB1RSTR |= (1<<20)); (RCC->APB1RSTR &= ~(1<<20));}while(0)
#define USART6_RST()    do{ (RCC->APB2RSTR |= (1<<5));	(RCC->APB2RSTR &= ~(1<<5)); }while(0)

#define GPIO_BASEADDR_TO_CODE(x) 	((x == GPIOA)?0 :\
									 (x == GPIOB)?1 :\
									 (x == GPIOC)?2 :\
									 (x == GPIOD)?3 :\
									 (x == GPIOE)?4 :\
									 (x == GPIOF)?5 :\
									 (x == GPIOG)?6 :\
									 (x == GPIOH)?7 :0 )

#define IRQ_NO_EXTI0		6
#define IRQ_NO_EXTI1		7
#define IRQ_NO_EXTI2		8
#define IRQ_NO_EXTI3		9
#define IRQ_NO_EXTI4		10
#define IRQ_NO_EXTI9_5		23
#define IRQ_NO_EXTI15_10 	40

#define ENABLE 			1
#define DISABLE 		0
#define SET 			ENABLE
#define RESET 			DISABLE
#define GPIO_PIN_SET 	SET
#define GPIO_PIN_RESET 	RESET
#define FLAG_SET		SET
#define FLAG_RESET		RESET

#define SPI_CR1_CPHA		0
#define SPI_CR1_CPOL		1
#define SPI_CR1_MSTR		2
#define SPI_CR1_BR			3
#define SPI_CR1_SPE			6
#define SPI_CR1_LSBFIRST	7
#define	SPI_CR1_SSI			8
#define	SPI_CR1_SSM			9
#define	SPI_CR1_RXONLY		10
#define	SPI_CR1_DFF			11
#define	SPI_CR1_CRCNEXT		12
#define	SPI_CR1_CRCEN		13
#define	SPI_CR1_BIDIOE		14
#define	SPI_CR1_BIDIMODE	15

#define SPI_CR2_RXDMAEN 	0
#define SPI_CR2_TXDMAEN		1
#define SPI_CR2_SSOE		2
#define SPI_CR2_FRF			4
#define SPI_CR2_ERRIE		5
#define SPI_CR2_RXNEIE		6
#define SPI_CR2_TXEIE		7

#define SPI_SR_RXNE			0
#define SPI_SR_TXE			1
#define SPI_SR_CHSIDE		2
#define SPI_SR_UDR			3
#define SPI_SR_CRCERR		4
#define SPI_SR_MODF			5
#define SPI_SR_OVR			6
#define SPI_SR_BSY			7
#define SPI_SR_FRE			8

#define USART_SR_PE			0
#define USART_SR_FE			1
#define USART_SR_NF			2
#define USART_SR_ORE		3
#define USART_SR_IDLE		4
#define USART_SR_RXNE		5
#define USART_SR_TC			6
#define USART_SR_TXE		7
#define USART_SR_LBD		8
#define USART_SR_CTS		9

#define USART_CR1_SBK 		0
#define USART_CR1_RWU 		1
#define USART_CR1_RE 		2
#define USART_CR1_TE 		3
#define USART_CR1_IDLEIE 	4
#define USART_CR1_RXNEIE 	5
#define USART_CR1_TCIE 		6
#define USART_CR1_TXEIE 	7
#define USART_CR1_PEIE 		8
#define USART_CR1_PS 		9
#define USART_CR1_PCE 		10
#define USART_CR1_WAKE 		11
#define USART_CR1_M			12
#define USART_CR1_UE 		13
#define USART_CR1_OVER8 	15

#define USART_CR2_ADD 		0
#define USART_CR2_LBDL 		5
#define USART_CR2_LBDIE 	6
#define USART_CR2_LBCL 		8
#define USART_CR2_CPHA 		9
#define USART_CR2_CPOL 		10
#define USART_CR2_CLKEN 	11
#define USART_CR2_STOP 		12
#define USART_CR2_LINEN 	14

#define USART_CR3_EIE		0
#define USART_CR3_IREN		1
#define USART_CR3_IRLP		2
#define USART_CR3_HDSEL		3
#define USART_CR3_NACK		4
#define USART_CR3_SCEN		5
#define USART_CR3_DMAR		6
#define USART_CR3_DMAT		7
#define USART_CR3_RTSE		8
#define USART_CR3_CTSE		9
#define USART_CR3_CTSIE		10
#define USART_CR3_ONEBIT	11



#endif /* STM32_F446RE_H_ */
