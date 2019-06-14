/* TODO list-
    1. move 1 tile function
    2. change encoder loop to function DONE
    3. implement rescue mechanism DONE
    4. Fix wall align using PID
    5. Fix encoder movement using PID
    6. Heated victim DONE
    7. Add basic wall follow algorithm
    8. Depth first search algorithm
    9. Visual victim Identification
    10. Turn 90 degree using mpu6050
    11. Implement colour sensor
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

  //Setup Input and Output pins
  setupIO();

  //Enable External interrupts on encoder pins
  attachInterrupt(digitalPinToInterrupt(encoder0PinA), doEncoderR, FALLING);
  attachInterrupt(digitalPinToInterrupt(encoder1PinA), doEncoderL, FALLING);

  beep();

  //Init Servo
  initServo();

  //begin IMU functions
  beginMotion();

  //Calibrate MPU6050
  CalibrateMPU6050(50);

  //setup VL53L0x tof sensors
  setupTOF();

  //begin MLX90614 sensor
  beginMLX();

  //start neopixel
  beginNeopixel();

  //Clear pixels
  clearPixels();//JHGKF

  beep();
  //start parallel loops
  Scheduler.startLoop(MLXloop); //constantly read temperatures
  Scheduler.startLoop(gyroLoop); //Calculate Gyro roll pitch and yaw reading
  delay(200);
  beep();
  //wallDistance = (getDistance(0) + getDistance(2)) / 2;
  //Wait until a signal is given
  waitForSignal();
}

void loop() {

  indicateWalls();
  indicatePath(1);
  moveStraight(300);

  indicateWalls();
  indicatePath(2);
  turn90(90, 1);

  indicateWalls();
  indicatePath(1);
  moveStraight(300);

  indicateWalls();
  indicatePath(1);
  moveStraight(300);

  indicateWalls();
  indicatePath(2);
  turn90(90, 1);

  indicateWalls();
  indicatePath(1);
  moveStraight(300);

  indicateWalls();
  indicatePath(2);
  turn90(90, -1);

  indicateWalls();
  indicatePath(1);
  moveStraight(300);

  indicateWalls();
  indicatePath(2);
  turn90(90, -1);

  indicateWalls();
  indicatePath(1);
  moveStraight(300);

  indicateWalls();
  indicatePath(2);
  turn90(90, -1);

  indicateWalls();
  indicatePath(2);
  turn90(90, -1);

  indicateWalls();
  indicatePath(1);
  moveStraight(300);

  indicateWalls();
  indicatePath(2);
  turn90(90, 1);

  indicateWalls();
  indicatePath(1);
  moveStraight(300);

  indicateWalls();
  indicatePath(2);
  turn90(90, -1);




  indicateWalls();
  indicatePath(1);
  moveStraight(300);

  indicateWalls();
  indicatePath(1);
  moveStraight(300);

  indicateWalls();
  indicatePath(2);
  turn90(90, -1);

  indicateWalls();
  indicatePath(1);
  moveStraight(300);

  indicateWalls();
  indicatePath(2);
  turn90(90, -1);

  indicateWalls();
  indicatePath(1);
  moveStraight(300);

  indicateWalls();
  indicatePath(2);
  turn90(90, -1);

  indicateWalls();
  indicatePath(2);
  turn90(90, -1);

  indicateWalls();
  indicatePath(1);
  moveStraight(300);

  indicateWalls();
  indicatePath(2);
  turn90(90, 1);

  indicateWalls();
  indicatePath(1);
  moveStraight(300);


  indicateWalls();
  indicatePath(1);
  moveStraight(300);

  indicateWalls();
  indicatePath(1);
  moveStraight(300);

  indicateWalls();
  indicatePath(2);
  turn90(90, 1);

  indicateWalls();
  indicatePath(1);
  moveStraight(300);

  indicateWalls();
  indicatePath(2);
  turn90(90, 1);

  indicateWalls();
  indicatePath(1);
  moveStraight(300);

  indicateWalls();
  indicatePath(2);
  turn90(90, -1);

  indicateWalls();
  indicatePath(1);
  moveStraight(300);

  indicateWalls();
  indicatePath(2);
  turn90(90, 1);

  indicateWalls();
  indicatePath(1);
  moveStraight(300);

  indicateWalls();
  indicatePath(2);
  turn90(90, -1);

  indicateWalls();
  indicatePath(1);
  moveStraight(300);

  indicateWalls();
  indicatePath(2);
  turn90(90, -1);

  indicateWalls();
  indicatePath(1);
  moveStraight(300);

  indicateWalls();
  indicatePath(1);
  moveStraight(300);

  indicateWalls();
  indicatePath(2);
  turn90(90, -1);

  indicateWalls();
  indicatePath(1);
  moveStraight(300);

  indicateWalls();
  indicatePath(2);
  turn90(90, -1);
  turn90(90, -1);

  delay(500);
  yield();
}
