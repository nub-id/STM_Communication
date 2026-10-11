/*
 * STM32f446re_GPIO.c
 *
 *  Created on: May 14, 2026
 *      Author: User
 */

#include "STM32f446re_GPIO.h"

void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOxAddr, uint8_t EnDis)
{
	if(EnDis == ENABLE)
	{
		if(pGPIOxAddr == GPIOA)
					GPIOA_PCLK_EN();
		else if(pGPIOxAddr == GPIOB)
					GPIOB_PCLK_EN();
		else if(pGPIOxAddr == GPIOC)
					GPIOC_PCLK_EN();
		else if(pGPIOxAddr == GPIOD)
					GPIOD_PCLK_EN();
		else if(pGPIOxAddr == GPIOE)
					GPIOE_PCLK_EN();
		else if(pGPIOxAddr == GPIOF)
					GPIOF_PCLK_EN();
		else if(pGPIOxAddr == GPIOG)
					GPIOG_PCLK_EN();
		else if(pGPIOxAddr == GPIOH)
					GPIOH_PCLK_EN();
	}
	else
	{
		if(pGPIOxAddr == GPIOA)
						GPIOA_PCLK_DI();
		else if(pGPIOxAddr == GPIOB)
						GPIOB_PCLK_DI();
		else if(pGPIOxAddr == GPIOC)
						GPIOC_PCLK_DI();
		else if(pGPIOxAddr == GPIOD)
						GPIOD_PCLK_DI();
		else if(pGPIOxAddr == GPIOE)
						GPIOE_PCLK_DI();
		else if(pGPIOxAddr == GPIOF)
						GPIOF_PCLK_DI();
		else if(pGPIOxAddr == GPIOG)
						GPIOG_PCLK_DI();
		else if(pGPIOxAddr == GPIOH)
						GPIOH_PCLK_DI();
	}
}

void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
	GPIO_PeriClockControl(pGPIOHandle->pGPIOxAddr, ENABLE);

	uint32_t temp = 0;
	const uint32_t pos = 2U * pGPIOHandle->GPIO_Config.GPIO_Pin_Number;
	const uint32_t mask = (0x3U << pos);

	if(pGPIOHandle->GPIO_Config.GPIO_PinMode <= GPIO_MODE_ANALOG)
	{
		pGPIOHandle->pGPIOxAddr->MODER &= ~mask;
		temp = (pGPIOHandle->GPIO_Config.GPIO_PinMode << pos);
		pGPIOHandle->pGPIOxAddr->MODER |= temp;
		temp = 0;
	}
	else
	{
		pGPIOHandle->pGPIOxAddr->MODER &= ~mask;

		if(pGPIOHandle->GPIO_Config.GPIO_PinMode == GPIO_MODE_IT_FT)
		{
			EXTI->FTSR |= (0x1U << pGPIOHandle->GPIO_Config.GPIO_Pin_Number);
			EXTI->RTSR &= ~(0x1U << pGPIOHandle->GPIO_Config.GPIO_Pin_Number);
		}
		else if(pGPIOHandle->GPIO_Config.GPIO_PinMode == GPIO_MODE_IT_RT)
		{
			EXTI->RTSR |= (0x1U << pGPIOHandle->GPIO_Config.GPIO_Pin_Number);
			EXTI->FTSR &= ~(0x1U << pGPIOHandle->GPIO_Config.GPIO_Pin_Number);
		}
		else if(pGPIOHandle->GPIO_Config.GPIO_PinMode == GPIO_MODE_IT_RFT)
		{
			EXTI->RTSR |= (0x1U << pGPIOHandle->GPIO_Config.GPIO_Pin_Number);
			EXTI->FTSR |= (0x1U << pGPIOHandle->GPIO_Config.GPIO_Pin_Number);
		}

		SYSCFG_PCLK_EN();

		uint8_t reg_idx, shift;
		reg_idx = pGPIOHandle->GPIO_Config.GPIO_Pin_Number / 4U;
		shift = 4U * (pGPIOHandle->GPIO_Config.GPIO_Pin_Number % 4U);
		uint8_t portcode = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOxAddr);

		SYSCFG->EXTICR[reg_idx] &= ~(0xFU << shift);
		SYSCFG->EXTICR[reg_idx] |= (portcode << shift);

		EXTI->IMR |= (0x1U << pGPIOHandle->GPIO_Config.GPIO_Pin_Number);
	}

	pGPIOHandle->pGPIOxAddr->OSPEEDR &= ~mask;
	temp = pGPIOHandle->GPIO_Config.GPIO_PinSpeed << pos;
	pGPIOHandle->pGPIOxAddr->OSPEEDR |= temp;
	temp = 0;

	pGPIOHandle->pGPIOxAddr->PUPDR &= ~mask;
	temp = pGPIOHandle->GPIO_Config.GPIO_PUPDControl << pos;
	pGPIOHandle->pGPIOxAddr->PUPDR |= temp;
	temp = 0;

	pGPIOHandle->pGPIOxAddr->OTYPER &= ~(1U << pGPIOHandle->GPIO_Config.GPIO_Pin_Number);
	pGPIOHandle->pGPIOxAddr->OTYPER |= (pGPIOHandle->GPIO_Config.GPIO_PinOPType << pGPIOHandle->GPIO_Config.GPIO_Pin_Number);

	if(pGPIOHandle->GPIO_Config.GPIO_PinMode == GPIO_MODE_ALTFN)
	{
		uint8_t t1, t2;
		t1 = pGPIOHandle->GPIO_Config.GPIO_Pin_Number / 8;
		t2 = 4U * (pGPIOHandle->GPIO_Config.GPIO_Pin_Number % 8);
		pGPIOHandle->pGPIOxAddr->AFR[t1] &= ~(0xFU << t2);
		pGPIOHandle->pGPIOxAddr->AFR[t1] |= (pGPIOHandle->GPIO_Config.GPIO_PinAltFuncMode << t2);
	}

}
void GPIO_DeInit(GPIO_RegDef_t *pGPIOxAddr)
{
	if(pGPIOxAddr == GPIOA)
				GPIOA_RST();
	else if(pGPIOxAddr == GPIOB)
				GPIOB_RST();
	else if(pGPIOxAddr == GPIOC)
				GPIOC_RST();
	else if(pGPIOxAddr == GPIOD)
				GPIOD_RST();
	else if(pGPIOxAddr == GPIOE)
				GPIOE_RST();
	else if(pGPIOxAddr == GPIOF)
				GPIOF_RST();
	else if(pGPIOxAddr == GPIOG)
				GPIOG_RST();
	else if(pGPIOxAddr == GPIOH)
				GPIOH_RST();
}

uint8_t GPIO_ReadFromPin(GPIO_RegDef_t *pGPIOxAddr, uint8_t PinNumber)
{
	uint8_t value;
	value = (uint8_t)((pGPIOxAddr->IDR >> PinNumber) & 0x1U);
	return value;
}

uint16_t GPIO_ReadFromPort(GPIO_RegDef_t *pGPIOxAddr)
{
	uint16_t value;
	value = (uint16_t)(pGPIOxAddr->IDR);
	return value;
}

void GPIO_WriteToPin(GPIO_RegDef_t *pGPIOxAddr, uint8_t PinNumber, uint8_t Value)
{
	if(Value == GPIO_PIN_SET)
	{
		pGPIOxAddr->ODR |= (0x1U << PinNumber);
	}
	else
	{
		pGPIOxAddr->ODR &= ~(0x1U << PinNumber);
	}
}

void GPIO_WriteToPort(GPIO_RegDef_t *pGPIOxAddr, uint16_t Value)
{
	 pGPIOxAddr->ODR = Value;
}

void GPIO_TogglePin(GPIO_RegDef_t *pGPIOxAddr, uint8_t PinNumber)
{
	pGPIOxAddr->ODR ^= (1 << PinNumber);
}

void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t EnDis){
	if(EnDis == ENABLE){
		if(IRQNumber <=31){
			*NVIC_ISER0 = (1U << IRQNumber);
		}
		else if(IRQNumber > 31 && IRQNumber < 64){
			*NVIC_ISER1 = (1U << IRQNumber%32);
		}
		else if(IRQNumber >=64 && IRQNumber < 96){
			*NVIC_ISER2 = (1U << IRQNumber%32);
		}
		else if(IRQNumber >= 96){
			*NVIC_ISER3 = (1U << IRQNumber%32);
		}
	}
	else{
		if(IRQNumber <=31){
			*NVIC_ICER0 = (1U << IRQNumber);
		}
		else if(IRQNumber > 31 && IRQNumber < 64){
			*NVIC_ICER1 = (1U << IRQNumber%32);
		}
		else if(IRQNumber >=64 && IRQNumber < 96){
			*NVIC_ICER2 = (1U << IRQNumber%32);
		}
		else if(IRQNumber >= 96){
			*NVIC_ICER3 = (1U << IRQNumber%32);
		}
	}
}

void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority){
	uint8_t iprx = IRQNumber / 4U;
	uint8_t iprx_section = IRQNumber % 4U;
	uint8_t shift_amount = (8U * iprx_section) + (8U - NO_PR_BITS_IMPLEMENTED);

	*(NVIC_IPR_BASE + iprx) &= ~(0xFFU << (8U * iprx_section));
	*(NVIC_IPR_BASE + (iprx)) |= ((uint32_t)(IRQPriority & 0x0FU) << shift_amount);
}

void GPIO_IRQHandle(uint8_t PinNumber){
	if(EXTI->PR & (1U<<PinNumber)){
		EXTI->PR = (1U<<PinNumber);
	}
}

void DAC_WriteToPin(GPIO_RegDef_t *pGPIOxAddr, uint8_t PinNumber, uint16_t Value){
    if ((pGPIOxAddr != GPIOA) ||
        ((PinNumber != GPIO_PIN_NO_4) &&
         (PinNumber != GPIO_PIN_NO_5)))
    {
        return;
    }

    if ((RCC->AHB1ENR & (1U << 0U)) == 0U)
    {
        return;
    }

    uint32_t pin_shift = 2U * PinNumber;

    if (((pGPIOxAddr->MODER >> pin_shift) & 3U) != GPIO_MODE_ANALOG)
    {
        return;
    }

    if (((pGPIOxAddr->PUPDR >> pin_shift) & 3U) != GPIO_NO_PUPD)
    {
        return;
    }

    if (Value > 4095U){
        Value = 4095U;
    }

    uint32_t channel_shift = (PinNumber == GPIO_PIN_NO_4) ? 0U : 16U;

    /* Enable DAC peripheral clock */
    DAC_PCLK_EN();
    (void)RCC->APB1ENR;

    /* Buffer enabled;
       trigger, wave generation and DMA disabled */
    DAC->CR &= ~(((1U << 1U)  |
                  (1U << 2U)  |
                  (3U << 6U)  |
                  (1U << 12U)) << channel_shift);

    if (PinNumber == GPIO_PIN_NO_4)
    {
        DAC->DHR12R1 = Value;
    }
    else
    {
        DAC->DHR12R2 = Value;
    }

    /* Enable DAC channel */
    DAC->CR |= (1U << channel_shift);
}


