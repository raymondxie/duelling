#include <Adafruit_NeoPixel.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

IRsend irsend(D2);  // An IR LED is controlled by GPIO pin 4 (D2)

// NodeMCU D3 pin
#define PIN            D3

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      15

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int delayval = 1000;

const int button1 = D1;    // the number of the pushbutton pin
int button1State = 0;


// onboard LED, for testing
const int ledPin =  D4;      


void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(button1, INPUT_PULLUP);

  irsend.begin();

  pixels.begin(); 
  pixels.setBrightness(32);

}

void loop() {
  button1State = digitalRead(button1);

  // check if the pushbutton is pressed.
  // if it is, the button1State is HIGH:
  if (button1State == HIGH) {
    // turn LED off:
    digitalWrite(ledPin, LOW);

    
  } else {
    // turn LED on:
    digitalWrite(ledPin, HIGH);

    // lightup wand with charge()
    charge(1);
    
    // send out IR signal
    irsend.sendSony(0xa90, 12, 2);
    delay(50);

    // discharge the wand light
    discharge();

    // pace it, so you can not shoot a lot of bullets at one trigger
    delay(2000);
  }
}



void charge(int button) {
    uint32_t color;
  
    if(button == 0) {
        // red
        color = pixels.Color(64,0,0);
    }
    else if(button == 1) {
        // green
        color = pixels.Color(0,64,0);
    }
    else if(button == 2) {
        // blue
        color = pixels.Color(0,0,64);
    }

    int numPixel = pixels.numPixels();
    
    for(int i=0; i < numPixel; i++) {
      pixels.setPixelColor(i, color );    
      pixels.show();
      delay(20);
    }
}

void discharge() {
    uint32_t color = pixels.Color(0,0,0);

    int numPixel = pixels.numPixels();
    for(int i=0; i < numPixel; i++) {
      pixels.setPixelColor(i, color );    
      pixels.show();
      delay(20);
    }

}


