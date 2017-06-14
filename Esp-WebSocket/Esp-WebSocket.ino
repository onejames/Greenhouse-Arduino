/*
WebSocketServer.ino
Created on: 22.05.2015
From EPS8266 examples
*/

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WebSocketsServer.h>   // https://github.com/Links2004/arduinoWebSockets/tree/async
#include <Hash.h>

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>        // https://github.com/tzapu/WiFiManager

WebSocketsServer webSocket = WebSocketsServer(81);

const String  SSID = "TheTaoOfJames";
const String  PWD  = "james1234";

      String  inputString    = "";    // a string to hold incoming data
      boolean stringComplete = false; // if the string is complete
      boolean expectJson     = false; // if we have recieved the json marker

void setup() {
    Serial.begin(115200);

    //Serial.setDebugOutput(true);

    Serial.println();
    Serial.println();
    Serial.println();

    //Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;

    //tries to connect to last known settings
    //if it does not connect it starts an access point with the name
    //and goes into a blocking loop awaiting configuration
    if (!wifiManager.autoConnect()) {
//    if (!wifiManager.autoConnect(SSID, PWD)) {
        Serial.println("Failed to connect to WiFi. Resetting and trying again.");
        delay(3000);
        ESP.reset();
        delay(5000);
    }

    webSocket.begin();
    webSocket.onEvent(webSocketEvent);

    Serial.println("STARTING TO MIRROR SERIAL");
    Serial.println(WiFi.localIP());

    inputString.reserve(256);
}

void loop() {

    serialEvent();

    if (stringComplete) {
        String line = inputString;
        inputString = "";
        stringComplete = false;

        if(expectJson) {
//            postData(line);
            expectJson = false;
        } else if( line == "{EXPECT_JSON}" ) {
            expectJson = true;
        } else {
            webSocket.broadcastTXT(line);
            Serial.println(line);
        }
    }

    webSocket.loop();
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {

    switch (type) {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected!\n", num);
            break;

        case WStype_CONNECTED:
            {
                IPAddress ip = webSocket.remoteIP(num);
                Serial.printf("[%u] Connected from %s url: %s\n", num, ip.toString().c_str(), payload);

                // send message to client
                webSocket.sendTXT(num, "Connected to Serial on " + WiFi.localIP().toString() + "\n");
            }
            break;

        case WStype_TEXT:
            Serial.printf("[%u] get Text: %s\n", num, payload);

            break;
            case WStype_BIN:
            Serial.printf("[%u] get binary lenght: %u\n", num, lenght);
            hexdump(payload, lenght);

            // send message to client
            // webSocket.sendBIN(num, payload, lenght);
        break;
    }

}

//void postData(String data) {
//    HTTPClient http;
//    http.begin("172.16.0.40", 80, "/rest/vars/set/1/12/999");
//    http.addHeader("Content-Type", "text/json");
//    http.addHeader("Authorization", "Basic dXNlcm5hbWU6cGFzc3dvcmQ=");
//    auto httpCode = http.POST(payload);
//}

void serialEvent() {
    while (Serial.available()) {
        char inChar = (char)Serial.read();

        if (inChar == '\n') {
            stringComplete = true;
            return;
        } else {
            inputString += inChar;
        }
    }
}
