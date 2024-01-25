#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>
#include <FastLED.h>

char ssid[] = "dlink";                  // wifi network SSID (name)
char pass[] = "";                       // wifi network password


// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;
const IPAddress outIp(10, 40, 10, 105); // remote IP (not needed for receive)
const unsigned int outPort = 9999;      // remote port (not needed for receive)
const unsigned int localPort = 7448;    // local port to listen for UDP packets (here's where we send the packets)

OSCErrorCode error;

#define NUM_LEDS 10
#define DATA_PIN 15
int CUR_COLOR = 3;
int count = 0;
int FLICKER = 30;
int brightness = 100;
int state = 2;
int saturation = 100;

CRGB  leds[NUM_LEDS];
int ledv[NUM_LEDS];
int brightmin, brightmax;

int wipeTemp = 0;

void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);

    WiFi.begin(ssid, pass);              //connect to Wifi

    while (WiFi.status() != WL_CONNECTED) { 
        delay(500);
        Serial.println(".");              // working on it...
    }
    // IPAddress local_IP(192, 168, 0, 199);
    IPAddress StaticIP(192, 168, 0, 210); //set staticIP
    IPAddress gateway(192,168,0,1);
    IPAddress subnet(255,255,255,0);
    // IPAddress ip(192, 168, 0, 199);


    Serial.println("WiFi connected");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());      //Print LocalIP for Serial Monitor

    Serial.println("Starting UDP");
    Udp.begin(localPort);
    Serial.print("Local port: ");


                                        // wait a few seconds so someone can see the IP address if they're watching
    delay(3000);

  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  for(int i = 0; i<NUM_LEDS; i++){
    leds[i] = CHSV(50,255,0); 
  }
}




void flicker() {
  brightmin = (brightness-FLICKER);     //Sets lower
  brightmax = (brightness+FLICKER);     //and upper brightness bounds for flicker
  
  
  switch (state) {
    case 1:
      basicFlicker();
      break;
    case 2:
      wipeOn();
      break;
    case 3:
      wipeOff();
      break;
  }
delay(random(20,50));

}

void basicFlicker(){
  for(int i = 0; i<NUM_LEDS; i++){
    int heatindex = (CUR_COLOR + random(-2,2));
    leds[i] = CHSV(heatindex, saturation, random(brightmin,brightmax));
  }
  FastLED.show();
}

void wipeOn(){
  brightness = 0;
  for(int i = 0; i<NUM_LEDS; i++){
    brightness = brightness + 22;
    brightmin = (brightness-FLICKER);     //Sets lower
    brightmax = (brightness+FLICKER);     //and upper brightness bounds for flicker
    for(int j = 0; j<=i; j++){
      int heatindex = (CUR_COLOR + random(-2,2));
      leds[j] = CHSV(heatindex, 254, random(brightmin,brightmax));
      FastLED.show();
    }
    delay(random(50,80));
  }
  state = 1;
}

void wipeOff(){   
  brightness = 220;
  for(int i = 0; i<NUM_LEDS; i++){
    brightness = brightness - 22;         //Gradually lowers brightness
    brightmin = (brightness-FLICKER);     //Sets lower
    brightmax = (brightness+FLICKER);     //and upper brightness bounds for flicker
    for(int j = 0; j<=i; j++){
      int heatindex = (CUR_COLOR + random(-2,2));
      leds[j] = CHSV(heatindex, 254, brightness);
      FastLED.show();
    }
    delay(random(50,80));
  }
  state = 4;
  for(int i = 0; i<NUM_LEDS; i++){
    leds[i] = CHSV(50,200,0);
    FastLED.show(); 
  }
}

void idleTime(){
  delay(5000);                           //delays for 5 seconds. Can be replaced by a wait for a message.
  state = 2;                             //returns to wipeOn, raising the candless to lit.
}


void button_msg(OSCMessage &msg, int x)  //Function when microcontroller recieves button input
{
  if (wipeTemp == 0) {
    state = 2;                           //wipeOn()
    wipeTemp = 1;                        //set status to on
  } else { 
    state = 3;                           //wipeOff()
    wipeTemp = 0;                        //set status to off
  }
}

void brightness_msg(OSCMessage &msg, int x)
{
  float data = msg.getFloat(0);
  Serial.print("slider: ");             //send brightness value to serial monitor
  Serial.println(data);

  brightness = (data*100) + 70;
}

void saturation_msg(OSCMessage &msg, int x)
{
  float data = msg.getFloat(0);
  Serial.print("slider: ");              //send saturation value to serial monitor
  Serial.println(data);

  saturation = data*254;
}

void loop() {

    OSCMessage msg; 
    int size = Udp.parsePacket();       //size of UDP message
    if (size > 0) {
        while (size--) {
            msg.fill(Udp.read());       //read the message
        }
        if (!msg.hasError()) {          //if microcontroller recieves message

            msg.route("/toggle", button_msg); //call button_msg
            msg.route("/brightness", brightness_msg); //call brightness_msg
            msg.route("/saturation", saturation_msg); //call saturation_msg

        } else {
            error = msg.getError();
            Serial.print("Error: ");
            Serial.println(error);
        }
    }

flicker();                             //call flicker loop to change state

}