// Include the libraries we need
// =======for scale==========
#include <Q2HX711.h>
#include "digitalpin_def.h"

//scale
// sensor specific

Q2HX711 scale(hx711_data_pin, hx711_clock_pin);

//scale
float averageWeight = 0;
/***********Reading and sending data*******/
float weight() { 
  Serial.print("Reading weight: ");
  for (int i = 0; i < 5; i++) {
    averageWeight +=  scale.read(); //reads 5 times and gets average with scale factor
  }

  averageWeight /= 5;
  
  Serial.print(averageWeight);
  Serial.print(" grams"); 
  Serial.println();

  return averageWeight;
} 
