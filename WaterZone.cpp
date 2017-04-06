
#include "Arduino.h"
#include "WaterZone.h"

const int MIN_DEG_TO_WATER = 40;
const int WATER_SENSOR_MAX_VALUE = 1024;

WaterZone::WaterZone(int _named, int sensorPin, int waterPin, int threshold)
{
     named        = _named;
    _sensorPin    =  sensorPin;
    _waterPin     =  waterPin;
    _threshold    =  threshold;

    pinMode(_waterPin, OUTPUT);
    digitalWrite(_waterPin, LOW);

    _status = false;
}

void WaterZone::check(int degF, int humidity)
{
    if( degF < MIN_DEG_TO_WATER) {
        Serial.print("        ");
        Serial.print(named);
        Serial.println(" Zone is to cold to water.");
        return void();
    }

    // int sensorValue = readAnalogreadAnalog(_sensorPin);
    int sensorValue = 0;
    if( digitalRead(_sensorPin) == HIGH ) {
        sensorValue = 987;
    }

    Serial.print("    ");
    Serial.print(named);
    Serial.print(" Moisture Sensor value: ");
    Serial.println(sensorValue);

    int adjustedValue = adjustValue(sensorValue, degF, humidity);

    if( sensorValue < _threshold ) {
        if( _status != true ) {
            waterOn();
        }
    } else {
        if( _status != false ) {
            waterOff();
        }
    }
}

void WaterZone::waterOn()
{
    digitalWrite(_waterPin, HIGH);
    _status = true;

    Serial.print("    ");
    Serial.print(named);
    Serial.println(" turning on");
}

void WaterZone::waterOff()
{
    digitalWrite(_waterPin, LOW);
    _status = false;

    Serial.print("    ");
    Serial.print(named);
    Serial.println(" turning off");
}

int WaterZone::adjustValue(int sensorValue, int degF, int humidity)
{
    if (degF < MIN_DEG_TO_WATER) {
        return degF;
    }

    // The hotter it is the more we want to water
    int adjusted = sensorValue * ( 1 + (degF * .01) );

    // The more humid it is the less we need to water
    if(humidity > 85) {
        int adjusted = ( sensorValue * .9 );
    }

    if(adjusted > WATER_SENSOR_MAX_VALUE) {
        return WATER_SENSOR_MAX_VALUE;
    }

    return adjusted;
}

int    WaterZone::readAnalog(int pin)
{
    int sensorValue = 0;

    for (int i = 0; i < 10; ++i) {
        sensorValue + analogRead(pin);
        delay(1);
    }

    sensorValue = sensorValue / 10;

    return sensorValue;
}
