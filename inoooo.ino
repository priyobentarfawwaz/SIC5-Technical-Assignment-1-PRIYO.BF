#include <WiFi.h>
#include <HTTPClient.h>

#define MQ135_PIN 34

const char* ssid = "HUAWEI-2.4G-PRIYO";
const char* password = "12345678";
const char* serverName = "http://192.168.100.24:5000/data";


const float VREF = 5;       
const int ADC_RESOLUTION = 4095;
const float RZERO = 76.63; 
const float PARA = 116.6020682;
const float PARB = 2.769034857;

void setup() {
  Serial.begin(115200);
  pinMode(MQ135_PIN, INPUT);
  WiFi.begin(ssid, password);
  
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
}

void loop() {
  int sensorValue = analogRead(MQ135_PIN);
  float voltage = sensorValue * (VREF / ADC_RESOLUTION);

  float sensorResistance = ((VREF * 10.0) / voltage) - 10.0;

  
  float ratio = sensorResistance / RZERO;
  float ppm = PARA * pow(ratio, -PARB);

 
  Serial.print("PPM: ");
  Serial.println(ppm);

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.setTimeout(5000);
    
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");
    
    String postData = "{\"ppm\":" + String(ppm) + "}";
    
    int httpResponseCode = http.POST(postData);
    
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("HTTP Response code: " + String(httpResponseCode));
      Serial.println("Response: " + response);
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
      
      if (httpResponseCode == 405) {
        Serial.println("Method Not Allowed - Check if the server allows POST requests on this endpoint.");
      } else if (httpResponseCode == 404) {
        Serial.println("Not Found - Check the URL and endpoint on the server.");
      }
    }
    
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
  
  delay(6000); 
}
