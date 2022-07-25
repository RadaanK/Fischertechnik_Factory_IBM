
#include <math.h>
#include <WiFi.h>         // Include the Wi-Fi library
#include <ArduinoJson.h>
#include <PubSubClient.h> // Include Library for MQTT
#include "DHTesp.h"       // Include Library for the temperature and humidity sensor
#include <SPI.h>
#include <MFRC522.h>


#define LED 2           //Set the onboard Led to indicate an active Wi-Fi connection     
#define DHTpin 4        //D2 of ESP32 DevKit for temperature and humidity sensor
#define RST_PIN   22    // Define pins for the RFID/NFC reader
#define SS_PIN    21    // Define pins for the RFID/NFC reader
int analogInPin = 34;   //Define the pin for the water-level-sensor

// variables to hold data
StaticJsonDocument<400> jsonDoc;                              //Is needed to store the sensor values transmitted by the arduino
JsonObject payload = jsonDoc.to<JsonObject>();
JsonObject state = payload.createNestedObject("State");
StaticJsonDocument<300> jsonReceiveDoc;                       //Is needed to store the commands send by the IOT platform/Node red
static char msg[400];

// variables
int h = 0;                            // value for humidity
int t = 0;                            // value for temperature
int32_t ReportingInterval = 10;       // Reporting Interval seconds
int Water_Level_Sensor = 0;
int Water_Level_Sensor_percent = 0;
int i = 0;                            //Counter variable

//Definitions for the RFID/NFC reader
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance
#define NEW_UID {0xDE, 0xAD, 0xBE, 0xEF};
String NFC_ID= "0"; 
MFRC522::MIFARE_Key key;

//definition for the temperature and humidity sensor
DHTesp dht;


void setup() {
  Serial.begin(115200);                   // Start the Serial communication to send messages to the computer
  Serial.setTimeout(2000);
  while (!Serial) { }                     // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();                            // Init SPI bus
  Serial.print("ESP32 started"); 
  delay(10);
  Serial.println('\n');

  pinMode(LED,OUTPUT);                    //Set pin Mode for LED

  // Setting up the temperature and humidity sensor
  dht.setup(DHTpin, DHTesp::DHT11); //for DHT11 Connect DHT sensor to GPIO 2

  //Setupt the RFID/NFC reader
  mfrc522.PCD_Init();                     // Init MFRC522 card
  for (byte i = 0; i < 6; i++) {          // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
    key.keyByte[i] = 0xFF;
  }
}
 
void loop() {

  //read the temperature and humidity
  h = dht.getHumidity();
  t = dht.getTemperature(); 

  //read the water level an concert it in a percentage value
  Water_Level_Sensor = analogRead(analogInPin); 
  Water_Level_Sensor_percent = (Water_Level_Sensor*100)/1024;

  // Send data to Raspberry pi
  state["Temperature"] = t;                           //Write Temperature to Json
  state["Humidity"] = h;                              //Write Humidity to Json
  state["Water_Level"] = Water_Level_Sensor_percent;  //Write water level to Json
  state["NFC_ID"] = "0";                              // Overwrite the value of the NFC-ID
  serializeJson(jsonDoc, msg);
  Serial.println(msg);

  //Serial.print("Interval:  ");                        //Inicate at which interval the temperature and humidity are recorded
  //Serial.println(ReportingInterval);

  // Pause - but keep polling for new NFC Tags
  i=0;                                                                            //Reset the counter
  while (i<ReportingInterval*10){
//    mqtt.loop();                                                                //Is onlyneedet if infomation is send by the IoT Platform
    
    // Look for new RFID/NFC-tags, and select one if present
    if (  mfrc522.PICC_IsNewCardPresent() and mfrc522.PICC_ReadCardSerial() ) {
      NFC_ID="0";                                                                 //First character of the ID is a 0
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        NFC_ID += String(mfrc522.uid.uidByte[i], HEX);                            //Convert the array to a string
      } 
      state["NFC_ID"] = NFC_ID;                                                   //Write string to Json so it can be send to the cloud
      serializeJson(jsonDoc, msg);                      
      Serial.println(msg);
      delay(2000);                                                                // wait 2s before searching for another NFC/RFID tag
      i=i+20;                                                                     //Take the delay of 2000ms into account
    }
    delay(100);
    i++;                                                                          //increase the counter variable by 1
  }
}
