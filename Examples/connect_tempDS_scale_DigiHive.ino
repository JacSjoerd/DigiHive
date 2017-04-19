#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include "WiFiManager.h"          //https://github.com/tzapu/WiFiManager
// cloud
#include "ThingSpeak.h"
// temp DS
#include <OneWire.h>
#include <DallasTemperature.h>
// for scale
#include <HX711.h>

// ==================definitions
//const int led = 13;
//cloud
unsigned long myChannelNumber = ;
const char * myWriteAPIKey = "";
const char * myReadAPIKey = "";
// temp DS
#define ONE_WIRE_BUS D2 // DS18
//== Dallas device setup 
const int MAX_DEVICES = 3;
static int nDeviceCount = 0;
 
//scale
// sensor specific
#define calibration_factor 38// set co constant after calibration
#define zero_factor 755840 //set to constant after calibration
const byte hx711_data_pin = D5;  //HX711
const byte hx711_clock_pin = D6; //HX711

//====objects 
//wifi
ESP8266WebServer server(80);
WiFiClient  client;
//cloud

//temp DS 
static DeviceAddress tempDev[MAX_DEVICES];
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature tempSensor(&oneWire);
//scale
HX711 scale(hx711_data_pin, hx711_clock_pin);
// on frame is currently displayed
float currentWeight = 0;
//temp DHT

//=====variables =====
//wifi

//temp DS 

//scale

//temp DHT


// wifi set up

//============functions=======
 void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
}
void handleRoot() {
  digitalWrite(LED_BUILTIN, 1);
  server.send(200, "text/plain", "hello from esp8266!");
  digitalWrite(LED_BUILTIN, 0);
}
void handleNotFound(){
  digitalWrite(LED_BUILTIN, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(204, "text/plain", message);
  digitalWrite(LED_BUILTIN, 0);

}

static void findDeviceAddr(void)
{
  
  nDeviceCount = tempSensor.getDeviceCount();

  if (nDeviceCount > MAX_DEVICES)
    nDeviceCount = MAX_DEVICES;

//  for (int i=0; i < nDeviceCount; i++)
//  {
//      uint8_t *devAddr = tempDev[i];
//    if (tempSensor.getAddress(devAddr,i))
//      Serial.println("getAddress ok");
//  }
}
static void sampleTemperature()
{ //float valueTemp[MAX_DEVICES]; 
  // request to all devices on the bus
  tempSensor.requestTemperatures(); // Send the command to get temperatures

  for (int i=0; i < nDeviceCount; i++)
  {
    float tempC = tempSensor.getTempCByIndex(i);
    Serial.print(String(i+1)+": "+tempC+"C ");
//valueTemp[i]=tempC;
    ThingSpeak.setField( i+1+2, tempC);
  }
  //    Serial.print(valueTemp+"C ");

//  ThingSpeak.setField(3, valueTemp);

  Serial.println();
  if (nDeviceCount > 0)
  {
    int rc = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    Serial.println(String("Post rc=")+rc);
     delay(15000);
  }
}
static void sampleWeight()
{ 
  Serial.print("Reading weight: ");
  // double read value because of only 2 sensors
  currentWeight = 2* scale.get_units(5); //reads 5 times and gets average with scale factor
  Serial.print(currentWeight);
  Serial.print(" grams"); 
  Serial.println();
 
  ThingSpeak.setField( 5, currentWeight);
  int rc = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  Serial.println(String("Post rc=")+rc);
     delay(15000);
}
//-------------------------setup------
void setup() {
  // wifi setup
 // put your setup code here, to run once:
  Serial.begin(115200);
  
  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  //reset settings - for testing
  //wifiManager.resetSettings();

  //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
  wifiManager.setAPCallback(configModeCallback);

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if(!wifiManager.autoConnect()) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(1000);
  } 
  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");
  //
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 0);
 //
  server.on("/", handleRoot);

  server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });
// server.on("/led", []() {
//  String state=server.arg("state");
//  if (state == "on") digitalWrite(LED_BUILTIN, LOW);
//  else if (state == "off") digitalWrite(LED_BUILTIN, HIGH);
//  server.send(200, "text/plain", "Led is now " + state);
//});

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");  
  //
  ThingSpeak.begin(client);
  Serial.println("ThingSpeak client connected");  
 
  tempSensor.begin();
  findDeviceAddr();   // locate OneWire devices on the bus
  // The HX711 functions do not yield. Watchdog would reset
  scale.set_scale(2158.45f); 

  scale.set_scale(calibration_factor); //Adjust to this calibration factor
  scale.set_offset(zero_factor);
 // scale.tare(); 
}

void loop() {  
  float temp;
  // put your main code here, to run repeatedly:
  server.handleClient();

// reads temperature and sends it to Thingspeak
  sampleTemperature();
// gets weight 
 sampleWeight();

  //delay(3600000);// in milliseconds
//deep sleep
//  ESP.deepSleep(SLEEP_DELAY_IN_SECONDS * 1000000, WAKE_RF_DEFAULT);
//  delay(1000); // wait for deep sleep to happen
}
