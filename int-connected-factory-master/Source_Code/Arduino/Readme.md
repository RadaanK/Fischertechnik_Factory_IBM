# Readme

This folder contains the source code for the several Aduinos.
The code can be uploaded to the Arduino using the Ardunio Ide (https://www.arduino.cc/en/main/software)

Two types of Arduinos were used.
First the Arduino Uno which is connected to the modules equipped with a NFC reader. 
For this ones the code from the folder i2c_and_NFC must be used

The second variant is the Arduino Nano. 
This one is used when no NFC reader is available and only the communication between Fischertechnik controller/ pressure sensor 
and the Raspberry Pi has to be done.
When using the Arduino Nano to connect to a Fischertechnik controller use the souce code i2c.
When connecting to a pressure sensor use the code Pressure_Sensor
Please pay attantion to use the correct bootloader when uploading the source code with the Arduino Ide

# How to connect the Arduinos

# Pin Assignment Fischertechnik Controller
This picture shows the pin assignment of the Fischertechnik Controllers. 
We use the i2c interface (SCL, SDA and GND were needed)
![controller_pin_assignment](https://media.github.ibm.com/user/234399/files/f174b480-e507-11e9-94a5-e54069692da0)

# Arduino Nano
When connected to the Arduino Nano, the Gnd is connected to the Gnd, the SCL to the pin A5 and the SDA to the pin A4 on the Arduino Nano.
The following picture shows how to connect the Arduino Nano to the Controller
![cabeling_arduino_nano](https://media.github.ibm.com/user/234399/files/fd13ab80-e506-11e9-92e9-dee392ad5e4e)

# Arduino Uno
The connection to a Fischertechnik controller is done the same way as when using an Arduino Nano. 
The following 2 pictures show how to connect the Arduino to a NFC reader
![cabeling_arduino_uno](https://media.github.ibm.com/user/234399/files/2a605980-e507-11e9-891e-2fc1a5753395)
![cabeling_nfc_reader](https://media.github.ibm.com/user/234399/files/3e0bc000-e507-11e9-8ab2-70d14e8b00a9)

# Pressure Sensor
Cabeling Pressure Sensor:
GND <--> GND

Vin <--> Vin

SDA <--> A4

SCL <--> A5


