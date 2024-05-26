#include <DHT.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>

#define DHTTYPE DHT11
#define DHTPIN D4

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

const char* netID = "Charot";
const char* psKey = "arnel123";

const int sslPort = 443;
int waitTime = 300;

int count = 0;

String URL = "https://xmfjtoxgbgomgshatylt.supabase.co/rest/v1/TempHumidLevel"; // Correct endpoint URL
String Key = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6InhtZmp0b3hnYmdvbWdzaGF0eWx0Iiwicm9sZSI6ImFub24iLCJpYXQiOjE3MTMxNjI2NDcsImV4cCI6MjAyODczODY0N30.ZZgd67BMlUUd_MX407ZrGZYrqeFRh_4ovhvp-xsfeKg";

HTTPClient https;
WiFiClientSecure client;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  client.setInsecure();
  WiFi.begin(netID, psKey);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  digitalWrite(LED_BUILTIN, HIGH);
   
  // Start the serial communication to print out the data
  Serial.begin(115200);
  Serial.println("DHT11 sensor initialization");

  // Initialize the DHT sensor
  dht.begin();
}

void loop() {
  delay(2000);

  // Reading temperature and humidity
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(); // Default is Celsius

  if (WiFi.status() == WL_CONNECTED) {
    String payload = "{\"Tlevel\":" + String(humidity) + ", \"Hlevel\":" + String(temperature) + "}";
    https.begin(client, URL);  // Initialize HTTPS connection
    https.addHeader("Content-Type", "application/json");
    https.addHeader("apikey", Key);
    https.addHeader("Authorization", "Bearer " + Key);
    digitalWrite(LED_BUILTIN, LOW);
    int httpCode = https.POST(payload);
    String httpReply = https.getString();
    Serial.println(httpCode);
    Serial.println(httpReply);
    https.end();
    digitalWrite(LED_BUILTIN, HIGH);
  } 

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" *C");

  delay(3000);
}
