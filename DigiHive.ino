/***************** DigiHive *****************/
#include <ESP8266WiFi.h>
#include <hx711.h>

// user libs
#include "digitalpin_def.h" //definition of pins
#include "mqtt.h"
#include "DHT.h"
#include "temperature.h"

using namespace std;

MQTT        mqttClient(MQTT_SERVER, MQTT_PORT);
DHT         dht22(DHT_PIN, DHTTYPE);
Temperature temperature(DS18B20_PIN);
Hx711       weight(HX711_DATA_PIN, HX711_CLOCK_PIN);
ADC_MODE(ADC_VCC); 

//============================================
void setup() {
  Serial.begin(115200);
  delay(1000);

  // Start Wifi
  mqttClient.initialize();

  dht22.begin();

  weight.setOffset(WEIGHTOFFSET);
  weight.setScale(WEIGHTFACTOR);
}

void loop() {

  if (mqttClient.reconnect() == MQTT_CONNECTED) {
    Serial.println("Connection to server OK!");

    float measuredValue = 0;

    // read voltage at ESP8266 
    measuredValue = ESP.getVcc() / 1000.0;
    if (isnormal(measuredValue)) {
      Serial.print("Measured voltage: ");
      Serial.print(measuredValue);
      Serial.println("V");
      mqttClient.publish(TOPIC_VOLTAGE, measuredValue);
    }

    // read temperature and humidity of DHT22
    measuredValue = dht22.readTemperature();
    if (isnormal(measuredValue)) {
      Serial.print("Measured dht temperature: ");
      Serial.print(measuredValue);
      Serial.println("C.");
      mqttClient.publish(TOPIC_IN_TEMP2, measuredValue);
    }
    measuredValue = dht22.readHumidity();
    if (isnormal(measuredValue)) {
      Serial.print("Measured dht humidity: ");
      Serial.print(measuredValue);
      Serial.println("%.");
      mqttClient.publish(TOPIC_IN_HUMID1, measuredValue);
    }
      
    // read temperature of all DS18B20 (upto 3)
    int devices = temperature.deviceCount();
    Serial.print(devices);
    Serial.println(" one wire devices found.");
    for (int i = 0; i < devices ;i++) {
      measuredValue = temperature.read(i);  
      Serial.print("Measured temperature: ");
      Serial.print(measuredValue);
      Serial.println("C.");
      mqttClient.publish(static_cast<Topic>(TOPIC_IN_TEMP1 + i), measuredValue);
    }
  
    // reads weight
    measuredValue = weight.getGram();
    Serial.print("Measured weight: ");
    Serial.print(measuredValue);
    Serial.println("gr.");
    mqttClient.publish(TOPIC_WEIGHT, measuredValue);
  } else {
    Serial.println("Not connected: Not trying to send data.");
  }

  // put ESP8266 to sleep for 10 mins to minimize power consumption
  // time is in micro seconds
  // ESP.deepSleep(600*1000000,WAKE_RF_DEFAULT);
  delay(10000);
}

