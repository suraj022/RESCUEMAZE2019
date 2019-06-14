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
  indicateWalls();
  //wallDistance = (getDistance(0) + getDistance(2)) / 2;
}

void loop() {
  while (getDistance(1) > 80) {
    for (int i = 0; i < 8; i++) {
      pixels.setPixelColor(i, pixels.Color(20, 20, 20));
      pixels.show();
    }
    delay(100);
  }
  //Move one tile sequence
  clearPixels();
  beep();
  while (getDistance(1) < 100) {
    delay(100);
  }
  delay(500);

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
