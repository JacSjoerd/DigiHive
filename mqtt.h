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
    WiFiManager wifiManager;
    const char* mqttServer;
    uint8_t mqttPort;
  public:
    MQTT(const char* server, uint8_t port);
    ~MQTT();

    void initialize();
    int reconnect();
    void publish(Topic topic, double message);
  
};

#endif MQTT_H

