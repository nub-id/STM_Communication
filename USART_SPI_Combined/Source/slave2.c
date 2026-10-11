/*
 * slave2.c
 *
 *  Created on: Oct 11, 2026
 *      Author: Shams-E-Sheefat Nabid
 */


#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "STM32_f446re.h"
#include "STM32f446re_GPIO.h"
#include "STM32f446re_SPI_driver.h"

#define CMD_STATUS  0x01
#define CMD_READ    0x02
#define CMD_WRITE   0x03

#define RESP_MARKER     0xA5
#define RESP_ERR        0xEE

#define SPI_TIMEOUT     200000U

#define NSS_PIN         GPIO_PIN_NO_9

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
    GPIO_Init(&SPI2_pins); 		// SCK PB10
    SPI2_pins.GPIO_Config.GPIO_Pin_Number = GPIO_PIN_NO_14;
    GPIO_Init(&SPI2_pins);		// MISO PB14
    SPI2_pins.GPIO_Config.GPIO_Pin_Number = GPIO_PIN_NO_15;
    GPIO_Init(&SPI2_pins);		// MOSI PB15

    SPI2_pins.GPIO_Config.GPIO_PUPDControl = GPIO_PU;
    SPI2_pins.GPIO_Config.GPIO_Pin_Number = GPIO_PIN_NO_9;
    GPIO_Init(&SPI2_pins);		// NSS PB9
}

void SPI2_Init(void)
{
    SPI_Handle_t SPI2_h;
    memset(&SPI2_h, 0, sizeof(SPI2_h));

    SPI2_h.pSPIx = SPI2;
    SPI2_h.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
    SPI2_h.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_SLAVE;
    SPI2_h.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
    SPI2_h.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
    SPI2_h.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
    SPI2_h.SPIConfig.SPI_SSM = SPI_SSM_DI;

    SPI_Init(&SPI2_h);
}

static inline uint8_t NSS_IsHigh(void){
	return GPIO_ReadFromPin(GPIOB, NSS_PIN);
}

static void SPI_Resync(void)
{
    volatile uint32_t t;

    SPI_PeripheralControl(SPI2, DISABLE);
    t = SPI2->DR;
    t = SPI2->SR;
    (void)t;
    SPI_PeripheralControl(SPI2, ENABLE);
}

static uint8_t Slave_RecvByte(uint8_t *b)
{
    uint32_t t = SPI_TIMEOUT;

    while(!SPI_GetFlagStatus(SPI2, SPI_RXNE_FLAG)){
        if(NSS_IsHigh() || --t == 0U) return 0;
    }
    *b = (uint8_t)SPI2->DR;
    return 1;
}

static void Slave_SendResponse(uint8_t resp)
{
    uint32_t t = SPI_TIMEOUT;

    while(!SPI_GetFlagStatus(SPI2, SPI_TXE_FLAG)){
        if(NSS_IsHigh() || --t == 0U) return;
    }
    SPI2->DR = RESP_MARKER;

    t = SPI_TIMEOUT;
    while(!SPI_GetFlagStatus(SPI2, SPI_TXE_FLAG)){
        if(NSS_IsHigh() || --t == 0U) return;
    }
    SPI2->DR = resp;

    t = SPI_TIMEOUT;
    while(!NSS_IsHigh() && --t);
}

GPIO_RegDef_t* DecodePort(uint8_t port_id)
{
    switch(port_id){
        case 0: return GPIOA;
        case 1: return GPIOB;
        case 2: return GPIOC;
        case 3: return GPIOD;
        case 4: return GPIOE;
        case 5: return GPIOF;
        case 6: return GPIOG;
        case 7: return GPIOH;
        default: return NULL;
    }
}

static uint8_t IsProtectedPin(uint8_t port_id, uint8_t pin)
{
    if(port_id == 1U && (pin == 9U || pin == 10U || pin == 14U || pin == 15U)) return 1;
    if(port_id == 0U && (pin == 13U || pin == 14U)) return 1;
    return 0;
}

void GPIO_init_out(GPIO_RegDef_t* port_id, uint8_t pin){
	GPIO_Handle_t temp_gpio;
	memset(&temp_gpio, 0, sizeof(temp_gpio));

	temp_gpio.pGPIOxAddr = port_id;
	temp_gpio.GPIO_Config.GPIO_PinMode = GPIO_MODE_OUTPUT;
	temp_gpio.GPIO_Config.GPIO_PUPDControl = GPIO_NO_PUPD;
	temp_gpio.GPIO_Config.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	temp_gpio.GPIO_Config.GPIO_PinSpeed = GPIO_SPEED_FAST;
	temp_gpio.GPIO_Config.GPIO_Pin_Number = pin;

	GPIO_Init(&temp_gpio);
}

void GPIO_init_in(GPIO_RegDef_t* port_id, uint8_t pin){
	GPIO_Handle_t temp_gpio;
	memset(&temp_gpio, 0, sizeof(temp_gpio));

	temp_gpio.pGPIOxAddr = port_id;
	temp_gpio.GPIO_Config.GPIO_PinMode = GPIO_MODE_INPUT;
	temp_gpio.GPIO_Config.GPIO_PUPDControl = GPIO_NO_PUPD;
	temp_gpio.GPIO_Config.GPIO_PinSpeed = GPIO_SPEED_FAST;
	temp_gpio.GPIO_Config.GPIO_Pin_Number = pin;

	GPIO_Init(&temp_gpio);
}

static uint8_t Slave_Execute(uint8_t cmd, uint8_t pin_id, uint8_t value)
{
    uint8_t port_id = pin_id >> 4;
    uint8_t pin_num = pin_id & 0x0F;
    GPIO_RegDef_t *pPort = DecodePort(port_id);

    if(pPort == NULL) return RESP_ERR;

    if(cmd == CMD_STATUS){

        GPIO_PeriClockControl(pPort, ENABLE);
        return GPIO_ReadFromPin(pPort, pin_num);
    }

    if(IsProtectedPin(port_id, pin_num)) return RESP_ERR;

    if(cmd == CMD_READ){
        GPIO_init_in(pPort, pin_num);
        return GPIO_ReadFromPin(pPort, pin_num);
    }

    if(cmd == CMD_WRITE){
        if(value > 1U) return RESP_ERR;
        GPIO_init_out(pPort, pin_num);
        GPIO_WriteToPin(pPort, pin_num, value);
        return value;
    }

    return RESP_ERR;
}

int main(void)
{
    uint8_t cmd, pin_id, value;

    SPI2_GPIO_Init();
    SPI2_Init();
    SPI_PeripheralControl(SPI2, ENABLE);

    while(1)
    {
        if(!Slave_RecvByte(&cmd)){
            if(NSS_IsHigh()) continue;
            goto resync;
        }

        if(cmd != CMD_STATUS && cmd != CMD_READ && cmd != CMD_WRITE){
            goto resync;
        }

        if(!Slave_RecvByte(&pin_id)) goto resync;

        value = 0;
        if(cmd == CMD_WRITE){
            if(!Slave_RecvByte(&value)) goto resync;
        }

        Slave_SendResponse(Slave_Execute(cmd, pin_id, value));

resync:
        {
            uint32_t t = SPI_TIMEOUT * 4U;
            while(!NSS_IsHigh() && --t);
        }
        SPI_Resync();
    }
    return 0;
}
