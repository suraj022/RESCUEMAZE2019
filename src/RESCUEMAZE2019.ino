/***************************************************
  TODO list-
  1. move 1 tile function                     DONE
  2. change encoder loop to function          DONE
  3. implement rescue mechanism               DONE
  4. Fix wall align using PID                 DONE
  5. Fix encoder movement using PID           DONE
  6. Implement Heated victim code             DONE
  7. try basic wall follow algorithm          DONE
  8. Depth first search algorithm
  9. Visual victim Identification             DONE
  10. Turn 90 degree using mpu6050            DONE
  11. Implement colour sensor                 DONE
  12. Add Heated victim code to main loop
  13. Implement visual victim code
  14. Add visual victim code to main loop
  15. Add ramp detection
  16. Lack of progress switch
***************************************************/

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

  beep(50);

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

  beep(50);
  // start parallel loops
  Scheduler.startLoop(MLXloop);    // constantly read temperatures
  Scheduler.startLoop(gyroLoop);   // Calculate Gyro roll pitch and yaw reading
  Scheduler.startLoop(bumpLoop);   // detect left and right bumps
  Scheduler.startLoop(colourLoop); // Constantly detect tile colour

  delay(200);
  beep(50);

  // Wait until a signal is given
  waitForSignal();

  // move forward once only if front wall is open
  // if (!(getDistance(FRONT) < WALLDISTANCE)) {
  //   indicatePath(FRONT);
  //   moveStraight(300);
  // }

  // Intialize grid variables
  COUNT = 1;
  gridX = 0;
  gridY = 0;
  HEAD = 3;
  // cell[COUNT].E = true;
}

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
    delay(300);

    /*switch (HEAD) {
    case 0: // north
      displayPos(0, 21, "next:", gridX, gridY + 1);
      break;
    case 1: // east
      displayPos(0, 21, "next:", gridX + 1, gridY);
      break;
    case 2: // south
      displayPos(0, 21, "next:", gridX, gridY - 1);
      break;
    case 3: // west
      displayPos(0, 21, "next:", gridX - 1, gridY);
      break;
    }
    delay(1000);
    */
    if (headingResult == true && nextTileFound == false) {
      // if available way and next tile is empty then move forward and count++
      moveStraight(300);
      COUNT++;
      // tile tmp;
      // cell[COUNT] = tmp;
    } else { // else backtrack to last node
      // backtrack code here and count-- until last node
      if (cell[COUNT].backWay >= 0) {
        orient(cell[COUNT].backWay);
      } else {
        setWalls();
        delay(50);
        bool tmmp = setHeading();
      }
      moveStraight(300);
      tile tmp;
      cell[COUNT] = tmp;
      COUNT--;
    }
    yield();
  } while (COUNT > 1);
  turnBot(90, 1, true);
  turnBot(90, 1, true);
  displayPos(0, 0, "FINISH!", 0, 0);
  waitForSignal();
  yield();
}
