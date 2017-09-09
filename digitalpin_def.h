
#ifndef DIGITALPIN_DEF_H
#define DIGITALPIN_DEF_H

// define the MQTT server
#define MQTT_SERVER "5.189.151.254"
#define MQTT_PORT 1883

//define pin for DS18B20
#define DS18B20_PIN 4 // GPIO4

//define pin for AM2320
#define DHT_PIN 2       // GPIO2
// Uncomment the DHT type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22     // DHT 22 (AM2302), AM2321, AM2320
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// define pin for HX711 scale controller
#define HX711_DATA_PIN 12  // GPIOI2  ESP8266
#define HX711_CLOCK_PIN 14 // GPIOI4  ESP8266
// #define HX711_DATA_PIN D6  // GPIOI2  NodeMCU
// #define HX711_CLOCK_PIN D5 // GPIOI4  NodeMCU
#define WEIGHTFACTOR 21 // factor to convert reading to gram

// define fixed network
//#define WIFISSID "network"
//#define WIFIPASSWORD "password"

#endif DIGITALPIN_DEF_H
