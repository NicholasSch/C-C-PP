# 🕹️ ESP32 Simon Says (Genius) Game Simulator

A reactive, state-machine driven replication of the classic electronic memory game "Genius" built specifically for the **ESP32** microcontroller architecture using the Arduino core C++ framework.

---

## 🔗 Live Interactive Simulation

You can test this entire embedded systems build immediately inside your browser without configuring physical hardware components:

👉 **[Launch Live ESP32 Wokwi Simulation Setup]([https://wokwi.com/](https://wokwi.com/projects/342477906400772690))**

---

## 📄 Project Overview

This firmware turns an **ESP32 platform** into an interactive sequence-matching cognitive memory trainer. The game tracks choices across 4 distinct LED indicators paired with native ESP32 GPIO input lines utilizing internal pull-up configurations.

The software logic operates as an asynchronous cyclic state loop:
1. **Standby State:** The game waits for any user button to be pressed while keeping all indicators illuminated. It builds a randomized 15-turn target puzzle array based on real-time hardware floating analog noise signals sampled from an un-connected ESP32 ADC channel.
2. **CPU Display State:** The engine flashes items incrementally up to the current round limit, testing the player's short-term recall.
3. **Player Evaluation State:** The game pauses to collect user button inputs, validating selections sequentially against the computer's memory bank using integrated software debouncing filters.

### Core Architecture Design
* **ESP32 ADC Entropy Seed Initialization:** Samples true electrical noise variations from an un-connected analog pin (`analogRead(0)`) to ensure the random sequence generation pattern changes completely on every boot.
* **Debounce State Filtering:** Implements contact-settling tracking filters to screen out mechanical button ripple spikes and prevent duplicate inputs on the ESP32 input lines.
* **Asymmetric Game State Control:** Switches smoothly between autonomous sequence reproduction modes and blocking player capture configurations.

---

## 🛡️ ESP32 Hardware Interconnection Protocol

The circuit schematic requires assigning discrete, native ESP32 GPIO channels. The mappings are selected to avoid strapping pins or restricted input-only channels (like GPIO 34-39), ensuring reliable boots:

| Component Profile | ESP32 Pin Assignment | Target Mode Configuration |
|:------------------|:--------------------:|:-------------------------:|
| 🔴 Red LED Pin    | GPIO 13              | `OUTPUT` Digital Drive    |
| 🔵 Blue LED Pin   | GPIO 12              | `OUTPUT` Digital Drive    |
| 🟡 Yellow LED Pin | GPIO 14              | `OUTPUT` Digital Drive    |
| 🟢 Green LED Pin  | GPIO 15              | `OUTPUT` Digital Drive    |
| 🔘 Red Button     | GPIO 33              | `INPUT_PULLUP` Active-LOW |
| 🔘 Blue Button    | GPIO 32              | `INPUT_PULLUP` Active-LOW |
| 🔘 Yellow Button  | GPIO 25              | `INPUT_PULLUP` Active-LOW |
| 🔘 Green Button   | GPIO 26              | `INPUT_PULLUP` Active-LOW |

---

## 📝 Functional Algorithm Pseudocode

This structured English pseudocode maps out the input evaluations and confirmation matching running inside the script loops:

```text
FUNCTION checkButtonState(buttonPin, buttonFlag):
    // Read active low pullup hardware configuration state from ESP32 GPIO
    IF readDigitalChannel(buttonPin) EQUALS LOW AND buttonFlag EQUALS 0 THEN:
        buttonFlag = 1
        ExecuteSettleDelay(7ms) // Ignore mechanical bounce ripples
        RETURN True
    ELSE IF readDigitalChannel(buttonPin) EQUALS HIGH AND buttonFlag EQUALS 1 THEN:
        buttonFlag = 0
        ExecuteSettleDelay(7ms)
        RETURN False
    RETURN False

FUNCTION executePlayerTurn(currentRoundOffset):
    inputsRecorded = 0
    
    WHILE inputsRecorded LESS THAN currentRoundOffset:
        IF checkButtonState(redButton) EQUALS True THEN:
            playerMemoryArray[inputsRecorded] = 0
            SetChannelHigh(redLed)
            ExecuteSettleDelay(200ms)
            inputsRecorded = inputsRecorded + 1
            
        ELSE IF checkButtonState(blueButton) EQUALS True THEN:
            playerMemoryArray[inputsRecorded] = 1
            SetChannelHigh(blueLed)
            ExecuteSettleDelay(200ms)
            inputsRecorded = inputsRecorded + 1
            
        ELSE IF checkButtonState(yellowButton) EQUALS True THEN:
            playerMemoryArray[inputsRecorded] = 2
            SetChannelHigh(yellowLed)
            ExecuteSettleDelay(200ms)
            inputsRecorded = inputsRecorded + 1
            
        ELSE IF checkButtonState(greenButton) EQUALS True THEN:
            playerMemoryArray[inputsRecorded] = 3
            SetChannelHigh(greenLed)
            ExecuteSettleDelay(200ms)
            inputsRecorded = inputsRecorded + 1
```

---

## 🚀 Execution Instructions (Local Deployment)

If compiling offline instead of using the live browser simulator, ensure you have the **ESP32 Arduino Core** libraries installed in your preferred environment:

### Using Arduino IDE
1. Open `Preferences` and add `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json` to the **Additional Boards Manager URLs**.
2. Go to `Tools > Board > Boards Manager`, search for **esp32**, and install the official package.
3. Select your exact development board (e.g., `ESP32 Dev Module`) and target COM port.
4. Upload `sketch.ino`.
