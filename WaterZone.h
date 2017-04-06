#include "Arduino.h"

#ifndef WaterZone_h
#define WaterZone_h

class WaterZone
{
  public:
    WaterZone(int named, int sensorPin, int waterPin, int threshold);
    int named;
    void check(int degF);
  private:
    int _sensorPin;
    int _waterPin;
    bool _threshold;
    bool _status;
    void waterOn();
    void waterOff();
    int adjustValue(int sensorValue, int degF);
    int readAnalog(int pin);
};

#endif