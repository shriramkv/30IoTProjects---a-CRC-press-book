#include <ESP8266WiFi.h>

#define LED D1

String ssid = "YOUR WIFI NAME";
String password = "YOUR WIFI PASSWORD";

String LEDstate; // State variable for LED to track its state

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  digitalWrite(LED,LOW);
  LEDstate = "OFF";
  
  Serial.println("");
  Serial.println("");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  server.begin();
  Serial.println("Server started");
  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  Serial.println("new client");
  while(!client.available()) {
    delay(1);
  }
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  // Depending on the present state of LED, toggle its state
  if (req.indexOf("/ledtoggle") != -1) {   
    if(LEDstate == "ON") {
      digitalWrite(LED,LOW);
      LEDstate = "OFF";
      Serial.println("LED OFF\n");
    } else {
      digitalWrite(LED,HIGH);
      LEDstate = "ON";
      Serial.println("LED ON\n");
    }
  }
    
  String web;
  web += "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
  web += "<html>\r\n";

  web += "<head>\r\n";
  web += "<meta charset=\"UTF-8\"/>\r\n";
  web += "<title> LED Control </title>\r\n";
  web += "</head>\r\n";
  
  web += "<body>\r\n";
  web += "<h1>Controlling LED using Webpage & Single Button</h1>\r\n";

  web += "<div>\r\n";
  // Use a single variable i.e, /ledtoggle instead of two
  web += "<a href=\"/ledtoggle\"> <button> Toggle LED state </button> </a>\r\n";
  web += "</div>\r\n";

  web += "<br/>\r\n";
  
  web += "<p>\r\n";
  // You can concatenate the state of the LED with normal text
  web += "LED State: "+ LEDstate +"\r\n";
  web += "</p>\r\n";
  
  web += "</body>\r\n";
  web += "</html>\r\n";
  
  client.print(web);
}
