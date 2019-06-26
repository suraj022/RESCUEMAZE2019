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
  15. Add ramp detection                      DONE
  16. Lack of progress switch
  17. Fix next tile detection                 DONE
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

  mazeNum = 0;
  HEAD = 3; // Start facing west direction
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
    do {
      // update x, y coordinates of current tile
      maze[mazeNum].cell[maze[mazeNum].COUNT].x = maze[mazeNum].gridX;
      maze[mazeNum].cell[maze[mazeNum].COUNT].y = maze[mazeNum].gridY;

      // set walls
      setWalls();

      //**********************
      // Add victim check code here
      // for both visual and heated victims
      //**********************

      // set heading
      bool headingResult = setHeading();
      bool nextTileFound = nextTile(maze[mazeNum].cell[maze[mazeNum].COUNT].x,
                                    maze[mazeNum].cell[maze[mazeNum].COUNT].y);
      if (nextTileFound)
        headingResult = setHeading();
      delay(300);

      if (headingResult) {
        // if available way and next tile is empty then move forward and count++
        clearScreen();
        // Display coordinates on Oled
        displayPos(0, 0, "X/Y:", maze[mazeNum].gridX, maze[mazeNum].gridY);
        displayPos(0, 21, "N/C:", mazeNum, maze[mazeNum].COUNT);

        indicatePath(FRONT);
        moveStraight(300);
        if (ramp()) {
          mazeNum++;
          maze[mazeNum].entryHead = HEAD;
          continue;
        } else {
          maze[mazeNum].COUNT++;
        }
        // else
        // continue;
      } else { // else backtrack to last node
        // backtrack code here and count-- until last node
        clearScreen();
        // Display coordinates on Oled
        displayPos(0, 0, "BACKTRACK:", maze[mazeNum].gridX,
                   maze[mazeNum].gridY);
        displayPos(0, 45, "COUNT:", maze[mazeNum].COUNT, 0);

        if (maze[mazeNum].cell[maze[mazeNum].COUNT].backWay >= 0) {
          orient(maze[mazeNum].cell[maze[mazeNum].COUNT].backWay);
        } else {
          setWalls();
          delay(50);
          setHeading();
        }
        indicatePath(FRONT);
        moveStraight(300);
        tile tmp;
        maze[mazeNum].cell[maze[mazeNum].COUNT] = tmp;
        maze[mazeNum].COUNT--;
        if (maze[mazeNum].COUNT == 1 &&
            maze[mazeNum].cell[maze[mazeNum].COUNT].testCount > 3 &&
            mazeNum > 0)
          maze[mazeNum].completed = true;
        else if (maze[mazeNum].COUNT == 1 && mazeNum == 0) {
          maze[mazeNum].completed = true;
        }
      }
      yield();
    } while (maze[mazeNum].completed == false); // end of maze

    for (int i = 0; i < 8; i++) {
      pixels.setPixelColor(i, pixels.Color(20, 20, 20));
      pixels.show();
    }
    delay(200);
    clearPixels();

    if (mazeNum > 0) {
      clearScreen();
      // Display coordinates on Oled
      displayPos(0, 0, "BACKTRACK:", 0, 0);

      int tmpHead = maze[mazeNum].entryHead;
      switch (tmpHead) {
      case 0:
        orient(2);
        HEAD = 2;
        break;
      case 1:
        orient(3);
        HEAD = 3;
        break;
      case 2:
        orient(0);
        HEAD = 0;
        break;
      case 3:
        orient(1);
        HEAD = 1;
        break;
      default:
        break;
      }
      moveStraight(300);
      while (getDistance(FRONT) > 80) {
        moveMotor(100, 100);
        beep(50);
        delay(100);
      }
      moveMotor(0, 0);
      mazeNum--;
      switch (HEAD) {
      case 0:
        maze[mazeNum].gridY++;
        break;
      case 1:
        maze[mazeNum].gridX++;
        break;
      case 2:
        maze[mazeNum].gridY--;
        break;
      case 3:
        maze[mazeNum].gridX--;
        break;
      }
    } else {
      mazeNum--;
    }
  } while (mazeNum >= 0);

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
