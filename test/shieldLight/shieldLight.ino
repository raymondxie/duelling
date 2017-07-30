#include <Adafruit_NeoPixel.h>

// NodeMCU D3 pin
#define PIN            D3

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      12

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 500; // delay for half a second
int count = 0;
uint32_t color[12]; // hold the color for each pixel

void setup() {
  pixels.begin(); 
  pixels.setBrightness(32);

  count = 0;
  delay(delayval);
}

void loop() {
  simulateShield();
  
  // pace the loop
  delay(delayval); 
}


void simulateShield() {
  // wait for IR sensor read, to increment count and get the color
  count++;

  if( count > 0 && count <= 12 ) {
    int randomColor = random(3);
    if(randomColor == 0) {
        // red
        color[count-1] = pixels.Color(64,0,0);
    }
    else if(randomColor == 1) {
        // green
        color[count-1] = pixels.Color(0,64,0);
    }
    else if(randomColor == 2) {
        // blue
        color[count-1] = pixels.Color(0,0,64);
    }
    
    pixels.setPixelColor(count-1, color[count-1] );    
    pixels.show();
  }
  
  if( count == 12 ) 
    gameOver();
  
}

void flashOnce() {
  uint16_t n = pixels.numPixels();
  
  // blink all lights for 10 times, then go dark

  for( int i=0; i<n; i++) {
    pixels.setPixelColor(i, color[i]);
  }
  pixels.show();
  delay(delayval);

  for( int i=0; i<n; i++) {
    pixels.setPixelColor(i, pixels.Color(0,0,0));
  }
  pixels.show();
  delay(delayval);

}

void gameOver() {
  for(int i=0; i<10; i++) {
    flashOnce();
  }
}

