// Include the libraries we need
// =======for scale==========
#include <HX711.h>

//scale
// sensor specific
#define calibration_factor -43// set co constant after calibration
#define zero_factor -262900 //set to constant after calibration

HX711 scale(hx711_data_pin, hx711_clock_pin);

//scale
float currentWeight = 0;
/***********Reading and sending data*******/
static void sampleWeight()
{ yield();
  Serial.print("Reading weight: ");
  // double read value because of only 2 sensors
  currentWeight = 2* scale.get_units(5); //reads 5 times and gets average with scale factor
  Serial.print(currentWeight);
  Serial.print(" grams"); 
  Serial.println();
} 
