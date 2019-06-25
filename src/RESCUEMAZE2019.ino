/***************************************************
  TODO list-
  1. move 1 tile function                     DONE
  2. change encoder loop to function          DONE
  3. implement rescue mechanism               DONE
  4. Fix wall align using PID                 DONE
  5. Fix encoder movement using PID           DONE
  6. Implement Heated victim code             DONE
  7. try basic wall follow algorithm          DONE
  8. Depth first search algorithm             DONE
  9. Visual victim Identification             DONE
  10. Turn 90 degree using mpu6050            DONE
  11. Implement colour sensor                 DONE
  12. Add Heated victim code to main loop
  13. Implement visual victim code
  14. Add visual victim code to main loop
  15. Add ramp detection
  16. Lack of progress switch
  17. Fix next tile detection
***************************************************/

#include "constants.h"
#include "libraries.h"
#include "objects.h"
#include "variables.h"
//#include "Stack.h"

// StackArray <int> stack1;
// StackArray <int> stack2;

// stack.push(s_cell[0]);
// stack.pop();

void setup() {

#ifdef DEBUG
  beginSerialUSB();
#else
  delay(100);
#endif

  // Setup Input and Output pins
  setupIO();

  // Enable External interrupts on encoder pins
  attachInterrupt(digitalPinToInterrupt(encoder0PinA), doEncoderR, FALLING);
  attachInterrupt(digitalPinToInterrupt(encoder1PinA), doEncoderL, FALLING);

  beep(50);

  // Init Servo
  initServo();

  // begin IMU functions
  beginMotion();

  // Calibrate Gyroscope
  CalibrateMPU6050(50);

  // Calibrate Accelerometer
  delay(100);
  CalibrateMPU6050_Acc(50);

  // setup VL53L0x tof sensors
  setupTOF();

  // begin MLX90614 sensor
  beginMLX();

  // start neopixel
  beginNeopixel();

  // Clear pixels
  clearPixels();

  // begin Oled function
  oledbegin();

  beep(50);
  // start parallel loops
  Scheduler.startLoop(MLXloop);    // constantly read temperatures
  Scheduler.startLoop(IMUloop);    // Calculate Gyro roll pitch and yaw reading
  Scheduler.startLoop(bumpLoop);   // detect left and right bumps
  Scheduler.startLoop(colourLoop); // Constantly detect tile colour

  delay(200);
  beep(50);

  // Wait until a signal is given
  waitForSignal();

  // Intialize grid variables
  COUNT = 1;
  gridX = 0;
  gridY = 0;
  HEAD = 3; // Start facing west direction
  // while (1) {
  //   SerialUSB.println(accY * 3);
  //   delay(50);
  // }
} // end setup part

/***************************************************
  DFS algorithm check sequence
    0. update x, y coordinates of current tile
    1. set walls
    2. set heading
    3. if deadend then backtrack to last node
    4. else check next tile
    5. if next tile is not visited then move forward and count++
    6. else backtrack to last node
***************************************************/

// Start main loop
void loop() {
  do {
    // update x, y coordinates of current tile
    cell[COUNT].x = gridX;
    cell[COUNT].y = gridY;

    clearScreen();
    // Display coordinates on Oled
    displayPos(0, 0, "curr:", gridX, gridY);

    // set walls
    setWalls();

    //**********************
    // Add victim check code here
    // for both visual and heated victims
    //**********************

    // set heading
    bool headingResult = setHeading();
    bool nextTileFound = nextTile(cell[COUNT].x, cell[COUNT].y);
    if (nextTileFound)
      headingResult = setHeading();
    delay(300);

    if (headingResult) {
      // if available way and next tile is empty then move forward and count++
      indicatePath(FRONT);
      moveStraight(300);
      ramp(1);
      COUNT++;
    } else { // else backtrack to last node
      // backtrack code here and count-- until last node
      if (cell[COUNT].backWay >= 0) {
        orient(cell[COUNT].backWay);
      } else {
        setWalls();
        delay(50);
        setHeading();
      }
      indicatePath(FRONT);
      moveStraight(300);
      ramp(-1);
      tile tmp;
      cell[COUNT] = tmp;
      COUNT--;
    }
    yield();
  } while (COUNT > 1);

  // display FINISH
  clearScreen();
  displayPos(0, 0, "FINISH!", 0, 0);

  // change neopixel to white
  for (int i = 0; i < 8; i++) {
    pixels.setPixelColor(i, pixels.Color(20, 20, 20));
    pixels.show();
  }

  // Halt indefinitely
  while (1) {
    delay(100);
    yield();
  }
} // End of main loop
