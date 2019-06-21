#ifndef _OBJECTS_H
#define _OBJECTS_H

// Object for MLX90614 IR temperature sensor
Adafruit_MLX90614 mlxL = Adafruit_MLX90614(0x0B); // Left
Adafruit_MLX90614 mlxR = Adafruit_MLX90614(0x0A); // Right

// Neopixel object with 8 pixels and is connected to pin A0
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(8, A0, NEO_GRB + NEO_KHZ800);

// Individual objects for Left, Front and Right TOF sensors
VL53L0X sensorL; // Left
VL53L0X sensorF; // front
VL53L0X sensorR; // right

// object for Servo on medkit dispencer
Servo dispencer;

SimpleKalmanFilter kalmanX(2, 2, 0.01);
SimpleKalmanFilter kalmanY(2, 2, 0.01);
SimpleKalmanFilter kalmanZ(2, 2, 0.01);
SimpleKalmanFilter kalmanColour(2, 2, 0.01);

// structure for tiles in the maze
struct tile { // num stored in array index
  bool N = false, E = false, S = false, W = false;
  int8_t backWay; // N = 0, E = 1, S = 2, W = 3;
  int8_t x, y;
  bool node = false;
  bool visited = false;
  bool nogo = false;
};

#endif
