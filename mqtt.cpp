#include "mqtt.h"
#include <PubSubClient.h>     // https://github.com/knolleary/pubsubclient

#ifdef WIFISSID
#include <ESP8266WiFi.h>
#else
#include <WiFiManager.h>      // https://github.com/tzapu/WiFiManager
#endif

MQTT::MQTT(const char* mqtt_server, int mqtt_port)
  :  mqttServer(mqtt_server), mqttPort(mqtt_port), PubSubClient("digihive.nl", 1883, espClient)
{
}

MQTT::~MQTT()
{  
}

void MQTT::initialize()
{
  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  // wifiManager.resetSettings();
  wifiManager.autoConnect("DigiHive");
  Serial.println("Connected to Wifi...");

  setServer(mqttServer, mqttPort);

  char myChipId[10] = "";
  sprintf(myChipId, "%06X", ESP.getChipId());
  sprintf(arduinoClientName, "DigiHive_%s", myChipId);

  // Put the ESP ID into the topics
  for (int i = 0; i < NUM_TOPICS; i++) {
    for (int j = 0; j < 6; j++) {
      topicList[i][j] = myChipId[j];
    }
  }
}

void MQTT::initialize(const char* wifiSSID, const char* wifiPassword)
{
  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  // WiFiManager wifiManager;
  // wifiManager.resetSettings();
  // wifiManager.autoConnect("DigiHive");
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifiSSID);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifiSSID, wifiPassword);
  while (WiFi.status() != WL_CONNECTED)
  {
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
  Serial.println();

  Serial.println("Connected to Wifi...");

  setServer(mqttServer, mqttPort);

  char myChipId[10] = "";
  sprintf(myChipId, "%06X", ESP.getChipId());
  sprintf(arduinoClientName, "DigiHive_%s", myChipId);

  // Put the ESP ID into the topics
  for (int i = 0; i < NUM_TOPICS; i++) {
    for (int j = 0; j < 6; j++) {
      topicList[i][j] = myChipId[j];
    }
  }
}
 
int MQTT::reconnect()
{
  // Loop until we're reconnected
  Serial.println("Attempting MQTT connection...");
  for (int i = 0; !connected() && i < 5; i++) {
    // Attempt to connect
    if (connect(arduinoClientName)) {
      Serial.println("MQTT connection succesful.");
    } else {
      Serial.print("failed, rc=");
      Serial.print(state());
      Serial.println(" try again in 2 seconds");
      // Wait 2 seconds before retrying
      delay(2000);
      i++;
    }
  }

  return state();
}

void MQTT::publish(Topic topic, double message)
{
  char stringMessage[50] = "";
  dtostrf(message, 4, 2, stringMessage);

  PubSubClient::publish(topicList[topic], stringMessage);
}

