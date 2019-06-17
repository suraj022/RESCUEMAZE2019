#ifndef _OBJECTS_H
#define _OBJECTS_H

//Object for MLX90614 IR temperature sensor
Adafruit_MLX90614 mlxL = Adafruit_MLX90614(0x0B);  //Left
Adafruit_MLX90614 mlxR = Adafruit_MLX90614(0x0A);  //Right

//Neopixel object with 8 pixels and is connected to pin A0
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(8, A0, NEO_GRB + NEO_KHZ800);

//Individual objects for Left, Front and Right TOF sensors
VL53L0X sensorL;    //Left
VL53L0X sensorF;    //front
VL53L0X sensorR;    //right

//object for Servo on medkit dispencer
Servo dispencer;

SimpleKalmanFilter kalmanX(2, 2, 0.01), kalmanY(2, 2, 0.01), kalmanZ(2, 2, 0.01);

//structure for tiles in the maze
struct tile {
  /* data */
  int f = 0, l = 0, r = 0;
  int pos_x, pos_y;
  int is_visited = 0; //0 for not visited 1 for visited
  int is_node = 0; //0 for not node 1 for node
  int no_go = 0; //0 for go 1 for no go
};

#endif
