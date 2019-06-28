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

  // Wait until a signal is given
  // waitForSignal();

  // Calibrate Gyroscope
  CalibrateMPU6050(50);

  delay(100);

  // Calibrate Accelerometer
  CalibrateMPU6050_Acc(50);

  // start parallel loops
  Scheduler.startLoop(MLXloop);    // constantly read temperatures
  Scheduler.startLoop(IMUloop);    // Calculate Gyro roll pitch and yaw reading
  Scheduler.startLoop(bumpLoop);   // detect left and right bumps
  Scheduler.startLoop(colourLoop); // Constantly detect tile colour

  // Wait until a signal is given
  waitForSignal();

  beep(50);

  mazeNum = 0;
  HEAD = 2; // Start facing north direction
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
      // if (maze[mazeNum].cell[maze[mazeNum].COUNT].testCount < 1)
      setWalls();

      //**********************
      // Add victim check code here
      // for both visual and heated victims
      //**********************

      // set heading
      bool headingResult = setHeading();
      bool nextTileFound = nextTile(maze[mazeNum].cell[maze[mazeNum].COUNT].x,
                                    maze[mazeNum].cell[maze[mazeNum].COUNT].y);

      bool condition = false;
      bool condition1 = false;
      condition = (maze[mazeNum].COUNT == 1 && !headingResult);
      int tmphead = maze[mazeNum].entryHead;
      tmphead -= 2;
      if (tmphead < 0)
        tmphead += 4;
      condition1 = (maze[mazeNum].COUNT == 1 && HEAD == tmphead);

      if (condition1) {
        beep(500);
        delay(500);
        beep(500);
        maze[mazeNum].completed = true;
        continue;
      }

      if (condition) {
        beep(50);
        delay(50);
        beep(50);
        delay(50);
        beep(50);
        delay(50);
        maze[mazeNum].completed = true;
        continue;
      }

      if (nextTileFound)
        headingResult = setHeading();

      if (headingResult) {
        // if available way and next tile is empty then move forward and count++
        clearScreen();
        // Display coordinates on Oled
        displayPos(0, 0, "X/Y:", maze[mazeNum].gridX, maze[mazeNum].gridY);
        displayPos(0, 21, "N/C:", mazeNum, maze[mazeNum].COUNT);

        indicatePath(FRONT);
        moveStraight(300);
        maze[mazeNum].COUNT++;
        if (ramp()) {
          mazeNum++;
          maze[mazeNum].entryHead = HEAD;
          continue;
        }
      } else { // else backtrack to last node
        clearScreen();
        displayPos(0, 0, "BACKTRACK:", 0, 0);
        if (maze[mazeNum].cell[maze[mazeNum].COUNT].backWay >= 0) {
          orient(maze[mazeNum].cell[maze[mazeNum].COUNT].backWay);
        } else {
          setWalls();
          delay(50);
          setHeading();
        }
        if (!maze[mazeNum].cell[maze[mazeNum].COUNT].node) {
          tile tmp;
          maze[mazeNum].cell[maze[mazeNum].COUNT] = tmp;
        }
        indicatePath(FRONT);
        moveStraight(300);
        maze[mazeNum].COUNT--;
      }
      yield();
    } while (!maze[mazeNum].completed); // end of maze

    if (mazeNum > 0) {
      clearScreen();
      // Display coordinates on Oled
      displayPos(0, 0, "BACKTRACK:", 0, 0);
      //
      // int tmpHead = maze[mazeNum].entryHead;
      // tmpHead -= 2;
      // if (tmpHead < 0)
      //   tmpHead += 4;
      // orient(tmpHead);
      // HEAD = tmpHead;
      moveStraight(300);
      ramp();
      mazeNum--;
      maze[mazeNum].COUNT--;
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

  // Halt indefinitely
  while (1) {
    for (int i = 0; i < 8; i++) {
      pixels.setPixelColor(
          i, pixels.Color(random(0, 255), random(0, 255), random(0, 255)));
    }
    pixels.show();
    delay(100);
    yield();
  }
} // End of main loop
