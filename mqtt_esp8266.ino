
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>
#include <ArduinoOTA.h>
#include <SimpleDHT.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#include <Hash.h>
#define USE_SERIAL Serial




// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      60
#define PIN            12


int pinDHT11 = 14;
SimpleDHT11 dht11;



Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
// Update these with values suitable for your network.

const char* ssid = "RMR";
const char* password = "rahulmr123";
const char* mqtt_server = "192.168.1.99";

WiFiClient wclient;
PubSubClient client(wclient, mqtt_server);

long lastMsg = 0;
char msg[50];
int value = 0;
#define BUFFER_SIZE 100

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  pixels.begin();
  setup_wifi();
  client.set_callback(callback);
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(const MQTT::Publish& pub) {
  Serial.print("Message arrived [");
  Serial.print(pub.topic());
  Serial.print("] ");

  //  for (int i = 0; i < length; i++) {
  //    Serial.print((char)payload[i]);
  //  }

  Serial.println(pub.payload_string());

  extractLEDDetails(pub.payload_string());

  Serial.println();

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {

    Serial.print("Attempting MQTT connection...");
    // Attempt to connect

    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("esp8266", "hello world");
      // ... and resubscribe
      client.subscribe("brightness");
      client.subscribe("saturation");
      client.subscribe("hue");
      client.subscribe("LIGHTON");
      client.subscribe("LIGHTOFF");
    } else {
      Serial.print("failed, rc=");
      //Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5004);
    }
  }
}


unsigned long interval;
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  ArduinoOTA.handle();
  //600000
  if (millis() - interval > 600000) {
    interval = millis();
    currTempHumidity();
  }
}
