#include "Arduino.h"
#include "Vector.h"
#include "Greenhouse.h"

const float SOLAR_PANEL_MAX_V = 15.8;
const int ANALOG_MAX_VALUE = 1023;

void Greenhouse::init(int _named, Vector<ClimateZone> zones, int batteryVoltagePin, int solarVoltagePin)
{
   named             = _named;
   climateZones      = zones;
  _batteryVoltagePin = batteryVoltagePin;
  _solarVoltagePin   = solarVoltagePin;
   batteryVoltage    = 0;
   solarVoltage      = 0;

   inited = true;
}

void Greenhouse::check()
{
  readSensors();

  for (size_t i = 0; i < climateZones.size(); ++i) {
    Serial.print("Checking ClimateZone ");
    Serial.println(climateZones[i].named);
    climateZones[i].check();
  }
}

void Greenhouse::readSensors()
{
  if(_batteryVoltagePin != 0) {
    batteryVoltage = readAnalog(_batteryVoltagePin);
    Serial.print("Battery Voltage: ");
    Serial.println(getBatteryPercentage());
  }

  if(_solarVoltagePin != 0) {
    solarVoltage = readAnalog(_solarVoltagePin);
    Serial.print("Solar Voltage: ");
    Serial.println(getSolarPercentage());
  }

  //vent status

}

int Greenhouse::getBatteryPercentage()
{
  if( batteryVoltage == 0 ) {
    return 0;
  }

  float orig = getOriginalVoltage(batteryVoltage, 2000, 1000);

  if( orig > 12.7) {
    return 100;
  }

  if( orig > 12.4) {
    return 75;
  }

  if( orig > 12.2) {
    return 50;
  }

  if( orig > 12) {
    return 25;
  }

  if( orig > 11.9) {
    return 13;
  }


return 0;

  // 100%	12.70
  // 75%	12.40
  // 50%	12.20
  // 25%	12.00
  // 0%	11.80
}

int Greenhouse::getSolarPercentage()
{
  if( solarVoltage == 0 ) {
    return 0;
  }
  Serial.print("SVOrig: ");
  Serial.println(solarVoltage);
  float org = getOriginalVoltage(solarVoltage, 2000, 1000);

  return org;
  // return org / SOLAR_PANEL_MAX_V;
}

float Greenhouse::getOriginalVoltage(float vOut, int r1, int r2)
{
  float voltage = vOut * (5 / ANALOG_MAX_VALUE);

  return voltage*(r1+r2)/r2;
}

int Greenhouse::readAnalog(int pin)
{
  // Serial.print("pin: ");
  // Serial.println(pin);
  int sensorValue = 0;

  for (int i = 0; i < 10; ++i) {
    sensorValue + analogRead(pin);
    // Serial.println(sensorValue);
    delay(1);
  }

  sensorValue = (sensorValue / 10);

  return sensorValue;
}

String Greenhouse::status()
{
//    return String('{"greenhouse": {  "id": "44/286asd3gppq12f34n", "status" : {"temp" : 78, "hym"  : 82 }, "errors" : { }}}');
    return String("{\"greenhouse\" }");
}
