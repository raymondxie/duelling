# Duelling
Duelling game built for Maker Faire. About wizarding culture of duelling, please read: http://harrypotter.wikia.com/wiki/Duelling

# Build
We utilize ESP8266 wifi chip for most of build, which needs to take command to send and receive InfraRed (IR) light signal, control the NeoPixel lighting effects, and communicate with backend cloud system.

For the fundamentals of IR communication, please read:
https://learn.sparkfun.com/tutorials/ir-communication
As always, Adafruit has detailed explanation of how thing works:
https://learn.adafruit.com/ir-sensor, and https://learn.adafruit.com/using-an-infrared-library/hardware-needed

For sending and receiving IR signals on ESP8266, we utilize this library:
https://github.com/markszabo/IRremoteESP8266

IR emitter has two legs, connect long leg to signal PIN and short leg to ground PIN; you may add a 200-500 ohms resistor.
IR receiver has three legs, see this image for the pin connector: https://github.com/raymondxie/duelling/blob/master/doc/ir-receiver.jpg


## Wand

1. Use 2 buttons to trigger IR sender to emit different IR codes (later on may extend to 3 buttons, for Fire, Water and Wood). 
2. Add Neopixel strip to simulate the effect of casting a spell
3. Build a wand which houses Neopxiel strip, IR emitter, NodeMCU board (may consider other smaller board), and connecting buttons, etc.

## Shield

1. Sew a Neopixel ring onto a safety vest, with an IR receiver.
2. Upon receiving IR code, lights up one LED on the ring with color corresponding to Fire/Water/Wood elements.
3. Send signal to cloud, reporting which vest was hit by wich wand, by which element.

## Scoring 

1. points for hitting a target with a specific element;
2. bonus points for hitting a target with a sequence of elements (we will define various patterns and their bonus points)
3. winner who successfully lit up his opponent's all lights on Neopixel ring

# Game rules

