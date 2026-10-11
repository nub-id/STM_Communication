# STM32F446RE SPI Master-Slave Controller

This project implements a custom bare-metal SPI communication protocol using three STM32F446RE microcontrollers. A master node receives text-based commands via USART, parses them, and routes them to one of two slave nodes over SPI2 to dynamically configure, read, or write to their GPIO pins.

## Hardware Setup

**Master Node Pinout**
* **SPI2 SCK:** PB10
* **SPI2 MISO:** PB14
* **SPI2 MOSI:** PB15
* **NSS Slave 1:** PB11
* **NSS Slave 2:** PB12
* **USART2 TX:** PA2
* **USART2 RX:** PA3

**Slave Nodes (1 & 2) Pinout**
* **SPI2 SCK:** PB10
* **SPI2 MISO:** PB14
* **SPI2 MOSI:** PB15
* **SPI2 NSS:** PB9 (Hardware managed)

**Wiring Instructions**
1. Connect PB10, PB14, and PB15 in parallel across all three boards.
2. Connect Master PB11 to Slave 1 PB9.
3. Connect Master PB12 to Slave 2 PB9.
4. Ensure all three microcontrollers share a common Ground (GND).

## Command Protocol

The master accepts commands over a serial terminal via USART2 (115200 baud, 8N1). Commands must be terminated with a carriage return (`\r`) or line feed (`\n`). 

**Base Syntax:** `<Slave_ID>.<Operation>.<Pin>.[Value]`

### Supported Operations

* **Status / Read Pin**
  Dynamically initializes the target pin as an input and returns its current state.
  * **Syntax:** `1.Status.A9` or `2.Read.B3`
  * **Master Output:** `Slave 1 Pin A9 is HIGH`

* **Write Pin**
  Dynamically initializes the target pin as an output and drives it to the requested state.
  * **Syntax:** `1.Write.C13.High` or `2.Write.A5.Low`
  * **Master Output:** `Slave 1 Pin C13 written HIGH OK`

## Architecture Details

* **Dynamic I/O Expansion:** The slave nodes do not require hardcoded pin configurations in their `main()` functions. They decode the upper and lower nibbles of the incoming command to identify the target Port (A-I) and Pin (0-15), dynamically configuring the hardware registers on the fly.
* **Full-Duplex Synchronization:** The master utilizes dummy byte transmission (`0xFF`) to generate the clock pulses necessary to retrieve the slave's execution confirmation byte, aggressively clearing its `RXNE` flag to maintain buffer alignment.
* **Chip Select Handling:** The master operates with Software Slave Management (SSM) enabled, manually asserting PB11 or PB12. The slaves operate with Hardware NSS enabled, automatically entering receive mode when their PB9 pin is pulled low by the master.
