//#include <AM2320.h>

/*********DigiHive*****************/
//#60 How to use the RTC Memory in ESP8266 and reduce power 
// https://youtu.be/r-hEOL007nw
//See details about deep sleep ESP -  http://www.esp8266.com/viewtopic.php?f=6&t=6494
//***********************************************************8

#include <ESP8266WiFi.h>         //https://github.com/esp8266/Arduino
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager

// user libs
#include "digitalpin_def.h" //definition of pins
#include "get_onewire.h"
#include "get_dht.h"
#include "get_weight.h"
#include "get_mqtt.h"


float calibration_factor = 22;  // set co constant to calibrate
float zero_factor = 0;          // read once at start for 0 calibration

//====objects 
//WiFiClient defined in get_mqtt.h
//WiFiClient  client;
//WiFiClient espClient;
/*
static void writeAllFields(){
  // write user defined fields to ThingSpeak
  if (!isnan(temp_DHT)){
   ThingSpeak.setField(1, temp_DHT);}
  if (!isnan(hum_DHT)){
   ThingSpeak.setField(2, hum_DHT);}
  if (nDeviceCount > 0) {
  for (int i=0; i < nDeviceCount; i++) {   ThingSpeak.setField( i+1+2, tempC[i]); }
  }
    ThingSpeak.setField( 5, currentWeight);
    
  Serial.print("Writing to the cloud: ");
    int rc = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    Serial.println(String("Post rc=")+rc);
// 
}*/
//============================================
void setup() {
     // put your setup code here, to run once:
    Serial.begin(115200);
    myWiFiSetup();
//    yield();
    delay(1000);
//
//  ThingSpeak.begin(espClient);
//  Serial.println("ThingSpeak client connected"); 
//
  start_mqtt();
  Serial.println("MQTT client connected"); 
  create_topics();
//   
  tempSensor.begin();
  findDeviceAddr();   // locate OneWire devices on the bus
    delay(100);
  dht.begin(); 

  // The HX711 functions do not yield. Watchdog would reset
  zero_factor = scale.read();
 // scale.tare(); 
   }

void loop() {
  //  reads temperature and humidity
  sampleTempHum();
  yield();
  // reads temperature OneWire
  sampleTemperature();

  // gets weight
  weight();
  //write all fields to ThingSpeak
  //    writeAllFields();

  // this is mqtt part
  if (!client.connected()) {
    reconnect();
    Serial.println("Reconecting...");
  }
  client.loop();
  writeMqttFields();   // write fields to MQTT
  
  Serial.println("begin");
  Serial.println("Loop");
//  ESP.deepSleep(3600*1000000,WAKE_RF_DEFAULT);
  delay(100);

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

