#ifndef _LIBRARIES_H
#define _LIBRARIES_H

// Scheduler library for multitasking
#include <Scheduler.h>

// Wire library to communicate with I2C devices
#include <Wire.h>

// Library for MLX90614 IR temperature sensor
#include <Adafruit_MLX90614.h>

// Simple kalman filter library
#include <SimpleKalmanFilter.h>

// Interface VL53L0X Time of flight sensor
#include <VL53L0X.h>

// Adafruit neopixel library
#include <Adafruit_NeoPixel.h>

// Servo library
#include <Servo.h>

// Oled related libraries
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>

#if (SSD1306_LCDHEIGHT != 64)
#error "incorrect height"
#endif

#endif
