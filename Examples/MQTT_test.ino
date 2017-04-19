  /***************************************************
  Adafruit MQTT Library ESP8266 Example 

  Must use ESP8266 Arduino from:
    https://github.com/esp8266/Arduino
Works great with Adafruit's Huzzah ESP board & Feather
  ----> https://www.adafruit.com/product/2471
  ----> https://www.adafruit.com/products/2821

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Tony DiCola for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// #include <WiFiManager.h>   

// Include the libraries we need
//#include <OneWire.h>
//#include <DallasTemperature.h>
//#include "DHT.h"
//// =======for scale==========
//#include <HX711.h>

// user libs
#include "get_data.h"

/**************************/
//#define AIO_SERVER      "m20.cloudmqtt.com"
//#define AIO_SERVERPORT  15115                   // use 8883 for SSL
//#define AIO_USERNAME    "frgcxbme"
//#define AIO_KEY         "UKJm7-63Rrfm"

//Prepare Mqtt connection data
//const char *MQTTBroker = "m20.cloudmqtt.com";
const char *MQTTBroker = "192.168.1.105";
const char *MQTTuser = "";
const char *MQTTpassword = "";
// variable chip ID
char myChipId[16] = {0};
char Topic_outside[50]="";
char Topic_inhum1[50]="";
char Topic_intem1[50]="";
char Topic_intem2[50]="";
char Topic_weight[50]="";

//uint16_t port = 8888;
uint16_t port = 1883;  //default port
const char* ssid     = "Your_Network";
const char* password = "Your_Password";
const char* host = "192.168.1.105";

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient espClient;
PubSubClient client(espClient);

char msg[50];
//=====variables =====
//wifi

// Connect to a WiFi network
void setup_wifi() {
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    switch (WiFi.status()) {
      case WL_NO_SHIELD:
        Serial.println("No ESP8266 shield available.");
        break;
      case WL_IDLE_STATUS:
        Serial.println("Nothing to do, waiting.");
        break;
      case WL_NO_SSID_AVAIL:
        Serial.println("Can't find network (No SSID)");
        break;
      case WL_SCAN_COMPLETED:
        Serial.println("Scan completed");
        break;
      case WL_CONNECTED:
        Serial.println("Connected.");
        break;
      case WL_CONNECT_FAILED:
        Serial.println("Failed to connect.");
        break;
      case WL_CONNECTION_LOST:
        Serial.println("Connection lost.");
        break;
      case WL_DISCONNECTED:
        Serial.println("Disconnected");
        break;
    }
  }
}


static void writeMqttFields(){
  Serial.println("Attempting MQTT write topics...");
  //String con = String(count);
  delay(1000);
  temp_DHT = 20;
  hum_DHT = 80;
  currentWeight = 2000;
  if (!isnan(temp_DHT)){
    dtostrf(temp_DHT,2,2,msg);
    if (client.publish(Topic_intem1, msg)) {
      Serial.print("Published value ");
      Serial.print(msg);
      Serial.print(" to topic ");
      Serial.println(Topic_intem1);  //publish
     };
  } else {
    Serial.println("Temp_DHT is not a number");
  }
  if (!isnan(hum_DHT)){  
    dtostrf(hum_DHT,2,2,msg);
    if (client.publish(Topic_inhum1, msg)) {
      Serial.print("Published value ");
      Serial.print(msg);
      Serial.print(" to topic ");
      Serial.println(Topic_inhum1);  //publish
    }
  }
  if (currentWeight > 0){ 
    dtostrf(currentWeight,6,0,msg);
    if (client.publish(Topic_weight, msg)) {
      Serial.print("Published value ");
      Serial.print(msg);
      Serial.print(" to topic ");
      Serial.println(currentWeight);  //publish
    }
  }
  
  for (int i=0; i < nDeviceCount; i++)
  {
    dtostrf(tempC[i],2,2,msg);
    if (i==0){
      if (client.publish(Topic_outside, msg)) {
        Serial.print("Published value ");
        Serial.print(msg);
        Serial.print(" to topic ");
        Serial.println(Topic_outside);  //publish
      }
    } else {
      if (client.publish(Topic_intem2, msg)) {
        Serial.print("Published value ");
        Serial.print(msg);
        Serial.print(" to topic ");
        Serial.println(Topic_intem2);  //publish
      }
    }
  }
}
/****************************** Feeds ***************************************/
//void callback(const MQTT::Publish& pub){
//  // handle message arrived
//  Serial.println(pub.payload_string());
//}

//void MQTT_connect();
/*
 * The setup function. We only start the sensors here
 */
void setup(void)
{
  // start serial port
  Serial.begin(115200);
  // Connect to WiFi access point.
  delay(10);
  setup_wifi();
  client.setServer(host, 1883);
  client.setCallback(callback);
  
   // put your setup code here, to run once:
  sprintf(myChipId, "%06X", ESP.getChipId());
  Serial.print("Chip ID is : ");
  Serial.println(myChipId);
  sprintf(Topic_outside, "%s/outside/temperature", myChipId);
  sprintf(Topic_inhum1, "%s/level1/humidity", myChipId);
  sprintf(Topic_intem1, "%s/level1/temperature", myChipId);
  sprintf(Topic_intem2, "%s/level2/temperature", myChipId);
  sprintf(Topic_weight, "%s/weight", myChipId);

  Serial.println(); Serial.println();
  Serial.println(Topic_outside);
  Serial.println(Topic_intem1);
  Serial.println(Topic_inhum1);
  Serial.println(Topic_intem2);
  Serial.println(Topic_weight);

  Serial.println("MQTT demo");

   // Start up the library   
  tempSensor.begin();
//  findDeviceAddr();   // locate OneWire devices on the bus
    delay(100);
  dht.begin(); 
 // The HX711 functions do not yield. Watchdog would reset
//  scale.set_scale(2158.45f); 
//
//  scale.set_scale(calibration_factor); //Adjust to this calibration factor
//  scale.set_offset(zero_factor);
// // scale.tare();  
}
/*
 * Main function, get and show the temperature
 */
void loop(void)
{ 
  if (!client.connected()) {
    reconnect();
    Serial.println("Reconecting...");
  }


  // reads temperature and humidity
  sampleTempHum();
  yield();
  sampleTemperature();
  sampleWeight();
  
//  mqtt.loop();
  writeMqttFields();

//mqtt.loop();
  // ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds
  delay(20000);
  
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
     if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

/*********************functions DS*******************************/
static void findDeviceAddr(void)
{
  nDeviceCount = tempSensor.getDeviceCount();

  if (nDeviceCount > MAX_DEVICES)
    nDeviceCount = MAX_DEVICES;
}
