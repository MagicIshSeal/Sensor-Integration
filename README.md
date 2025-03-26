# Sensor Integration Project

This project is designed to integrate multiple sensors and actuators with an ESP32 microcontroller. It includes functionalities for logging sensor data, controlling servos, and displaying information on an OLED screen. The code is modular and well-documented, making it beginner-friendly.

## General Layout of the Code

The project is divided into several header files (`.h`) located in the `/src` folder. Each file encapsulates specific functionality:

- **`logger.h`**: Handles sensor initialization, calibration, and data logging.
- **`NLG.h`**: Manages the nose landing gear (NLG) system, including servo control for landing gear and hatch.
- **`smoothservo.h`**: Provides a class for smooth servo control.
- **`state.h`**: Manages the RGB LED and OLED display for visual feedback.
- **`cargo_code.h`**: Controls a cargo bay system with servos for opening/closing and balloon dropping.
- **`test.h`**: Contains test code for servo control using potentiometers.

> **Note**: Each `.h` file in the `/src` folder contains inline comments that supplement the explanations provided here. These comments are designed to make the code easier to understand and follow.

The main program (`main.cpp`) initializes all components and runs their respective loops.

## How the Code Works

1. **Setup Phase**:

   - Initializes sensors, servos, and the OLED display.
   - Configures timers and other peripherals.

2. **Loop Phase**:
   - Continuously logs sensor data.
   - Updates the state of the landing gear and cargo bay based on user input.
   - Provides visual feedback via the OLED and RGB LED.

## Using the Code with Arduino IDE

### Step 1: Install Required Libraries

The project uses several libraries. You can install them in the Arduino IDE via the Library Manager (`Sketch > Include Library > Manage Libraries`) or download them from their respective GitHub repositories:

- [ESP32Servo](https://github.com/madhephaestus/ESP32Servo)
- [SSD1306Ascii](https://github.com/greiman/SSD1306Ascii)
- [TinyGPSPlus](https://github.com/mikalhart/TinyGPSPlus)
- [Adafruit BMP280 Library](https://github.com/adafruit/Adafruit_BMP280_Library)
- [FRLibBasics](https://github.com/josmeuleman/FRLibBasics)
- [FRLibIntegration](https://github.com/josmeuleman/FRLibIntegration)

### Step 2: Configure the Arduino IDE

1. Install the ESP32 board package:

   - Go to `File > Preferences`.
   - Add the following URL to the "Additional Board Manager URLs" field: `https://dl.espressif.com/dl/package_esp32_index.json`.
   - Open `Tools > Board > Boards Manager`, search for "ESP32," and install it.

2. Select the correct board and port:
   - Go to `Tools > Board` and select your ESP32 board (e.g., "DOIT ESP32 DEVKIT V1").
   - Go to `Tools > Port` and select the port your ESP32 is connected to.

### Step 3: Upload the Code

1. Copy all `.h` files from the `/src` folder into the Arduino IDE's sketch folder.
2. Open `main.cpp` in the Arduino IDE.
3. Verify and upload the code to your ESP32.

### Step 4: Connect the Hardware

- **Sensors**: Connect the sensors (e.g., GPS, BMP280) to the ESP32 as per their datasheets.
- **Servos**: Connect the servo motors to the specified pins in the code.
- **OLED**: Connect the OLED display to the I2C pins of the ESP32.

### Step 5: Run the Code

- Open the Serial Monitor (`Tools > Serial Monitor`) to view logs.
- Interact with the system using buttons or switches as configured in the code.

## Explanation of Each Header File

### `logger.h`

- **Purpose**: Initializes sensors, performs calibration, and logs data.
- **Usage**: Call `loggerSetup()` in `setup()` and `loggerLoop()` in `loop()`.
- **Key Features**:
  - Supports multiple sensors (e.g., GPS, IMU, Pitot tube).
  - Displays calibration and status messages on the OLED.

### `NLG.h`

- **Purpose**: Controls the nose landing gear system.
- **Usage**: Call `NLGSetup()` in `setup()` and `NLGLoop()` in `loop()`.
- **Key Features**:
  - Smoothly retracts/extends landing gear and opens/closes the hatch.
  - Uses a tri-state switch for user input.

### `smoothservo.h`

- **Purpose**: Provides smooth control for servo motors.
- **Usage**: Create `SmoothServo` objects and use methods like `SetTargetStart()` and `Update()`.
- **Key Features**:
  - Limits servo movement speed for smooth transitions.
  - Supports setting target positions dynamically.

### `state.h`

- **Purpose**: Manages the RGB LED and OLED display.
- **Usage**: Call `OLEDSetup()` in `setup()` and use `Message()` to display messages.
- **Key Features**:
  - Displays messages and progress bars on the OLED.
  - Changes LED color based on system state.

### `cargo_code.h`

- **Purpose**: Controls the cargo bay system.
- **Usage**: Call `servoSetup()` in `setup()` and `servoLoop()` in `loop()`.
- **Key Features**:
  - Opens/closes the cargo bay.
  - Drops balloons in sequence using a servo.

### `test.h`

- **Purpose**: Provides test code for servo control using potentiometers.
- **Usage**: Uncomment `servoSetup()` and `servoLoop()` in `main.cpp` to test.
- **Key Features**:
  - Maps potentiometer values to servo angles.
  - Prints servo angles to the Serial Monitor.

## Additional Notes

- The code includes detailed comments to help you understand its functionality.
- For more information on concepts like I2C, PWM, and tri-state switches, refer to the following resources:
  - [I2C Communication](https://learn.sparkfun.com/tutorials/i2c)
  - [PWM for Servo Control](https://www.arduino.cc/en/Tutorial/LibraryExamples/Sweep)
  - [Tri-State Switches](https://en.wikipedia.org/wiki/Three-state_logic)

Happy coding!
