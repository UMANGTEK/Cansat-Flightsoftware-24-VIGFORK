#include <Servo.h>
// parachute and probe : servo 
// landing legs and flag : motor

// onboard transistor : pin 5

#define probePin 28
#define probeDeployAngle 180
#define probeLockAngle 0

#define parachutePin 29
#define parachuteDeployAngle 90
#define parachuteLockAngle 0

#define flagPin 7
#define landinglegsPin 5 

Servo probeServo;
Servo paraServo;

void actuatorSetup()
{
    probeServo.attach(probePin);
    paraServo.attach(parachutePin);
    pinMode(flagPin,OUTPUT);
    pinMode(landinglegsPin,OUTPUT);
}
void deployProbe()
{
    // Turn servo to deploy probe
    probeServo.write(probeDeployAngle);
    return ;
}
void lockProbe()
{
    // Turn servo to lock probe
    probeServo.write(probeLockAngle);
    return ;
}


void deployParachute()
{
    // Turn servo to deploy prachute
    PC_deployed = true ;
    paraServo.write(parachuteDeployAngle);
    return ;
}

void lockPrachute()
{
    // Turn servo to lock prachute
   
    paraServo.write(parachuteLockAngle);
    return ;
}

void raiseFlag()
{
    // Turn servo to raise flag
    MAST_raised = true ;
    digitalWrite(flagPin,HIGH);
    return;
}
void stopRaisingFlag()
{
    // Turn servo to raise flag
    digitalWrite(flagPin,LOW);
    return;
}


void deployHeatSheild()
{
    // Turn motor ( Turn pin High )
    HS_deployed = true ;
    digitalWrite(landinglegsPin,HIGH);
    return ;
}

void stopDeployingHeatSheild()
{
    // Stop turning motor ( Turn pin Low )
    digitalWrite(landinglegsPin,LOW);
    return ;
}

