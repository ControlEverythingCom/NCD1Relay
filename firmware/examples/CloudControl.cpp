/* Includes ------------------------------------------------------------------*/
#include "NCD1Relay/NCD1Relay.h"

NCD1Relay relayController;

SYSTEM_MODE(AUTOMATIC);

int triggerRelay(String command);

bool tripped[7];

int debugTrips[7];

int minTrips = 5;

/* This function is called once at start up ----------------------------------*/
void setup()
{
	Particle.function("controlRelay", triggerRelay);
	Serial.begin(115200);
	relayController.setAddress(0,0,0);
}

/* This function loops forever --------------------------------------------*/
void loop()
{
	int status = relayController.readAllInputs();
	int a = 0;
	for(int i = 1; i < 65; i*=2){
		if(status & i){
			debugTrips[a]++;
			if(debugTrips[a] >= minTrips){
				if(!tripped[a]){
					tripped[a] = true;
					//set input trip event to true
					String eventName = "Input_";
					eventName+=(a+1);
					Particle.publish(eventName, "ON");
					Serial.print("eventName: ");
					Serial.println(eventName);
					Serial.print("eventContents: ");
					Serial.println("ON");
				}
			}
		}else{
			debugTrips[a] = 0;
			if(tripped[a]){
				tripped[a] = false;
				//set input trip event to false
				String eventName = "Input_";
				eventName+=(a+1);
				Particle.publish(eventName, "OFF");
				Serial.print("eventName: ");
				Serial.println(eventName);
				Serial.print("eventContents: ");
				Serial.println("OFF");
			}
		}
		a++;
	}
}

int triggerRelay(String command){
	if(command.equalsIgnoreCase("on")){
		Serial.println("Turning on relay");
		relayController.turnOnRelay();
		Serial.println("returning");
		return 1;
	}
	if(command.equalsIgnoreCase("off")){
		relayController.turnOffRelay();
		return 1;
	}
	if(command.equalsIgnoreCase("toggle")){
		relayController.toggleRelay();
		return 1;
	}
	if(command.equalsIgnoreCase("momentary")){
		relayController.turnOnRelay();
		delay(300);
		relayController.turnOffRelay();
		return 1;
	}
	return 0;
}
