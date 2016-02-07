// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>

// replace with your channel’s thingspeak API key,
String apiKey = "XXXXXXXXXX";
const char* server = "api.thingspeak.com";


// Setup WiFi
const char* ssid = "SSID";
const char* password = "PASSWORD";

WiFiClient client;



// Data wire is plugged into port
#define ONE_WIRE_BUS D4

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

/*
 * The setup function. We only start the sensors here
 */
void setup(void)
{
  // start serial port
  Serial.begin(9600);
  Serial.println("Dallas Temperature IC on a ESP8266");

  delay(10);

  WiFi.begin(ssid, password);


  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
   
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start up the library
  sensors.begin();
}

/*
 * Main function, get and show the temperature
 */
void loop(void)
{ 

  sensors.requestTemperatures(); // Send the command to get temperatures

  float c = sensors.getTempCByIndex(0);

     Serial.print("Temperature: ");
     Serial.print(c);
     Serial.println("C send to Thingspeak");   
     
  if (client.connect(server,80)) {  //   "184.106.153.149" or api.thingspeak.com
    String postStr = apiKey;
           postStr +="&field1=";
           postStr += String(c);
           postStr += "\r\n\r\n";
 
     client.print("POST /update HTTP/1.1\n"); 
     client.print("Host: api.thingspeak.com\n"); 
     client.print("Connection: close\n"); 
     client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n"); 
     client.print("Content-Type: application/x-www-form-urlencoded\n"); 
     client.print("Content-Length: "); 
     client.print(postStr.length()); 
     client.print("\n\n"); 
     client.print(postStr);
           
  }
  client.stop();
  
  Serial.println("Waiting...");    
  // thingspeak needs minimum 15 sec delay between updates
  delay(600000); 
}
