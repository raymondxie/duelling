#include <Adafruit_NeoPixel.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

IRsend irsend(D2);  // An IR LED is controlled by GPIO pin 4 (D2)

// NodeMCU D3 pin, for connecting to LED strip
#define stripPin            D3

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      15

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, stripPin, NEO_GRB + NEO_KHZ800);
int delayval = 1000;


// color array
uint32_t colors[3];
const int modePin = D1;
const int shootPin = D6;
int counter = 0;
int currentMode = 0;

// IR code array
unsigned long ircodes[3];


// onboard LED, for testing
const int ledPin =  D4;      


void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);

  pixels.begin(); 
  pixels.setBrightness(32);

  pinMode(modePin, INPUT_PULLUP);
  pinMode(shootPin, INPUT_PULLUP);
  pinMode(stripPin, OUTPUT);

  delay(delayval);

  // spell color
  colors[0] = pixels.Color(64,0,0);
  colors[1] = pixels.Color(0,64,0);
  colors[2] = pixels.Color(0,0,64);

  // spell IR code
  ircodes[0] = 0xa90;
  ircodes[1] = 0xb61;
  ircodes[2] = 0xc51;

  irsend.begin();

}

void loop() {
  int modeRead = digitalRead(modePin);
  if( modeRead == LOW ) {
      counter++;
      currentMode = counter % 3;  // 0, 1, 2

      pixels.setPixelColor(0, colors[currentMode] );    
      pixels.show();

      delay(200);
      Serial.println("mode, LOW");
  }
  else {
      Serial.println("mode, HIGH");
  }

  int shootRead = digitalRead(shootPin);
  if( shootRead == LOW ) {
    charge(currentMode);
    Serial.println("shoot, LOW");
    digitalWrite(ledPin, LOW);
  }
  else {
    Serial.println("shoot, HIGH");
    discharge();
  }

}



void charge(int modeCnt) {

    int numPixel = pixels.numPixels();
    
    for(int i=0; i < numPixel; i++) {
      pixels.setPixelColor(i, colors[modeCnt] );    
      pixels.show();
      delay(20);
    }

    irsend.sendSony(ircodes[modeCnt], 12, 2);
        
    delay(delayval);
}

void discharge() {
    uint32_t color = pixels.Color(0,0,0);

    int numPixel = pixels.numPixels();
    for(int i=1; i < numPixel; i++) {
      pixels.setPixelColor(i, color );    
      pixels.show();
      delay(20);
    }

}


