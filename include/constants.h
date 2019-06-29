#ifndef _CONSTANTS_H
#define _CONSTANTS_H

// Uncomment the following to enable USB debug messages
//#define DEBUG

// uncomment the following to enable colour calibration
//#define COLOURCHECK

// uncomment the following to enable colour calibration
//#define TEMPCHECK

// uncomment the following to enable accelerometer calibration
//#define ACCELCHECK

// Maze ramp angle
#define RAMPANGLE 32

// Vistims temperature
#define victimTemperature 29

// define i2c address of mpu6050 imu
#define MPU_addr 0x68

// Define maximum speed of the motor (Out of 255)
#define MAXSPEED 100

// Define maximum speed of the motor during turns (Out of 255)
#define TURNSPEED 100

// Define base speed of the motor
#define BASESPEED 90

// Motor variables
#define LMF 7 // Left Motor Forward
#define LMR 6 // Left Motor Reverse
#define RMF 5 // Right Motor Forward
#define RMR 4 // Right Motor Reverse

// define Wall Detect distance
#define WALLDISTANCE 180

#define LEFTBUMP A1
#define RIGHTBUMP A3

// Encoder Pins (0 = Right Encoder , 1 = Left Encoder)
#define encoder0PinA 0
#define encoder0PinB 1
#define encoder1PinA 3
#define encoder1PinB 2

// colour Values
#define COLOURPIN A4
#define SILVER 837
#define NORMAL 898
#define HAZARD 941

// sensor numbers for TOF sensors
#define LEFT 0
#define FRONT 1
#define RIGHT 2

// colours
#define RED 1
#define BLUE 2
#define GREEN 3

// Pixels
#define LEFTPIXEL 2
#define FRONTPIXEL 4
#define RIGHTPIXEL 6

// define starting heading
#define startHead 2

// Buzzer connected to this pin
#define buzzPin A5

// Lack of progress Switch
#define Chkpt 11

#endif
