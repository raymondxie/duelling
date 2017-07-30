#include <Adafruit_NeoPixel.h>

// NodeMCU D3 pin
#define PIN            D3

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      15

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int delayval = 1000;

void setup() {
  pixels.begin(); 
  pixels.setBrightness(32);

  delay(delayval);
}

void loop() {
  int buttonPressed = random(3);
  charge(buttonPressed);
  delay(delayval);

  discharge();
  delay(delayval); 

  // pace the loop
  delay(delayval);
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


