// Adafruit IO REST API access with ESP8266
//
// For use with ESP8266 Arduino from:
//   https://github.com/esp8266/Arduino
//
// Works great with ESP8266 modules like the Adafruit Huzzah ESP:
//  ----> https://www.adafruit.com/product/2471
//
// Written by Tony DiCola for Adafruit Industries.  
// MIT license, all text above must be included in any redistribution.
#include <ESP8266WiFi.h>
#include "Adafruit_IO_Client.h"
#include "DHT.h"

// DHT 11 sensor
#define DHTPIN 2
#define DHTTYPE DHT22 

// DHT sensor
DHT dht(DHTPIN, DHTTYPE, 15);

// Configure WiFi access point details.
#define WLAN_SSID  "XXXXXX"
#define WLAN_PASS  "XXXXXXX"

// Configure Adafruit IO access.
#define AIO_KEY    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"



// Create an ESP8266 WiFiClient class to connect to the AIO server.
WiFiClient client;

// Create an Adafruit IO Client instance.  Notice that this needs to take a
// WiFiClient object as the first parameter, and as the second parameter a
// default Adafruit IO key to use when accessing feeds (however each feed can
// override this default key value if required, see further below).
Adafruit_IO_Client aio = Adafruit_IO_Client(client, AIO_KEY);

// Finally create instances of Adafruit_IO_Feed objects, one per feed.  Do this
// by calling the getFeed function on the Adafruit_IO_FONA object and passing
// it at least the name of the feed, and optionally a specific AIO key to use
// when accessing the feed (the default is to use the key set on the
// Adafruit_IO_Client class).
Adafruit_IO_Feed testFeed = aio.getFeed("esptestfeed");
Adafruit_IO_Feed TempFeed = aio.getFeed("officetemp");
Adafruit_IO_Feed HumidityFeed = aio.getFeed("officehumidity");


// Alternatively to access a feed with a specific key:
//Adafruit_IO_Feed testFeed = aio.getFeed("esptestfeed", "...esptestfeed key...");

// Global state to increment a number and send it to the feed.
unsigned int count = 0;

void setup() {
  // Setup serial port access.
  Serial.begin(115200);
  delay(10);
  Serial.println(); Serial.println();
  Serial.println(F("Adafruit IO ESP8266 test!"));

  // Connect to WiFi access point.
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");  
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
  
  // Initialize the Adafruit IO client class (not strictly necessary with the
  // client class, but good practice).
  aio.begin();

  Serial.println(F("Ready!"));
}

void loop() {
  int humidity_data = (int)dht.readHumidity();
  int temperature_data = (int)dht.readTemperature(true);
  // Increment the count value and write it to the feed.
  count += 1;
  // To write a value just call the feed's send function and pass it the value.
  // Send will create the feed on Adafruit IO if it doesn't already exist and
  // then add the value to it.  Send returns a boolean that's true if it works
  // and false if it fails for some reason.
  if (testFeed.send(count)) {
    Serial.print(F("Wrote value to feed: ")); Serial.println(count, DEC);
  }
  else {
    Serial.println(F("Error writing value to feed!"));
  }
  
  if (TempFeed.send(temperature_data)){
    Serial.print(F("Wrote temperature value to feed: ")); Serial.println(temperature_data, DEC); 
  }
  else {
    Serial.println(F("Error writing temperature value to feed!"));
  }

  if (HumidityFeed.send(humidity_data)){
    Serial.print(F("Wrote humidity value to feed: ")); Serial.println(humidity_data, DEC); 
  }
  else {
    Serial.println(F("Error writing humidity value to feed!"));
  }

  Serial.println(F("Waiting 5 minutes and then writing a new feed value."));
  delay(300000);
}
