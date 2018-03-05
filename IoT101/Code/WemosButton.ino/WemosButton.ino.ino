/*
 Board: WeMos D1 MINI (https://tronixlabs.com.au/arduino/boards/wireless/wemos-d1-mini-arduino-compatible-esp8266-wifi-australia/)
 Shield: https://tronixlabs.com.au/esp8266/wemos-esp8266/wemos-d1-mini-one-button-shield-australia/
 Libraries: 
  ESP8266Wifi (Add via Additional Boards Link): http://www.instructables.com/id/Wemos-ESP8266-Getting-Started-Guide-Wemos-101/
  PubSubClient (Add via Library Manager): https://github.com/knolleary/pubsubclient

 Whacked together by James Rule and Ethan Dunne for the MakerSpaceIoT 101 Workshop
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network
const char* ssid = "";
const char* password = "";
// The MQTT Server you want to use, this is https://test.mosquitto.org/ 
const char* mqtt_server = "test.mosquitto.org"; 

// Variables for the MQTT Library
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

const int buttonPin = D3; // For the WeMos D1 Mini Button Shield
int buttonState = 0;  // Initialize the button state
const int ledPin = BUILTIN_LED; 

void setup() {
        Serial.begin(115200);
        setup_wifi();
        client.setServer(mqtt_server, 1883);        
        pinMode(buttonPin, INPUT);
        pinMode(ledPin, OUTPUT);  // set initial state, LED OFF
        digitalWrite(ledPin, buttonState);  // LED Off
}

void setup_wifi() {
        delay(10);
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
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
}

void reconnect() {
        while (!client.connected()) {
                Serial.print("Attempting MQTT connection...");
                if (client.connect("ESP8266Client")) {
                        Serial.println("connected");
                } else {
                        Serial.print("failed, rc=");
                        Serial.print(client.state());
                        Serial.println(" try again in 5 seconds");
                        delay(5000);
                }
        }
}

void loop() {
        buttonState = digitalRead(buttonPin);
        if (buttonState == LOW) {
                reconnect();
                Serial.println("Triggered!");
                client.publish("/MSIOT101/IN", "B");
                digitalWrite(ledPin, LOW);  // LED ON
                delay(1000);
        } else {
                digitalWrite(ledPin, HIGH); // LED OFF
        }
}
