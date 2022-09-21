#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

TinyGPSPlus gps;
SoftwareSerial gpsSerial(D5, D6);
String ssid = "YOUR WIFI NAME";
String password = "YOUR WIFI PASSWORD";
int year , month , date, hour , minute , second;
String date_str , time_str , lat_str , lng_str, loc_link;
String web;
int isPM;

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  gpsSerial.begin(9600);
  Serial.println();
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
  while (gpsSerial.available() > 0)
    if (gps.encode(gpsSerial.read())) {
      if (gps.location.isValid()) {
        lat_str = String(gps.location.lat() , 6);
        lng_str = String(gps.location.lng() , 6);
        loc_link = "https://www.google.com/maps/search/?api=1&query="+lat_str+","+lng_str;
      }
      if (gps.date.isValid()) {
        date_str = "";
        date = gps.date.day();
        month = gps.date.month();
        year = gps.date.year();
        if (date < 10)
          date_str = '0';
        date_str += String(date);
        date_str += " / ";

        if (month < 10)
          date_str += '0';
        date_str += String(month);
        date_str += " / ";
        if (year < 10)
          date_str += '0';
        date_str += String(year);
      }
      if (gps.time.isValid()) {
        time_str = "";
        hour = gps.time.hour();
        minute = gps.time.minute();
        second = gps.time.second();
        minute = (minute + 30);
        if (minute > 59) {
          minute = minute - 60;
          hour = hour + 1;
        }
        hour = (hour + 5) ;
        if (hour > 23)
          hour = hour - 24;
        if (hour >= 12)
          isPM = 1;
        else
          isPM = 0;
        hour = hour % 12;
        if (hour < 10)
          time_str = '0';
        time_str += String(hour);
        time_str += " : ";
        if (minute < 10)
          time_str += '0';
        time_str += String(minute);
        time_str += " : ";
        if (second < 10)
          time_str += '0';
        time_str += String(second);
        if (isPM == 1)
          time_str += " PM ";
        else
          time_str += " AM ";
      }
    }
 
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  web = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n <!DOCTYPE html> <html>";
  web += "<head> <title>Logistics Tracker</title> </head>";
  web += "<body> <h1> LOGISTICS TRACKER </h1>";
  web += "<h3> Location Details </h3>";
  web += "<table> <tr> <th>Latitude:</th>";
  web += "<td>";
  web += lat_str;
  web += "</td> </tr> <tr> <th>Longitude:</th> <td>";
  web += lng_str;
  web += "</td> </tr> <tr> <th>Maps Link:</th> <td>";
  web += "<a href=\""+loc_link+"\">Google Maps Link</a>";
  web += "</td> </tr> <tr> <th>Date:</th> <td>";
  web += date_str;
  web += "</td></tr> <tr> <th>Time:</th> <td>";
  web += time_str;
  web += "</td>  </tr> </table> ";
 
  web += "</body> </html>";

  client.print(web);
  delay(100);
}
