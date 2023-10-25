//Fikira Smart farming

/****************************************
 * Include Libraries
 ****************************************/
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include "DHT.h"
#include "Ubidots.h"

/****************************************
 * Define Instances and Constants
 ****************************************/
static const int RXPin = 14, TXPin = 12; //D5 and D6
static const uint32_t GPSBaud = 9600;

// The TinyGPSPlus object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);
 
#define DHTPIN 2 
#define DHTTYPE DHT11  
const char* UBIDOTS_TOKEN = "BBFF-doQqzD0N2zBXfdRkR3Dh3FY2WReNR5";  
const char* WIFI_SSID = "MTN_4G_4578D8";      
const char* WIFI_PASS = "C23C8BCD";      
Ubidots ubidots(UBIDOTS_TOKEN, UBI_HTTP);
DHT dht(DHTPIN, DHTTYPE);
int sensor_pin = A0;
int mois ;
float latitude;
float longitude;

void setup() {
   Serial.begin(115200);
   ss.begin(GPSBaud);
   ubidots.wifiConnect(WIFI_SSID, WIFI_PASS);
   ubidots.setDebug(true);// Uncomment this line for printing debug messages
   dht.begin();
}

//Location function

void sendLocation(){
   float value = random(0, 9) * 10;
  /* Hardcoded Coordinates */
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
  
  latitude = 11.9666;
  longitude = 8.50564;
  
    
  /* Reserves memory to store context key values, add as much as you need */
  char* str_lat = (char*)malloc(sizeof(char) * 10);
  char* str_lng = (char*)malloc(sizeof(char) * 10);
  /* Saves the coordinates as char */
  sprintf(str_lat, "%f", latitude);
  sprintf(str_lng, "%f", longitude);

  /* Reserves memory to store context array */
  char* context = (char*)malloc(sizeof(char) * 30);

  /* Adds context key-value pairs */
  ubidots.addContext("lat", str_lat);
  ubidots.addContext("lng", str_lng);

  /* Builds the context with the coordinates to send to Ubidots */
  ubidots.getContext(context);

  /* Sends the position */
  ubidots.add("position", value, context);  // Change for your variable name

   bool bufferSent = false;
  bufferSent = ubidots.send();  // Will send data to a device label that matches the device Id

  if (bufferSent) {
    // Do something if values were sent properly
    Serial.println("Location sent by the device");
    Serial.println(longitude);
    Serial.println(latitude);
  }
  
  /* free memory */
  free(str_lat);
  free(str_lng);
  free(context);

}

//Weather and soil condition function

void sendWeather(){
  double t = dht.readTemperature();
  double h = dht.readHumidity();
  int mois= analogRead(sensor_pin);
  mois = map(mois,1024,0,0,100);
  ubidots.add("temp", t);  
  ubidots.add("hum", h);
  ubidots.add("moist", mois);
  bool bufferSent = false;
  bufferSent = ubidots.send();  

  if (bufferSent) {
    // Do something if values were sent properly
    Serial.println("Weather values  sent by the device");
  }
 }

void loop() {
  
  sendWeather();
  sendLocation();
  delay(5000);
}
