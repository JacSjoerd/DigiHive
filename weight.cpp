#include "weight.h"
#include <Q2HX711.h>

Weight::Weight(byte output_pin, byte clock_pin)
  : Q2HX711(output_pin, clock_pin)
{
  
}

Weight::~Weight()
{
	
}

void Weight::calibrate()
{
  pointZero = read();
}

void Weight::calibrate(long factor)
{
  adjustmentFactor = factor;
  pointZero = Q2HX711::read();
}

long Weight::read() 
{
  long latestWeight = 0;

  // Get an average of 5 reading
  for (int i = 0; i < 5; i++)
  {
    latestWeight = latestWeight + Q2HX711::read();
  }
  latestWeight = latestWeight / 5;
  
  latestWeight = latestWeight - pointZero;
  latestWeight = latestWeight / adjustmentFactor;

  return latestWeight;
}

