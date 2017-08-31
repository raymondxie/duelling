
#include <WiFiManager.h>

#include <MQTT.h>
#include <PubSubClient.h>
#include <PubSubClient_JSON.h>

#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include <ir_Daikin.h>
#include <ir_Fujitsu.h>
#include <ir_Kelvinator.h>
#include <ir_LG.h>
#include <ir_Mitsubishi.h>
#include <ir_Trotec.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRtimer.h>
#include <IRutils.h>

#include <Adafruit_NeoPixel.h>

#include <ESP8266WiFi.h>

//const char *ssid =  "XIESLAND";   // cannot be longer than 32 characters!
//const char *pass =  "719fc8a4be";  

WiFiManager wifiManager;
ESP8266WebServer server ( 80 );

const char *mqtt_server = "m11.cloudmqtt.com";
const int mqtt_port = 14375; 
const char *mqtt_user = "nodeuser";
const char *mqtt_pass = "nodepass"; 
const char *mqtt_topic = "";  
String mqtt_clientid = "mega";

byte mac[]    = {  0xB8, 0x53, 0xDB, 0x12, 0x0C, 0x09 };

//Defining Receiver Pin: GPIO pin 14 (D5 on a NodeMCU)
uint16_t RECV_PIN = D5;
const int resetButtonPin = D7; //D7
int resetButtonState = 0;

IRrecv irrecv(RECV_PIN);

decode_results results;

#define BUFFER_SIZE 100

bool LedState = false;

// Neopixel Pin (NodeMCU D3 pin)
#define PIN           D3

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS     12

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 500; // delay for half a second
// uint32_t color[12]; // hold the color for each pixel
char color[12]; // hold the color for each pixel
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


void callback(const MQTT::Publish& pub) {
  // In order to republish this payload, a copy must be made
  // as the orignal payload buffer will be overwritten whilst
  // constructing the PUBLISH packet.
  Serial.println("Calling back");
  // Copy the payload to a new message
  if (digitalRead(resetButtonPin) == LOW){
    //uint8_t* temp = pub.payload();
    //String boardId = temp.substr(6);
    Serial.println(pub.payload_string());
  }
}




void setup() {
  pixels.begin(); 
  pixels.setBrightness(32);

  delay(delayval);

  Serial.begin(115200);
  irrecv.enableIRIn();

  pinMode(resetButtonPin, INPUT_PULLUP);

  wifiManager.setAPCallback(configModeCallback);

  Serial.println("Trying to connect to WiFi network");

  if (!wifiManager.autoConnect("AP-Shield")) {
    Serial.println("failed to connect to new WiFi network and hit timeout");
  }

  Serial.print("network connected to ..."); Serial.println(WiFi.SSID());

  if ( MDNS.begin ( "esp8266" ) ) {
    Serial.println ( "MDNS responder started" );
  }
   
}


/*void callback(const MQTT::Publish& pub) {
  LedState = !LedState;
  digitalWrite(0, LedState);  
}
*/

void loop() {
  //simulateShield();
  
  // pace the loop

  //Mqtt Connect
 /*if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting to ");
    Serial.print(ssid);
    Serial.println("...");
    WiFi.begin(ssid, pass);

    if (WiFi.waitForConnectResult() != WL_CONNECTED)
      return;
    Serial.println("WiFi connected");
  }
*/
  if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) {
      Serial.println("Connecting to MQTT server");
      if (client.connect(MQTT::Connect("arduinoClient")
                         .set_auth(mqtt_user, mqtt_pass))) {
        Serial.println("Connected to MQTT server");
        client.publish("shield","hello world");
        client.subscribe("player");
        client.set_callback(callback);
      } else {
        Serial.println("Could not connect to MQTT server");   
      }
    }

    if (client.connected())
      client.loop();
  }
  
  
  //Reset Button 

  
  resetButtonState = digitalRead(resetButtonPin);

  if(resetButtonState == LOW){
    Serial.println("Reset is being pressed");
    resetGame();
  }
  else{
    if (irrecv.decode(&results)) {
    // print() & println() can't handle printing long longs. (uint64_t)
    if(results.value == 0xa90){
      ledCount++;
      //color[ledCount-1] = pixels.Color(64,0,0);
      color[ledCount-1] = 'r';
      lightUpShield();
      client.publish("shield", "hit by fire");
      Serial.println("hit by fire");
    }
    else if(results.value == 0xb61){
      ledCount++;
      // r = pixels.Color(0,64,0)
      color[ledCount-1] = 'g';
      lightUpShield();
      client.publish("shield", "hit by earth");
      Serial.println("hit by earth");
    }
    else if(results.value == 0xc51){
      ledCount++;
      // color[ledCount-1] = pixels.Color(0,0,64);
      color[ledCount-1] = 'b';
      lightUpShield();
      client.publish("shield", "hit by ice");
      Serial.println("hit by ice"); 
    }
    irrecv.resume();
  }    
  }
  //delay(delayval); 
  if( ledCount == 12 ) 
    gameOver();
   
    String str(color);
    Serial.println(str);
    client.publish("shield", str);
    delay(1000);
}

void lightUpShield(){
  int i = ledCount-1;
  pixels.setPixelColor(i, color[i]);    
  pixels.show();
  delay(1000);
}


void resetGame(){
    ledCount = 0;
    for(int i=0; i<12; i++) {
      color[i] = pixels.Color(0,0,0);
    }
    displayLed();
    delay(1000);
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
  resetGame();
}

void displayLed(){
  for( int i=0; i<12; i++) {
    
    if( color[i] == 'r' ){
      pixels.setPixelColor(i, pixels.Color(64,0,0));
  }
    if( color[i] == 'g'){
      pixels.setPixelColor(i, pixels.Color(0,64,0));
    }
    if(color[i] == 'b'){
      pixels.setPixelColor(i, pixels.Color(0,0,64));
    }
  pixels.show();
}
}

