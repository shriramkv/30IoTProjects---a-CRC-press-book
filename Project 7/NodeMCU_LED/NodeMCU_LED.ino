#include <ESP8266WiFi.h>

#define LED D1 //Initialize LED pin

String ssid = "YOUR WIFI NAME"; //Write your WiFi name
String password = "YOUR WIFI PASSWORD"; //Write your WiFi Password

WiFiServer server(80); //Initialize a server at port 80

void setup() {
  Serial.begin(115200); //Initialization of serial communication
  pinMode(LED, OUTPUT); //Define the direction of LED pin
  digitalWrite(LED,LOW); //Initially LED is set to OFF state
  
  //Connecting the NodeMCU to WiFi
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

  //After the WiFi gets connected, start the server
  server.begin();
  Serial.println("Server started");
  Serial.println(WiFi.localIP()); //Print the local IP address of NodeMCU
}

void loop() {
  //Check whether a new client is available
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  Serial.println("new client");
  while(!client.available()) {
    delay(1);
  }

  // If new request is available, read the request from the client
  String req = client.readStringUntil('\r');
  Serial.println(req); //Print the request
  client.flush(); //Waits until all outgoing characters in the buffer are sent
  
  if (req.indexOf("/ledoff") != -1) { //If the request is ledoff   
    digitalWrite(LED,LOW); //Then switch off the LED
    Serial.println("LED OFF\n");
  }
  else if(req.indexOf("/ledon") != -1) { //If the request is ledon
    digitalWrite(LED,HIGH); //Then switch on the LED
    Serial.println("LED ON\n");
  }

  // Attach the HTML code to a String
  String web;
  web += "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
  web += "<html>\r\n";

  web += "<head>\r\n";
  web += "<meta charset=\"UTF-8\"/>\r\n";
  web += "<title> LED Control </title>\r\n";
  web += "</head>\r\n";
  
  web += "<body>\r\n";
  web += "<h1>Controlling LED using Webpage</h1>\r\n";

  web += "<div>\r\n";
  web += "<a href=\"/ledon\"> <button> Switch ON LED </button> </a>\r\n";
  web += "</div>\r\n";

  web += "<br/>\r\n";
  
  web += "<div>\r\n";
  web += "<a href=\"/ledoff\"> <button> Switch OFF LED </button> </a>\r\n";
  web += "</div>\r\n";
  
  web += "</body>\r\n";
  web += "</html>\r\n";

  client.print(web); //Show the webpage to the client
}
