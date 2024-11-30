# Digital and Microprocessor Techniques Projects

Welcome to the **Digital and Microprocessor Techniques** repository. This repository contains a collection of projects developed using C++ in the Arduino IDE, focusing on microcontroller programming and interfacing with various peripherals. The projects were implemented on **EDU-TAR** boards, utilizing components such as buzzers, LEDs, 7-segment displays, LCDs, buttons, potentiometers, photoresistors, and more.

---

## Repository Overview

This repository is organized into several projects, each demonstrating different aspects of microcontroller programming and digital interfacing:

1. **Counter with 7-Segment Display and Buttons** (`Counter_7Segment_Display_Buttons.cpp`)
2. **Timer and Interrupts with LED Control** (`Timer_Interrupts_LED_Control.cpp`)
3. **ADC, LCD Display, and Relay Control** (`ADC_LCD_Relay_Control.cpp`)
4. **UART Communication and ADC Monitoring** (`UART_Communication_ADC_Monitoring.cpp`)
5. **Temperature Sensor with RGB LED and LCD Display** (`Temperature_Sensor_RGB_LED_LCD.ino`)

---

## Project Descriptions

### 1. Counter with 7-Segment Display and Buttons

- **File**: `Counter_7Segment_Display_Buttons.cpp`
- **Description**: This project implements a digital counter displayed on a 4-digit 7-segment display. The counter can be incremented, decremented, reset, or started via buttons connected to the microcontroller. It also includes buzzer control and LED indications.
- **Features**:
  - Display numbers from 0 to 9999 on a 7-segment display.
  - Use buttons to increment (`SW2`), decrement (`SW1`), reset (`SW3`), or start an automatic countdown (`SW4`).
  - Visual and audible feedback using LEDs and a buzzer.

### 2. Timer and Interrupts with LED Control

- **File**: `Timer_Interrupts_LED_Control.cpp`
- **Description**: Demonstrates the use of timers and external interrupts. The project controls an LED based on button inputs and utilizes a timer interrupt to update a display.
- **Features**:
  - Configure Timer1 for CTC mode to generate interrupts at specific intervals.
  - Use external interrupts to respond to button presses.
  - Display counter values on a 7-segment display.

### 3. ADC, LCD Display, and Relay Control

- **File**: `ADC_LCD_Relay_Control.cpp`
- **Description**: This project reads analog values from a potentiometer and a photoresistor using the ADC. It displays the readings on an I2C LCD and controls a relay (or LED) based on light intensity.
- **Features**:
  - Initialize and use the ADC to read analog inputs.
  - Interface with an I2C LCD to display voltage readings.
  - Control a relay or LED based on the comparison of potentiometer and photoresistor values.

### 4. UART Communication and ADC Monitoring

- **File**: `UART_Communication_ADC_Monitoring.cpp`
- **Description**: Implements UART communication to send and receive data between the microcontroller and a PC. It monitors ADC values and allows control of an LED via serial commands.
- **Features**:
  - Initialize UART for serial communication.
  - Transmit ADC readings over UART.
  - Receive commands to control an LED and provide status updates.

### 5. Temperature Sensor with RGB LED and LCD Display

- **File**: `Temperature_Sensor_RGB_LED_LCD.ino`
- **Description**: An Arduino sketch that reads temperature data from a sensor (simulated using a potentiometer), displays the temperature on an I2C LCD, and changes the color of an RGB LED based on the temperature range.
- **Features**:
  - Read analog input from a potentiometer representing temperature.
  - Calculate voltage and temperature values.
  - Display readings on an LCD.
  - Control an RGB LED to visually represent temperature ranges.

---

## Hardware Components

- **EDU-TAR Board**: Microcontroller development board used for all projects.
- **7-Segment Display**: Used in projects 1 and 2 to display numerical values.
- **Buttons/Switches**: For user input to control counters and LEDs.
- **Buzzer**: Provides audible feedback in project 1.
- **LEDs**: Used for visual indicators and relay simulation.
- **LCD Display (I2C)**: Used in projects 3 and 5 to display text and readings.
- **Potentiometer**: Acts as an adjustable voltage divider, simulating sensor inputs.
- **Photoresistor**: Measures light intensity in project 3.
- **RGB LED**: Displays different colors based on temperature in project 5.
- **Temperature Sensor**: Simulated using a potentiometer in project 5.
