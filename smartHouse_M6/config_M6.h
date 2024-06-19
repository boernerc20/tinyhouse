/*
Configuration file for project MESH
17/05/2024, Simon-Emmanuel HAIM
*/


// Only include this file once
#ifndef __CONFIG_M6
#define __CONFIG_M6

//House LEDs
#define led_lounge      22
#define led_car         23
#define led_bedside     24
#define led_boiler      25
#define led_bedroom2    26
#define led_bedroom1    27
#define led_oven        28
#define led_television  29
#define led_kitchen     30
#define led_fridge      31
#define led_hotplates   32
#define led_washing     34

Device devices6[] = {  
  {"oven", 2000, led_oven, 0, 0,{0},{24},1},
  {"boiler", 500, led_boiler, 0, 0,{0},{24},1},
  {"hotplates", 2500, led_hotplates, 0, 0, {12,19},{13,20},2},
  {"bedroom2",30,led_bedroom2,0,0,{6,20},{8,22},2},
  {"kitchen",30,led_kitchen,0,0,{6,20},{8,22},2},
  {"fridge", 100, led_fridge, 0, 0,{0},{24},1},
  {"washing", 1000, led_washing, 0, 0, {17},{20},1},
  {"television", 100, led_television, 0, 0,{0},{24},1},
  {"lounge",30,led_lounge,0,0,{6,20},{8,22},2},
  {"car",30,led_car,0,0,{6,20},{8,22},2},
  {"bedside",30,led_bedside,0,0,{6,20},{8,22},2},
  {"bedroom1",30,led_bedroom1,0,0,{6,20},{8,22},2}
};

#endif