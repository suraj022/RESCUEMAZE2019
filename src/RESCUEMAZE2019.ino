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
12. Add Heated victim code to main loop			DONE
13. Implement visual victim code            DONE
14. Add visual victim code to main loop			DONE
15. Add ramp detection                      DONE
16. Lack of progress switch                 DONE
17. Fix next tile detection                 DONE
18. Ramp up return fix                      DONE
19. Increase led light birghtness           DONE
20. Redesign bumpers and side skirts        CANCELLED

***************************************************/

/***************************************************
CHECK list for sensor calibration-
1. Notedown and set TILELENGTH dimension
2. Calibrate victim temperature
3. Calculate brightness threshold in camera
4. Calibrate 3 tile brightness values
5. Ramp elevation
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
  Scheduler.startLoop(camLoop);    // Get victim values from camera

  // Start Calibration process if enabled in constants
  CalibrationProcess();

  // Wait until a signal is given
  waitForSignal();

  beep(50);

} // end setup part

/***************************************************
DFS algorithm sequence
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

      // victim check code
      if (Lvictim) {
        dispence(1, -1);
        Lvictim = false;
        Rvictim = false;
      }

      if (Rvictim) {
        dispence(1, 1);
        Lvictim = false;
        Rvictim = false;
      }

      switch (victim) {
      case 3:
        dispence(2, -1); // case for harmed victim
        victim = 0;
        delay(10);
        break;
      case 2:
        dispence(1, -1); // case for harmed victim
        victim = 0;
        delay(10);
        break;
      case 1:
        dispence(0, -1); // case for harmed victim
        victim = 0;
        delay(10);
        break;
      default:
        break;
      }

      // store store checkpoint of available
      storeCheckpoint();

      // set heading
      bool headingResult = setHeading();
      bool nextTileFound = nextTile(maze[mazeNum].cell[maze[mazeNum].COUNT].x,
                                    maze[mazeNum].cell[maze[mazeNum].COUNT].y);

      if (!digitalRead(Chkpt)) {
        clearPixels();
        clearScreen();
        displayTxt(0, 0, "LACK OF   PROGRESS!");
        while (!digitalRead(Chkpt)) {
          delay(100);
        }
        clearScreen();
        displayTxt(0, 0, "START!");
        restartCheckpoint();
        headingResult = true;
      }

      bool condition = false;
      bool condition1 = false;

      condition = (maze[mazeNum].COUNT == 1 && !headingResult);

      int tmphead = maze[mazeNum].entryHead;
      tmphead -= 2;
      if (tmphead < 0)
        tmphead += 4;

      condition1 = (maze[mazeNum].COUNT == 1 && HEAD == tmphead);

      if (condition1) {
        maze[mazeNum].completed = true;
        continue;
      }

      if (condition) {
        maze[mazeNum].completed = true;
        continue;
      }

      if (nextTileFound)
        headingResult = setHeading();

      if (headingResult) {
        // if available way and next tile is empty then move forward and count++

        clearScreen();
        // Display coordinates on Oled
        String msg = "X/Y:";
        msg += maze[mazeNum].gridX;
        msg += ",";
        msg += maze[mazeNum].gridY;
        displayTxt(0, 0, msg);
        msg = "N/C:";
        msg += mazeNum;
        msg += ",";
        msg += maze[mazeNum].COUNT;
        displayTxt(0, 21, msg);
        msg = "HEAD:";
        msg += HEAD;
        displayTxt(0, 42, msg);

        indicatePath(FRONT);
        moveStraight(TILELENGTH);
        ////////////////////////////////////////////////////
        // check for black tile
        if (blackFlag) {
          clearPixels();
          beep(100);
          for (int i = 1; i < 8; i += 2) {
            pixels.setPixelColor(i, pixels.Color(50, 0, 0));
            pixels.show();
          }
          delay(100);
          clearPixels();
          moveStraight(-300);
          offsetStraight(125, false);
          delay(200);
        } else {
          maze[mazeNum].COUNT++;
        }
        ////////////////////////////////////////////////////

        if (ramp()) {
          mazeNum++;
          maze[mazeNum].entryHead = HEAD;
          continue;
        }
      } else { // else backtrack to last node
        clearScreen();
        String msg = "B/T:";
        msg += maze[mazeNum].gridX;
        msg += ",";
        msg += maze[mazeNum].gridY;
        displayTxt(0, 0, msg);
        msg = "N/C:";
        msg += mazeNum;
        msg += ",";
        msg += maze[mazeNum].COUNT;
        displayTxt(0, 21, msg);
        msg = "HEAD:";
        msg += HEAD;
        displayTxt(0, 42, msg);

        if (maze[mazeNum].cell[maze[mazeNum].COUNT].backWay >= 0) {
          orient(maze[mazeNum].cell[maze[mazeNum].COUNT].backWay);
        } else {
          setWalls();
          delay(50);
          setHeading();
        }
        tile tmp;
        maze[mazeNum].cell[maze[mazeNum].COUNT] = tmp;
        indicatePath(FRONT);
        moveStraight(TILELENGTH);
        maze[mazeNum].COUNT--;
      }
      yield();
    } while (!maze[mazeNum].completed); // end of maze

    if (mazeNum > 0) {
      clearScreen();
      displayTxt(0, 0, "back to last maze");
      // int tmpHead = maze[mazeNum].entryHead;
      // tmpHead -= 2;
      // if (tmpHead < 0)
      //   tmpHead += 4;
      // orient(tmpHead);
      // HEAD = tmpHead;
      // moveStraight(TILELENGTH);
      moveMotor(100, 100);
      delay(1000);
      ramp();
      mazeNum--;
      offsetStraight(125, false);
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
      moveMotor(0, 0);
    } else {
      mazeNum--;
    }
  } while (mazeNum >= 0);

  // display FINISH
  clearScreen();
  displayTxt(0, 0, "FINISH!");

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
