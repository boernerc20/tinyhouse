/*
Configuration file for project MESH
17/05/2024, Simon-Emmanuel HAIM
*/


// Only include this file once
#ifndef __CONFIG_M2
#define __CONFIG_M2

//Serial communication
#define BaudRate 9600

//House LEDs
#define led_washing     22
#define led_lounge      23
#define led_dish        24
#define led_fridge      25
#define led_bedroom2    26
#define led_kitchen     27
#define led_bedside     28
#define led_boiler      29
#define led_hotplates   30
#define led_television  31
#define led_oven        32
#define led_bedroom1    34 

//LCD screen (to be completed)
#define RS  6
#define EN  7
#define D4  12
#define D5  13
#define D6  20
#define D7  21

//Interface with ?
#define CH1 41 
#define CH2 43
#define CH3 45
#define CH4 47
#define CH5 49
#define CH6 51
#define CH7 53

//Class for the simulated charges
#define Max_slot 3  //maximum numbre of time slots programmed
struct  Device{   //no need of typedef in C++
  const char* name;
  float consumption;
  int pinLED;
  bool manualControl; //False if the consumption depends only of time
  bool manualOrder;   //Taken into account if manualControl == True
  int beginHours[Max_slot];
  int endHours[Max_slot];
  int numbSlot; //represent the number of slots programmed, between 1 and Max_slot
};

//Time interval
#define deltaT 1000

#endif