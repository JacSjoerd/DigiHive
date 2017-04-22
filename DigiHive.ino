//#include <AM2320.h>

/*********DigiHive*****************/
//#60 How to use the RTC Memory in ESP8266 and reduce power 
// https://youtu.be/r-hEOL007nw
//See details about deep sleep ESP -  http://www.esp8266.com/viewtopic.php?f=6&t=6494
//***********************************************************8

//011#include <ESP8266WiFi.h>         //https://github.com/esp8266/Arduino
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager

// user libs
#include "digitalpin_def.h" //definition of pins
#include "mqtt.h"
#include "DHT.h"
#include "temperature.h"
#include "weight.h"


WiFiClient espClient;
MQTT mqttClient(espClient);
DHT dht22(DHT_PIN, DHTTYPE);
Temperature temperature(DS18B20_PIN);
Weight weight(HX711_DATA_PIN, HX711_CLOCK_PIN);

//============================================
void setup() {
  Serial.begin(115200);
  myWiFiSetup();
  delay(1000);
  
  mqttClient.initialize();
  dht22.begin();
  weight.calibrate(WEIGHTFACTOR);
}

void loop() {

  if (mqttClient.reconnect() == MQTT_CONNECTED) {
      Serial.println("Connection to server OK!");
  }

  float measuredValue = 0;

  // read temperature and humidity of DHT22
  measuredValue = dht22.readTemperature();
  Serial.print("Measured temperature: ");
  Serial.print(measuredValue);
  Serial.println("C.");
  mqttClient.publish(TOPIC_IN_TEMP2, measuredValue);
  measuredValue = dht22.readHumidity();
  Serial.print("Measured humidity: ");
  Serial.print(measuredValue);
  Serial.println("%.");
  mqttClient.publish(TOPIC_IN_HUMID1, measuredValue);
  
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
  measuredValue = weight.read();
  Serial.print("Measured weight: ");
  Serial.print(measuredValue);
  Serial.println("gr.");
  mqttClient.publish(TOPIC_WEIGHT, measuredValue);
  
//  ESP.deepSleep(3600*1000000,WAKE_RF_DEFAULT);
  delay(10000);

}
//===============functions==============
void myWiFiSetup(){
   //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;
    //reset saved settings
    //wifiManager.resetSettings();
    
    //set custom ip for portal
    //wifiManager.setAPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

    //fetches ssid and pass from eeprom and tries to connect
    //if it does not connect it starts an access point with the specified name
    //here  "AutoConnectAP"
    //and goes into a blocking loop awaiting configuration
    wifiManager.autoConnect("AutoConnectAP");
    //or use this for auto generated name ESP + ChipID
    //wifiManager.autoConnect();
    
    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
}

