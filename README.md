# SmartEGAT Task

## 📖 Project Overview
The **SmartEGAT Task** is an embedded systems application developed for STM32 microcontrollers. 
It enables seamless communication with sensors and actuators using JSON commands via UART. 
The system is built on FreeRTOS to ensure real-time task management and efficiency.

## 🖥️ System Requirements
### Hardware
- STM32 microcontroller.
- Sensors:
  - **Temperature Sensor (LM35)**: For temperature measurements.
  - **Light Sensor (LDR)**: For light intensity readings.
- Actuator:
  - **Relay Actuator**: To control external devices.

### Software
- **Eclipse IDE** with GNU Arm Embedded Toolchain (or equivalent).
- **Proteus Simulation Software**.
- Libraries:
  - FreeRTOS
  - cJSON

## ⚙️ Architecture and Design
### Key Components
1. **Sensors and Actuators**:
   - Temperature Sensor (LM35) to monitor ambient temperature.
   - Light Sensor (LDR) to measure light intensity.
   - Relay Actuator for switching external devices.

2. **Communication Interface**:
   - UART is used to transmit and receive JSON messages between the STM32 microcontroller and the UART monitor.

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
   - Open the project in Eclipse IDE.
   - Configure the project for your STM32 microcontroller.
   - Build and flash the firmware to the STM32.

2. **Hardware Setup:**
   - Build the circuit using the provided schematic.
   - Connect the temperature sensor, light sensor, and relay actuator to the STM32.

3. **Testing:**
   - Use a UART monitor to send JSON commands and observe responses.

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
  - Schematic diagram and bill of materials.
    ![Screenshot 2024-12-09 193641](https://github.com/user-attachments/assets/ff1ad263-7635-4afc-bd14-5c692abf70fd)
  - Proteus simulation files.



