# About

This Library is intended for use with NCD 1 Relay Particle Core/Photon compatible relay controllers.

The intention of this library is to make use of the NCD 1 channel relay controller with Particle development web IDE as simple as possible for users.

###Developer information

NCD has been designing and manufacturing computer control products since 1995.  We have specialized in hardware design and manufacturing of Relay controllers for 20 years.  We pride ourselves as being the industry leader of computer control relay products.  Our products are proven reliable and we are very excited to support Particle.  For more information on NCD please visit www.controlanything.com 

###Requirements

- NCD 1 Channel Particle Core/Photon Compatible Relay board
- Particle Core/Photon module
- Knowledge base for developing and programming with Particle Core/Photon modules.

### Version

1.0.0

### How to use this library

The libary must be imported into your application.  This can be done through the Particle WEB IDE by selecting Libraries, then select the NCD1Relay.  Click Include in App button.  Select the App you want to include the library in.  Finally click Add to this app.  For more information see [Particles's documentation] [sparkIncludeLibrary] 

### Example use

Once the Library is included in your application you should see an include statement at the top like this:

```cpp
//This #include statement was automatically added by the Particle IDE.
#include "NCD1Relay/NCD1Relay.h"
```

Now you need to instanciate an object of the library for use in your application like this:
```cpp
NCD1Relay relayController;
```

Here is an example use case for the class

```cpp
// This #include statement was automatically added by the Particle IDE.
#include "NCD1Relay/NCD1Relay.h"
NCD1Relay relayController;

void setup() {
Serial.begin(115200);
relayController.setAddress(0, 0, 0);
}

void loop() {
int Status = relayController.readAllInputs();
if(Status != 0){
Serial.print("Status: ");
Serial.println(Status);
}
delay(50);
relayController.toggleRelay();
delay(500);
}
```

###Public accessible methods

```cpp
void setAddress(int a0, int a1, int a2);
```

>Must be called first before using the object.  This method should also be called any time communication with
>the controller is lost or broken to recover communication  This method accepts two int arguments.  This
>tells the Library what address to direct commands to.  a0 and a1 ints are representations of the two
>jumpers on the 4 channel relay controller which are labeled on the board A0, A1, and A2.  If the jumper is
>installed then that int in this call should be set to 1.  If it is not installed then the int should be set to 
So if I have A0, A1, and A2 installed I would call ```relayController.setAddress(1, 1, 1).```

```cpp
void turnOnRelay();
```

>A call to this method will turn on the relay.

```cpp
void turnOffRelay(int Relay);
```

>A call to this method will turn off the relay.

```cpp
void toggleRelay();
```

>A call to this method will toggle the state of the relay.  If the relay was previously off a call to this method
>turn the relay on and vice versa.

```cpp
int readRelayStatus();
```

A call to this method will read the status of the relay and return the current on/off
>status of the relay.  1 will be returned if the relay is on and 0 will be returned if the relay is off. 
>256 will be returned if an error has occured(generally due to lack of communication with the controller).

```cpp
int readInputStatus(int input);
```

>This method accepts one int argument and returns one int.  Valid input int arguments 1-7.  A call to this
>method will read the status of the given input passed by the input argumetn and return the current closed/open
>status of the given input.  1 will be returned if the input is closed and 0 will be returned if the input is open.
>256 will be returned if an error has occured(generally due to lack of communication with the controller).

```cpp
byte readAllInputs();
```

>This method accepts no arguments and returns one byte.  A call to this
>method will read and return the status of all 7 inputs on the board. 
>Each input on the board is represented as a bit in the returned byte.  Valid returns are 0-128.  If the input is closed 
>the bit in the byte is set to 1, if the input is open the bit in the byte is set to 0.  256 will be
>returned if an error has occured(generally due to lack of communciation with controller).

###Public accessible variables

```cpp
bool initialized;
```

>This boolean indicates the current status of the interface connection to the controller.  This variable should
>be checked often throughout your application.  If communication to the board is lost for any reason this
>boolean variable will return false.  If all is well it will return true.

License
----

GNU
[sparkIncludeLibrary]:http://docs.spark.io/build/#flash-apps-with-spark-build-using-libraries
