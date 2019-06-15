#ifndef VARIABLES_H
#define VARIABLES_H

unsigned long currentpulsetime1 = 0;
unsigned long currentpulsetime2 = 0;

unsigned long previoustime1 = 0;
unsigned long previoustime2 = 0;

//Servo position
int pos = 0;

// Timers
const int MPU_addr = 0x68; // I2C address of the MPU-6050
int16_t CGyX, CGyY, CGyZ, GyX, GyY, GyZ;
float pitch = 0;
float roll = 0;
float yaw = 0;

//Triggers for temperature sensors
bool Ltemp = false, Rtemp = false;

//wall distance
int wallDistance=105;

//flag to enable and disable encoders
bool enableL = true, enableR = true;

//PIDs for motors
float P, I, D;

//PIDs for rotation using gyro
float Pr, Ir, Dr;

//Timer flag for Left and Right encoder
//int timerL = 0, timerR = 0;

//pwm for Left and Right motors
//int pwmL = 0, pwmR = 0;

//Setpoint for Left and Right motors
int desiredposR = 0, desiredposL = 0;

//last error for PIDs on motors
int lastError = 0, lastErrorTurn = 0;

//Direction Headed 0-> F  1-> R  2->B  3->L
int head=0;


//Variables to store encoder positions for Left and Right encoders
//set to volatile type to ensure it doesn't get omitted during compile time.
volatile int encoderposR = 0, encoderposL = 0;

#endif
