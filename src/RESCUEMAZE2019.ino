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
  clearPixels();

  beep();
  // start parallel loops
  Scheduler.startLoop(MLXloop);    // constantly read temperatures
  Scheduler.startLoop(gyroLoop);   // Calculate Gyro roll pitch and yaw reading
  Scheduler.startLoop(bumpLoop);   // detect left and right bumps
  Scheduler.startLoop(colourLoop); // Constantly detect tile colour

  delay(200);
  beep();

  // Wait until a signal is given
  // while (1) {
  //   SerialUSB.println(getDistance(FRONT) % 300, DEC);
  //   delay(100);
  // }
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
  // update x,y
  cell[count].x = gridX;
  cell[count].y = gridY;

  // Check walls
  if (!cell[count].visited) {
    cell[count].visited = true;
    switch (HEAD) {
    case 0: // Heading north
      cell[count].N = (getDistance(FRONT) < WALLDISTANCE);
      cell[count].E = (getDistance(RIGHT) < WALLDISTANCE);
      cell[count].S = false;
      cell[count].W = (getDistance(LEFT) < WALLDISTANCE);
      cell[count].backWay = 2;
      break;
    case 1: // heading east
      cell[count].N = (getDistance(LEFT) < WALLDISTANCE);
      cell[count].E = (getDistance(FRONT) < WALLDISTANCE);
      cell[count].S = (getDistance(RIGHT) < WALLDISTANCE);
      cell[count].W = false;
      cell[count].backWay = 3;
      break;
    case 2: // heading south
      cell[count].N = false;
      cell[count].E = (getDistance(LEFT) < WALLDISTANCE);
      cell[count].S = (getDistance(FRONT) < WALLDISTANCE);
      cell[count].W = (getDistance(RIGHT) < WALLDISTANCE);
      cell[count].backWay = 0;
      break;
    case 3: // heading west
      cell[count].N = (getDistance(RIGHT) < WALLDISTANCE);
      cell[count].E = false;
      cell[count].S = (getDistance(LEFT) < WALLDISTANCE);
      cell[count].W = (getDistance(FRONT) < WALLDISTANCE);
      cell[count].backWay = 1;
      break;
    }
    int8_t num = 0;
    bitWrite(num, 2, (getDistance(RIGHT) < WALLDISTANCE));
    bitWrite(num, 1, (getDistance(FRONT) < WALLDISTANCE));
    bitWrite(num, 0, (getDistance(LEFT) < WALLDISTANCE));
    cell[count].node = ((num >= 0 && num <= 2) || num == 4);
  }
  int8_t testCount = setHeading(cell[count]);
  cell[count].testCount = testCount;
  if (testCount == -1) {
    beep();
    delay(100);
    beep();
    delay(100);
    beep();
    cell[count].nogo = true;
    while (cell[count].node) {
      orient(cell[count].backWay);
      moveStraight(300);
      delay(300);
      count--;
    }
    // rest backtrack code here

  } else {
    int flag = false;
    int tmpX = cell[count].x;
    int tmpY = cell[count].y;
    int i = countMax;
    // N = 0, E = 1, S = 2, W = 3;
    switch (HEAD) {
    case 0:
      while (i >= 0) {
        if (cell[i].x == tmpX && cell[i].y == tmpY + 1) {
          if (cell[i].visited == true) {
            flag = true;
            break;
          }
        }
        i--;
      }
      break;
    case 1:
      while (i >= 0) {
        if (cell[i].x == tmpX + 1 && cell[i].y == tmpY) {
          if (cell[i].visited == true) {
            flag = true;
            break;
          }
        }
        i--;
      }
      break;
    case 2:
      while (i >= 0) {
        if (cell[i].x == tmpX && cell[i].y == tmpY - 1) {
          if (cell[i].visited == true) {
            flag = true;
            break;
          }
        }
        i--;
      }
      break;
    case 3:
      while (i >= 0) {
        if (cell[i].x == tmpX - 1 && cell[i].y == tmpY) {
          if (cell[i].visited == true) {
            flag = true;
            break;
          }
        }
        i--;
      }
      break;
    }
    if (!flag) {
      moveStraight(300);
      count++;
      countMax++;
    }
    // int8_t back = HEAD - 2;
    // if (back < 0)
    //   back += 4;
    // cell[count].backWay = back;
  }
  yield();
}

int8_t setHeading(tile currtile) {
  int head = 0;
  int testCount = 0;
  if (!currtile.S && currtile.testCount < 1) {
    head = 2;
    testCount = 1;
  } else if (!currtile.W && currtile.testCount < 2) {
    head = 3;
    testCount = 2;
  } else if (!currtile.N && currtile.testCount < 3) {
    head = 0;
    testCount = 3;
  } else if (!currtile.E && currtile.testCount < 4) {
    head = 1;
    testCount = 4;
  } else {
    return -1;
  }

  orient(head);
  return testCount;
}

void orient(int head) {
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
  indicateWalls();
  delay(200);
}
