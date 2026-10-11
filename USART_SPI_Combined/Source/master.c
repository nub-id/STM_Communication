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

#define RESP_MARKER	0xA5
#define RESP_ERR	0xEE
#define MARKER_TRIES	64U
#define SPI_TIMEOUT		100000U
#define LAST_PORT	'H'

#define MAX_FIELDS	4U

static void Print(const char *s){
	USART_SendData(USART2, (uint8_t*)s, (uint32_t)strlen(s));
}

static void Guard(void){
	for(volatile uint32_t i=0; i<200U; i++);
}

void delay(uint32_t n)
{
    for(uint32_t i = 0; i < n * 50000U; i++);
}

void SPI2_GPIO_Init(void)
{
    GPIO_Handle_t SPI2_pins;
    memset(&SPI2_pins, 0, sizeof(SPI2_pins));

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
    GPIO_WriteToPin(GPIOB, GPIO_PIN_NO_11, GPIO_PIN_SET);

    NSS_pins.GPIO_Config.GPIO_Pin_Number = GPIO_PIN_NO_12;
    GPIO_Init(&NSS_pins);   	// Slave 2 NSS PB12
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
    SPI2_h.SPIConfig.SPI_Speed = SPI_SCLK_SPEED_DIV16; //16MHz/16 = 1MHz
    SPI2_h.SPIConfig.SPI_SSM = SPI_SSM_EN;

    SPI_Init(&SPI2_h);
}

void USART2_Init(void)
{
    GPIO_Handle_t usart_pins;
    memset(&usart_pins, 0, sizeof(usart_pins));

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
    memset(&usart2_h, 0, sizeof(usart2_h));

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
	static uint8_t last_was_cr = 0;
    uint8_t rx_byte;
    uint32_t i = 0;
    uint8_t overflow = 0;

    if(size < 2U){
    	if(size == 1U){
    		buf[0] = '\0';
    	}
    	return;
    }

    while(1)
    {
        USART_ReceiveData(USART2, &rx_byte, 1);


        if(rx_byte == '\n' && last_was_cr){
        	last_was_cr = 0;
        	continue;
        }

        last_was_cr = (rx_byte == '\r');

        if(rx_byte == '\r' || rx_byte == '\n'){

            char newline[] = "\r\n";
            Print(newline);
            break;
        }

        if(rx_byte == 0x08U || rx_byte == 0x7FU){
        	if(i>0U){
        		i--;
        		Print("\b \b");
        	}
        	continue;
        }

        if(rx_byte < 0x20U || rx_byte > 0x7EU) continue;

        if(i < size - 1U){
        	buf[i++] = (char)rx_byte;
        	USART_SendData(USART2, &rx_byte, 1);
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

    if(port >= 'A' && port <= 'H'){
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
    uint8_t pin = 0;

    if(text == NULL || pin_id == NULL) return 0;
    if(text[0] < 'A' || text[0] > 'H') return 0;
    if(text[1] < '0' || text[1] > '9') return 0;

    pin = (uint8_t)(text[1] - '0');

    if(text[2] != '\0'){
        if(text[1] == '0') return 0;                    // "A05"
        if(text[2] < '0' || text[2] > '9') return 0;
        if(text[3] != '\0') return 0;                   // "A123"
        pin = (uint8_t)(pin * 10U + (uint8_t)(text[2] - '0'));
    }

    if(pin > 15U) return 0;

    *pin_id = EncodePin(text[0], pin);
    return (*pin_id != INVALID_PIN);
}

static uint8_t SplitCommand(char *cmd, char *fields[], uint8_t max_fields){
    uint8_t n = 0;
    fields[n++] = cmd;

    for(char *p = cmd; *p != '\0'; p++){
        if(*p == '.'){
            *p = '\0';
            if(n >= max_fields) return 0;
            fields[n++] = p + 1;
        }
    }

    for(uint8_t i = 0; i < n; i++){
        if(fields[i][0] == '\0') return 0;
    }
    return n;
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

static uint8_t SPI_TransferByte(uint8_t tx, uint8_t *rx)
{
    uint32_t t = SPI_TIMEOUT;

    while(!SPI_GetFlagStatus(SPI2, SPI_TXE_FLAG)){
        if(--t == 0U) return 0;
    }
    SPI2->DR = tx;

    t = SPI_TIMEOUT;
    while(!SPI_GetFlagStatus(SPI2, SPI_RXNE_FLAG)){
        if(--t == 0U) return 0;
    }
    *rx = (uint8_t)SPI2->DR;
    return 1;
}

static uint8_t SPI_Transaction(const uint8_t *data, uint32_t length, uint8_t *response)
{
    uint8_t rx;

    if(SPI_GetFlagStatus(SPI2, SPI_RXNE_FLAG)){
        rx = (uint8_t)SPI2->DR;
        (void)rx;
    }

    for(uint32_t i = 0; i < length; i++){
        if(!SPI_TransferByte(data[i], &rx)) return 0;
    }

    for(uint32_t tries = 0; tries < MARKER_TRIES; tries++){
        if(!SPI_TransferByte(0xFF, &rx)) return 0;

        if(rx == RESP_MARKER){
            if(!SPI_TransferByte(0xFF, response)) return 0;
            return 1;
        }
    }
    return 0;   // slave never answered (not connected/ hung)
}

uint8_t SPI_ExecuteCommand(uint8_t slave, uint8_t *data, uint32_t length, uint8_t *response)
{
    uint8_t ok;
    uint32_t t = SPI_TIMEOUT;

    if(slave != 1U && slave != 2U) return 0;

    SPI_SelectSlave(slave);
    Guard();

    ok = SPI_Transaction(data, length, response);

    while(SPI_GetFlagStatus(SPI2, SPI_BSY_FLAG) && t--);
    SPI_DeselectSlave(slave);

    return ok;
}

void ProcessCommand(char *command)
{
    static const char usage[] = "ERROR: use <1|2>.Status.<Pin> | <1|2>.Read.<Pin> | <1|2>.Write.<Pin>.<High|Low>  (Pin: A0..H15)\r\n";

    char *fields[MAX_FIELDS];
    uint8_t nfields = SplitCommand(command, fields, MAX_FIELDS);

    uint8_t slave;
    uint8_t pin_id = 0;
    uint8_t data[3];
    uint32_t length;
    uint8_t resp = 0;
    char out_msg[96];

    if(nfields < 3U){
    	Print(usage);
    	return;
    }

    if(strcmp(fields[0], "1") == 0)      slave = 1;
    else if(strcmp(fields[0], "2") == 0) slave = 2;
    else{
    	Print(usage);
    	return;
    }

    const char *operation = fields[1];

    if(strcmp(operation, "Status") == 0 || strcmp(operation, "Read") == 0){

        if(nfields != 3U || !ParsePin(fields[2], &pin_id)){ Print(usage); return; }

        data[0] = (strcmp(operation, "Status") == 0) ? CMD_STATUS : CMD_READ;
        data[1] = pin_id;
        length = 2;
    }
    else if(strcmp(operation, "Write") == 0){

        if(nfields != 4U || !ParsePin(fields[2], &pin_id)){ Print(usage); return; }

        if(strcmp(fields[3], "High") == 0)     data[2] = VAL_HIGH;
        else if(strcmp(fields[3], "Low") == 0) data[2] = VAL_LOW;
        else{
        	Print(usage);
        	return;
        }

        data[0] = CMD_WRITE;
        data[1] = pin_id;
        length = 3;
    }
    else { Print(usage); return; }

    char port_ch = (char)('A' + (pin_id >> 4));
    uint32_t pin_no = (uint32_t)(pin_id & 0x0FU);

    if(!SPI_ExecuteCommand(slave, data, length, &resp)){
        snprintf(out_msg, sizeof(out_msg), "ERROR: Slave %u did not respond\r\n", (unsigned)slave);
        Print(out_msg);
        return;
    }

    if(resp == RESP_ERR){
        snprintf(out_msg, sizeof(out_msg), "ERROR: Slave %u rejected %c%lu\r\n", (unsigned)slave, port_ch, (unsigned long)pin_no);
    }
    else if(data[0] == CMD_WRITE){
        if(resp == data[2]){
            snprintf(out_msg, sizeof(out_msg), "Slave %u Pin %c%lu written %s OK\r\n", (unsigned)slave, port_ch, (unsigned long)pin_no,
                     (resp == VAL_HIGH) ? "HIGH" : "LOW");
        }
        else{
            snprintf(out_msg, sizeof(out_msg), "ERROR: Slave %u bad write echo 0x%02X\r\n", (unsigned)slave, (unsigned)resp);
        }
    }
    else{
        if(resp == VAL_HIGH || resp == VAL_LOW){
            snprintf(out_msg, sizeof(out_msg), "Slave %u Pin %c%lu is %s\r\n", (unsigned)slave, port_ch, (unsigned long)pin_no, (resp == VAL_HIGH) ? "HIGH" : "LOW");
        }
        else{
            snprintf(out_msg, sizeof(out_msg), "ERR: Slave %u bad reply 0x%02X\r\n", (unsigned)slave, (unsigned)resp);
        }
    }

    Print(out_msg);
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

    Print("Type your command.:\r\n> ");

    while(1)
    {
        ReadCommand(rx_buffer, sizeof(rx_buffer));

        if(rx_buffer[0] != '\0')
        {
            ProcessCommand(rx_buffer);
        }

        Print("> ");
    }
    return 0;
}
