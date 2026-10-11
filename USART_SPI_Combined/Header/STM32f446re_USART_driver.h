#ifndef STM32F446RE_USART_DRIVER_H
#define STM32F446RE_USART_DRIVER_H

#include "STM32_f446re.h"

#define USART_MODE_TxOnly 	0
#define USART_MODE_RxOnly 	1
#define USART_MODE_TxRx		2

#define USART_PARITY_EN_ODD		2
#define USART_PARITY_EN_EVEN	1
#define USART_PARITY_DIS		0

#define USART_WORDLEN_8BITS		0
#define USART_WORDLEN_9BITS		1

#define USART_STOPBITS_1	0U
#define USART_STOPBITS_0_5	1U
#define USART_STOPBITS_2	2U
#define USART_STOPBITS_1_5	3U

#define USART_BAUD_1200			1200
#define USART_BAUD_2400			2400
#define USART_BAUD_9600			9600
#define USART_BAUD_19200		19200
#define USART_BAUD_38400		38400
#define USART_BAUD_57600		57600
#define USART_BAUD_115200		115200
#define USART_BAUD_230400		230400
#define USART_BAUD_460800		460800
#define USART_BAUD_921600		921600
#define USART_BAUD_2M			2000000
#define USART_BAUD_3M			3000000

#define USART_HWFLOW_CTRL_NONE		0
#define USART_HWFLOW_CTRL_CTS		1
#define USART_HWFLOW_CTRL_RTS		3
#define USART_HWFLOW_CTRL_CTSRTS	4

#define USART_OVERSAMPLING_BY16	0
#define USART_OVERSAMPLING_BY8	1

#define USART_PE_FLAG       (1U << USART_SR_PE)
#define USART_FE_FLAG       (1U << USART_SR_FE)
#define USART_NF_FLAG       (1U << USART_SR_NF)
#define USART_ORE_FLAG      (1U << USART_SR_ORE)
#define USART_IDLE_FLAG     (1U << USART_SR_IDLE)
#define USART_RXNE_FLAG     (1U << USART_SR_RXNE)
#define USART_TC_FLAG       (1U << USART_SR_TC)
#define USART_TXE_FLAG      (1U << USART_SR_TXE)
#define USART_LBD_FLAG      (1U << USART_SR_LBD)
#define USART_CTS_FLAG      (1U << USART_SR_CTS)

typedef struct{
	uint8_t USART_Mode;
	uint32_t USART_Baud;
	uint8_t USART_NoOfStopBits;
	uint8_t USART_WordLength;
	uint8_t USART_ParityControl;
	uint8_t USART_HWFlowControl;
	uint8_t	USART_OverSampling;
}USART_Config_t;

typedef struct{
	USART_RegDef_t *pUSARTx;
	USART_Config_t USART_Config;
}USART_Handle_t;

void USART_PeriClockControl(USART_RegDef_t *pUSARTx, uint8_t EnDis);

void USART_Init(USART_Handle_t *pUSARTHandle);
void USART_DeInit(USART_RegDef_t *pUSARTx);

void USART_SendData(USART_RegDef_t *pUSARTx, uint8_t *pTxBuffer, uint32_t Len);
void USART_ReceiveData(USART_RegDef_t *pUSARTx, uint8_t *pTxBuffer, uint32_t Len);
void USART_SendData_IT(USART_RegDef_t *pUSARTx, uint8_t *pTxBuffer, uint32_t Len);
void USART_ReceiveData_IT(USART_RegDef_t *pUSARTx, uint8_t *pTxBuffer, uint32_t Len);

void USART_IRQConfig(uint32_t IRQNumber, uint8_t EnDis);
void USART_IRQPriorityConfig(uint32_t IRQNumber, uint32_t IRQPriority);
void USART_IRQHandling(USART_Handle_t *pUSARTHandle);

void USART_PeripheralControl(USART_RegDef_t *pUSARTx, uint8_t EnDis);
uint8_t USART_GetFlagStatus(USART_RegDef_t *pUSARTx, uint32_t FlagName);
void USART_ClearFlag(USART_RegDef_t *pUSARTx, uint32_t FlagName);

void USART_ApplicationEventCallback(USART_Handle_t *pUSARTHandle, uint8_t AppEv);

#endif
