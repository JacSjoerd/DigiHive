#ifndef WEIGHT_H
#define WEIGHT_H

#include <Q2HX711.h>

class Weight : public Q2HX711
{
private:
  long pointZero = 0;
  long adjustmentFactor = 0;
public:
	Weight(byte output_pin, byte clock_pin);
	virtual ~Weight();

  // calibrate the HX711 to 0gr and set a factor to convert to gr.
  void calibrate();
  void calibrate(long factor);

  // get functions for the private data
  long getPointZero() {return pointZero;};
  long getAdjustmentFactor() {return adjustmentFactor;};

  // Get the average data from the HX711 over 5 readings and adjust according the calibration values 
  long read();
}; 

#endif WEIGHT_H
