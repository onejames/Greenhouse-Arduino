#include "Arduino.h"

#ifndef WaterZone_h
#define WaterZone_h

class WaterZone
{
  public:
    WaterZone(int named, int sensorPin, int waterPin, int threshold);
    int named;
    void check(int degF, int humidity);
  private:
    int   _sensorPin;
    int   _waterPin;
    bool  _threshold;
    bool  _status;
    int   _microtimeStarted;
    int   _microtimeStopped;
    void   waterOn();
    void   waterOff();
    int    adjustValue(int sensorValue, int degF, int humidity);
    int    readAnalog(int pin);
};

#endif