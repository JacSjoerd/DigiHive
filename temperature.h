#ifndef TEMPERATURE_H
#define TEMPERATURE_H
#define REQUIRESALARMS false

#include <OneWire.h>
#include <DallasTemperature.h>      // https://github.com/milesburton/Arduino-Temperature-Control-Library

const int MAX_DEVICES = 3;

class Temperature : public DallasTemperature
{
  private:
    int nDeviceCount;
    byte deviceList[8];
    OneWire* oneWire;

  public:
    Temperature(int pinDefinition);
    ~Temperature();

    uint8_t deviceCount();
    float read(uint8_t index);
};

#endif TEMPERATURE_H
