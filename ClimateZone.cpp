
// DHT sensor library - Version: Latest
#include <DHT.h>
#include <DHT_U.h>

#include "Arduino.h"
#include "Vector.h"
#include "ClimateZone.h"

ClimateZone::ClimateZone(int _id, int _named, int thPin,  Vector<WaterZone> zones)
// : _dht(_thPin, DHT11)
{
   id         = _id;
   named      = _named;
   waterZones =  zones;
  _thPin      =  thPin;

  _dht = new DHT_Unified(thPin, DHT11);

  ClimateZone::initSensor();
}

void ClimateZone::check()
{
  ClimateZone::readDHT();

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
  _dht->begin();
  sensor_t sensor;
  _dht->temperature().getSensor(&sensor);
  _dht->humidity().getSensor(&sensor);

  Serial.print("DHT on pin ");
  Serial.print(_thPin);
  Serial.println(" started");
}

void ClimateZone::readDHT()
{
  sensors_event_t event;
  _dht->temperature().getEvent(&event);

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

  _dht->humidity().getEvent(&event);
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

String ClimateZone::getJson()
{
    String json = String("{ 'id': " + id);
    json += ", 'temprature': " + temprature;
    json += ", 'humidity':" + humidity;

    json += ", 'waterZones': {";
    for (size_t i = 0; i < waterZones.size(); ++i) {
      json += waterZones[i].getJson();
      if( i + 1 != waterZones.size() ) {
          json += ", ";
      }
    }

    json += "} }";

    return json;
}
