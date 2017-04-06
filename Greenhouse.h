#include "Arduino.h"
#include "Vector.h"

#include "ClimateZone.h"

#ifndef Greenhouse_h
#define Greenhouse_h

class Greenhouse
{
  public:
    void   init(int named, Vector<ClimateZone> zones, int batteryVoltagePin = 0, int solarVoltagePin = 0);
    int    named;
    bool   inited = false;
    float  batteryVoltage;
    float  solarVoltage;
    void   check();
    int    getBatteryPercentage();
    int    getSolarPercentage();
    String status();
  private:
    int  _batteryVoltagePin;
    int  _solarVoltagePin;
    Vector<ClimateZone> climateZones;
    void  readSensors();
    float getOriginalVoltage(float vOut, int r1, int r2);
    int   readAnalog(int pin);
};

#endif
