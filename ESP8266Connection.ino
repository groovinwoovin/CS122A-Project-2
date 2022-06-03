/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID           "TMPL_1ODY1Qb"
#define BLYNK_DEVICE_NAME           "Quickstart Device"
#define BLYNK_AUTH_TOKEN            "D2jM7dY8WKywt39RyPDsCrQnodoIOfKI"


// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "The Boys 2.4G";
char pass[] = "kangaroo384";

// GLOBAL CONSTANTS
#define RED_LED 16
#define BLUE_LED 5
#define GREEN_LED 4
#define YELLOW_LED 0

#define RED_BUTTON 14
#define BLUE_BUTTON 12
#define GREEN_BUTTON 13
#define YELLOW_BUTTON 15

BlynkTimer timer;

int spot1Status = 0;  // 1 if taken, 0 if free
int spot2Status = 0;
int spot3Status = 0;
int spot4Status = 0;

int v0 = 0; // 1 if the switch is on, 0 if off
int v1 = 0;
int v2 = 0;
int v3 = 0;

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();
  v0 = value;

  // Check if the spot is occupied
  if(spot1Status){
    // Don't let the user register for a spot
    Blynk.virtualWrite(V0, 0);
    Blynk.virtualWrite(V9, "Spot 1 Unavailable");
  }
  else{
    // Let the user know if the spot is reserved
    digitalWrite(RED_LED, value);
    Blynk.virtualWrite(V9, "Spot 1 Reserved!");
  }
}

BLYNK_WRITE(V1){
  // Set incoming value from pin V1 to a variable
  int value = param.asInt();
  v1 = value;

  // Check if the spot is occupied
  if(spot2Status){
    // Don't let the user register for a spot
    Blynk.virtualWrite(V1, 0);
    Blynk.virtualWrite(V9, "Spot 2 Unavailable");
  }
  else{
    // Let the user know if the spot is reserved
    digitalWrite(BLUE_LED, value);
    Blynk.virtualWrite(V9, "Spot 2 Reserved!");
  }
}

BLYNK_WRITE(V2){
  // Set incoming value from pin V2 to a variable
  int value = param.asInt();
  v2 = value;
  
  // Check if the spot is occupied
  if(spot3Status){
    // Don't let the user register for a spot
    Blynk.virtualWrite(V2, 0);
    Blynk.virtualWrite(V9, "Spot 3 Unavailable");
  }
  else{
    // Let the user know if the spot is reserved
    digitalWrite(GREEN_LED, value);
    Blynk.virtualWrite(V9, "Spot 3 Reserved!");
  }
}

BLYNK_WRITE(V3){
  // Set incoming value from pin V3 to a variable
  int value = param.asInt();
  v3 = value;
  
  // Check if the spot is occupied
  if(spot4Status){
    // Don't let the user register for a spot
    Blynk.virtualWrite(V3, 0);
    Blynk.virtualWrite(V9, "Spot 4 Unavailable");
  }
  else{
    // Let the user know if the spot is reserved
    digitalWrite(YELLOW_LED, value);
    Blynk.virtualWrite(V9, "Spot 4 Reserved!");
  }
}

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED(){}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V8, millis() / 1000);
}

void setup()
{
  // Debug console
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);

  // Setup I/O
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_BUTTON, INPUT);
  pinMode(BLUE_BUTTON, INPUT);
  pinMode(GREEN_BUTTON, INPUT);
  pinMode(YELLOW_BUTTON, INPUT);

  // Clear App inputs
  Blynk.virtualWrite(V0, 0);
  Blynk.virtualWrite(V1, 0);
  Blynk.virtualWrite(V2, 0);
  Blynk.virtualWrite(V3, 0);

  // Reset App message
  Blynk.virtualWrite(V9, "Welcome! Select an open spot.");
}

void readSpots();

void loop()
{
  Blynk.run();
  timer.run();
  readSpots();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
}

void readSpots(){ // Checks the status of the spots and updates them as needed
  spot1Status = digitalRead(RED_BUTTON);
  spot2Status = digitalRead(BLUE_BUTTON);
  spot3Status = digitalRead(GREEN_BUTTON);
  spot4Status = digitalRead(YELLOW_BUTTON);

  // Set the status of the spots in the app
  Blynk.virtualWrite(V4, (spot1Status == HIGH) ? 1023 : 0);
  Blynk.virtualWrite(V5, (spot2Status == HIGH) ? 1023 : 0);
  Blynk.virtualWrite(V6, (spot3Status == HIGH) ? 1023 : 0);
  Blynk.virtualWrite(V7, (spot4Status == HIGH) ? 1023 : 0);

  // If any of the spots get stolen, the application will reset the spots
  if(spot1Status && v0 == 1){
    digitalWrite(RED_LED, 0);
    Blynk.virtualWrite(V0, 0);
  }
  if(spot2Status && v1 == 1){
    digitalWrite(BLUE_LED, 0);
    Blynk.virtualWrite(V1, 0);
  }
  if(spot3Status && v2 == 1){
    digitalWrite(GREEN_LED, 0);
    Blynk.virtualWrite(V2, 0);
  }
  if(spot4Status && v3 == 1){
    digitalWrite(YELLOW_LED, 0);
    Blynk.virtualWrite(V3, 0);
  }
}
