//https://github.com/adafruit/DHT-sensor-library
#include "DHT.h"

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321, AM2320
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

//temp DHT variables
float temp_DHT=0;
float hum_DHT=100;

/***********Reading and sending data*******/
static void sampleTempHum(){
  int i=0;
 yield();
  temp_DHT=dht.readTemperature();
  // Check if any reads failed and exit early (to try again).
  while (isnan(temp_DHT)&&i<5 ) {
    Serial.println("Failed to read temperature from DHT sensor!");
    temp_DHT=dht.readTemperature();
    i++;
  }
    Serial.print("Temperature: ");
    Serial.print(temp_DHT);
    Serial.println(" *C");
  
  // Get humidity event and print its value.
  hum_DHT = dht.readHumidity();
  i=0;
    while (isnan(hum_DHT)&&i<5 ) {
    Serial.println("Failed to read humidity from DHT sensor!");
    hum_DHT=dht.readHumidity();
    i++;
  }
    Serial.print("Humidity: ");
    Serial.print(hum_DHT);
    Serial.println("%");
}

