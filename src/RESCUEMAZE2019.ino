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

  //begin Oled function
  oledbegin();

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
}

void loop(){

  clearPixels();
  beep();


if (cell[p_x][p_y].is_visited==0) {
update_cell();
}
else {
      if(cell[p_x][p_y].is_visited>=1)
      {cell[p_x][p_y].is_visited+=1;}


        if(cell[p_x][p_y].is_node==1){
            while(cell[p_x][p_y].h!=head){
                switch (head) {
                  case -1:turn90(90, -1, false);
                          //heading();
                          break;
                  case -2:turn90(90, -1, false);
                          //heading();
                          break;
                  case -3:turn90(90, 1, false);
                          //heading();
                          break;
                  case  1:turn90(90, 1, false);
                          //heading();
                          break;
                  case  2:turn90(90, 1, false);
                          break;
                  case  3:turn90(90,-1, false);
                          //heading();
                          break;
            }
        }
}}




indicateWalls();
delay(300);

if (cell[p_x][p_y].l == 0) {             //left wall open
  // pr_x = p_x;
  // pr_y = p_y;
  indicatePath(LEFT);
  turn90(90, -1,true);
  indicatePath(FRONT);
  moveStraight(300);
  if (cell[p_x][p_y].is_node==1) {
    cell[p_x][p_y].l=1;
  }
  heading();
  disp();
    nodeBlock();

  }

  else if(cell[p_x][p_y].f == 0) { //wall on left and right
  // pr_y = p_y;
  // pr_x = p_x;
  indicatePath(FRONT);
  moveStraight(300);
  if (cell[p_x][p_y].is_node==1) {
    cell[p_x][p_y].f=1;
  }
  heading();
  disp();
nodeBlock();
  }


  else if (cell[p_x][p_y].r == 0) { //wall on left and front
  // pr_x = p_x;
  // pr_y = p_y;
  indicatePath(RIGHT);
  turn90(90, 1,true);
  indicatePath(FRONT);
  moveStraight(300);
  if (cell[p_x][p_y].is_node==1) {
    cell[p_x][p_y].l=1;
  }
  heading();
  disp();
nodeBlock();

} else {//if (L && F && R) { //all sides closed
  indicatePath(RIGHT);
  turn90(90, 1,true);
  indicatePath(RIGHT);
  turn90(90, 1,true);
  indicatePath(FRONT);
  moveStraight(300);
  heading();
  disp();

  while(cell[p_x][p_y].is_node==0){
    beep();
    retrace();
  }
  //   if (cell[p_x][p_y].l==0) {
  //   cell[p_x][p_y].l=1;
  // } else if (cell[p_x][p_y].f==0) {
  //   cell[p_x][p_y].f=1;
  // } else if(cell[p_x][p_y].r==0){
  //   cell[p_x][p_y].r=1;
  // }

  cell[p_x][p_y].check = 0;
  bitWrite(cell[p_x][p_y].check, 0, cell[p_x][p_y].r);
  bitWrite(cell[p_x][p_y].check, 1, cell[p_x][p_y].f);
  bitWrite(cell[p_x][p_y].check, 2, cell[p_x][p_y].l);

  if (cell[p_x][p_y].check == 7){
    while(p_x!=0 &&p_y!=0) {
        beep();
        for (int i = 0; i < 8; i++) {
          pixels.setPixelColor(i, pixels.Color(20, 20, 20));
          pixels.show();
        }
        retrace();
      }
  }


}


}//loop end


void update_cell() {
  cell[p_x][p_y].nos = count;
  cell[p_x][p_y].h= head;
  if (getDistance(1) < WALLDISTANCE)
    cell[p_x][p_y].f = 1;
  if (getDistance(2) < WALLDISTANCE)
    cell[p_x][p_y].r = 1;
  if (getDistance(0) < WALLDISTANCE)
    cell[p_x][p_y].l = 1;

    // switch (cell[p_x][p_y].h) {
    //   case 0: cell[p_x][p_y].pre_x = p_x-1;
    //           cell[p_x][p_y].pre_y = p_y;
    //           if (cell[p_x][p_y].pre_x<0) {
    //             cell[p_x][p_y].pre_x = 20 + cell[p_x][p_y].pre_x;
    //           }
    //           break;
    //   case 1:cell[p_x][p_y].pre_y = p_y-1;
    //           cell[p_x][p_y].pre_x = p_x;
    //           if (cell[p_x][p_y].pre_y<0) {
    //             cell[p_x][p_y].pre_y = 20 + cell[p_x][p_y].pre_y;
    //           }
    //           break;
    //   case 2:cell[p_x][p_y].pre_x = p_x +1;
    //           cell[p_x][p_y].pre_y = p_y;
    //           if (cell[p_x][p_y].pre_x>19) {
    //             cell[p_x][p_y].pre_x = 0;
    //           }
    //           break;
    //   case 3:cell[p_x][p_y].pre_y = p_y-1;
    //           cell[p_x][p_y].pre_x = p_x;
    //           if (cell[p_x][p_y].pre_y>19) {
    //             cell[p_x][p_y].pre_y = 0;
    //           }
    //           break;
    // }
    // cell[p_x][p_y].pre_x = pr_x;
    // cell[p_x][p_y].pre_y = pr_y;

  cell[p_x][p_y].check = 0;
  bitWrite(cell[p_x][p_y].check, 0, cell[p_x][p_y].r);
  bitWrite(cell[p_x][p_y].check, 1, cell[p_x][p_y].f);
  bitWrite(cell[p_x][p_y].check, 2, cell[p_x][p_y].l);

  if (cell[p_x][p_y].check == 0 || cell[p_x][p_y].check == 1 || cell[p_x][p_y].check == 2 || cell[p_x][p_y].check == 4){
    cell[p_x][p_y].is_node = 1;
    for (int i = 0; i < 8; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 20, 20));
      pixels.show();
          }
          delay(100);
          clearPixels();
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
  //basic Left wall follow
  bool L = (getDistance(LEFT) < WALLDISTANCE) ? true : false;
  bool F = (getDistance(FRONT) < WALLDISTANCE) ? true : false;
  bool R = (getDistance(RIGHT) < WALLDISTANCE) ? true : false;

  indicateWalls();
  delay(300);

  if (!L) {             //left wall open
    indicatePath(LEFT);
    turn90(90, -1,true);
    indicatePath(FRONT);
    moveStraight(300);
    heading();
    displayPos(0,0,"cur:",p_x,p_y);
    displayPos(0,21,"pre:",cell[p_x][p_y].pre_x,cell[p_x][p_y].pre_y);
    delay(200);
  } else if (L && !F && !R) { //wall on left
    indicatePath(FRONT);
    moveStraight(300);
    heading();
    displayPos(0,0,"cur:",p_x,p_y);
    displayPos(0,21,"pre:",cell[p_x][p_y].pre_x,cell[p_x][p_y].pre_y);
    delay(200);
  } else if (L && !F && R) { //wall on left and right
    indicatePath(FRONT);
    moveStraight(300);
    heading();
    displayPos(0,0,"cur:",p_x,p_y);
    displayPos(0,21,"pre:",cell[p_x][p_y].pre_x,cell[p_x][p_y].pre_y);
    delay(200);
  } else if (L && F && !R) { //wall on left and front
    indicatePath(RIGHT);
    turn90(90, 1,true);
    indicatePath(FRONT);
    moveStraight(300);
    heading();
    displayPos(0,0,"cur:",p_x,p_y);
    displayPos(0,21,"pre:",cell[p_x][p_y].pre_x,cell[p_x][p_y].pre_y);
    delay(200);
  } else if (L && F && R) { //all sides closed
    indicatePath(RIGHT);
    turn90(90, 1,true);
    indicatePath(RIGHT);
    turn90(90, 1,true);
    indicatePath(FRONT);
    moveStraight(300);
    heading();
    displayPos(0,0,"cur:",p_x,p_y);
    displayPos(0,21,"pre:",cell[p_x][p_y].pre_x,cell[p_x][p_y].pre_y);
    delay(200);
  }



}


void nodeBlock(){
if (cell[p_x][p_y].is_node==1) {
  if(cell[p_x][p_y].is_visited>=1)

  switch (head) {
  case 0: cell[p_x][p_y].f=1;
          break;
  case 1: cell[p_x][p_y].r=1;
          break;
  case 2:
          break;
  case 3: cell[p_x][p_y].l=1;
          break;
  }
}
}
