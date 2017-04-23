//#include <ESP8266WiFi.h>
char myChipId[16] = {0};
void setup() {
  // put your setup code here, to run once:
    // start serial port
  Serial.begin(115200);
  // Connect to WiFi access point.
  Serial.println(); Serial.println();
Serial.println(ESP.getChipId());
sprintf(myChipId, "%06X", ESP.getChipId());
  Serial.print("Chip ID is : ");
  Serial.println(myChipId);
}

void loop() {
  // put your main code here, to run repeatedly:

}
