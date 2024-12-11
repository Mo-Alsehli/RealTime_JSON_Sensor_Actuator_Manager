# SmartEGAT Task

## 📖 Project Overview
The **SmartEGAT Task** is an embedded systems application developed for STM32 microcontrollers. 
It enables seamless communication with sensors and actuators using JSON commands via UART. 
The system is built on FreeRTOS to ensure real-time task management and efficiency.

## 🖥️ System Requirements
### Hardware
- STM32 microcontroller (STM32F103C8).
- Sensors:
  - **Temperature Sensor (LM35)**: For temperature measurements.
  - **Light Sensor (LDR)**: For light intensity readings.
- Actuator:
  - **Relay Actuator (5V)**: To control external devices.

### Software
- **Eclipse IDE** with GNU Arm Embedded Toolchain Or [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html).
- **[Proteus](https://www.labcenter.com/whatsnew/8.10/) (8.10) Simulation Software** .
- Libraries:
  - [FreeRTOS](https://www.freertos.org/Documentation/02-Kernel/01-About-the-FreeRTOS-kernel/01-FreeRTOS-kernel).
  - [cJSON](https://github.com/DaveGamble/cJSON)

## ⚙️ Architecture and Design
### Key Components
1. **Sensors and Actuators**:
   - Temperature Sensor [LM35](https://www.amazon.eg/-/en/Electro-Hub-Temperature-Sensor-LM35/dp/B091DK861W) to monitor ambient temperature.
   - Light Sensor (LDR) to measure light intensity.
   - Relay Actuator for switching external devices.

2. **Communication Interface**:
   - UART transmits and receives JSON messages between the STM32 microcontroller and the UART monitor.

3. **Task Management**:
   - FreeRTOS tasks are used for command parsing, storage, and execution.
   - Periodic Timers handle reporting on periodic sensor data.

4. **JSON Message Handling**:
   - JSON commands are used to control nodes and retrieve their status.
   - Nodes respond with JSON messages containing status or data.

### System Flow Diagram
```
Input (UART) ➡️ Parse Command ➡️ Execute Action ➡️ Output (UART Response)
```

## 🛠️ Functional Specifications
### JSON Command Formats
- **Enable Node:** `{ "command": "ENA", "nodeID": "<ID>", "data": null }`
- **Disable Node:** `{ "command": "DIS", "nodeID": "<ID>", "data": null }`
- **Activate Node:** `{ "command": "ACT", "nodeID": "<ID>", "data": "<value>" }`
- **Request Status:** `{ "command": "STA", "nodeID": "<ID>", "data": null }`
- **Set Duration:** `{ "command": "DUR", "nodeID": "<ID>", "data": "<seconds>" }`

### JSON Response Formats
- **Sensor Data:** `{ "nodeType": "NS", "nodeID": "<ID>", "data": "<value>" }`
- **Actuator Status:** `{ "nodeType": "NA", "nodeID": "<ID>", "data": "<value>" }`

## 🚀 Setup and Installation
1. **Firmware Setup:**
   - Download the Project From GITHUB.
   - Open the project in STM32cubeIDE.
   - Configure the project for your STM32F103C8 microcontroller.
   - Build and flash the firmware to the STM32.

3. **Hardware Setup:**
   - Build the circuit using the provided schematic.
   - Connect the temperature sensor, light sensor, and relay actuator to the STM32.

4. **Testing:**
   - Use a UART monitor to send JSON commands and observe responses.
   - Open Keil micro-vision and start a debugging Session.
   - [Video Demonstration](https://drive.google.com/drive/u/0/folders/131mmyU037Xqvxbwd14iwhPaWtUSHlOnH) for Testing.
   - ## STM32cubeIDE Configuration for Project Setup:
      1. Open CubeIDE -> click on file -> click open projects from file system.
      2. Make sure to add all Headers to the Compiler path.
         - Right-click on the project name -> click on C/C++ Build -> click on Settings -> choose tool Settings -> MCU GCC Compiler -> include paths.
         - Under MCU Settings Check Use float with printf.
         - Under C/C++ build under the Build Artifact Section make Artifact extension as (axf) for Keil micro-vision simulation.
      3.  Build project and go to Keil micro vision.
      4.  Open Keil micro vision and choose STM32F103C8 Board.
      5.  Open Keil micro Vision -> choose 'Project' from taskbar -> options for target -> output -> select folder for Objects -> put the .axf file path from the STM23 project.
      6.  choose 'Project' -> Debug -> check Use Simulator.
      7.  Start the Simulation session from the Debug Icon.

## ✅ Testing and Validation
### Test Scenarios
- Enable or disable nodes and confirm responses.
- Modify sensor reporting intervals and verify outputs.
- Activate actuators and observe operations.

### Example JSON Transactions
1. **Enable Temperature Sensor:**
   - **Tx:** `{ "command": "ENA", "nodeID": "0x80", "data": null }`
   - **Rx:** `{ "nodeType": "NS", "nodeID": "0x80", "data": "DONE" }`

2. **Request Light Sensor Status:**
   - **Tx:** `{ "command": "STA", "nodeID": "0x81", "data": null }`
   - **Rx:** `{ "nodeType": "NS", "nodeID": "0x81", "data": "122" }`

## 📂 Repository Structure
- **Firmware:**
  - Source code and drivers for STM32 peripherals.
- **Hardware:**
  - # Hardware components on Proteus:
     1. LM35.
     2. TORCH_LDR.
     3. STM32F103C6.
     4. Relay (5V).
     5. LAMP.
     6. Resistor.
     7. BC547 (Transistor).
     8. 1N4001 (Diod.
     9. Assemble the circuit as shown in the Picture.
  - Schematic diagram and bill of materials.
    ![Screenshot 2024-12-09 193641](https://github.com/user-attachments/assets/ff1ad263-7635-4afc-bd14-5c692abf70fd)
  - Proteus simulation files.

