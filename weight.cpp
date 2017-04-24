#include "weight.h"
#include <Q2HX711.h>      // https://github.com/queuetue/Q2-HX711-Arduino-Library

Weight::Weight(byte output_pin, byte clock_pin)
  : Q2HX711(output_pin, clock_pin)
{
  
}

Weight::~Weight()
{
	
}

void Weight::calibrate(long factor)
{
  adjustmentFactor = factor;
  pointZero = Q2HX711::read();
}

float Weight::read() 
{
  long latestWeight = 0;

  // Get an average of 5 reading
  for (int i = 0; i < 5; i++)
  {
    latestWeight = latestWeight + Q2HX711::read();
    delay(1000);
  }
  latestWeight = latestWeight / 5;
  
  latestWeight = latestWeight - pointZero;
  latestWeight = latestWeight / adjustmentFactor;

  return latestWeight;
}

