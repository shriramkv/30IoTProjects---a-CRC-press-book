#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char authToken[] = "YOUR AUTH TOKEN";
char ssid[] = "YOUR WIFI NAME";
char password[] = "YOUR WIFI PASSWORD";

void setup()
{
  Serial.begin(115200);
  Blynk.begin(authToken, ssid, password);
}

void loop()
{
  Blynk.run();
}
