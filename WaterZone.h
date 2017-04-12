#include "Arduino.h"

#ifndef WaterZone_h
#define WaterZone_h

class WaterZone
{
  public:
    WaterZone(int _id, int named, int sensorPin, int waterPin, int threshold);
    int    id;
    int    named;
    void   check(int degF, int humidity);
    String getJson();
  private:
    int   _sensorPin;
    int   _waterPin;
    bool  _threshold;
    bool  _status;
    int   _sensorValue;
    int   _microtimeStarted;
    int   _microtimeStopped;
    void   waterOn();
    void   waterOff();
    int    adjustValue(int sensorValue, int degF, int humidity);
    int    readAnalog(int pin);
};

#endif
