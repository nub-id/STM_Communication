/*
 * master.c
 *
 *  Created on: Oct 11, 2026
 *      Author: Shams-E-Sheefat Nabid
 */

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "STM32_f446re.h"
#include "STM32f446re_GPIO.h"
#include "STM32f446re_SPI_driver.h"
#include "STM32f446re_USART_driver.h"

#define CMD_STATUS  0x01
#define CMD_READ    0x02
#define CMD_WRITE   0x03

#define VAL_HIGH    0x01
#define VAL_LOW     0x00

#define INVALID_PIN 0xFF

void delay(uint32_t n)
{
    for(uint32_t i = 0; i < n * 50000U; i++);
}

void SPI2_GPIO_Init(void)
{
    GPIO_Handle_t SPI2_pins;

    SPI2_pins.pGPIOxAddr = GPIOB;
    SPI2_pins.GPIO_Config.GPIO_PinMode = GPIO_MODE_ALTFN;
    SPI2_pins.GPIO_Config.GPIO_PinAltFuncMode = 5;
    SPI2_pins.GPIO_Config.GPIO_PUPDControl = GPIO_NO_PUPD;
    SPI2_pins.GPIO_Config.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    SPI2_pins.GPIO_Config.GPIO_PinSpeed = GPIO_SPEED_FAST;

    SPI2_pins.GPIO_Config.GPIO_Pin_Number = GPIO_PIN_NO_10;
    GPIO_Init(&SPI2_pins);  	// SCK PB10

    SPI2_pins.GPIO_Config.GPIO_Pin_Number = GPIO_PIN_NO_14;
    GPIO_Init(&SPI2_pins);  	// MISO PB14

    SPI2_pins.GPIO_Config.GPIO_Pin_Number = GPIO_PIN_NO_15;
    GPIO_Init(&SPI2_pins);  	// MOSI PB15

    //I faced an issue here due to open drain system

	SPI2_pins.GPIO_Config.GPIO_Pin_Number = GPIO_PIN_NO_9;
	SPI2_pins.GPIO_Config.GPIO_PinMode = GPIO_MODE_OUTPUT;
	GPIO_Init(&SPI2_pins);
	GPIO_WriteToPin(GPIOB, GPIO_PIN_NO_9, GPIO_PIN_SET);
}

void Master_NSS_Init(void)
{
    GPIO_Handle_t NSS_pins;

    NSS_pins.pGPIOxAddr = GPIOB;
    NSS_pins.GPIO_Config.GPIO_PinMode = GPIO_MODE_OUTPUT;
    NSS_pins.GPIO_Config.GPIO_PUPDControl = GPIO_NO_PUPD;
    NSS_pins.GPIO_Config.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    NSS_pins.GPIO_Config.GPIO_PinSpeed = GPIO_SPEED_FAST;

    NSS_pins.GPIO_Config.GPIO_Pin_Number = GPIO_PIN_NO_11;
    GPIO_Init(&NSS_pins);   	// Slave 1 NSS PB11

    NSS_pins.GPIO_Config.GPIO_Pin_Number = GPIO_PIN_NO_12;
    GPIO_Init(&NSS_pins);   	// Slave 2 NSS PB12

    GPIO_WriteToPin(GPIOB, GPIO_PIN_NO_11, GPIO_PIN_SET);
    GPIO_WriteToPin(GPIOB, GPIO_PIN_NO_12, GPIO_PIN_SET);
}

void SPI2_Init(void)
{
    SPI_Handle_t SPI2_h;

    SPI2_h.pSPIx = SPI2;
    SPI2_h.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
    SPI2_h.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
    SPI2_h.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
    SPI2_h.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
    SPI2_h.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
    SPI2_h.SPIConfig.SPI_Speed = SPI_SCLK_SPEED_DIV8;
    SPI2_h.SPIConfig.SPI_SSM = SPI_SSM_EN;

    SPI_Init(&SPI2_h);
}

void USART2_Init(void)
{
    GPIO_Handle_t usart_pins;

    usart_pins.pGPIOxAddr = GPIOA;
    usart_pins.GPIO_Config.GPIO_PinMode = GPIO_MODE_ALTFN;
    usart_pins.GPIO_Config.GPIO_PinAltFuncMode = 7;
    usart_pins.GPIO_Config.GPIO_PUPDControl = GPIO_PU;
    usart_pins.GPIO_Config.GPIO_PinSpeed = GPIO_SPEED_FAST;
    usart_pins.GPIO_Config.GPIO_PinOPType = GPIO_OP_TYPE_PP;

    usart_pins.GPIO_Config.GPIO_Pin_Number = GPIO_PIN_NO_2;
    GPIO_Init(&usart_pins);  	// TX PA2

    usart_pins.GPIO_Config.GPIO_Pin_Number = GPIO_PIN_NO_3;
    GPIO_Init(&usart_pins);  	// RX PA3

    USART_Handle_t usart2_h;

    usart2_h.pUSARTx = USART2;
    usart2_h.USART_Config.USART_Baud = USART_BAUD_115200;
    usart2_h.USART_Config.USART_HWFlowControl = USART_HWFLOW_CTRL_NONE;
    usart2_h.USART_Config.USART_Mode = USART_MODE_TxRx;
    usart2_h.USART_Config.USART_NoOfStopBits = USART_STOPBITS_1;
    usart2_h.USART_Config.USART_WordLength = USART_WORDLEN_8BITS;
    usart2_h.USART_Config.USART_ParityControl = USART_PARITY_DIS;
    usart2_h.USART_Config.USART_OverSampling = USART_OVERSAMPLING_BY16;

    USART_Init(&usart2_h);
}

void ReadCommand(char *buf, uint32_t size)
{
    uint8_t rx_byte;
    uint32_t i = 0;
    uint8_t overflow = 0;

    if(size < 2U) return;

    while(1)
    {
        USART_ReceiveData(USART2, &rx_byte, 1);
        USART_SendData(USART2, &rx_byte, 1);

        if(rx_byte == '\r' || rx_byte == '\n'){

            char newline[] = "\r\n";
            USART_SendData(USART2, (uint8_t*)newline, 2);
            break;
        }

        if(i < size - 1U){
        	buf[i++] = (char)rx_byte;
        }
        else{
        	overflow = 1;
        }
    }

    buf[i] = '\0';
    if(overflow){
    	buf[0] = '\0';
    }
}

uint8_t EncodePin(char port, uint8_t pin)
{
    uint8_t port_id;

    if(port >= 'A' && port <= 'I'){
    	port_id = (uint8_t)(port - 'A');
    }
    else{
    	return INVALID_PIN;
    }

    if(pin > 15U){
    	return INVALID_PIN;
    }
    return (uint8_t)((port_id << 4) | pin);
}

uint8_t ParsePin(const char *text, uint8_t *pin_id)
{
    uint16_t pin = 0;
    uint8_t i = 1;

    if(text == NULL || pin_id == NULL) return 0;
    if(text[0] < 'A' || text[0] > 'I') return 0;
    if(text[1] < '0' || text[1] > '9') return 0;

    while(text[i] != '\0')
    {
        if(text[i] < '0' || text[i] > '9') return 0;

        pin = (uint16_t)(pin * 10U + (uint16_t)(text[i] - '0'));
        if(pin > 15U) return 0;
        i++;
    }

    *pin_id = EncodePin(text[0], (uint8_t)pin);
    return (*pin_id != INVALID_PIN);
}

void SPI_SelectSlave(uint8_t slave)
{
    if(slave == 1U){
    	GPIO_WriteToPin(GPIOB, GPIO_PIN_NO_11, GPIO_PIN_RESET);
    }
    else if(slave == 2U){
    	GPIO_WriteToPin(GPIOB, GPIO_PIN_NO_12, GPIO_PIN_RESET);
    }
}

void SPI_DeselectSlave(uint8_t slave)
{
    if(slave == 1U){
    	GPIO_WriteToPin(GPIOB, GPIO_PIN_NO_11, GPIO_PIN_SET);
    }
    else if(slave == 2U){
    	GPIO_WriteToPin(GPIOB, GPIO_PIN_NO_12, GPIO_PIN_SET);
    }
}

uint8_t SPI_ExecuteCommand(uint8_t slave, uint8_t *data, uint32_t length)
{
    uint8_t dummy = 0xFF;
    uint8_t response = 0;
    uint8_t temp;

    if(slave != 1U && slave != 2U) return 0;

    SPI_SelectSlave(slave);

    SPI_SendData(SPI2, data, length);

    delay(1);

    if(SPI_GetFlagStatus(SPI2, (1U << SPI_SR_RXNE)))
    {
        temp = SPI2->DR;
        (void)temp;
    }

    SPI_SendData(SPI2, &dummy, 1);

    SPI_RecieveData(SPI2, &response, 1);

    SPI_DeselectSlave(slave);

    return response;
}

void ProcessCommand(char *command)
{
    char *slave_text = strtok(command, ".");
    char *operation  = strtok(NULL, ".");
    char *pin_text   = strtok(NULL, ".");
    char *value_text = strtok(NULL, ".");
    char *extra      = strtok(NULL, ".");

    uint8_t slave;
    uint8_t pin_id;
    uint8_t data[3];
    uint32_t length;
    char out_msg[64];

    if(slave_text == NULL || operation == NULL) return;

    if(strcmp(slave_text, "1") == 0){
    	slave = 1;
    }
    else if(strcmp(slave_text, "2") == 0){
    	slave = 2;
    }
    else return;

    if(strcmp(operation, "Status") == 0 || strcmp(operation, "Read") == 0){

        if(pin_text == NULL || value_text != NULL || extra != NULL) return;
        if(!ParsePin(pin_text, &pin_id)) return;

        data[0] = (strcmp(operation, "Status") == 0) ? CMD_STATUS : CMD_READ;
        data[1] = pin_id;
        length = 2;
    }

    else if(strcmp(operation, "Write") == 0){

        if(pin_text == NULL || value_text == NULL || extra != NULL) return;
        if(!ParsePin(pin_text, &pin_id)) return;

        if(strcmp(value_text, "High") == 0) data[2] = VAL_HIGH;
        else if(strcmp(value_text, "Low") == 0) data[2] = VAL_LOW;
        else return;

        data[0] = CMD_WRITE;
        data[1] = pin_id;
        length = 3;
    }
    else return;

    uint8_t resp = SPI_ExecuteCommand(slave, data, length);

    if(data[0] == CMD_STATUS || data[0] == CMD_READ){
        sprintf(out_msg, "Slave %d Pin %s is %s\r\n", slave, pin_text, (resp == VAL_HIGH) ? "HIGH" : "LOW");
    }
    else if(data[0] == CMD_WRITE){
        sprintf(out_msg, "Slave %d Pin %s written %s OK\r\n", slave, pin_text, (resp == VAL_HIGH) ? "HIGH" : "LOW");
    }

    USART_SendData(USART2, (uint8_t*)out_msg, strlen(out_msg));
}

int main(void)
{
    char rx_buffer[50];

    SPI2_GPIO_Init();
    Master_NSS_Init();
    SPI2_Init();
    USART2_Init();

    SPI_SSIConfig(SPI2, ENABLE);
    SPI_PeripheralControl(SPI2, ENABLE);
    USART_PeripheralControl(USART2, ENABLE);

    char start_msg[] = "Type your command.:\r\n> ";
    USART_SendData(USART2, (uint8_t*)start_msg, strlen(start_msg));

    while(1)
    {
        ReadCommand(rx_buffer, sizeof(rx_buffer));

        if(rx_buffer[0] != '\0')
        {
            ProcessCommand(rx_buffer);

            char prompt[] = "> ";
			USART_SendData(USART2, (uint8_t*)prompt, strlen(prompt));
        }
    }
    return 0;
}
