/*
 * The pressure sensor use I2C to communicate, 2 pins (SCL+SDA) are required
*/
 
#include <Wire.h>
#include "Adafruit_MPRLS.h"
#include <ArduinoJson.h>

// variables to hold data
StaticJsonDocument<100> jsonDoc;                              //Is needed to store the sensor values transmitted by the arduino
JsonObject payload = jsonDoc.to<JsonObject>();
//JsonObject state = payload.createNestedObject("State");
StaticJsonDocument<100> jsonReceiveDoc;                       //Is needed to store the commands send by the IOT platform/Node red
static char msg[100];

// You dont *need* a reset and EOC pin for most uses, so we set to -1 and don't connect
#define RESET_PIN  -1  // set to any GPIO pin # to hard-reset on begin()
#define EOC_PIN    -1  // set to any GPIO pin to read end-of-conversion by pin
Adafruit_MPRLS mpr = Adafruit_MPRLS(RESET_PIN, EOC_PIN);

//Variables to measure the time 
uint32_t ts1=0;
uint32_t ts2=0;

void setup() {
  Serial.begin(19200);
  //Serial.println("MPRLS Simple Test");
  if (! mpr.begin()) {
    Serial.println("Failed to communicate with MPRLS sensor, check wiring?");
    while (1) {
      delay(10);
    }
  }
  //Serial.println("Found MPRLS sensor");
}


void loop() {
  //Start timer to measure the excecution time
  ts1 = millis();
  
  //Read sensor information
  float pressure_hPa = mpr.readPressure();
  //Serial.print("Pressure (hPa): "); Serial.println(pressure_hPa);

  // Send data to Raspberry pi
  payload["controller"] = 0;                  //
  payload["subadress"] = 9;                   //The pressure sesnor has the ID/subadress 9
  payload["timestamp"] = millis();            //Store the uptime
  payload["data"] = pressure_hPa;             //Store the sensor value
  serializeJson(jsonDoc, msg);          
  Serial.println(msg);                        //Send Data over serial(USB) to Raspberry Pi

  //Stop timer to measure the excectuion time
  ts2 = millis();
  
  delay(1000-(ts2-ts1));                      //Substract the excecution time, so that everey second a value is stored
}
