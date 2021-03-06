#include "Arduino.h"
#include "Vector.h"

#include "WaterZone.h"

// Adafruit Unified Sensor - Version: Latest
#include <Adafruit_Sensor.h>

// DHT sensor library - Version: Latest
#include <DHT.h>
#include <DHT_U.h>

#ifndef ClimateZone_h
#define ClimateZone_h

class ClimateZone
{
  public:
    ClimateZone(int _id, int named, int thPin, Vector<WaterZone> zones);
    int  id;
    int  named;
    int  temperature;
    int  humidity;
    void check();
    String getJson();
  private:
    int  _thPin;
    DHT_Unified* _dht;
    bool _status;
    Vector<WaterZone> waterZones;
    void  initSensor();
    void  readDHT();
};

#endif
