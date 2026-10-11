

#include "STM32_f446re.h"
#include "STM32f446re_USART_driver.h"

uint8_t USART_GetFlagStatus(USART_RegDef_t *pUSARTx, uint32_t FlagName){
	if(pUSARTx->SR & FlagName){
		return FLAG_SET;
	}
	return FLAG_RESET;
}

void USART_PeriClockControl(USART_RegDef_t *pUSARTx, uint8_t EnDis)
{
	if (EnDis == ENABLE){
		if (pUSARTx == USART1){
			USART1_PCLK_EN();
		}
		else if (pUSARTx == USART2){
			USART2_PCLK_EN();
		}
		else if (pUSARTx == USART3){
			USART3_PCLK_EN();
		}
		else if (pUSARTx == UART4){
			UART4_PCLK_EN();
		}
		else if (pUSARTx == UART5){
			UART5_PCLK_EN();
		}
		else if (pUSARTx == USART6){
			USART6_PCLK_EN();
		}
	}
	else
	{
		if (pUSARTx == USART1){
			USART1_PCLK_DI();
		}
		else if (pUSARTx == USART2){
			USART2_PCLK_DI();
		}
		else if (pUSARTx == USART3){
			USART3_PCLK_DI();
		}
		else if (pUSARTx == UART4){
			UART4_PCLK_DI();
		}
		else if (pUSARTx == UART5){
			UART5_PCLK_DI();
		}
		else if (pUSARTx == USART6){
			USART6_PCLK_DI();
		}
	}
}

void USART_PeripheralControl(USART_RegDef_t *pUSARTx, uint8_t EnDis){
	if (EnDis == ENABLE){
		pUSARTx->CR1 |= (1U << USART_CR1_UE);
	}
	else{
		pUSARTx->CR1 &= ~(1U << USART_CR1_UE);
	}
}

void USART_Init(USART_Handle_t *pUSARTHandle){
	USART_PeriClockControl(pUSARTHandle->pUSARTx, ENABLE);

	uint32_t temp = 0;

	if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_TxOnly){
		temp |= (1U << USART_CR1_TE);
	}
	else if(pUSARTHandle->USART_Config.USART_Mode == USART_MODE_RxOnly){
		temp |= (1U << USART_CR1_RE);
	}
	else if(pUSARTHandle->USART_Config.USART_Mode == USART_MODE_TxRx){
		temp |= (1U << USART_CR1_TE);
		temp |= (1U << USART_CR1_RE);
	}

	if (pUSARTHandle->USART_Config.USART_OverSampling == USART_OVERSAMPLING_BY16){
		switch(pUSARTHandle->USART_Config.USART_Baud){
			case USART_BAUD_1200:
				pUSARTHandle->pUSARTx->BRR = 0x3415;
				break;
			case USART_BAUD_2400:
				pUSARTHandle->pUSARTx->BRR = 0x1A0B;
				break;
			case USART_BAUD_9600:
				pUSARTHandle->pUSARTx->BRR = 0x0683;
				break;
			case USART_BAUD_19200:
				pUSARTHandle->pUSARTx->BRR = 0x0341;
				break;
			case USART_BAUD_38400:
				pUSARTHandle->pUSARTx->BRR = 0x01A1;
				break;
			case USART_BAUD_57600:
				pUSARTHandle->pUSARTx->BRR = 0x0116;
				break;
			case USART_BAUD_115200:
				pUSARTHandle->pUSARTx->BRR = 0x008B;
				break;
			case USART_BAUD_230400:
				pUSARTHandle->pUSARTx->BRR = 0x0045;
				break;
			case USART_BAUD_460800:
				pUSARTHandle->pUSARTx->BRR = 0x0023;
				break;
			case USART_BAUD_921600:
				pUSARTHandle->pUSARTx->BRR = 0x0011;
				break;
			case USART_BAUD_2M:
				/* Cannot achieve 2 MBaud with 16 MHz and OVER8=0 (requires USARTDIV >= 1.0000, here 16M/(16*2M)=0.5) */
				/* Achievable only if OVER8=1 (BRR = 0x0001) or with higher f_PCLK */
				pUSARTHandle->pUSARTx->BRR = 0x0000;
				break;
			case USART_BAUD_3M:
				/* Cannot achieve 3 MBaud with 16 MHz clock */
				pUSARTHandle->pUSARTx->BRR = 0x0000;
				break;
			default:
				break;
			}
	}
	else if (pUSARTHandle->USART_Config.USART_OverSampling == USART_OVERSAMPLING_BY8){
		switch (pUSARTHandle->USART_Config.USART_Baud)
			{
			case USART_BAUD_1200:
				pUSARTHandle->pUSARTx->BRR = 0x6825;
				break;
			case USART_BAUD_2400:
				pUSARTHandle->pUSARTx->BRR = 0x3413;
				break;
			case USART_BAUD_9600:
				pUSARTHandle->pUSARTx->BRR = 0x0D03;
				break;
			case USART_BAUD_19200:
				pUSARTHandle->pUSARTx->BRR = 0x0681;
				break;
			case USART_BAUD_38400:
				pUSARTHandle->pUSARTx->BRR = 0x0341;
				break;
			case USART_BAUD_57600:
				pUSARTHandle->pUSARTx->BRR = 0x0226;
				break;
			case USART_BAUD_115200:
				pUSARTHandle->pUSARTx->BRR = 0x0113;
				break;
			case USART_BAUD_230400:
				pUSARTHandle->pUSARTx->BRR = 0x0085;
				break;
			case USART_BAUD_460800:
				pUSARTHandle->pUSARTx->BRR = 0x0043;
				break;
			case USART_BAUD_921600:
				pUSARTHandle->pUSARTx->BRR = 0x0021;
				break;
			case USART_BAUD_2M:
				pUSARTHandle->pUSARTx->BRR = 0x0010;
				break;
			case USART_BAUD_3M:
				/* Cannot achieve 3M baud with 16 MHz (USARTDIV = 0.667 < 1.000) */
				pUSARTHandle->pUSARTx->BRR = 0x0000;
				break;
			default:
				break;
			}
	}

	pUSARTHandle->pUSARTx->CR2 &= ~(3U << USART_CR2_STOP);
	pUSARTHandle->pUSARTx->CR2 |= (pUSARTHandle->USART_Config.USART_NoOfStopBits << USART_CR2_STOP);

	temp |= (pUSARTHandle->USART_Config.USART_WordLength << USART_CR1_M);

	if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DIS){
	}
	else{
		temp |= (1U << USART_CR1_PCE);
		if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_EN_ODD){
			temp |= (1U << USART_CR1_PS);
		}
	}

	temp |= (pUSARTHandle->USART_Config.USART_OverSampling << USART_CR1_OVER8);

	pUSARTHandle->pUSARTx->CR1 = temp;

	if (pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HWFLOW_CTRL_CTS){
		pUSARTHandle->pUSARTx->CR3 |= (1U << USART_CR3_CTSE);
	}
	else if(pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HWFLOW_CTRL_RTS){
		pUSARTHandle->pUSARTx->CR3 |= (1U << USART_CR3_RTSE);
	}
	else if(pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HWFLOW_CTRL_CTSRTS){
		pUSARTHandle->pUSARTx->CR3 |= (1U << USART_CR3_CTSE);
		pUSARTHandle->pUSARTx->CR3 |= (1U << USART_CR3_RTSE);
	}
}

void USART_DeInit(USART_RegDef_t *pUSARTx){
	if (pUSARTx == USART1){
		USART1_RST();
	}
	else if (pUSARTx == USART2){
		USART2_RST();
	}
	else if (pUSARTx == USART3){
		USART3_RST();
	}
	else if (pUSARTx == UART4){
		UART4_RST();
	}
	else if (pUSARTx == UART5){
		UART5_RST();
	}
	else if (pUSARTx == USART6){
		USART6_RST();
	}
}

void USART_SendData(USART_RegDef_t *pUSARTx, uint8_t *pTxBuffer, uint32_t Len){

    uint8_t wordlength9 = (pUSARTx->CR1 & (1U << USART_CR1_M)) != 0U;

    uint8_t parityEnabled = (pUSARTx->CR1 & (1U << USART_CR1_PCE)) != 0U;

    if (Len == 0U){
        return;
    }

    if (wordlength9 && !parityEnabled && (Len & 1U)){ //9 bit word without parity must have even length
        return;
    }

    while (Len > 0U){
        while (USART_GetFlagStatus(pUSARTx, USART_TXE_FLAG) == FLAG_RESET);

        if (wordlength9){
        	if(!parityEnabled){
				uint16_t word = *((uint16_t *)pTxBuffer);

				word &= 0x1FFU;

				pUSARTx->DR = word;

				pTxBuffer += 2U;
				Len -= 2U;
        	}
        	else{
        		pUSARTx->DR = *pTxBuffer & 0xFFU;

        		pTxBuffer++;
				Len--;
        	}
        }
        else{
            if (parityEnabled){
                pUSARTx->DR = *pTxBuffer & 0x7FU;
            }
            else{
                pUSARTx->DR = *pTxBuffer & 0xFFU;
            }

            pTxBuffer++;
            Len--;
        }
    }

    while (USART_GetFlagStatus(pUSARTx, USART_TC_FLAG) == FLAG_RESET);
}

void USART_ReceiveData(USART_RegDef_t *pUSARTx, uint8_t *pRxBuffer, uint32_t Len)
{
	uint8_t data;
    uint8_t word9 = (pUSARTx->CR1 & (1U << USART_CR1_M)) != 0U;

    uint8_t parityEnabled = (pUSARTx->CR1 & (1U << USART_CR1_PCE)) != 0U;

    if (word9 && !parityEnabled && (Len & 1U)){
        return;
    }

    while (Len > 0U){
        while (USART_GetFlagStatus(pUSARTx, USART_RXNE_FLAG) == FLAG_RESET);

        if (word9){
        	if(!parityEnabled){
				uint16_t word = (uint16_t)pUSARTx->DR;

				word &= 0x01FFU;

				pRxBuffer[0] = (uint8_t)word;
				pRxBuffer[1] = (uint8_t)(word >> 8U);

				pRxBuffer += 2U;
				Len -= 2U;
        	}
        	else{
        		data = (uint8_t)pUSARTx->DR;
        		*pRxBuffer = (uint8_t)(data & 0xFFU);

        		pRxBuffer++;
        		Len--;
        	}
        }
        else{
        	data = (uint8_t)pUSARTx->DR;
            if (parityEnabled){
                *pRxBuffer = (uint8_t)(data & 0x7FU);
            }
            else{
                *pRxBuffer = (uint8_t)(data & 0xFFU);
            }

            pRxBuffer++;
            Len--;
        }
    }
}
