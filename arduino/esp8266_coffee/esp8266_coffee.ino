#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "VM6709628";
const char* password = "w2jqHmxfswzg";
const char* host = "192.168.0.49";

int rainPin = A0;
int greenLED = 13;
int redLED = 15;
// you can adjust the threshold value
int thresholdValue = 800;

void setup()
{
  Serial.begin(115200);
  Serial.println();

  pinMode(rainPin, INPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  digitalWrite(greenLED, LOW);
  digitalWrite(redLED, LOW);
  Serial.begin(9600);

  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");
}


void loop()
{
  WiFiClient client;
  HTTPClient http;

  String plant = "coffee";
  int moisture = analogRead(rainPin);
  int temp = random(1,5);
  String PostData = "{\"plant\": \"" + plant + "\", \"moisture\": \"" + moisture + "\", \"temp\": \"" + temp + "\" }";

  Serial.print(moisture);
  if (moisture < thresholdValue) {
    Serial.println(" - Doesn't need watering");
    digitalWrite(redLED, LOW);
    digitalWrite(greenLED, HIGH);
  }
  else {
    Serial.println(" - Time to water your plant");
    digitalWrite(redLED, HIGH);
    digitalWrite(greenLED, LOW);
  }
  delay(500);
  
  http.begin("http://192.168.0.49/temp");
  http.addHeader("Content-Type", "application/json");
  http.POST(PostData);
  http.writeToStream(&Serial);
  http.end();
  delay(100000);
 
  long interval = 2000;
  unsigned long currentMillis = millis(), previousMillis = millis();
  
  while(!client.available()){
  
    if( (currentMillis - previousMillis) > interval ){
  
      Serial.println("Timeout");
      client.stop();     
      return;
    }
    currentMillis = millis();
  }
  
  while (client.connected())
  {
    if ( client.available() )
    {
      char str=client.read();
     Serial.println(str);
    }      
  }
}
