MICROMOUSE PLAN

Intro:
First, your score is your best single run, you get ten minutes and up to five runs, and a slow exploratory first run doesn't count against you

Second, the simple "follow the left wall" robot won't work here. The goal block in the centre is an island, so a wall follower loops around the outside forever and never gets in. You need a mouse that builds a map and searches it, which is what flood fill does. 

Third, the algorithm is the easy half. The hard half is making the robot actually go where the algorithm says and knowing where it is while it does. Teams will mostly fail on motion and sensing, not on maze logic. 

So a realistic, strong goal for a first-time team is a mouse that reliably reaches the centre using flood fill, then banks one safe speed run. The organisers say it bluntly: a mouse that reliably finishes in 60 seconds beats one that would have done 12 and didn't. There are also separate awards for design and reliability, and a disciplined beginner team can win the reliability one. 

Preparation:
The maze-solving code (fully testable in a simulator), pre-written hardware code, and a team plan. 

With five people working in parallel

<img width="827" height="1167" alt="image" src="https://github.com/user-attachments/assets/5586f694-baa6-409f-8e50-802a543089dd" />



Orientation: From the reading list, watch Veritasium's micromouse video first, then the IEEE Bruins flood fill lecture. The S1–S5 pages are each about a five-minute read. Everyone should read all of them, not just their own area 


Toolchain. Install Arduino IDE 2 with the Espressif ESP32 board package (version 3.x), select the ESP32-C6 board, and install the Pololu VL53L0X library and an MPU6050 library. The organisers promised a pre-recorded video series on driving the sensors with the ESP32, plus setup docs published in advance. I couldn't find those on the site yet. 




Track 1: The maze brain (Ross, Safwan)

This is the part can be finished completely before Saturday. Use the mms simulator. It lets you test maze-solving code without a robot, shows known and unknown walls, can simulate a crash-and-reset, and works in any language. There's a Java template, but write it in C++ using the mms-cpp template. The ESP32 is programmed in Arduino C++, which is close enough to Java that your OOP knowledge carries over, and the code will move straight onto the robot. 
github

The key design choice is to keep the maze logic separate from movement. Write a small interface with functions like wallLeft(), wallFront(), wallRight(), moveForward(n), turnLeft() and turnRight(). In the simulator these call mms. On Saturday you replace them with your real motor and sensor code, and nothing else changes.

Build the features in this order:

Flood fill to the goal. Treat walls you haven't seen yet as open. Re-flood the whole maze every time you find a new wall. An ESP32 re-floods all 256 cells in well under a millisecond, so don't bother with clever incremental updates. Remember the goal is the four centre cells, not one cell. 
ucdelecsoc
Return to start. Flood toward the start cell instead. The return trip sees walls from the other side for free. 
ucdelecsoc
Speed-run path. Treat unknown walls as walls and turn the route into a list of moves. Merge consecutive forward moves into a single "forward N" so the motor code can speed up on straights.
Stretch goal: decide when to stop exploring. Flood once with unknowns open and once with unknowns walled. If the two route costs match, stop exploring and race. 
ucdelecsoc

Test against real competition mazes. The micromouseonline/mazefiles repo has decades of them, so don't just test on the default maze. Skip the turn-cost weighting and diagonals. Saturday's bottleneck won't be here. 
ucdelecsoc



Track 2: The hardware code (Neel, Oran, Harsh)

Write every module before you have the hardware, so Saturday is "flash and fix" rather than "write from scratch." Also write a tiny test sketch for each part (I2C scan, spin one motor, print encoder counts, print distances, print gyro), since the organisers' recommended bring-up order is to test one part at a time and commit after every stage that works. 
ucdelecsoc

Pin map. Put all pin numbers in one file, pins.h. You need 2 PWM and 2 direction outputs for the motor driver, 4 encoder inputs, a shared I2C bus and 3 XSHUT outputs. Avoid the strapping pins (GPIO 4, 5, 8, 9, 15 on the DevKitC-1), and remember GPIO 12 and 13 are the USB pins. 
ucdelecsoc
ucdelecsoc

Motors. This is a trap for tutorials and AI assistants alike. In the ESP32 board package version 3.x, ledcAttach replaced the old ledcSetup/ledcAttachPin pair, ledcWrite now takes just the pin and duty, and you no longer assign channels yourself. Most older examples online won't compile. Add a maximum-PWM constant as a safety cap. 
Random Nerd Tutorials

Encoders. The encoder A and B signals tell you distance (pulse count), speed (pulse frequency) and direction (which channel leads). Don't trust a ticks-per-revolution figure from a datasheet; measure it. The ESP32Encoder library uses the chip's hardware pulse counter, but its docs list ESP32 and ESP32-C2 as supported and don't mention the C6. Have a plain interrupt-based counter ready as a fallback. It's easy to write: use volatile counters and attachInterrupt. 
ucdelecsoc
github

Distance sensors. All three VL53L0X sensors start at the same I2C address, and address changes are lost on reset, so you reassign them to 0x30, 0x31 and 0x32 on every boot using the XSHUT pins. Two tips for speed: use continuous-ranging mode so the three sensors measure at the same time, and drop the timing budget from the default of about 33 ms to 20 ms. 
ucdelecsoc
mbed

IMU. The IMU is an MPU-6050 at address 0x68. Calibrate the gyro by averaging readings for a few seconds at power-up while the mouse is completely still. Then add up the gyro's Z-axis readings over time to get your heading. 
ucdelecsoc

You can test some of this tonight. The online simulator Wokwi simulates the ESP32-C6-DevKitC-1 and includes an MPU6050 part, so the IMU code and PWM output can be checked in a browser. 
wokwi
wokwi

Motion. Stub out the two functions everything else depends on: driveCells(n) and turn(degrees). Control motion by distance, not by time. Accelerate at a fixed rate to a cruise speed, hold, then decelerate to stop at the target, so the careful run and the fast run are the same code with a different speed setting. 
ucdelecsoc

If you use an AI assistant for any of this, give it your pin map and exact board revision, check any library functions it uses against the installed version, and ask for small changes rather than rewrites. 
ucdelecsoc

Track 3: Build and power (Neel, the hardware lead)

Chassis layout. These decisions are permanent once you solder:

Put the wheel axle under the centre of rotation, keep the mouse light, and keep it narrow. Where the battery sits affects handling. 
ucdelecsoc
Corridors are 168 mm between walls, and walls are 50 mm high, so aim much narrower than the corridor. 
ucdelecsoc
Angle the side sensors slightly forward, aimed at mid-height on the wall, so they never see the floor or over the top. Mount them adjustably at first, with hot glue or a screw, so you can tweak them. 
ucdelecsoc

Power. There are two things to check at the briefing:

The battery is a 2S pack, 8.4 V when full, but the motors are rated for 6 V. Use a suitable motor supply or an approved limit, so ask the organisers what they approve. 
ucdelecsoc
ucdelecsoc
Set the buck converter to 5.00 V with nothing connected to it, and never connect USB and the external 5 V supply at the same time. 
ucdelecsoc

Wiring and soldering. Turn the hardware pages into one wiring table so Saturday's wiring is copying, not deciding. If anyone can get even 30 minutes on a soldering iron before Saturday, do it. Header pins are what you'll be soldering most.

On the day: building a mouse that finishes

Follow the organisers' six-hour playbook exactly, with a named owner for each stage:

Time	Goal
Hour 0–1	Get motors turning and encoder counts changing the right way
Hour 1–2	Drive exactly one cell and turn exactly 90°
Hour 2–3	See walls reliably
Hour 3–4	Solve the maze slowly
Hour 4–5	Add the return trip and a speed run
Hour 5–6	Stop adding features and rehearse

This comes from the organisers' hour-by-hour plan. Hour 1–2 is the most important. Repeat the one-cell drive and the 90° turn ten times each and measure how far off they end up, because everything later depends on them. 
ucdelecsoc
ucdelecsoc

Use only pivot turns (stop, rotate, go). Smooth curved turns are for teams with time to spare.

You need three corrections working together, because the wheels alone will drift. Use the gyro for heading. Use the difference between the left and right walls to stay centred, but switch it off where a wall disappears. And use every visible front wall to reset your distance error. 
ucdelecsoc

Wall detection needs care. Use two thresholds rather than one for "wall present" and "wall absent", and read walls at the same point in every cell. One false wall sends the mouse confidently into a dead end. The corner posts also cause sensor spikes you must not read as walls. 
ucdelecsoc
ucdelecsoc

Keep speeds modest. The motors run at about 500 rpm with no load. That's my rough estimate of under 1 m/s flat out on a typical small wheel, so a slow search and a moderate first speed run is realistic. 
ucdelecsoc

Two practical details. Add a button press at startup that picks the run mode (search, safe speed, fast speed), so you're never re-flashing code during your ten minutes. And make sure someone placing the mouse in a hurry can't put it down facing the wrong way. 
ucdelecsoc

Race strategy

Plan your five runs as a ladder:

Search slowly and reach the centre.
Run a conservative speed run to get a time on the board.
Go faster.
and 5. Push until it breaks.

Once you have a time, risk is cheap. Before that, it's ruinous. Teams lose by trying to win on run one. 
ucdelecsoc

If a judge lets you recover a crashed mouse, its memory of the maze gets wiped, so decide in advance whether that trade is worth it. Charge the battery between sessions. 
ucdelecsoc

The diagonal practice maze is fun to try, but get the normal grid speed run reliable first. 
ucdelecsoc

What to learn, and what to skip:

The Java-to-Arduino gap is smaller than it looks. The main differences:
Fixed-size arrays and explicit integer sizes like uint8_t and int16_t.
volatile for any variable changed inside an interrupt.
A non-blocking loop() that checks millis() instead of pausing with delay().
Splitting code into .h and .cpp files.

Beyond that, it's enough to understand PWM, I2C and interrupts at a conceptual level, and PID control intuitively (in practice, a P term plus a little D gets you most of the way).

Skip for now: diagonals, curved turns, turn-cost flood fill, incremental flooding, Wi-Fi debugging and ESP-IDF.
