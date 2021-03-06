#include "Arduino.h"
#include "Vector.h"
#include "Greenhouse.h"

const float SOLAR_PANEL_MAX_V = 15.8;
const int ANALOG_MAX_VALUE = 1023;

void Greenhouse::init(int _id, int _named, Vector<ClimateZone> zones, int batteryVoltagePin, int solarVoltagePin)
{
   id                = _id;
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
    Serial.print("Battery - Raw: ");
    Serial.print(batteryVoltage);
    Serial.print(" Percentage: ");
    Serial.print(getBatteryPercentage());
    Serial.println("%");
  }

  if(_solarVoltagePin != 0) {
    solarVoltage = readAnalog(_solarVoltagePin);
    Serial.print("Solar - Raw: ");
    Serial.print(solarVoltage);
    Serial.print(" Calculated Voltage: ");
    Serial.print( getOriginalVoltage(batteryVoltage, 2000, 1000) );
    Serial.print(" Percentage: ");
    Serial.print(getSolarVoltage());
    Serial.println("V");
  }

  //vent status

}

float Greenhouse::getBatteryPercentage()
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

float Greenhouse::getSolarVoltage()
{
  if( solarVoltage == 0 ) {
    return 0;
  }

  float org = getOriginalVoltage(solarVoltage, 2000, 1000);

  return org;
}

float Greenhouse::getOriginalVoltage(float vOut, int r1, int r2)
{
  float voltage = vOut * (5.0 / ANALOG_MAX_VALUE);
  voltage = voltage * ( ( r1 + r2 ) / r2 );
  return voltage;
}

int Greenhouse::readAnalog(int pin)
{
  int sensorValue = 0;

  for (int i = 0; i < 10; ++i) {
    sensorValue += analogRead(pin);
    delay(1);
  }

  sensorValue = (sensorValue / 10);

  return sensorValue;
}

String Greenhouse::getJson()
{
    String json = String("{ \"id\": " + id);
    json += ", \"batteryVoltage\": ";
    json += batteryVoltage;
    json += ", \"solarVoltage\":";
    json += solarVoltage;
    json += ", \"climateZones\": [";

    for (size_t i = 0; i < climateZones.size(); ++i) {
      json += climateZones[i].getJson();
      if( i + 1 != climateZones.size() ) {
          json += ", ";
      }
    }

    json += "] }";

    return json;
}
