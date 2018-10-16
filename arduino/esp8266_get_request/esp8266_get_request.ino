#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "VM4276606";
const char* password = "H3ll0nurse!#";

const char* host = "192.168.4.1";

char PostData[] = "{\"name\": \"Fred\", \"age\": 31}";


void setup()
{
  Serial.begin(115200);
  Serial.println();

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
  http.begin("http://192.168.0.48/temp");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  http.POST(PostData);
  http.writeToStream(&Serial);
  http.end();
  
 
  
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
