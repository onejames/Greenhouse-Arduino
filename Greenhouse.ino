#include <SoftwareSerial.h>
#include <Arduino.h>

#include "Vector.h"
#include "WaterZone.h"
#include "ClimateZone.h"
#include "Greenhouse.h"

const int DELAY_TIME = 5000;

unsigned long millTime;

Greenhouse greenhouse;

// SoftwareSerial webSocket(10, 11); // RX, TX

void setup()
{
  // analogReference(INTERNAL);
  
  Serial.begin(115200);
  // webSocket.begin(115200);
  
  art();
  
  Serial.println("Booting...");
  Serial.println("");
  
  initGreenhouse();
  
  millTime = 0;
  
  Serial.println("");
  Serial.println("...Ready");
  Serial.println("");
}

void loop()
{
  greenhouse.check();

  doDelay();
}

void initGreenhouse()
{
  Vector<WaterZone> cz1Zones;
  cz1Zones.push_back(WaterZone(1, 6, 4, 512));
  cz1Zones.push_back(WaterZone(2, , 7, 512));
  
  Vector<ClimateZone> climateZones1;
  climateZones1.push_back(ClimateZone(1, 11, cz1Zones));
  
  greenhouse.init(1, climateZones1, A6, A7); // bat, solar
}


void doDelay()
{
  unsigned long diff = millis() - millTime;
  millTime = diff + millTime;  
  
  if( diff < DELAY_TIME ) {
    delay(DELAY_TIME); 
  }
  
  Serial.println("");
  //print("loop: ");
  //print(millTime);
}

void art()
{
  Serial.println("");
  Serial.println(" _____                     _                          ");
  Serial.println("|  __ \\                   | |                         ");
  Serial.println("| |  \\/_ __ ___  ___ _ __ | |__   ___  _   _ ___  ___ ");
  Serial.println("| | __| \'__/ _ \/ _ \ \'_ \\| \'_ \ / _ \\| | | / __|/ _ \\");
  Serial.println("| |_\\ \\ | |  __/  __/ | | | | | | (_) | |_| \\__ \\  __/");
  Serial.println(" \____/_|  \\___|\\___|_| |_|_| |_|\\___/ \\__,_|___/\\___|");
  Serial.println("");
  delay(1);
}