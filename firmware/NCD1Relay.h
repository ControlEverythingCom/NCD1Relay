#include "spark_wiring_i2c.h"
#include "spark_wiring_usbserial.h"
#include "spark_wiring_constants.h"

class NCD1Relay{
public:
    //Constructor
    NCD1Relay(void);
    //Set Address.  Indicate status of jumpers on board.  Send 0 for not installed, send 1 for installed
    void setAddress(int a0, int a1, int a2);
    //Turn on Relay
    void turnOnRelay();
    //Turn off Relay
    void turnOffRelay();
    //Toggle Relay
    void toggleRelay();
    //Read status of relay. Valid return 0 for off 1 for on.  256 returned if there is an error
    int readRelayStatus();
    //Read status of input
    int readInputStatus(int input);
    //Read status of all inputs
    int readAllInputs();
    
    //User Accessible Variables
    //Whether or not the controller is ready to accept commands
    bool initialized;
    
private:
    //internal use method for refreshing bank status variables
    void readStatus();
    //Status of relays in bank 1
    byte bankOneStatus;

};
