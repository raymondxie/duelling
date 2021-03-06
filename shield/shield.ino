//Defining Librarys 
#include <MQTT.h>
#include <PubSubClient.h>

#include <WiFiManager.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>

#include <IRrecv.h>
#include <IRremoteESP8266.h>

#include <Adafruit_NeoPixel.h>

//Defining WifiManager
WiFiManager wifiManager;

//Define PlayerId
String playerId = "";

//Defining Mqtt Credentials
const char *mqtt_server = "m11.cloudmqtt.com";
const int mqtt_port = 14375; 
const char *mqtt_user = "nodeuser";
const char *mqtt_pass = "nodepass"; 
const char *mqtt_topic_out = "shield";
const char *mqtt_topic_in = "player";

String mqtt_clientid = "mega3";

//Defining Receiver Pin: GPIO pin 14 (D5 on a NodeMCU)
uint16_t RECV_PIN = D5;
const int resetButtonPin = D7; //D7
int resetButtonState = 0;

IRrecv irrecv(RECV_PIN);
decode_results results;

// Neopixel Pin (NodeMCU D3 pin)
#define PIN           D3
// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS     12
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 500; // delay for half a second


char color[12]; // hold the color name (rgb) for each pixel
int ledCount = 0;

//Defining Wifi Client and Pubsub Client
WiFiClient wclient;
PubSubClient client(wclient, mqtt_server, mqtt_port);

//gets called when WiFiManager enters configuration mode
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
  //entered config mode, make led toggle faster
}

//Defining MQTT callback to receive message (player=[left|right]
void mqttCallback(const MQTT::Publish& pub) {
  Serial.println("Getting incoming MQTT message");
  
  if (digitalRead(resetButtonPin) == LOW) {
    //uint8_t* temp = pub.payload();
    //String boardId = temp.substr(6);
    String player = pub.payload_string();
    Serial.println(player);

    playerId = player.substring(7);
    Serial.println(playerId);

    // show an effect to indicate it is paired
    ringCircle('r');
  }
}

void ringCircle(char color) {
    uint16_t n = pixels.numPixels();
    
    for(int i=0; i<n; i++) {
      pixels.setPixelColor(i, getLedColor(color));
      pixels.show();
      delay(100);
    }
    delay(1000);

    resetGame();
}



//Reset Game: turn all LED light off, and clear color array to ' '
void resetGame(){
    ledCount = 0;
    uint16_t n = pixels.numPixels();

    for(int i=0; i<n; i++) {
      color[i] = ' ';
      pixels.setPixelColor(i, pixels.Color(0,0,0));
    }
    pixels.show();
    delay(1000);
 }

//Flash Once
void flashOnce() {
  uint16_t n = pixels.numPixels();
  
  // blink all lights for 10 times, then go dark
  for( int i=0; i<n; i++) {
    pixels.setPixelColor(i, getLedColor(color[i]));
  }
  pixels.show();
  delay(delayval);

  for( int i=0; i<n; i++) {
    pixels.setPixelColor(i, pixels.Color(0,0,0));
  }
  pixels.show();
  delay(delayval);
}

//Game Over effect
void gameOver() {
  for(int i=0; i<10; i++) {
    flashOnce();
  }
  resetGame();
  client.publish(mqtt_topic_out, "game=over");
}

// convert color name to color value
uint32_t getLedColor(char colorName){
  if( colorName == 'r' ){
      return pixels.Color(64,0,0);
  }
  else if( colorName == 'g' ){
      return pixels.Color(0,64,0);
  }
  else if( colorName == 'b' ){
      return pixels.Color(0,0,64);
  }
  else if( colorName == ' ' ){
      return pixels.Color(0,0,0);
  }
}

// When receiving an IR signal, we determine what color it is, and then process accordingly
void processIRInput(char colorName) {
  color[ledCount] = colorName;
  pixels.setPixelColor(ledCount, getLedColor(colorName));    
  pixels.show();

  ledCount++;
  
  String message = playerId + "=" + color;
  Serial.println(message);
  client.publish(mqtt_topic_out, message);

  delay(1000);
}

// Connect to MQTT broker
void reconnect() {
  while (!client.connected()) {
    mqtt_clientid = String(ESP.getChipId());
    Serial.print("Attempting MQTT connection...");
    Serial.print(mqtt_clientid);
    Serial.print("  ");
    Serial.print(mqtt_user);
    
    // Attempt to connect
    if (client.connect(MQTT::Connect(mqtt_clientid).set_auth(mqtt_user, mqtt_pass))) {
      Serial.println("MQTT connected");
      client.subscribe(mqtt_topic_in);
      client.set_callback(mqttCallback);
    } 
    else {
      Serial.print("failed, rc=");
      // Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {

  Serial.begin(9600);
  pinMode(resetButtonPin, INPUT_PULLUP);

  pixels.begin(); 
  pixels.setBrightness(32);

  ringCircle('r');
  ringCircle('g');
  ringCircle('b');

  wifiManager.setAPCallback(configModeCallback);

  Serial.println("Trying to connect to WiFi network");

  if (!wifiManager.autoConnect("AP-Shield")) {
    Serial.println("failed to connect to new WiFi network and hit timeout");
  }

  Serial.print("network connected to ..."); Serial.println(WiFi.SSID());

  if ( MDNS.begin ( "esp8266" ) ) {
    Serial.println ( "MDNS responder started" );
  }

  // ready for IR input
  irrecv.enableIRIn();

  // blink BLUE led once to indicate ready to go!
  pinMode(D0, OUTPUT);
  digitalWrite(D0, LOW);
  delay(1000);
  digitalWrite(D0, HIGH);
  
}


void loop() {

  //Reset Button 
  resetButtonState = digitalRead(resetButtonPin);
  if(resetButtonState == LOW){
    Serial.println("Reset is being pressed");
    resetGame();
  }
  //Differentiate Spells when hit
  else if(playerId.length() > 0) {
    if (irrecv.decode(&results)) {
    // print() & println() can't handle printing long longs. (uint64_t)
      if(results.value == 0xa90){
        Serial.println("Receiving Red");
        processIRInput('r');        
      }
      else if(results.value == 0xb61){
        processIRInput('g');
      }
      else if(results.value == 0xc51){
        processIRInput('b');
      }
      irrecv.resume();
    }    
  }

  if( ledCount == 12 ) 
    gameOver();

  // 
  if (!client.connected()) {
    reconnect();
  }
  else {
    client.loop();
  }
}


