/*
 * STM32f446re_GPIO.h
 *
 *  Created on: May 14, 2026
 *      Author: User
 */
#include "STM32_f446re.h"

#ifndef STM32F446RE_GPIO_H_
#define STM32F446RE_GPIO_H_

#define GPIO_PIN_NO_0	0
#define GPIO_PIN_NO_1	1
#define GPIO_PIN_NO_2	2
#define GPIO_PIN_NO_3	3
#define GPIO_PIN_NO_4	4
#define GPIO_PIN_NO_5	5
#define GPIO_PIN_NO_6	6
#define GPIO_PIN_NO_7	7
#define GPIO_PIN_NO_8	8
#define GPIO_PIN_NO_9	9
#define GPIO_PIN_NO_10	10
#define GPIO_PIN_NO_11	11
#define GPIO_PIN_NO_12	12
#define GPIO_PIN_NO_13	13
#define GPIO_PIN_NO_14	14
#define GPIO_PIN_NO_15	15

#define GPIO_MODE_INPUT 	0
#define GPIO_MODE_OUTPUT 	1
#define GPIO_MODE_ALTFN 	2
#define GPIO_MODE_ANALOG 	3

#define GPIO_MODE_IT_FT		4 //Falling Edge Trigger
#define GPIO_MODE_IT_RT 	5 //Rising Edge Trigger
#define GPIO_MODE_IT_RFT	6 //Rising Falling Edge Trigger

#define GPIO_SPEED_LOW		0
#define GPIO_SPEED_MEDIUM	1
#define GPIO_SPEED_FAST		2
#define GPIO_SPEED_HIGH		3

#define GPIO_NO_PUPD  		0
#define GPIO_PU				1
#define GPIO_PD				2

#define GPIO_OP_TYPE_PP 	0
#define GPIO_OP_TYPE_OD 	1




typedef struct
{
	uint8_t GPIO_Pin_Number;
	uint8_t	GPIO_PinMode;
	uint8_t GPIO_PinSpeed;
	uint8_t GPIO_PUPDControl; //PullUp PullDown
	uint8_t GPIO_PinOPType;
	uint8_t GPIO_PinAltFuncMode;
}GPIO_Config_t;

typedef struct
{
	GPIO_RegDef_t *pGPIOxAddr;
	GPIO_Config_t GPIO_Config;
}GPIO_Handle_t;

void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOxAddr, uint8_t EnDis);

void GPIO_Init(GPIO_Handle_t *pGPIOHandle);
void GPIO_DeInit(GPIO_RegDef_t *pGPIOxAddr);

uint8_t GPIO_ReadFromPin(GPIO_RegDef_t *pGPIOxAddr, uint8_t PinNumber);
uint16_t GPIO_ReadFromPort(GPIO_RegDef_t *pGPIOxAddr);
void GPIO_WriteToPin(GPIO_RegDef_t *pGPIOxAddr, uint8_t PinNumber, uint8_t Value);
void GPIO_WriteToPort(GPIO_RegDef_t *pGPIOxAddr, uint16_t Value);
void GPIO_TogglePin(GPIO_RegDef_t *pGPIOxAddr, uint8_t PinNumber);

void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t EnDis);
void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority);
void GPIO_IRQHandle(uint8_t PinNumber);
void DAC_WriteToPin(GPIO_RegDef_t *pGPIOxAddr, uint8_t PinNumber, uint16_t Value);


#endif /* STM32F446RE_GPIO_H_ */
