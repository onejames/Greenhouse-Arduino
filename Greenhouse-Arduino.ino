#include <SoftwareSerial.h>
#include <Arduino.h>

#include "Vector.h"
#include "WaterZone.h"
#include "ClimateZone.h"
#include "Greenhouse.h"

const    int  DELAY_TIME  = 20000;
const    int  JSON_SEND_INTERVAL = 4;
unsigned long millTime    = 0;
         int  loopCounter = 0;

Greenhouse greenhouse;

void setup()
{
    Serial.begin(115200);

    art();

    Serial.println("Booting...");
    Serial.println("");
}

void loop()
{
    if(!greenhouse.inited) {
        // table config = getGreenhouseConfig();
        // initGreenhouse(config);
        initGreenhouse();
    }

    greenhouse.check();

    loopCounter++;

    if(loopCounter == JSON_SEND_INTERVAL) {
//        sendStatus();
        loopCounter = 0;
    }

    doDelay();
}

void initGreenhouse()
{
    /* WATER ZONES */
    Vector<WaterZone> cz1Zones;

    // cz1Zones.push_back(WaterZone(1, "Left Primary", A0, 4, 768));
    cz1Zones.push_back(WaterZone(1, 1, A0, 4, 768));

    cz1Zones.push_back(WaterZone(2, 2, A0, 7, 768));
    // cz1Zones.push_back(WaterZone(2, "Right Primary", A1, 7, 768));

    // Vector<WaterZone> cz2Zones;
    // cz2Zones.push_back(WaterZone(3, 3, A2, 4, 768));
    // cz2Zones.push_back(WaterZone(3, "Coldframe", A2, 4, 768));

    /* CLIMATE ZONES*/
    Vector<ClimateZone> climateZones;

    // climateZones.push_back(ClimateZone(1, "Main", 11, cz1Zones));
    climateZones.push_back(ClimateZone(1, 1, 11, cz1Zones));

    // climateZones.push_back(ClimateZone(2, Coldframe, 10, cz1Zones));
    // climateZones.push_back(ClimateZone(2, 3, 10, cz1Zones));
    // climateZones.push_back(ClimateZone(3, Outside, 9, cz1Zones));

    greenhouse.init(1, 1, climateZones, A6, A7); // bat, solar
    // greenhouse.init(1, "Laster Greenhouse", climateZones, A6, A7); // bat, solar

    Serial.println("");
    Serial.println("...Ready");
    Serial.println("");
}

void sendStatus()
{
    String json = greenhouse.getJson();

    Serial.println("");
    Serial.println("");

    Serial.println("{EXPECT_JSON}");

    Serial.println(json);

    Serial.println("");
}

void doDelay()
{
  unsigned long diff = millis() - millTime;
  millTime = diff + millTime;

  if( diff < DELAY_TIME ) {
    delay(diff);
  }

  Serial.println("");
}

void art()
{
    Serial.println("");
    Serial.println(" ____                    _____");
    Serial.println("/ __ \\                  / ____|");
    Serial.println("| |  | |_ __   ___ _ __ | |  __ _ __ ___  ___ _ __  ");
    Serial.println("| |  | | '_ \\ / _ \\ '_ \\| | |_ | '__/ _ \\/ _ \\ '_ \\ ");
    Serial.println("| |__| | |_) |  __/ | | | |__| | | |  __/  __/ | | |");
    Serial.println("\\____/ | \\__/ \\___|_| |_|\\_____|_|  \\___|\___|_| |_|");
    Serial.println("     | |");
    Serial.println("     |_|");
    Serial.println("");
}
