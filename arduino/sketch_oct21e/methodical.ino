#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

// WiFi Parameters
const char* ssid = "VM6709628";
const char* password = "w2jqHmxfswzg";

// API
String apiKey = "9adcac14e6c0f2826cd845a717e42d9e";
String weather_query= "http://api.openweathermap.org/data/2.5/weather?q=Reading,uk&appid=9adcac14e6c0f2826cd845a717e42d9e";
String schedule_query = "http://192.168.0.49/vaxer_schedule/";

// Pins
int lightRelay = 2;
int pumpRelay = 0;
int payload = 0;
int greenLED = 13;
int redLED = 15;

const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
DynamicJsonBuffer jsonBuffer(bufferSize);

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

client open_client(string* query, string* client_name) {
  HTTPClient client_name;
  client_name.begin(query);
  int client_code = client_name.GET();
  Serial.println(client_code);
  JsonObject& json_object = jsonBuffer.parseObject(client_name.getString());
  client_name.end();
  if (client_code > 0 ) {
    return json_object;
  }
  else {
    return 1;
  }

}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    // Weather API Query
    open_client(weather_query, "weather");
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

    // Schedule API Query
    open_client(schedule_query, "schedule");

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
