#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

// WiFi Parameters
const char* ssid = "VM6709628";
const char* password = "w2jqHmxfswzg";

// API details
String apiKey = "9adcac14e6c0f2826cd845a717e42d9e";
String cityName = "Reading";
String countryCode = "uk";

String apiQuery= "http://api.openweathermap.org/data/2.5/weather?q=Reading,uk&appid=9adcac14e6c0f2826cd845a717e42d9e";

// Pins
int lightRelay = 2;
int pumpRelay = 0;
int payload = 0;
int greenLED = 13;
int redLED = 15;

void get_json_object(query) {
  http.begin(query);
          const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
          DynamicJsonBuffer jsonBuffer(bufferSize);
          JsonObject& schedule = jsonBuffer.parseObject(http.getString());
}

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");

  pinMode(lightRelay, OUTPUT);
  pinMode(pumpRelay, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  digitalWrite(greenLED, LOW);
  digitalWrite(redLED, LOW);
  }
}

void loop() {

   // Check WiFi Status
   if (WiFi.status() == WL_CONNECTED) {

     // Scheduler
     Serial.println("connected");
     HTTPClient http;
     Serial.println("initialised HTTP client");
     String get_daylight_check = "http://192.168.0.49/vaxer_schedule/";
     http.begin(get_daylight_check);
     int httpCode = http.GET();        
                                                                  
        if (httpCode > 0) {
          // Parsing
          
          http.begin(get_daylight_check);
          const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
          DynamicJsonBuffer jsonBuffer(bufferSize);
          JsonObject& schedule = jsonBuffer.parseObject(http.getString());
          // Parameters
          int daylight_toggle = schedule["daylight"];
          int pump_toggle = schedule["pump"];
          Serial.println("daylight:");
          Serial.println(daylight_toggle);
          Serial.println("pump:");
          Serial.println(pump_toggle);
          Serial.print("\n");

          // Weather Block
          Serial.println("beginning weather block\n");
          if (daylight_toggle == 1) {
            http.begin(apiQuery);
            int httpCode = http.GET();   

            // Light                                                          
            if (httpCode > 0) {
                // Parsing
                const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
                DynamicJsonBuffer jsonBuffer(bufferSize);
                JsonObject& daylight = jsonBuffer.parseObject(http.getString());
                // Parameters
                int datetime = daylight["dt"];
                int sunrise = daylight["sys"]["sunrise"];
                int sunset = daylight["sys"]["sunset"];
                 
                if (sunrise <= datetime && datetime <= sunset) {
                  digitalWrite(lightRelay,HIGH);
                  Serial.print("daylight\n");
                }
               else if (datetime < sunrise || datetime > sunset) {
                  digitalWrite(lightRelay,LOW);
                  Serial.print("nighttime\n");
                }
                else {
                  Serial.println("no data\n");
                }

            // Pump
            if (pump_toggle == 1) {
              digitalWrite(pumpRelay, HIGH);
              Serial.print("pumping\n");
              delay(60000);
              digitalWrite(pumpRelay, LOW);
              Serial.print("end pumping\n");
              
            }
            http.end();
            }
          }   
        } 
   Serial.print("end cycle");
   delay(120000);
   }
}
