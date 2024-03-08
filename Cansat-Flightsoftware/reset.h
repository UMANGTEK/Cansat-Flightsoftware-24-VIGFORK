#define resetPin 33

void resetSetup(){
    pinMode(resetPin,INPUT_PULLUP);
}

bool resetShort(){
    return ( !digitalRead(resetPin));
}
