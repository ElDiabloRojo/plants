#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

// WiFi Parameters
const char* ssid = "VM6709628";
const char* password = "w2jqHmxfswzg";

// API
String apiKey = "9adcac14e6c0f2826cd845a717e42d9e";
String apiQuery= "http://api.openweathermap.org/data/2.5/weather?q=Reading,uk&appid=9adcac14e6c0f2826cd845a717e42d9e";

// Pins
int lightRelay = 2;
int pumpRelay = 0;
int payload = 0;
int greenLED = 13;
int redLED = 15;

const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
int datetime, sunrise, sunset;
String weather;


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

  // Scheduler
  Serial.println("connected...\n");
  HTTPClient http;
  Serial.println("initialised HTTP client");
}

void loop() {
   DynamicJsonBuffer jsonBuffer(bufferSize);

   // Check WiFi Status
   if (WiFi.status() == WL_CONNECTED) {
     HTTPClient weather;
     weather.begin(apiQuery);
     int weatherCode = weather.GET();   
     Serial.println(weatherCode);
                                                      
     if (weatherCode > 0) {
        // Parsing
        Serial.println(weather.getString());
        JsonObject& weather_object = jsonBuffer.parseObject(weather.getString());
        // Parameters
        int datetime = weather_object["dt"];
        int sunrise = weather_object["sys"]["sunrise"];
        int sunset = weather_object["sys"]["sunset"];
        String weather = weather_object["weather"]["main"];
        Serial.print("\nWeather Object");
        Serial.print("\ndatetime: ");
        Serial.println(datetime);
        Serial.print("sunrise: ");
        Serial.println(sunrise);
        Serial.print("sunset: ");
        Serial.println(sunset);
        Serial.print("weather: ");
        Serial.println(weather); 
        Serial.print("\n");
     }  
     weather.end();

         
     String schedule_check = "http://192.168.0.49/vaxer_schedule/"; 
     HTTPClient schedule;
     schedule.begin(schedule_check);
     int scheduleCode = schedule.GET();
     Serial.println(scheduleCode);

     if (scheduleCode > 0) {
     Serial.println("initialised HTTP client");
                                                              
       Serial.println(schedule.getString());
       JsonObject& schedule_object = jsonBuffer.parseObject(schedule.getString());
       // Parameters
       int daylight_toggle = schedule_object["daylight"];
       int pump_toggle = schedule_object["pump"];
     
       // Light
       Serial.println("checking daylight schedule");
       Serial.println("daylight:");
       Serial.println(daylight_toggle);
       if (daylight_toggle == 1) {
       Serial.println("beginning light sequence\n");
         
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
     }
     schedule.end();
     // Pump
     Serial.println("pump:");
     Serial.println(pump_toggle);
     Serial.println("checking pump schedule");
     if (pump_toggle == 1) {
       Serial.println("beginning pump sequence\n");
       digitalWrite(pumpRelay, HIGH);
       Serial.print("pumping\n");
       delay(60000);
       digitalWrite(pumpRelay, LOW);
       Serial.print("end pumping\n"); 
     } 
     }
  Serial.print("end cycle...\n");
  delay(120000);
}
