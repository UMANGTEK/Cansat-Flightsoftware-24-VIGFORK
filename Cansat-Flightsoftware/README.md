# Cansat-Flightsoftware
Cansat Flightsoftware for Teensy 4.1

### sensors 
This folder contains all the code required to setup, fetch data and restart the sensors using their respective libraries.

### smartDelay.h
Does nessary functions like sending data , collecting data froms sensors every 1 sec as required by the competition

### cmdProcessing.h
Takes the incoming commands from the Ground Station as strings parses them and does the nessasary actions.

### reset.h
Resets the cansat to default state. ( inital state )

### checkheight.h
Usings a filter to on the altitude data and then checking if the altitude is below or above certain marks

### eeprom_rw.h
Contains helper functions to read and write to internal Teensy EEPROM easily 

### led_buzzer.h
Contains helper functions to setup and control led and buzzer

### telemetry.h
Creates telemetry packets and returns them as a string

### returnstring.h
Takes in different types of data ( float , int , text ) and returns them as a string with nessasary padding 

### xbeeComms.h
Setup, receiving and transmition helper functions using xbee libaray 

### sdcard.h
Setup and writes data to sd card

### actuators.h
Functions that can be called to control all the actuators ( servos and motors )

### RTCtime.h
sets and gets RTC time

### Cansat-Flightsoftware.ino
Main Code file that is uploaded to Teesny. Containers variable definitions and other nessasary function calls 
