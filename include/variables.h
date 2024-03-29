#ifndef _VARIABLES_H
#define _VARIABLES_H

unsigned long currentpulsetime1 = 0;
unsigned long currentpulsetime2 = 0;

unsigned long previoustime1 = 0;
unsigned long previoustime2 = 0;

// Servo position
int pos = 0;

// bump flags
bool leftBumpFlag = false;
bool rightBumpFlag = false;
bool bumpcheck = true;

// Colour flags
bool whiteFlag = false;
bool blackFlag = false;
bool silverFlag = false;

// MPU-6050 related variables
int16_t CGyX, CGyY, CGyZ, GyX, GyY, GyZ;
volatile long accX, accY, accZ, accCX, accCY, accCZ;
float pitch = 0;
float roll = 0;
float yaw = 0;

// Triggers for temperature sensors
bool Lvictim = false, Rvictim = false;
int8_t victim = 0;

// wall align distance
int wallDistance = 105;

// flag to enable and disable encoders
bool enableL = true, enableR = true;

// PIDs for motors
float P, I, D;

// PIDs for rotation using gyro
float Pr, Ir, Dr;

// Setpoint for Left and Right motors
int desiredposR = 0, desiredposL = 0;

// last error for PIDs on motors
int lastError = 0, lastErrorTurn = 0;

// Variables to store encoder positions for Left and Right encoders
// set to volatile type to ensure it doesn't get omitted during compile time.
volatile int encoderposR = 0, encoderposL = 0;

// Variables for each tile
// tile cell[80];

MAZE maze[5];

// global heading
/*
  0=NORTH
  1=EAST
  2=SOUTH
  3=WEST
*/
int8_t HEAD = startHead;

int8_t saveHead = startHead;
int8_t saveX = 0;
int8_t saveY = 0;

// maze number
int8_t mazeNum = 0;

// Nogo tiles (backup use only)
// int8_t nogoIndex = 0, nogoX[20], nogoY[20];

// Stored last checkpoint count
int chkCount = 1;
#endif
