#include <PubSubClient.h>

//Prepare Mqtt connection data
const char* MQTTBroker = "5.189.151.254";
uint16_t port = 1883;  //default port

// variable chip ID
char myChipId[16] = {0};
char Topic_outside[50]="";
char Topic_inhum1[50]="";
char Topic_intem1[50]="";
char Topic_intem2[50]="";
char Topic_weight[50]="";

WiFiClient espClient;
PubSubClient client("digihive.nl", 1883, espClient);


// Create an ESP8266 WiFiClient class to connect to the MQTT server.
char msg[50] = "";

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}
void reconnect() {
  // Loop until we're reconnected
  int i=0;
  while (!client.connected() && i < 5) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 2 seconds before retrying
      delay(2000);
      i++;
    }
  }
  
}
static void writeMqttFields(){
  Serial.println("Attempting MQTT write topics...");
  //String con = String(count);
  delay(1000);
  if (!client.connected()) {
    reconnect();
    Serial.println("Reconecting...");
  }
  
  if (!isnan(temp_DHT)){
    dtostrf(temp_DHT,2,2,msg);
    Serial.println(msg);
    if (client.publish(Topic_intem1, msg)) {
//      Serial.println(sprintf("Published value %s to topic %s",msg,Topic_intem1));  //publish
      Serial.println("Published DHT temp");
    }
  }
  
  if (!isnan(hum_DHT)){  
    dtostrf(hum_DHT,2,2,msg);
    Serial.println(msg);
    if (client.publish(Topic_inhum1, msg)) {
//      Serial.println(sprintf("Published value %s to topic %s",msg,Topic_inhum1));  //publish
      Serial.println("Published DHT hum");
    }
  }

  for (int i=0; i < nDeviceCount; i++)
  {
    dtostrf(tempC[i],2,2,msg);
    Serial.println(msg);
    if (i==0) {
      if (client.publish(Topic_outside, msg)) {
//        Serial.println(sprintf("Published value %s to topic %s",msg,Topic_outside));  //publish
        Serial.println("Published temp1");
      }
    } else {
      if (client.publish(Topic_intem2, msg)) {
//        Serial.println(sprintf("Published value %s to topic %s",msg,Topic_intem2));  //publish
        Serial.println("Published  temp 2");
      }
    }
  }
}
/****************************** Feeds ***************************************/
void create_topics() {
  sprintf(myChipId, "%06X", ESP.getChipId());
  Serial.print("Chip ID is : ");
  Serial.println(myChipId);
  
//  prepare topic names
  sprintf(Topic_outside, "%s/outside/temperature", myChipId);
  sprintf(Topic_inhum1, "%s/level1/humidity", myChipId);
  sprintf(Topic_intem1, "%s/level1/temperature", myChipId);
  sprintf(Topic_intem2, "%s/level2/temperature", myChipId);
  sprintf(Topic_weight, "%s/weight", myChipId);
//  Serial.println();
//// check
  Serial.println(); Serial.println();
  Serial.println(Topic_outside);
  Serial.println(Topic_intem1);
  Serial.println(Topic_inhum1);
  Serial.println(Topic_intem2);
  Serial.println(Topic_weight);
}


