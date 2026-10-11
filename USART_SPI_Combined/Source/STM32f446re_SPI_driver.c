/*
 * SPI.c
 *
 *  Created on: May 9, 2026
 *      Author: Shams-E-Sheefat Nabid
 */

#include "STM32_f446re.h"
#include "STM32f446re_SPI_driver.h"

void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnDis){
	if(EnDis == ENABLE){
		if(pSPIx == SPI1){
			SPI1_PCLK_EN();
		}
		else if(pSPIx == SPI2){
			SPI2_PCLK_EN();
		}
		else if(pSPIx == SPI3){
			SPI3_PCLK_EN();
		}
		else if(pSPIx == SPI4){
			SPI4_PCLK_EN();
		}
	}
	else{
		if(pSPIx == SPI1){
			SPI1_PCLK_DI();
		}
		else if(pSPIx == SPI2){
			SPI2_PCLK_DI();
		}
		else if(pSPIx == SPI3){
			SPI3_PCLK_DI();
		}
		else if(pSPIx == SPI4){
			SPI4_PCLK_DI();
		}
	}
}

void SPI_Init(SPI_Handle_t *pSPIHandle){

	SPI_PeriClockControl(pSPIHandle->pSPIx, ENABLE);

	uint32_t tempreg =0;

	tempreg |= pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_CR1_MSTR;

	if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD){
		tempreg &= ~(1U<<SPI_CR1_BIDIMODE);
		tempreg &= ~(1U<<SPI_CR1_RXONLY);
	}
	else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD){
		tempreg |= (1U<<SPI_CR1_BIDIMODE);
	}
	else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RX){
		tempreg &= ~(1U<<SPI_CR1_BIDIMODE);

		tempreg |= (1U<<SPI_CR1_RXONLY);
	}

	tempreg |= pSPIHandle->SPIConfig.SPI_DFF << SPI_CR1_DFF;

	tempreg |= pSPIHandle->SPIConfig.SPI_Speed << SPI_CR1_BR;

	tempreg |= pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA;

	tempreg |= pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL;

	tempreg |= pSPIHandle->SPIConfig.SPI_SSM << SPI_CR1_SSM;

	pSPIHandle->pSPIx->CR1 = tempreg;

}

void SPI_DeInit(SPI_RegDef_t *pSPIx){
	if(pSPIx == SPI1){
		SPI1_RST();
	}
	else if(pSPIx == SPI2){
		SPI2_RST();
	}
	else if(pSPIx == SPI3){
		SPI3_RST();
	}
	else if(pSPIx == SPI4){
		SPI4_RST();
	}
}

void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len){
	if((pSPIx->CR1 & (1U << SPI_CR1_DFF)) && (Len & 1U)){
		return;
	}
	while(Len > 0){
		while(SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG) == FLAG_RESET);

		if(pSPIx->CR1 & (1U << SPI_CR1_DFF)){
			uint16_t word = *((uint16_t *)pTxBuffer);

			pSPIx->DR = word;
			pTxBuffer += 2U;
			Len -= 2U;
		}
		else{
			pSPIx->DR = *pTxBuffer;
			Len --;
			pTxBuffer++;
		}
	}

	while(!SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG));
	//while (!(pSPIx->SR & (1U << SPI_SR_TXE))) {}
	while(SPI_GetFlagStatus(pSPIx, SPI_BSY_FLAG));
	//while (pSPIx->SR & (1U << SPI_SR_BSY)) {}
}
void SPI_RecieveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len){
	if((pSPIx->CR1 & (1U << SPI_CR1_DFF)) && (Len & 1U)){
		return;
	}
	while(Len >0){
		while(SPI_GetFlagStatus(pSPIx, SPI_RXNE_FLAG) == FLAG_RESET);

		if(pSPIx->CR1 & (1U << SPI_CR1_DFF)){
			*((uint16_t *)pRxBuffer) = (uint16_t)pSPIx->DR;
			pRxBuffer += 2U;
			Len -= 2U;
		}
		else{
			*pRxBuffer = (uint8_t)pSPIx->DR;
			pRxBuffer ++;
			Len --;
		}
	}

	while(SPI_GetFlagStatus(pSPIx, SPI_BSY_FLAG));
	//while(pSPIx->SR & (1U << SPI_SR_BSY));
}

void SPI_IRQConfig(uint8_t IRQNumber, uint8_t EnDis);
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority);
void SPI_IRQHandle(SPI_Handle_t *pSPIHandle);

uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName){
	if(pSPIx->SR & FlagName){
		return FLAG_SET;
	}
	return FLAG_RESET;
}


void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnDis){
	if(EnDis == ENABLE){
		pSPIx->CR1 |= (1U<<SPI_CR1_SPE);
	}
	else{
		pSPIx->CR1 &= ~(1U<<SPI_CR1_SPE);
	}
}

void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnDis){
	if(EnDis == ENABLE){
		pSPIx->CR1 |= (1U<<SPI_CR1_SSI);
	}
	else{
		pSPIx->CR1 &= ~(1U<<SPI_CR1_SSI);
	}

}

void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnDis){
	if(EnDis == ENABLE){
		pSPIx->CR2 |= (1U<<SPI_CR2_SSOE);
	}
	else{
		pSPIx->CR2 &= ~(1U<<SPI_CR2_SSOE);
	}
}
