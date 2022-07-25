# Readme

This folder contains the code for the Node-RED instance on a Raspberry Pi 
(the one connected to a Pi camera and responsible for the visual inspection).

The flow listens to a command send by the IoT Platform, than takes a photo of a workpiece (if present) an sends it to the 
Waston Visual Recognition. 
This one classifies the image and sends back a result to the Node-RED flow. 
The result is then forwarded to the IoT Platform  

The following picture shows the hardware setup with the raspberry pi and a camera connected to it:
![picture_of_setup](https://media.github.ibm.com/user/234399/files/3d2c5b80-e512-11e9-9fbf-11fb5f14a849)
