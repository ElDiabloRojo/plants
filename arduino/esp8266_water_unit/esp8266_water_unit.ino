#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "VM6709628";
const char* password = "w2jqHmxfswzg";
const char* host = "192.168.0.49";

int relayInput = 2;
int payload = 0;
int greenLED = 13;
int redLED = 15;

void setup()
{
  Serial.begin(115200);

  pinMode(relayInput, OUTPUT);

  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  digitalWrite(greenLED, LOW);
  digitalWrite(redLED, LOW);
  
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

  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status

    HTTPClient http;  //Declare an object of class HTTPClient

    http.begin("http://192.168.0.49/feedme/");
    int httpCode = http.GET();                                                                  //Send the request
    payload = http.getString().toInt();
    
    if (httpCode > 0) { 
        if (payload > 800) {
          digitalWrite(relayInput,HIGH);
          delay(10000);
          digitalWrite(relayInput,LOW);
          Serial.println('Watering:');
          Serial.println(payload);
          digitalWrite(redLED, HIGH);
          digitalWrite(greenLED, LOW);
        }
        else {
          Serial.println('No need to water, current moisture:');
          Serial.println(payload);
          digitalWrite(greenLED, HIGH);
          digitalWrite(redLED, LOW);
        }
    }

    http.end();   
    
  }

  delay(30000);

}
