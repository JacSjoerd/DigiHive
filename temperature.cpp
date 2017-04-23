#include "temperature.h"
#include <OneWire.h>
#include <DallasTemperature.h>      // https://github.com/milesburton/Arduino-Temperature-Control-Library

Temperature::Temperature(int pinDefinition)
{
  oneWire = new OneWire(pinDefinition);
  setOneWire(oneWire);
}

Temperature::~Temperature()
{

}

uint8_t Temperature::deviceCount()
{
  begin();
  nDeviceCount = getDeviceCount();
  if (nDeviceCount > MAX_DEVICES) {
    nDeviceCount = MAX_DEVICES;
  }
  requestTemperatures();

  return nDeviceCount;
}

float Temperature::read(uint8_t index)
{
  return getTempCByIndex(index);
}

