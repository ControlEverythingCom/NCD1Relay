#include "NCD1Relay.h"
#include "spark_wiring_usbserial.h"

//Comment line below out to turn off Serial logging
//#define LOGGING

int address = 0x20;
int address2 = 0x21;
int retrys = 0;
byte outputRegister = 0x0A;

//Constructor
NCD1Relay::NCD1Relay(){
}

//Retry added
void NCD1Relay::setAddress(int a0, int a1, int a2){
    address = 0x20;
    if(a0 == 1){
        address = address | 1;
    }
    if(a1 == 1){
        address = address | 2;
    }
    if(a2 == 1){
        address = address | 4;
    }
    //Start I2C port
    Wire.begin();
    //Open connection to specified address
    retryAddress1:
    Wire.beginTransmission(address);
    //Set all channel directions
    Wire.write(0x00);
    Wire.write(0xfe);
    //Determine if device is present at that address
    byte status = Wire.endTransmission();
    
    Wire.beginTransmission(address);
    Wire.write(0x06);
    Wire.write(0xfe);
    status = Wire.endTransmission();
    if(status != 0){
        if(retrys < 3){
    #ifdef LOGGING
            Serial.println("Retry set address command");
    #endif
            retrys++;
            goto retryAddress1;
        }else{
    #ifdef LOGGING
            Serial.println("Set Address failed");
    #endif
            initialized = false;
            retrys = 0;
        }
        
    }else{
        // Serial.println("Command Successful");
        initialized = true;
        retrys = 0;
        readStatus();
    }
}

void NCD1Relay::turnOnRelay(){
    byte registerAddress = 0x0A;

    turnOnRelayRetry:
    Wire.beginTransmission(address);
    Wire.write(registerAddress);
    Wire.write(1);
    byte status = Wire.endTransmission();
    if(status != 0){
        if(retrys < 3){
    #ifdef LOGGING
            Serial.println("Retry Turn On Relay command");
    #endif
            retrys++;
            goto turnOnRelayRetry;
        }else{
    #ifdef LOGGING
            Serial.println("Turn On Relay failed");
    #endif
            initialized = false;
            retrys = 0;
        }
    }else{
        initialized = true;
        retrys = 0;
        readStatus();
    }
}

void NCD1Relay::turnOffRelay(){
    byte registerAddress = 0x0A;
turnOffRelaysRetry:
    Wire.beginTransmission(address);
    Wire.write(registerAddress);
    Wire.write(0);
    byte status = Wire.endTransmission();
    if(status != 0){
        if(retrys < 3){
#ifdef LOGGING
            Serial.println("Retry Turn Off Relay command");
#endif
            retrys++;
            goto turnOffRelaysRetry;
        }else{
#ifdef LOGGING
            Serial.println("Turn Off Relay command failed");
#endif
            initialized = false;
            retrys = 0;
        }
    }else{
        initialized = true;
        retrys = 0;
        readStatus();
    }
}

void NCD1Relay::toggleRelay(){

	byte bankValue =  bankOneStatus;
	bankValue = bankValue ^ 1;

	toggleRelayRetry:
	Wire.beginTransmission(address);
	Wire.write(0x0A);
	Wire.write(bankValue);
	byte status = Wire.endTransmission();
	if(status != 0){
		if(retrys < 3){
#ifdef LOGGING
			Serial.println("Retry toggle relay command");
#endif
			retrys++;
            goto toggleRelayRetry;
        }else{
#ifdef LOGGING
            Serial.println("Toggle relay command failed");
#endif
            initialized = false;
            retrys = 0;
        }
    }else{
        initialized = true;
        retrys = 0;
        readStatus();
    }
}

int NCD1Relay::readRelayStatus(){

    getRelayStatusRetry:
    Wire.beginTransmission(address);
    Wire.write(0x0A);
    byte status = Wire.endTransmission();
    if(status != 0){
        if(retrys < 3){
    #ifdef LOGGING
            Serial.println("Retry read relay status command");
    #endif
            retrys++;
            goto getRelayStatusRetry;
        }else{
    #ifdef LOGGING
            Serial.println("Read relay status command failed");
    #endif
            initialized = false;
            retrys = 0;
            return 256;
        }
    }else{
        retrys = 0;
        initialized = true;
    }
    Wire.requestFrom(address, 1);
    byte bankStatus = Wire.read();
    if(bankStatus & 1){
        return 1;
    }else{
        return 0;
    }
}

void NCD1Relay::readStatus(){
readBankOneRetry:
    //Open Connection to controller
    Wire.beginTransmission(address);
    //Get current status of relays 1-8
    Wire.write(0x0A);
    byte status = Wire.endTransmission();
    if(status != 0){
        if(retrys < 3){
    #ifdef LOGGING
            Serial.println("Retry read status banks 1-2 command");
    #endif
            retrys++;
            goto readBankOneRetry;
        }else{
    #ifdef LOGGING
            Serial.println("Read ststus banks 1-2 command failed");
    #endif
            initialized = false;
            retrys = 0;
        }
    }else{
        retrys = 0;
        initialized = true;
        Wire.requestFrom(address, 1);
        bankOneStatus = Wire.read();
    }
    
}

int NCD1Relay::readInputStatus(int input){
    if(input > 7 || input < 0){
        return 256;
    }
    int value = 0;
    switch(input){
        case 1:
            value = 2;
            break;
        case 2:
            value = 4;
            break;
        case 3:
            value = 8;
            break;
        case 4:
            value = 16;
            break;
        case 5:
        	value = 32;
        	break;
        case 6:
        	value = 64;
        	break;
        case 7:
        	value = 128;
        	break;

    }
    getGPIOStatusRetry:
    Wire.beginTransmission(address);
    Wire.write(0x09);
    byte status = Wire.endTransmission();
    if(status != 0){
        if(retrys < 3){
    #ifdef LOGGING
            Serial.println("Retry read input status command");
    #endif
            retrys++;
            goto getGPIOStatusRetry;
        }else{
    #ifdef LOGGING
            Serial.println("Read input status command failed");
    #endif
            initialized = false;
            retrys = 0;
            return 256;
        }
    }else{
        retrys = 0;
        initialized = true;
    }
    Wire.requestFrom(address, 1);
    byte bankStatus = Wire.read();
    byte inverted = ~bankStatus;
    if((inverted & value) == value){
        return 1;
    }else{
        return 0;
    }
}

int NCD1Relay::readAllInputs(){
    readInputStatusRetry:
    Wire.beginTransmission(address);
    Wire.write(0x09);
    byte status = Wire.endTransmission();
    if(status != 0){
        if(retrys < 3){
    #ifdef LOGGING
            Serial.println("Retry read bank status command");
    #endif
            retrys++;
            goto readInputStatusRetry;
        }else{
    #ifdef LOGGING
            Serial.println("Read bank status command failed");
    #endif
            initialized = false;
            retrys = 0;
            return 256;
        }
    }else{
        retrys = 0;
        initialized = true;
    }
    Wire.requestFrom(address, 1);
    byte bankStatus = Wire.read();
    byte inverted = ~bankStatus;
    byte shifted = inverted >> 1;
    return shifted;
}
