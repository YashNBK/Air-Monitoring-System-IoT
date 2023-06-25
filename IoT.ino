#include <DHT.h>  // Including library for dht
#include <ESP8266WiFi.h>
 
String apiKey = "IO97JJVC20V7DTE2";     //  Enter your Write API key from ThingSpeak
 
const char *ssid =  "Yash";     // replace with your wifi ssid and wpa2 key
const char *pass =  "katkamwar123";
const char* server = "api.thingspeak.com";
 
#define DHTPIN D3  
DHT dht(DHTPIN, DHT11);

        //pin where the dht11 is connected
 
WiFiClient client;
 
void setup()
{
       Serial.begin(115200);
       delay(10);
       dht.begin();
 
       Serial.println("Connecting to ");
       Serial.println(ssid);
 
 
       WiFi.begin(ssid, pass);
 
      while (WiFi.status() != WL_CONNECTED)
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");
 
}
 
void loop()
{
 
      float h = dht.readHumidity();
      float t = dht.readTemperature();
      float g = analogRead(A0);
     
              if (isnan(h) || isnan(t) || isnan(g))
                 {
                     Serial.println("Failed to read from DHT sensor!");
                      return;
                 }
 
                         if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                      {  
                           
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(t);
                             postStr +="&field2=";
                             postStr += String(h);
                             postStr +="&field3=";
                             postStr += String(g);
                             postStr += "\r\n\r\n";
 
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);
         
 
                             Serial.print(" Temperature: ");
                             Serial.print(t);
                             Serial.print("(upto 40 degree-safe & above-danger)");
                             Serial.print(" Gas Level: ");
                             Serial.print(g/1023*100);
                             Serial.print(" (Above 400-high & Below 400-moderate) ");
                         
                             Serial.print(" Humidity: ");
                             Serial.print(h);
                             Serial.print("(upto 70%-safe & above 70%-danger)");
                           
                             Serial.println("%. Send to Thingspeak.");
                        }
          client.stop();
 
          Serial.println("Waiting...");
 
  // thingspeak needs minimum 15 sec delay between updates
  delay(1000);
}
