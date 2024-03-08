#define packetTimePeriod 1000
#include <TimeLib.h>


enum states {
  IDLE ,
  LAUNCH_WAIT ,
  ASCENT ,
  DECENT ,
  PAYLOAD_SEPARATED ,
  PARACHUTE_DEPLOYED ,
  LANDED
};
enum modes {
  FLIGHT,
  SIMULATION
};

int currentState = IDLE ;
int currentMode = FLIGHT ;
int packet_count = 0;
bool HS_deployed = false;
bool PC_deployed = false;
bool MAST_raised = false;


float zero_alt_calib = 0;
bool telemetry = true;
bool tilt_calibration = false ;
bool simulation_enabled = false;

String CMD_ECHO = "";
float voltage = 0;

float temprature = 0 , altitude = 0 , pressure = 0 ;
bool bmpValid = false ;

float xAngle = 0 , yAngle = 0 , zAngle = 0 , acceleration = 0 ;
bool bnoValid = false ;

float noSats = 0 , lat = 0 , lng = 0 , gpsAltitude = 0 ;
bool satsValid = false, locValid = false, altValid = false;

int gpsSecond = 0 , gpsMinute = 0 , gpsHour = 0  , gpsDay = 0 , gpsMonth = 0, gpsYear = 0 ;
bool timeValid = false , dateValid = false ;

float adjusted_alt= 0 ;
float adjusted_pressure= 0 ;
bool pressureValid = false ;
bool SD_works = false;

#include "actuators.h"
#include "reset.h"
#include "sdcard.h"
#include "led_buzzer.h"
#include "./sensors/battery.h"
#include "./sensors/gpssensor.h"
#include "RTCtime.h"
#include "checkheight.h"
#include "eeprom_rw.h"
#include "telemetry.h"
#include "./sensors/bmpsensor.h"
#include "xbeeComms.h"
#include "./sensors/bnosensor.h"
#include "smartDelay.h"
#include "camera.h"
#include "cmdProcessing.h"

void setup() {
  // put your setup code here, to run once:
  // Go to EEPROM and get values of state, mode, packet_count and zero_alt_calibration
  currentState = EEreadInt(1);
  currentMode = EEreadInt(2);
  packet_count = EEreadInt(3);
  zero_alt_calib = EEreadFloat(4);
  HS_deployed = EEreadInt(5);
  PC_deployed = EEreadInt(6);
  MAST_raised = EEreadInt(7);
  led_buzzer_Setup();
  setSyncProvider(getTeensy3Time);
  resetSetup();
  SDsetup();
  bnoSetup();
  bmpSetup();
  gpsSetup();
  xbeeSetup();
  cameraSetup();
  actuatorSetup();
  RTCsetup();

  buzzerON();
  greenON();
  redON();
  lockProbe();
  lockPrachute();
  stopDeployingHeatSheild();
  stopRaisingFlag();
}

void loop() {
  // put your main code here, to run repeatedly:
  
  if ( telemetry ){
    blueON();
  }
  else{
    blueOFF();
  }

  if ( bmpValid && bnoValid && timeValid && dateValid && satsValid && locValid && altValid ){
    greenON();
  }
  else{
    greenOFF();
  }

  switch (currentState) {
    case IDLE:
      if (tilt_calibration) {
        int tilt_cal_status = bnoCalibration();
        sendDataTelemetry(String("Tilt Calibration: ") + String(tilt_cal_status) + String("|"));
        if ( !tilt_cal_status ) {
          tilt_calibration = false;
        }
      }
      break;
    case LAUNCH_WAIT:
      // check if cansat has started accending if yes change state
      if ( movingUp() ) {
        currentState = ASCENT;
      }
      else if ( movingDown() ) {
        currentState = DECENT;
      }
      break;
    case ASCENT:
      // check if cansat has stopped accent and started going downwards ( decreasing altitude)
      if ( movingDown() ) {
        currentState = DECENT;
      }
      break;
    case DECENT:
      // Check if altitude is less than 500m if yes change state to payload_separated
      if ( checkAlt(500) ) {
        currentState = PAYLOAD_SEPARATED;
        deployProbe();
      }
      break;
    case PAYLOAD_SEPARATED:
      //Check if altitude is 200m then change state and relase parachute
      deployProbe();
      deployHeatSheild();//probably release this after little height
      if ( checkAlt(200) ) {
        currentState = PARACHUTE_DEPLOYED;
      }
      break;
    case PARACHUTE_DEPLOYED:
      deployProbe();
      deployHeatSheild();//probably release this after little height
      
      //deploy parachute function
      deployParachute();
      //If there is no movement then move to landed state
      if ( notMoving() ) {
        currentState = LANDED;
      }
      break;
    case LANDED:
      // Open flag and just wait do nothing
      raiseFlag();
      // Turn off heatsheild motor
      stopDeployingHeatSheild();
      break;
    default:
      break;


  }

  WriteALL();
  smartDelay(1000 -183 );
  repetitive_Task();
}


void repetitive_Task( ) {
  packet_count ++;

  // Read Sensor Data
  //GPS data
  gpsGetTime( &gpsSecond , &gpsMinute, &gpsHour , &gpsDay, &gpsMonth , &gpsYear , &dateValid , &timeValid);
  gpsReading(&noSats , &lat , &lng , &gpsAltitude , &satsValid, &locValid  , &altValid );
  //BNO data
  bnoGetValues();
  readVoltage();
  //BMP data
  bmpGetValues();

  // Apply filter


  //Process recieved commmands
  //get packet
  if ( packetAvailable() ) {
    String packetRecieved = getOnePacket();
    packetCheck(packetRecieved);
  }

  updateAlt(adjusted_alt);
  //Make telemetry packet
  String telemetry_string = makeTelemetryPacket();

  //Transmit data to GCS over Xbee
  if ( telemetry ){
    sendDataTelemetry(telemetry_string);
  }

  //Save Data to sd card  
  saveTelemetryInSdCard(telemetry_string);

  // Save state to EEPROM
  WriteALL();

}
