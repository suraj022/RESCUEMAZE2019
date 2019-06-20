/* TODO list-
  1. move 1 tile function
  2. change encoder loop to function DONE
  3. implement rescue mechanism DONE
  4. Fix wall align using PID DONE
  5. Fix encoder movement using PID
  6. Heated victim DONE
  7. Add basic wall follow algorithm DONE
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
  clearPixels(); // JHGKF

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
  count = 0;
  countMax = 0;
  gridX = 0;
  gridY = 0;
  HEAD = 3;
}

void loop() {
  // Check walls
  switch (HEAD) {
  case 0: // Heading north
    cell[count].N = (getDistance(FRONT) < WALLDISTANCE);
    cell[count].E = (getDistance(RIGHT) < WALLDISTANCE);
    cell[count].S = false;
    cell[count].W = (getDistance(LEFT) < WALLDISTANCE);
    break;
  case 1: // heading east
    cell[count].N = (getDistance(LEFT) < WALLDISTANCE);
    cell[count].E = (getDistance(FRONT) < WALLDISTANCE);
    cell[count].S = (getDistance(RIGHT) < WALLDISTANCE);
    cell[count].W = false;
    break;
  case 2: // heading south
    cell[count].N = false;
    cell[count].E = (getDistance(LEFT) < WALLDISTANCE);
    cell[count].S = (getDistance(FRONT) < WALLDISTANCE);
    cell[count].W = (getDistance(RIGHT) < WALLDISTANCE);
    break;
  case 3: // heading west
    cell[count].N = (getDistance(RIGHT) < WALLDISTANCE);
    cell[count].E = false;
    cell[count].S = (getDistance(LEFT) < WALLDISTANCE);
    cell[count].W = (getDistance(FRONT) < WALLDISTANCE);
    break;
  }

  setHeading();
  moveStraight(300);
  count++;
  yield();
}

int8_t setHeading(tile currtile) {
  int head = 0;
  if (!currtile.S) {
    head = 2;
  } else if (!currtile.W) {
    head = 3;
  } else if (!currtile.N) {
    head = 0;
  } else if (!currtile.E) {
    head = 1;
  } else {
    return -1;
  }

  int result = head - HEAD;
  switch (result) {
  case -3:
  case 1:
    turn90(90, 1, true);
    break;
  case -2:
  case 2:
    if (getDistance(LEFT) > getDistance(RIGHT)) {
      turn90(90, -1, false);
      turn90(90, -1, true);
    } else {
      turn90(90, 1, false);
      turn90(90, 1, true);
    }
    break;
  case -1:
  case 3:
    turn90(90, -1, true);
    break;
  case 0:
    break;
  }
}
