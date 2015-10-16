/* Includes ------------------------------------------------------------------*/
#include "NCD1Relay/NCD1Relay.h"

SYSTEM_MODE(AUTOMATIC);

NCD1Relay relayController;

/* This function is called once at start up ----------------------------------*/
void setup()
{
	Serial.begin(115200);
	relayController.setAddress(0,0,0);
}

/* This function loops forever --------------------------------------------*/
void loop()
{
	int Status = relayController.readAllInputs();
	if(Status != 0){
		Serial.print("Status: ");
		Serial.println(Status);
	}
	delay(50);
	relayController.toggleRelay();
	delay(500);
}
