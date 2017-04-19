#include "Arduino.h"
#include "Vector.h"

#include "ClimateZone.h"

#ifndef Greenhouse_h
#define Greenhouse_h

class Greenhouse
{
  public:
    void   init(int _id,int named, Vector<ClimateZone> zones, int batteryVoltagePin = 0, int solarVoltagePin = 0);
    int    id;
    int    named;
    bool   inited = false;
    float  batteryVoltage;
    float  solarVoltage;
    void   check();
    float  getBatteryPercentage();
    float  getSolarVoltage();
    String getJson();
  private:
    int  _batteryVoltagePin;
    int  _solarVoltagePin;
    Vector<ClimateZone> climateZones;
    void  readSensors();
    float getOriginalVoltage(float vOut, int r1, int r2);
    int   readAnalog(int pin);
};

#endif
