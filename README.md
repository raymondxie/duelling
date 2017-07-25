# Duelling
Duelling game built for Maker Faire. About wizarding culture of duelling, please read: http://harrypotter.wikia.com/wiki/Duelling

# Build
We utilize ESP8266 wifi chip for most of build, which needs to take command to send and receive InfraRed (IR) light signal, control the NeoPixel lighting effects, and communicate with backend cloud system.

For the fundamentals of IR communication, please read:
https://learn.sparkfun.com/tutorials/ir-communication

For sending and receiving IR signals on ESP8266, we utilize this library:
https://github.com/markszabo/IRremoteESP8266

IR emitter has two legs, connect long leg to signal PIN and short leg to ground PIN; you may add a 200-500 ohms resistor.
IR receiver has three legs, see this image for the pin connector: https://github.com/raymondxie/duelling/blob/master/doc/ir-receiver.jpg


## Wand


## Shield


## Scoring 



# Game rules

