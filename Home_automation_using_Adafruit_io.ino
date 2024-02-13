
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
//Relays for switching appliances
#define Relay1            D1
#define Relay2            D2
#define Relay3            D3
#define Relay4            D4
#define Relay5            D5


#define WLAN_SSID       "theinfoflux"
#define WLAN_PASS       "12345678"

// Adafruit.io Setup 

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   
#define AIO_USERNAME    "esp8266monitoring"
#define AIO_KEY         "aio_KbMf91sP6V2O0IpQ7iw8XMzdyC11"


WiFiClient client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);



// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe LED1 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/relay1");
Adafruit_MQTT_Subscribe LED2 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/relay2");
Adafruit_MQTT_Subscribe LED3 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/relay3");
Adafruit_MQTT_Subscribe LED4 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/relay4");
Adafruit_MQTT_Subscribe LED5 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/relay5");



uint32_t delayMS;


void MQTT_connect();

void setup() {
  Serial.begin(115200);
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);
  pinMode(Relay5, OUTPUT);
  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
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
 

  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&LED1);
  mqtt.subscribe(&LED2);
  mqtt.subscribe(&LED3);
  mqtt.subscribe(&LED4);
  mqtt.subscribe(&LED5);
}

uint32_t x = 0;

void loop() {
  MQTT_connect();
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(20000))) {
    if (subscription == &LED1) {
      Serial.print(F("Got: "));
      Serial.println((char *)LED1.lastread);
      int LED1_State = atoi((char *)LED1.lastread);
      digitalWrite(Relay1, LED1_State); 
    }
    if (subscription == &LED3) {
      Serial.print(F("Got: "));
      Serial.println((char *)LED3.lastread);
      int LED3_State = atoi((char *)LED3.lastread);
      digitalWrite(Relay2, LED3_State);
    }
    if (subscription == &LED2) {
      Serial.print(F("Got: "));
      Serial.println((char *)LED2.lastread);
      int LED2_State = atoi((char *)LED2.lastread);
      digitalWrite(Relay3, LED2_State);
    }
    if (subscription == &LED4) {
      Serial.print(F("Got: "));
      Serial.println((char *)LED4.lastread);
      int LED4_State = atoi((char *)LED4.lastread);
      digitalWrite(Relay4, LED4_State);
      
    }
       if (subscription == &LED5) {
      Serial.print(F("Got: "));
      Serial.println((char *)LED5.lastread);
      int LED5_State = atoi((char *)LED5.lastread);
      digitalWrite(Relay5, LED5_State);
      
    }
  }

  
}


void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
 
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }
  Serial.println("MQTT Connected!");

}
