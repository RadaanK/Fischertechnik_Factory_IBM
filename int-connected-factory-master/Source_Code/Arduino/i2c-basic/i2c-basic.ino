// Coypright IBM: rootkrause (Steffen Krause)

#include <Wire.h>
#include <ArduinoJson.h>

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
boolean ready2send = false;  // JSOn has been parsed and is ready for tranfer in wire
char command4arduino = '0';

int protocolState = 0;
boolean ready2transmit = false;

int cx = 0;
int cA = -1;

void setup() {
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent); // register event
  
  Serial.begin(19200);           // start serial for output
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {

  if(command4arduino==0){
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
    // print the string when a newline arrives:
  if (stringComplete) {
    // Serial.println(inputString);
    // JSON vars
    // Memory pool for JSON object tree.
    //
    // Inside the brackets, 200 is the size of the pool in bytes,
    // If the JSON object is more complex, you need to increase that value.
    StaticJsonBuffer<200> jsonBuffer;
    
    // StaticJsonBuffer allocates memory on the stack, it can be
    // replaced by DynamicJsonBuffer which allocates in the heap.
    // It's simpler but less efficient.
    //
    // DynamicJsonBuffer  jsonBuffer;

    // JSON input string.
    //
    // It's better to use a char[] as shown here.
    // If you use a const char* or a String, ArduinoJson will
    // have to make a copy of the input in the JsonBuffer.
    //char json[] = inputString.
        //"{\"controller\":8,\"subadress\":0,\"timestamp\":1351824120,\"command\":15}";      

    char json[inputString.length()];
    inputString.toCharArray(json, inputString.length());
  
    // Root of the object tree.
    //
    // It's a reference to the JsonObject, the actual bytes are inside the
    // JsonBuffer with all the other nodes of the object tree.
    // Memory is freed when jsonBuffer goes out of scope.
    JsonObject& root = jsonBuffer.parseObject(json);
  
    // Test if parsing succeeds.
    if (!root.success()) {
      Serial.println("ERROR: parseObject() failed");
    } else {
      if (root.containsKey("command")) {          //check if a command has been send
        // Fetch values.
        int controller = root["controller"];
        int subadress = root["subadress"];
        long timestamp = root["timestamp"];
        int command = root["command"];
  
        command4arduino = command;
/*        if(command==15){
          digitalWrite(LED_BUILTIN, HIGH);
        } else {
          digitalWrite(LED_BUILTIN, LOW);
        }
*/
        // parse and create wire char
        //command4arduino = 0xE;
        ready2send = true;      
      } else {
        // ignore the data in the wire beaucse its no command for us
        Serial.println("ERROR: parseObject() failed: else...");
      }
    }
          
    // clear the original sring
    inputString = "";
    stringComplete = false;
  }
}

//____________________________________________________________________________________________________________________________

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == ';') {
      stringComplete = true;
    }
  }
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  while (Wire.available()) { // loop through all but the last
    ready2transmit = false;
    // read until found first marker char
    if (protocolState < 1) {
      int cM0 = Wire.read();
      int cM1 = Wire.read();
      int cM = (cM0 << 8) + cM1;
      if (cM == 0xFFFF) {
        protocolState = 1;
      };
    } else if (protocolState == 1) {
      // read the subadress
      protocolState = 2;
      // read the device subadress
      int cA0 = Wire.read();
      int cA1 = Wire.read();
      cA = (cA0 << 8) + cA1;    
    } else if (protocolState == 2) {
      // read the actual data and reset
      protocolState = 0;
      // and now read the next 2 bytes - we do expect 16 Bit MSBF
      int c0 = Wire.read();
      int c1 = Wire.read();
      cx = (c0 << 8) + c1;      
      ready2transmit = true;
    }
  }

  if (ready2transmit) {                       //Check if there is an device event which can be send
    if (cx!=0){                               //Check is the data is not equal to 0
      // Step 1: Reserve memory space
      StaticJsonBuffer<200> jsonBuffer;
    
      // Step 2: Build object tree in memory
      JsonObject& root = jsonBuffer.createObject();
      root["controller"] = 0;
      root["subadress"] = cA;
      root["timestamp"] = millis();
      root["data"] = cx;
    
      // Step 3: Generate the JSON string
      root.printTo(Serial);
      Serial.write('\n');
    }                               
      ready2transmit = false;                 //reset ready2transmit 
      protocolState = 0;
  };
}

void requestEvent() {
  //Serial.println("Data requested"); 
  // send data only when you receive data:
  if (ready2send) { 
    Wire.write(command4arduino);
    // and clear    
    command4arduino = 0; 
    ready2send = false;  
  } else {
    //Serial.println("No Data available"); 
    // FIXME: Error handling
  }
}
