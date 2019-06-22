/* TODO list-
  1. move 1 tile function DONE
  2. change encoder loop to function DONE
  3. implement rescue mechanism DONE
  4. Fix wall align using PID DONE
  5. Fix encoder movement using PID DONE
  6. Heated victim DONE
  7. try basic wall follow algorithm DONE
  8. Depth first search algorithm
  9. Visual victim Identification DONE
  10. Turn 90 degree using mpu6050 DONE
  11. Implement colour sensor DONE
*/

#include "constants.h"
#include "libraries.h"
#include "objects.h"
#include "variables.h"

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

  beep();

  // Init Servo
  initServo();

  // begin IMU functions
  beginMotion();

  // Calibrate MPU6050
  CalibrateMPU6050(50);

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

  beep();
  // start parallel loops
  Scheduler.startLoop(MLXloop);    // constantly read temperatures
  Scheduler.startLoop(gyroLoop);   // Calculate Gyro roll pitch and yaw reading
  Scheduler.startLoop(bumpLoop);   // detect left and right bumps
  Scheduler.startLoop(colourLoop); // Constantly detect tile colour

  delay(200);
  beep();

  // Wait until a signal is given
  waitForSignal();

  // move forward once only if front wall is open
  // if (!(getDistance(FRONT) < WALLDISTANCE)) {
  //   indicatePath(FRONT);
  //   moveStraight(300);
  // }
  COUNT = 0;
  gridX = 0;
  gridY = 0;
  HEAD = 3;
  cell[COUNT].E = true;
}

/*
  DFS tile sequence
    0. update x, y coordinates of current tile
    1. set walls
    2. set heading
    3. if deadend then backtrack to last node
    4. else check next tile
    5. if next tile is not visited then move forward and count++
    6. else teleCount++ and backtrack to last node
*/

void loop() {
  // update x, y coordinates of current tile
  cell[COUNT].x = gridX;
  cell[COUNT].y = gridY;

  displayPos(0, 0, "cur:", HEAD, 0);
  delay(100);
  // set walls
  setWalls();

  // set heading
  bool headingResult = setHeading();

  // displayPos(0, 0, "result:", headingResult, NULL);
  delay(100);
  if (!headingResult) { // if deadend then backtrack to last node
    // backtrack code here
    while (1) {
      beep();
      delay(100);
      beep();
      delay(100);
    }
  } else { // else check next tile
    bool nexttile = nextTile(cell[COUNT].x, cell[COUNT].y);
    if (!nexttile) { // if next tile not found then move forward and count++
      moveStraight(300);
      COUNT++;
    } else { // else teleCount++ and backtrack to last node
      // backtrack code here
      while (1) {
        beep();
        delay(100);
        beep();
        delay(100);
      }
    }
  }

  yield();
}
