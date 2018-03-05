#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "iAccelerate";
const char* password = "AcceLer47eIC$";
const char* mqtt_server = "13.210.157.158";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

const int relayPin = D1;
const long interval = 500; 
const int ledPin = BUILTIN_LED;

void setup() {
        Serial.begin(115200);
        setup_wifi();
        client.setServer(mqtt_server, 1883);
        pinMode(relayPin, OUTPUT);
        pinMode(ledPin, OUTPUT);
        client.setCallback(callback);
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
                String clientId = "ESP8266Client-";
                clientId += String(random(0xffff), HEX);
                if (client.connect(clientId.c_str())) {
                        Serial.println("connected");
                        client.subscribe("/MSIOT101/OUT");
                } else {
                        Serial.print("failed, rc=");
                        Serial.print(client.state());
                        Serial.println(" try again in 5 seconds");
                        delay(5000);
                }
        }
}

void callback(char* topic, byte* payload, unsigned int length) {
        Serial.print("Message arrived [");
        Serial.print(topic);
        Serial.print("] ");
        for (int i = 0; i < length; i++) {
                Serial.print((char)payload[i]);
        }
        Serial.println();        
        if ((char)payload[0] == '1') {        
                digitalWrite(relayPin, HIGH); // turn on relay with voltage HIGH
                digitalWrite(ledPin, LOW); // LED On
                delay(interval);   
        } else {
                digitalWrite(relayPin, LOW);  // turn off relay with voltage LOW
                digitalWrite(ledPin, HIGH); // LED Off
                delay(interval);
        }
}

void loop() {
        if (!client.connected()) {
          reconnect();
        }
        client.loop();
}
