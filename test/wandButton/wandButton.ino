const int buttonPin = D2;    // the number of the pushbutton pin
const int ledPin =  D4;      // the number of the LED pin

int buttonState = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    // turn LED on:
    digitalWrite(ledPin, HIGH);

    // lightup wand with charge()

    // send out IR signal
    
  } else {
    // turn LED off:
    digitalWrite(ledPin, LOW);

    // discharge the wand light
        
  }
}
