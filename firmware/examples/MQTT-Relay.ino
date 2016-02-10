// This #include statement was automatically added by the Particle IDE.
#include "NCD1Relay/NCD1Relay.h"

#include "NCD1Relay/MQTT/MQTT.h"

void callback(char* topic, byte* payload, unsigned int length);

/**
 * if want to use IP address,
 * byte server[] = { XXX,XXX,XXX,XXX };
 * MQTT client(server, 1883, callback);
 * want to use domain name,
 * MQTT client("www.sample.com", 1883, callback);
 **/
byte server[] = {192,168,10,124};
MQTT client(server, 1883, callback);
NCD1Relay controller;
int relayStatus;
unsigned long lastSend = 0;
unsigned long sendInterval = 2000;

// recieve message
void callback(char* topic, byte* payload, unsigned int length) {
    char p[length + 1];
    memcpy(p, payload, length);
    p[length] = NULL;
    String message(p);
    String t = String(topic);
    Serial.println(t);
    Serial.println(message);

    if(message.equalsIgnoreCase("on")){
        controller.turnOnRelay();
    }
    if(message.equalsIgnoreCase("off")){
        controller.turnOffRelay();
    }
    
    delay(1000);
}


void setup() {
    controller.setAddress(0,0,0);
    relayStatus = controller.readRelayStatus();
    
    // connect to the server
    client.connect("particlerelay");

    // publish/subscribe
    if (client.isConnected()) {
        client.subscribe("setrelaystatus");
    }
}

void loop() {
    if (client.isConnected()){
        client.loop();
        if(millis() >= lastSend+sendInterval){
            lastSend = millis();
            client.publish("relaystatus",String(relayStatus));
        }
        
    }
    relayStatus = controller.readRelayStatus();
}
