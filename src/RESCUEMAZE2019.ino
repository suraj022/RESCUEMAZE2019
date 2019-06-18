/* TODO list-
  1. move 1 tile function DONE
  2. change encoder loop to function DONE
  3. implement rescue mechanism DONE
  4. Fix wall align using PID DONE
  5. Fix encoder movement using PID DONE
  6. Heated victim DONE
  7. Add basic wall follow algorithm DONE
  8. Depth first search algorithm
  9. Visual victim Identification
  10. Turn 90 degree using mpu6050 DONE
  11. Implement colour sensor DONE
*/

#include "constants.h"
#include "libraries.h"
#include "objects.h"
#include "variables.h"

void setup() {

  tile *s_cell;

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
  // wallDistance = (getDistance(0) + getDistance(2)) / 2;
  // Wait until a signal is given
  waitForSignal();
  if ((getDistance(FRONT) < WALLDISTANCE) ? false : true) {
    indicatePath(FRONT);
    moveStraight(300);
  }
}

void loop() {

  // basic Left wall follow
  bool L = (getDistance(LEFT) < WALLDISTANCE) ? true : false;
  bool F = (getDistance(FRONT) < WALLDISTANCE) ? true : false;
  bool R = (getDistance(RIGHT) < WALLDISTANCE) ? true : false;

  indicateWalls();
  delay(300);

  if (blackFlag) {
    for (int i = 0; i < 8; i++) {
      setPixelColour(i, RED, 50);
    }
    delay(200);
    clearPixels();
    delay(200);
    for (int i = 0; i < 8; i++) {
      setPixelColour(i, RED, 50);
    }
    delay(200);
    clearPixels();
    moveStraight(-300);
    indicatePath(LEFT);
    turn90(90, -1, false);
    moveMotor(-50, -50);
    delay(200);
    turn90(90, -1, true);
    moveMotor(-50, -50);
    delay(200);
  } else {
    if (!L) { // left wall open
      indicatePath(LEFT);
      turn90(90, -1, true);
      indicatePath(FRONT);
      moveStraight(300);
    } else if (L && !F && !R) { // wall on left
      indicatePath(FRONT);
      moveStraight(300);
    } else if (L && !F && R) { // wall on left and right
      indicatePath(FRONT);
      moveStraight(300);
    } else if (L && F && !R) { // wall on left and front
      indicatePath(RIGHT);
      turn90(90, 1, true);
      indicatePath(FRONT);
      moveStraight(300);
    } else if (L && F && R) { // all sides closed
      indicatePath(RIGHT);
      turn90(90, 1, false);
      turn90(90, 1, true);
      indicatePath(FRONT);
      moveStraight(300);
    }
  }
  yield();
}
