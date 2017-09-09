#ifndef MQTT_H
#define MQTT_H

#include <PubSubClient.h>     // https://github.com/knolleary/pubsubclient
#include <WiFiManager.h>      // https://github.com/tzapu/WiFiManager

enum Topic {
  TOPIC_IN_TEMP1,
  TOPIC_OUT_TEMP,
  TOPIC_IN_TEMP2,
  TOPIC_IN_HUMID1,
  TOPIC_IN_HUMID2,
  TOPIC_OUT_HUMID,
  TOPIC_WEIGHT,
  NUM_TOPICS
};

class MQTT : public PubSubClient
{
  private:
    char topicList[NUM_TOPICS][40] = { 
      "000000/level1/temperature",
      "000000/outside/temperature",
      "000000/level2/temperature",
      "000000/level1/humidity",
      "000000/level2/humidity",
      "000000/outside/humidity",
      "000000/weight"
    };
    Client* mClient;
    WiFiClient espClient;
    char  arduinoClientName[40];      // ID to connect to the MQTT server, must be unique on the server
    const char* mqttServer;
    int mqttPort;
    
  public:
    MQTT(const char* mqtt_server, int mqtt_port);
    ~MQTT();

    void initialize();
    void initialize(const char* wifiSSID, const char* wifiPassword);
    int reconnect();
    void publish(Topic topic, double message);
  
};

#endif MQTT_H

