# ESP32-C6 Micromouse (Arduino IDE starter)

Open `MicromouseESP32.ino` in Arduino IDE. Install **esp32 by Espressif Systems** board package and libraries **Adafruit VL53L0X**, **Adafruit MPU6050**, **Adafruit Unified Sensor**, and dependencies requested by Library Manager. Select the exact ESP32-C6 board and port, then Verify.

**Before powering motors:** Confirm your actual board exposes every listed pin (especially GPIO22/23); verify driver board model and that it accepts 3.3V control. VCC logic/sensors must match breakout ratings. Driver VM requires a *separate motor-rated supply*; a 2S battery is 8.4V fully charged and may overvoltage 3V/6V motors. Do not put the motors or driver VM on the ESP32 3V3 rail. Use a common ground. Verify battery wiring, buck output and polarity with a multimeter.

**Calibration required before autonomous driving:** `Config.h` contains PLACEHOLDER wheel diameter, encoder counts/revolution, wall threshold and PWM. With wheels off the floor, verify forward wheel polarity and both encoder signs. Verify MPU6050 yaw sign (positive left) and turning direction. A reversed gyro axis can cause a spin until timeout. Test VL53L0X address assignment and distance readings individually. Confirm that each physical cell move ends centred, with room to brake. This example uses basic encoder correction, not a tuned motion controller; high-speed runs are NOT safe/reliable without tuning.

Serial Monitor at 115200: send `g` to begin **only after calibration**. Keep the robot lifted for initial tests. Movement stops on sensor failure or timeout, but there is no physical emergency stop. For real use add an accessible power cutoff and test on a short clear track first.

The solver uses the existing 16x16 `Maze.h`, four centre goals, unknown-open exploration, start return and unknown-closed confirmed route. The maze is RAM-only: a real reset erases it. The code intentionally performs slow cell-by-cell movements even for a merged route until straight-line acceleration and braking are implemented.

**Limitations:** This is a hardware-adaptation starter, not verified on your exact driver, board or physical robot. Check the Adafruit VL53L0X installed library's `begin(address, debug, Wire*)` signature. `GPIO22/23` may be unavailable on certain C6 dev boards. `analogWrite` requires a compatible modern Arduino-ESP32 core. Some TB6612 modules require a STBY pin held high; your stated DRI0044 board must be verified against its actual pin labels.
