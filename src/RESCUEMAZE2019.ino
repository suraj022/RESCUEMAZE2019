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
//#include "Stack.h"

// StackArray <int> stack1;
// StackArray <int> stack2;

// stack.push(s_cell[0]);
// stack.pop();
int tracePath[100];
int count1=0;
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
  clearPixels();
  beep();

  update_cell();

  if (cell[p_x][p_y].l == 0) {
    turn90(90, -1, true);
    tracePath[count1]=1;
    count1++;

    moveStraight(300);
    tracePath[count1]=0;
    count1++;

    pr_x = p_x;
    pr_y = p_y;

  } else if (cell[p_x][p_y].f == 0) {
    moveStraight(300);
    tracePath[count1]=0;
    count1++;

    pr_x = p_x;
    pr_y = p_y;

  }

  else if (cell[p_x][p_y].r == 0) {
    turn90(90, 1, true);
    tracePath[count1]=-1;
    count1++;

    moveStraight(300);
    tracePath[count1]=0;
    count1++;

    pr_x = p_x;
    pr_y = p_y;

  } else {
    turn90(90, 1, false);
    //heading();
    turn90(90, 1, true);
    //heading();
    retrace();
  }

  count++;
  // pr_x=p_x;
  // pr_y=p_y;
  heading();
  delay(500);
  yield();
}

void update_cell() {
  cell[p_x][p_y].nos = count;
  cell[p_x][p_y].pre_x = pr_x;
  cell[p_x][p_y].pre_y = pr_y;
  if (getDistance(1) < WALLDISTANCE)
    cell[p_x][p_y].f = 1;
  if (getDistance(2) < WALLDISTANCE)
    cell[p_x][p_y].r = 1;
  if (getDistance(0) < WALLDISTANCE)
    cell[p_x][p_y].l = 1;

  int check = 0;
  bitWrite(check, 0, cell[p_x][p_y].r);
  bitWrite(check, 1, cell[p_x][p_y].f);
  bitWrite(check, 2, cell[p_x][p_y].l);

  if (check == 0 || check == 1 || check == 2 || check == 4){
    cell[p_x][p_y].is_node = 1;
    for (int i = 0; i < 8; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 20, 20));
      pixels.show();
    }
  }

  if (cell[p_x][p_y].is_visited != 1)
    cell[p_x][p_y].is_visited = 1;
}

void heading() {
  switch (head) {
  case 0:
    p_x++;
    if (p_x > 19) {
      p_x = 0;
    }
    break;
  case 1:
    p_y++;
    if (p_y > 19) {
      p_y = 0;
    }
    break;
  case 2:
    p_x--;
    if (p_x < 0) {
      p_x = 20 + p_x;
    }
    break;
  case 3:
    p_y--;
    if (p_y < 0) {
      p_y = 20 + p_y;
    }
    break;
  }
}


void retrace(){
  while(cell[p_x][p_y].is_node==0){
    if(tracePath[count1]==0){
      moveStraight(300);
      count1--;
      heading();
      for (int i = 0; i < 8; i++) {
        pixels.setPixelColor(i, pixels.Color(20, 0, 0));
        pixels.show();
      }
    }
    else if(tracePath[count1]==-1){
      turn90(90,-1, true);
      count1--;
      for (int i = 0; i < 8; i++) {
        pixels.setPixelColor(i, pixels.Color(0, 20, 0));
        pixels.show();
      }
    }
    else if(tracePath[count1]==1){
      turn90(90,1, true);
      count1--;
      for (int i = 0; i < 8; i++) {
        pixels.setPixelColor(i, pixels.Color(0, 0, 20));
        pixels.show();
      }
    }

    //heading();
  }
//  heading();

}
