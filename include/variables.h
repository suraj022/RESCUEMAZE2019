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
float pitch = 0;
float roll = 0;
float yaw = 0;
long accX, accY, accZ, accCX, accCY, accCZ;

// Triggers for temperature sensors
bool Ltemp = false, Rtemp = false;

// wall distance
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
tile cell[40][40];

// Stacks to store paths.
StackArray<int> stack1;
StackArray<int> stack2;

#endif
