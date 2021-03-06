
#include "Arduino.h"
#include "WaterZone.h"

const int MIN_DEG_TO_WATER = 40;
const int WATER_SENSOR_MAX_VALUE = 1024;
//const int MAX_WATER_RUN_MILLS    = 300000; // 5 min/
const int MAX_WATER_RUN_MILLS    = 30000; // 30 sec
//const int WATER_DELAY_MILLS      = 300000; // 5 min/
const int WATER_DELAY_MILLS      = 30000; // 30 sec

WaterZone::WaterZone(int _id, int _named, int sensorPin, int waterPin, int threshold)
{
     id           = _id;
     named        = _named;
    _sensorPin    =  sensorPin;
    _waterPin     =  waterPin;
    _threshold    =  threshold;

    pinMode(_waterPin, OUTPUT);
    digitalWrite(_waterPin, HIGH);

    _status = false;
}

void WaterZone::check(int degF, int humidity)
{

//    if(_status == true && (_microtimeStarted + MAX_WATER_RUN_MILLS) < millis() ) {
//        Serial.println("Water on time excedded, turning off for now.");
//        waterOff();
//        return void();
//    } else if(_status == false && (_microtimeStopped + WATER_DELAY_MILLS) > millis() ) {
//        Serial.println("Watering in paused mode");
//        Serial.println(millis());
//        Serial.println(_microtimeStopped + WATER_DELAY_MILLS);
//        return void();
//    }

    if( _status == false && degF != 0 && degF < MIN_DEG_TO_WATER ) {
        Serial.print("        ");
        Serial.print(named);
        Serial.println(" Zone is to cold to water.");
        return void();
    }

    _sensorValue = readAnalog(_sensorPin);

    int adjustedValue = _sensorValue;

    if(degF != 0 && humidity != 0) {
        int adjustedValue = adjustValue(_sensorValue, degF, humidity);
    }

    Serial.print("    ");
    Serial.print(named);
    Serial.print(" Moisture Sensor (adjusted) value: ");
    Serial.println(adjustedValue);

    if( adjustedValue < _threshold ) {
        if( _status != true ) {
            waterOn();
        } else {
            Serial.println("Water already on");
        }
    } else {
        if( _status != false ) {
            waterOff();
        } else {
            Serial.println("Water already off");
        }
    }
}

void WaterZone::waterOn()
{
    digitalWrite(_waterPin, LOW);
    _status = true;
    _microtimeStopped = 0;
    _microtimeStarted = millis();

    Serial.print("    ");
    Serial.print(named);
    Serial.println(" turning on");
}

void WaterZone::waterOff()
{
    digitalWrite(_waterPin, HIGH);
    _status = false;
    _microtimeStarted = 0;
    _microtimeStopped = millis();

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

int WaterZone::readAnalog(int pin)
{
    int sensorValue = 0;

    for (int i = 0; i < 10; ++i) {
        sensorValue += analogRead(pin);
        delay(1);
    }

    sensorValue = sensorValue / 10;

//    for (int i = 0; i < sensorValue; i + 100) {
//        digitalWrite(13, HIGH);
//        delay(100);
//        digitalWrite(13, LOW);
//        delay(100);
//    }
//
//        digitalWrite(13, HIGH);
//        delay(1000);
//        digitalWrite(13, LOW);

    return sensorValue;
}

String WaterZone::getJson()
{
    String json = String("{ \"id\": " + id);
    json += ", \"status\": " + _status;
    json += ", \"sensorValue\":" + _sensorValue;
    json += "}";

    return json;
}
