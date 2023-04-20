void parsePacket( String packet , String *arr , int n , char a )
{
    unsigned int pos = 0;
    for (int i = 0 ; (pos = packet.indexOf(a)) != packet.length() && i < n-1 ; i++ )
    {
        arr[i] = packet.substring(0, pos);
        packet = packet.substring(pos + 1);
    }   
    arr[n-1] = packet;
}

bool ST(String p[])
{   
    if ( p[3] == "GPS" ){
        //set time using gps
        setTimeGps();
        return true;
    }
    else{
        String arr[3];
        int arrInt[3];
        parsePacket(p[3],arr,3,':');
        
        for ( int i = 0 ; i < 3; i++ ){
            arrInt[i] = arr[i].toInt(); 
            if ( arr[i] != "00" && arrInt[i] == 0 ){
                return false;
            }
        }
        
        int hr = arrInt[0];
        int min = arrInt[1];
        int sec = arrInt[2];
        //set RTCtime 
        setTime_td(hr,min,sec);
        return true;
    }
}

void packetCheck(String packet)
{
    int no_of_fields = 4;
    String p[no_of_fields];
    parsePacket(packet,p,no_of_fields,',');
    if (p[2] == "CX")
    {
        if (p[3] == "ON"){
            telemetry = true;
            CMD_ECHO="CX";
        }
        else if (p[3] == "OFF")
        {
            telemetry = false;
            CMD_ECHO="CX";
        }
    }
    else if (p[2] == "ST")
    {
        if ( ST(p) ){
            CMD_ECHO="ST";
        }
    }
    else if (p[2] == "SIM")
    {
        if ( currentState == IDLE ){
            if (p[3] == "ENABLE"){
                simulation_enabled = true ;
                CMD_ECHO="SIM";
            }
            else if (p[3] == "DISABLE"){
                simulation_enabled = false;
                currentMode = FLIGHT;
                CMD_ECHO="SIM";
            }
            else if (p[3] == "ACTIVATE" && simulation_enabled){
                currentMode = SIMULATION;
                CMD_ECHO="SIM";
            }
        }
    }
    else if (p[2] == "SIMP")
    {
        //Set pressure to recieved value 
        if ( currentMode == SIMULATION ){
            float tempPressure = p[3].toFloat();
            if ( tempPressure != 0 ){
                pressure = tempPressure;
                // impliment conversion from pressure to altitude 
                // 44330 * [1 - (P/p0)^(1/5.255) ]
                altitude =( 44330 * ( 1 - pow( (pressure / 1013.25 ), (1/5.225) )  )) - zero_alt_calib;
                CMD_ECHO="SIMP";
            }
        }
    }
    else if (p[2] == "CAL")
    {
        //Set zero alt calibration ( only if in idle mode )
        if ( currentState == IDLE && bnoValid ){
                zero_alt_calib = altitude; 
                EEwriteFloat(altitude, 4);
                CMD_ECHO = "CAL";
        }
    }
    else if (p[2] == "CAM")
    {
        if (p[3] == "ON"){
            
        }
        else if (p[3] == "OFF"){
            
        }
    }
    else if (p[2] == "START")
    {
        //only if in idle mode
        if( currentState == IDLE ){
            currentState = LAUNCH_WAIT;
        }
    }
    else if (p[2] == "IDLE")
    {
        currentState = IDLE;
    }
    else if (p[2] == "LAUNCH_WAIT")
    {
        currentState = LAUNCH_WAIT;
    }
    /*
     * no need because it is taken care by SIM command
    else if (p[2] == "MODE")
    {
        //only if in idle mode
        if (p[3] == "F")
        {
           currentMode = FLIGHT; 
        }
    }*/
    else if (p[2] == "CAL_TILT")
    {
        if ( currentState == IDLE ){
            tilt_calibration = true;
        }
    }
}