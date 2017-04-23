#include <HX711.h>

//#include <Q2HX711.h>

const byte hx711_data_pin = D6;
const byte hx711_clock_pin = D5;

// on frame is currently displayed
float currentWeight = 0;
float calibration_factor = -38;// 2 sensors

//#define calibration_factor  38;// set co constant after calibration
//#define zero_factor 768703; //set to constant after calibration

float units;

// unsigned long uptime_minutes =0;
// boolean uptime_impending_rollover = false;
// insigned int uptime_rollover_counter = 0;


//Q2HX711 hx711(hx711_data_pin, hx711_clock_pin);
HX711 scale(hx711_data_pin, hx711_clock_pin);

void setup() {
  Serial.begin(115200);
  Serial.println("Starting up...");
// The HX711 functions do not yield. Watchdog would reset
  scale.set_scale(2158.45f); 

  scale.set_scale(calibration_factor); //Adjust to this calibration factor
//  scale.set_offset(zero_factor);
  scale.tare();
//  currentWeight = scale.get_units(1);
  
//    long zero_factor = scale.read_average(); //Get a baseline reading
//  Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
//  Serial.println(zero_factor);
    long zero_factor = scale.read_average(); //Get a baseline reading
  Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor);
  delay(10000);
}

void loop() {
//  Serial.println(scale.read());

  Serial.print("Reading: ");
  currentWeight = scale.get_units(5); //reads 5 times and gets average with scale factor
  Serial.print(currentWeight);
  Serial.print(" grams"); 
  Serial.println();
  delay(1000);

  
// calibration script
//https://github.com/sparkfun/HX711-Load-Cell-Amplifier/blob/master/firmware/SparkFun_HX711_Calibration/SparkFun_HX711_Calibration.ino
  scale.set_scale(calibration_factor); //Adjust to this calibration factor
//
  Serial.print("Reading: ");
  units = scale.get_units(), 10;
  if (units < 0)
  {
    units = 0.00;
  }
//  ounces = units * 0.035274;
  Serial.print(units);
  Serial.print(" grams"); 
  Serial.print(" calibration_factor: ");
  Serial.print(calibration_factor);
  Serial.println();

  if(Serial.available())
 {
    char temp = Serial.read();
    if(temp == '+' || temp == 'a')
      calibration_factor += 1;
    else if(temp == '-' || temp == 'z')
      calibration_factor -= 1;
  }

}
