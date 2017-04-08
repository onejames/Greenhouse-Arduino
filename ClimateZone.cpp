#include "Arduino.h"
#include "Vector.h"
#include "ClimateZone.h"

DHT_Unified dht(11, DHT11);

ClimateZone::ClimateZone(int _named, int thPin,  Vector<WaterZone> zones)
// : _dht(_thPin, DHT11)
{
  named  = _named;
  _thPin = thPin;
  waterZones = zones;

  ClimateZone::initSensor();
}

void ClimateZone::check()
{
  ClimateZone::getTHValues();

  for (size_t i = 0; i < waterZones.size(); ++i) {
    Serial.print("  ");
    Serial.print("Climate Zone ");
    Serial.print(named);
    Serial.print(" checking Water Zone: ");
    Serial.println(waterZones[i].named);
    waterZones[i].check(temprature, humidity);
  }
}

void ClimateZone::initSensor()
{
  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);

  Serial.print("DHT on pin ");
  Serial.print(_thPin);
  Serial.println(" started");
}

void ClimateZone::getTHValues()
{
  sensors_event_t event;
  dht.temperature().getEvent(&event);

  if (isnan(event.temperature)) {
    Serial.println("  Error reading temperature!");
    temprature = 0;
  }
  else {
    Serial.print("  ");
    Serial.print(named);
    Serial.print(" Temperature: ");
    temprature = (event.temperature * 1.8 ) + 32;
    Serial.print(temprature);
    Serial.println(" *F");
  }

  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println("  Error reading humidity!");
    humidity = 0;
  }
  else {
    Serial.print("  ");
    Serial.print(named);
    Serial.print(" Humidity: ");
    humidity = event.relative_humidity;
    Serial.print(humidity);
    Serial.println("%");
  }
}
