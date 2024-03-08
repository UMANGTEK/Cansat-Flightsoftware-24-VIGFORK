
// This custom version of delay() ensures that the gps object
// is being "fed".
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
      if ( !bnoValid ){
        bno.begin();
        bnoValid = true;
     }
      if(!bmpValid){
        bmpSetup();
      }
      recieveDataTelemetry(); 
      processGps();
      if ( currentMode == FLIGHT ){
        bmpGetValues();
        updateAlt(adjusted_alt);
      }
      if (resetShort()){
        lockProbe();
        lockPrachute();
        stopDeployingHeatSheild();
        stopRaisingFlag();

        currentState = IDLE ;
        currentMode = FLIGHT ;
        packet_count = 0;
        HS_deployed = false;
        PC_deployed = false;
        MAST_raised = false;
        zero_alt_calib = 0;
        
        WriteALL();
        
        telemetry = true;
        tilt_calibration = false ;
        simulation_enabled = false;

      }
      /*
      if( !SD_works && currentState == IDLE ){
        SDsetup();
        SD_works = true ;
      }
      */
  } while (millis() - start < ms );

}
