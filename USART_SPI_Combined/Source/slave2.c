/*
 * slave2.c
 *
 *  Created on: Oct 11, 2026
 *      Author: Shams-E-Sheefat Nabid
 */

#include <stdint.h>
#include <stddef.h>
#include "STM32_f446re.h"
#include "STM32f446re_GPIO.h"
#include "STM32f446re_SPI_driver.h"

#define CMD_STATUS  0x01
#define CMD_READ    0x02
#define CMD_WRITE   0x03

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
    GPIO_Init(&SPI2_pins); 		// SCK PB10
    SPI2_pins.GPIO_Config.GPIO_Pin_Number = GPIO_PIN_NO_14;
    GPIO_Init(&SPI2_pins);		// MISO PB14
    SPI2_pins.GPIO_Config.GPIO_Pin_Number = GPIO_PIN_NO_15;
    GPIO_Init(&SPI2_pins);		// MOSI PB15

    SPI2_pins.GPIO_Config.GPIO_Pin_Number = GPIO_PIN_NO_9;
    GPIO_Init(&SPI2_pins);		// NSS PB9
}

void SPI2_Init(void)
{
    SPI_Handle_t SPI2_h;

    SPI2_h.pSPIx = SPI2;
    SPI2_h.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
    SPI2_h.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_SLAVE;
    SPI2_h.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
    SPI2_h.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
    SPI2_h.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
    SPI2_h.SPIConfig.SPI_SSM = SPI_SSM_DI;

    SPI_Init(&SPI2_h);
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

void GPIO_init_out(GPIO_RegDef_t* port_id, uint8_t pin){
	GPIO_Handle_t temp_gpio;
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
	temp_gpio.pGPIOxAddr = port_id;
	temp_gpio.GPIO_Config.GPIO_PinMode = GPIO_MODE_INPUT;
	temp_gpio.GPIO_Config.GPIO_PUPDControl = GPIO_NO_PUPD;
	temp_gpio.GPIO_Config.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	temp_gpio.GPIO_Config.GPIO_PinSpeed = GPIO_SPEED_FAST;
	temp_gpio.GPIO_Config.GPIO_Pin_Number = pin;

	GPIO_Init(&temp_gpio);
}

int main(void)
{
    uint8_t command_byte;
    uint8_t payload[2];

    SPI2_GPIO_Init();
    SPI2_Init();
    SPI_PeripheralControl(SPI2, ENABLE);

    while(1)
    {
        SPI_RecieveData(SPI2, &command_byte, 1);

        if (command_byte == CMD_STATUS)
        {
            SPI_RecieveData(SPI2, payload, 1);

            uint8_t port_id = payload[0] >> 4;
            uint8_t pin_num = payload[0] & 0x0F;

            GPIO_RegDef_t *pPort = DecodePort(port_id);
            uint8_t response = 0xFF;

            if (pPort != NULL && pin_num <= 15U){
                response = GPIO_ReadFromPin(pPort, pin_num);
            }

            SPI_SendData(SPI2, &response, 1);
        }
        else if (command_byte == CMD_READ){
            SPI_RecieveData(SPI2, payload, 1);

            uint8_t port_id = payload[0] >> 4;
            uint8_t pin_num = payload[0] & 0x0F;

            GPIO_RegDef_t *pPort = DecodePort(port_id);
            uint8_t response = 0xFF;

            if (pPort != NULL && pin_num <= 15U){
                GPIO_init_in(pPort, pin_num);

                response = GPIO_ReadFromPin(pPort, pin_num);
            }

            SPI_SendData(SPI2, &response, 1);
        }
        else if(command_byte == CMD_WRITE)
        {
            SPI_RecieveData(SPI2, payload, 2);

            uint8_t port_id = payload[0] >> 4;
            uint8_t pin_num = payload[0] & 0x0F;
            uint8_t value = payload[1];

            GPIO_RegDef_t *pPort = DecodePort(port_id);

            if(pPort != NULL)
            {
            	GPIO_init_out(pPort, pin_num);
                GPIO_WriteToPin(pPort, pin_num, value);
            }

            SPI_SendData(SPI2, &value, 1);
        }

        if(SPI_GetFlagStatus(SPI2, (1U << SPI_SR_RXNE)))
        {
            uint8_t junk = SPI2->DR;
            (void)junk;
        }
    }
    return 0;
}
