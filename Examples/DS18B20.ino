#include <OneWire.h>

// DS18S20 Temperature chip i/o
OneWire ds(3);  // on pin 10

unsigned long timerOn = 30000;   // milli sec
unsigned long timerOff = 30000; // milli sec
int targetTemperature = 37;      // Celcius

unsigned long timer = 0;
bool isOn = false;

void setup(void) {
  // initialize inputs/outputs
  // start serial port
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(4, OUTPUT);
}

void loop(void) {
  byte i;
  byte present = 0;
  byte data[12];
  byte addr[8];
  int HighByte, LowByte, TReading, SignBit, Tc_100, Whole, Fract;
  unsigned long currentTime;
  
  ds.reset_search();
  if ( !ds.search(addr)) {
      Serial.print("No more addresses.\n");
      ds.reset_search();
      return;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44);         // start conversion, with parasite power on at the end

  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.

  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  }

  LowByte = data[0];
  HighByte = data[1];
  TReading = (HighByte << 8) + LowByte;
  SignBit = TReading & 0x8000;  // test most sig bit
  if (SignBit) // negative
  {
    TReading = (TReading ^ 0xffff) + 1; // 2's comp
  }
  Tc_100 = (6 * TReading) + TReading / 4;    // multiply by (100 * 0.0625) or 6.25

  Whole = Tc_100 / 100;  // separate off the whole and fractional portions
  Fract = Tc_100 % 100;


  Serial.print("Temp: ");
  if (SignBit) // If its negative
  {
     Serial.print("-");
  }
  Serial.print(Whole);
  Serial.print(".");
  if (Fract < 10)
  {
     Serial.print("0");
  }
  Serial.print(Fract);

  Serial.print(" C\n");

  currentTime = millis();
//  Serial.print("Time is: ");
//  Serial.print(currentTime);
//  Serial.print("\tTimer is set at ");
//  Serial.print(timer);
//  Serial.print("\tLight is ");
//  Serial.print(isOn);
//  Serial.println();

  if (Whole < targetTemperature)
  {
    if (currentTime > timer)
    {
      Serial.println("Timer has passed");
      if(isOn)
      {
        Serial.println("Putting heater off.");
        timer = timer + timerOff;
        isOn = false;
        digitalWrite(13, LOW);
        digitalWrite(4, HIGH);
      }
      else
      {
        Serial.println("Putting heater on.");
        timer = timer + timerOn;
        isOn = true;
        digitalWrite(13, HIGH);
        digitalWrite(4, LOW);
      }
      
    } 
  }
  else
  {
    isOn=false;
    digitalWrite(4, HIGH);
    digitalWrite(13, LOW);
    Serial.println("Destination temperature reached");
  }
} 

