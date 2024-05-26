#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>

#define moisturePin A0

const char* netID = "Charot";
const char* psKey = "arnel123";

const int sslPort = 443;
int waitTime = 300;

int count = 0;

String URL = "https://xmfjtoxgbgomgshatylt.supabase.co";
String Key = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6InhtZmp0b3hnYmdvbWdzaGF0eWx0Iiwicm9sZSI6ImFub24iLCJpYXQiOjE3MTMxNjI2NDcsImV4cCI6MjAyODczODY0N30.ZZgd67BMlUUd_MX407ZrGZYrqeFRh_4ovhvp-xsfeKg";
String tbl = "MoistureLevel";

HTTPClient https;
WiFiClientSecure client;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(moisturePin, INPUT);
  digitalWrite(LED_BUILTIN, LOW);
  client.setInsecure();
  WiFi.begin(netID, psKey);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  digitalWrite(LED_BUILTIN, HIGH);
}

int prevValue = 0;

void loop() {
  int moistureValue = analogRead(moisturePin); // Read analog value from the moisture sensor
  int mapValue = map(moistureValue, 0, 1023, 100, 1);

  if (WiFi.status() == WL_CONNECTED && prevValue != mapValue) {
    prevValue = mapValue;
    String payload = "{\"Mlevel\":\"" + String(mapValue) + "\"}";
    https.begin(client, URL + "/rest/v1/" + tbl);
    https.addHeader("Content-Type", "application/json");
    https.addHeader("apikey", Key);
    https.addHeader("Authorization", "Bearer " + Key);
    digitalWrite(LED_BUILTIN, LOW);
    int httpCode = https.POST(payload);
    String httpReply = https.getString();

    https.end();
    digitalWrite(LED_BUILTIN, HIGH);
  } 
  delay(waitTime);
}
