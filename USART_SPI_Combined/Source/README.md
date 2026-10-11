# STM32 Bare-Metal SPI Master-Slave Control

This project implements a multi-slave SPI communication system using bare-metal C programming on STM32F446RE microcontrollers. It allows a single **Master** microcontroller to remotely control and read the GPIO pins of up to two **Slave** microcontrollers via a serial command-line interface (CLI).

##  Features
* **Interactive CLI:** Control remote slave pins from your computer using a Serial Terminal (UART).
* **Multi-Slave Support:** Master can dynamically select between Slave 1 and Slave 2.
* **Pin Protections:** Slaves actively prevent the master from altering critical SPI and SWD pins, preventing accidental disconnection or bricking.
* **Robust Error Handling:** Includes SPI synchronization, timeout recovery, and buffer overflow protection.

---

##  Hardware Setup & Wiring

You will need **three STM32F446RE Nucleo boards** (1 Master, 2 Slaves). 

### 1. SPI Bus Connections
All three boards must share the same SPI bus (SCK, MISO, MOSI) and a **common Ground**.

| Master Pin | Slave 1 Pin | Slave 2 Pin | Function |
| :--- | :--- | :--- | :--- |
| **PB10** | PB10 | PB10 | SCK (Clock) |
| **PB14** | PB14 | PB14 | MISO (Master In, Slave Out) |
| **PB15** | PB15 | PB15 | MOSI (Master Out, Slave In) |
| **GND** | GND | GND | Common Ground |

### 2. Chip Select (NSS) Connections
The Master uses separate GPIO pins to select which slave it wants to talk to.

| Master Pin | Slave Pin | Function |
| :--- | :--- | :--- |
| **PB11** | **PB9** (on Slave 1) | NSS for Slave 1 |
| **PB12** | **PB9** (on Slave 2) | NSS for Slave 2 |

---

## Flashing & Powering the Boards (Important!)

To prevent USB power conflicts and ensure a stable SPI bus, follow this exact sequence when setting up the hardware:

### Step 1: Program the Slaves
1. Plug **Slave 1** into your computer via USB.
2. Compile and flash `slave1.c` (or `slave2.c`) to the board.
3. **Disconnect Slave 1 from the computer.**
4. Power Slave 1 using an **external 5V power source** (e.g., connecting a 5V adapter to the `5V` and `GND` pins, or using a USB wall charger).
5. Repeat the exact same process for **Slave 2**. Both slaves should now be running independently on external power.

### Step 2: Program the Master
1. Connect the **Master** board to your computer via USB.
2. Compile and flash `master.c` to the Master board.
3. **Leave the Master connected to your PC.** Its USB connection will provide power *and* act as the Serial Virtual COM port for the CLI interface.

>  **Warning:** Ensure all three boards have their `GND` pins connected together before attempting communication!

---

##  How to Use the Command Line Interface

Once everything is powered and connected, open a Serial Terminal program on your PC (such as Tera Term, PuTTY, or the Arduino Serial Monitor). 

**Serial Port Settings:**
* **Baud Rate:** 115200
* **Data Bits:** 8
* **Parity:** None
* **Stop Bits:** 1

You will be greeted with the prompt:
```text
Type your command.:
>
