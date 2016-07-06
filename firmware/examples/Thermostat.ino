// This #include statement was automatically added by the Particle IDE.
#include "NCD1Relay/SI7020_A20.h"

// This #include statement was automatically added by the Particle IDE.
#include "NCD1Relay/NCD1Relay.h"

NCD1Relay controller;

SI7020_A20 sensor;

String temperature = "...";
String setTemperature = "75";
String setState = "off";
String currentState = "off";
double setTempInt = 75;
#define Addr 0x40

double fTemp = 0.0;
double cTemp = 0.0;

int setTemp(String temp);
int setStatus(String status);

void setup() {
    controller.setAddress(0,0,0);
    Particle.variable("data", temperature);
    Particle.variable("setTemp", setTemperature);
    Particle.variable("status", setState);
    Particle.variable("cStatus", currentState);
    Particle.function("setTemp", setTemp);
    Particle.function("setStatus", setStatus);
    Wire.begin();
}

void loop() {
    fTemp = sensor.temperatureF();
    temperature = String(fTemp);
    
    if(setState.equalsIgnoreCase("on")){
        if(fTemp - setTempInt >= 2){
            //Turn on AC
            controller.turnOnRelay();
            currentState = "on";
        }
        if(setTempInt > fTemp){
            //Turn off AC
            controller.turnOffRelay();
            currentState = "off";
        }
    }else{
        if(controller.readRelayStatus() == 1){
            controller.turnOffRelay();
        }
    }
        
    int relayState = controller.readRelayStatus();
    if(relayState == 1){
        currentState = "on";
    }else{
        currentState = "off";
    }
}
int setTemp(String temp){
    setTemperature = temp;
    setTempInt = temp.toInt();
    setState = "on";
}

int setStatus(String status){
    if(status.equalsIgnoreCase("on")){
        //turn on AC
        setState = "on";
        return 1;
    }
    if(status.equalsIgnoreCase("off")){
        //turn off AC
        setState = "off";
        currentState = "off";
        controller.turnOffRelay();
        return 1;
    }
    return 0;
}