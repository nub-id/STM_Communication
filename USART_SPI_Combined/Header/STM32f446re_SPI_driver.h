/*
 * SPI.h
 *
 *  Created on: May 9, 2026
 *      Author: User
 */
#include "STM32_f446re.h"

#ifndef STM32F446RE_SPI_DRIVER_H_
#define STM32F446RE_SPI_DRIVER_H_

#define SPI_DEVICE_MODE_SLAVE 	0
#define SPI_DEVICE_MODE_MASTER	1

#define SPI_BUS_CONFIG_FD				1
#define SPI_BUS_CONFIG_HD				2
#define SPI_BUS_CONFIG_SIMPLEX_RX	 	3

#define SPI_SCLK_SPEED_DIV2			0
#define SPI_SCLK_SPEED_DIV4			1
#define SPI_SCLK_SPEED_DIV8			2
#define SPI_SCLK_SPEED_DIV16		3
#define SPI_SCLK_SPEED_DIV32		4
#define SPI_SCLK_SPEED_DIV64		5
#define SPI_SCLK_SPEED_DIV128		6
#define SPI_SCLK_SPEED_DIV256		7

#define SPI_DFF_8BITS 	0
#define SPI_DFF_16BITS 	1

#define SPI_CPOL_LOW	0
#define SPI_CPOL_HIGH	1

#define SPI_CPHA_LOW	0
#define SPI_CPHA_HIGH	1

#define SPI_SSM_DI		0
#define SPI_SSM_EN		1

#define SPI_RXNE_FLAG	(1U<<SPI_SR_RXNE)
#define SPI_TXE_FLAG	(1U<<SPI_SR_TXE)
#define SPI_CHSIDE_FLAG	(1U<<SPI_SR_CHSIDE)
#define SPI_UDR_FLAG	(1U<<SPI_SR_UDR)
#define SPI_CRCERR_FLAG	(1U<<SPI_SR_CRCERR)
#define SPI_MODF_FLAG 	(1U<<SPI_SR_MODF)
#define SPI_OVR_FLAG	(1U<<SPI_SR_OVR)
#define SPI_BSY_FLAG	(1U<<SPI_SR_BSY)
#define SPI_FRE_FLAG	(1U<<SPI_SR_FRE)

typedef struct
{
	uint8_t SPI_DeviceMode;
	uint8_t SPI_BusConfig;
	uint8_t SPI_DFF;
	uint8_t SPI_CPHA;
	uint8_t SPI_CPOL;
	uint8_t SPI_SSM;
	uint8_t SPI_Speed;
}SPI_Config_t;

typedef struct
{
	SPI_RegDef_t *pSPIx;
	SPI_Config_t SPIConfig;
}SPI_Handle_t;

void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnDis);

void SPI_Init(SPI_Handle_t *pSPIHandle);
void SPI_DeInit(SPI_RegDef_t *pSPIx);

void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len);
void SPI_RecieveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len);

void SPI_IRQConfig(uint8_t IRQNumber, uint8_t EnDis);
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority);
void SPI_IRQHandle(SPI_Handle_t *pSPIHandle);

void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnDis);
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnDis);
void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnDis);

uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName);
#endif /* STM32F446RE_SPI_DRIVER_H_ */
