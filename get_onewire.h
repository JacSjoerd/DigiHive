// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>

const int MAX_DEVICES = 3;
static int nDeviceCount = 0;

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature tempSensor(&oneWire);
//temp DS 
float tempC[2];
/*********************functions DS*******************************/
static void findDeviceAddr(void)
{
  nDeviceCount = tempSensor.getDeviceCount();

  if (nDeviceCount > MAX_DEVICES)
    nDeviceCount = MAX_DEVICES;
}
/*********************functions DS*******************************/
static void sampleTemperature()
{ //float valueTemp[MAX_DEVICES]; 
  yield();
  // request to all devices on the bus
  tempSensor.requestTemperatures(); // Send the command to get temperatures

  for (int i=0; i < nDeviceCount; i++)
  {
    //float 
    tempC[i] = tempSensor.getTempCByIndex(i);
    Serial.print(String(i+1)+": "+tempC[i]+"C ");
  }

  Serial.println();
}
