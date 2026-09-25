# Micromouse flood-fill solver (MMS C++ template extension)

## Setup (Windows)
1. Download and extract https://github.com/mackorone/mms-cpp . Copy its **API.cpp** and **API.h** into this directory. They are unmodified upstream files and are not bundled here.
2. Install a C++17 compiler (e.g. MinGW-w64) and compile from this directory:
   `g++ -std=c++17 -O2 -Wall -Wextra Main.cpp API.cpp -o mouse.exe`
3. Download the Windows MMS simulator from https://github.com/mackorone/mms/releases . Open it, add an algorithm, set its directory to this folder, its build command to the above command, and its run command to `mouse.exe` (or its absolute path).
4. Load a competition maze from https://github.com/micromouseonline/mazefiles and run it. Check that walls display, center is reached, robot returns to start, and the confirmed-only speed run reaches center.
5. To unit-test the map/flood/route logic without MMS: `g++ -std=c++17 test.cpp -o test.exe` and run `test.exe`.

## Design
- `Maze.h`: 16x16 wall map, reciprocal known/wall flags, 4-center-cell flood fill, confirmed-edge route and shortest-route equality test.
- `RobotIO.h`: hardware-independent wall sensors and movement interface.
- `MmsRobot.h`: MMS-specific adapter. On the ESP32, implement `RobotIO` with your sensors/motors and keep `Maze.h`.
- `Main.cpp`: sense, update wall map, full flood, move, return, explore until a shortest path is confirmed, then merged-forward speed run.
- `test.cpp`: basic unit tests. Does **not** establish competition-maze or physical-robot performance.

## Important limitations
- Simulator reset handling preserves the map in process memory, not in nonvolatile storage. On a real power cycle, persist it to ESP32 flash/NVS if desired.
- MMS reset is polled between commands. A reset in the middle of a blocking movement may require additional handling in the hardware adapter.
- No turn-time weighting, diagonals, speed profile, or hardware control is included.
- The speed run is only triggered when the known-only shortest path has the same *cell count* as the optimistic shortest path. This guarantees a shortest route in the discovered model, but not a minimum-time route.
- Core unit tests passed locally; MMS GUI and competition-maze runs must be completed on your computer.
