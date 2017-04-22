//#include <AM2320.h>

/*********DigiHive*****************/
//#60 How to use the RTC Memory in ESP8266 and reduce power 
// https://youtu.be/r-hEOL007nw
//See details about deep sleep ESP -  http://www.esp8266.com/viewtopic.php?f=6&t=6494
//***********************************************************8

//011#include <ESP8266WiFi.h>         //https://github.com/esp8266/Arduino
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <PubSubClient.h>

// user libs
#include "digitalpin_def.h" //definition of pins
#include "get_onewire.h"
#include "get_dht.h"
#include "weight.h"
#include "mqtt.h"

WiFiClient espClient;
MQTT mqttClient(espClient);
Weight weight(hx711_data_pin, hx711_clock_pin);

//============================================
void setup() {
  Serial.begin(115200);
  myWiFiSetup();
  delay(1000);
  
  mqttClient.initialize();
   
  tempSensor.begin();
  findDeviceAddr();   // locate OneWire devices on the bus
  delay(100);
  dht.begin(); 
  weight.calibrate(weightFactor);
}

void loop() {

  if (mqttClient.reconnect() == MQTT_CONNECTED) {
      Serial.println("Connection to server OK!");
  }


  //  reads temperature and humidity
  sampleTempHum();
  // reads temperature OneWire
  sampleTemperature();

  float measuredWeight = weight.read();
  Serial.print("Measured weight: ");
  Serial.print(measuredWeight);
  Serial.println("gr.");
  mqttClient.publish(TOPIC_WEIGHT, measuredWeight);
  
  //write all fields to ThingSpeak
  //    writeAllFields();

  // this is mqtt part
  if (!mqttClient.connected()) {
    mqttClient.reconnect();
    Serial.println("Reconecting...");
  }

//  writeMqttFields();   // write fields to MQTT
  
//  ESP.deepSleep(3600*1000000,WAKE_RF_DEFAULT);
  delay(1000);

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

