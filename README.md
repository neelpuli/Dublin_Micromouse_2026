# Micromouse 2026

A maze-solving robot built for the Dublin Micromouse 2026 event. It runs on an ESP32-C6, senses walls with three VL53L0X time-of-flight sensors, tracks turns with an MPU-6050 gyro, measures distance with wheel encoders, and finds the centre of the maze using a flood-fill algorithm.

## Repository layout

```
MicromouseESP32/          Final robot firmware (open MicromouseESP32.ino in Arduino IDE)
  MicromouseESP32.ino     Serial command menu and main loop
  Config.h                Pin map and calibration values
  Maze.h, Mouse.h         Flood-fill solver and navigation state machine
  ESP32Robot.*            Hardware layer: walls, forward moves, gyro turns, safety halts
  Sensors.*               Three VL53L0X sensors (Pololu library)
  IMU.*                   MPU-6050 gyro read directly over I2C (no library)
  Motors.*, Encoders.*    DRI0044 motor driver and quadrature encoders
  RobotIO.h               Interface the solver uses to talk to the robot
sensor_tests/ToF_Sensors/ Standalone sensor test sketch (built up one sensor at a time)
```

## Hardware and wiring

| Part | Connection |
|------|------------|
| Motor driver DRI0044 | Left: DIR1 → GPIO0, PWM1 → GPIO2 · Right: DIR2 → GPIO3, PWM2 → GPIO10 |
| I2C bus (all sensors + IMU) | SDA → GPIO6, SCL → GPIO7 |
| VL53L0X XSHUT | Left → GPIO18, Front → GPIO1, Right → GPIO20 |
| Encoders | Left: A → GPIO21, B → GPIO22 · Right: A → GPIO11, B → GPIO23 |

Note: the standalone sensor test used GPIO19 for the front XSHUT; the final firmware uses GPIO1 to match the event wiring.

## Setup

1. Install the **esp32 by Espressif Systems** board package and select your ESP32-C6 board.
2. Install **VL53L0X by Pololu** from Library Manager (not the Adafruit one). No IMU library is needed.
3. Open `MicromouseESP32/MicromouseESP32.ino`, then upload.
4. Keep the robot still for about a second after power-up while the gyro calibrates.

## Serial commands (115200 baud)

| Key | Action |
|-----|--------|
| `s` | Show sensor distances, encoder counts, gyro rate |
| `z` | Zero the encoder counts |
| `f` | Drive one cell forward |
| `l` / `r` | Turn left / right 90° |
| `g` | Start solving the maze |
| `x` | Stop |

## Calibration

The values at the bottom of `Config.h` must be tuned on the real robot: wheel diameter, encoder counts per wheel revolution (turn a wheel once by hand and read it with `s`), the wall threshold, motor PWM, and `TURN_STOP_DEG`. If a wheel spins backwards on `f`, set `INVERT_LEFT` or `INVERT_RIGHT`.

## How the solver works

The robot treats unknown walls as open and floods distances from the four centre cells, always stepping to the neighbour with the lowest distance. After reaching the centre it explores back to the start. When the shortest route through confirmed-open walls equals the optimistic shortest route, it performs a run using only known walls.

## Development history

All work was done on 26 September 2026 over about six hours. Each branch shows one strand of the work:

| Branch | What it shows |
|--------|---------------|
| `sensor-tests` | Left sensor alone, then left + front, then all three |
| `floodfill-algorithm` | Ross's flood-fill solver and hardware drivers, as first received by email |
| `integration` | Combining both: the solver switched to the Pololu sensor start-up proven in the tests, a direct-register gyro driver, the event pin map, and a serial test menu |

This git history was reconstructed on 27 September 2026 from the saved files of each stage. The order of the stages is accurate; commit times within the session are approximate.

## Team

| Member | Contribution |
|--------|--------------|
| Neel Puli | Distance sensor bring-up and testing |
| Oran McGrath | Distance sensor bring-up and testing |
| Harsh Rajesh Kumar | Distance sensor bring-up and testing |
| Ross Therville | Flood-fill solver and original hardware layer; final integration |
| Safwan Islam | Final integration |
