/****Includes****/
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <WiFiClient.h>
#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the &onfiguration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <ESP8266mDNS.h>          //Allow custom URL
#include "FS.h"

/*****Initialization*****/
ESP8266WebServer server(80);
const char *ssid = "iot_capteur";

/*****WebPage*****/
// Warning: only use simple quotes in the html (no double)

String linkP1 = "<iframe width='560' height='315' src='";
String linkP2 = "?autoplay=1' frameborder='0' allowfullscreen></iframe>";
String linkAudioP1 = "<audio controls autoplay> <source src=";
String linkAudioP2 = " type='audio/mpeg'> Your browser does not support the audio element.</audio>";

String rootHTMLP1 = "\
<!doctype html> \n\
<html>\n\
   <head>\n\
      <script type='text/javascript' src='https://code.jquery.com/jquery-3.2.1.min.js'></script>\n\
      <script type='text/javascript'>\n\
            function listen(time = 1000) { \n\
              setTimeout(function(){ \n\
              var xhr = new XMLHttpRequest();\n\
              xhr.open('GET', 'http://' + window.location.hostname + '/msg?msg=' + 'nothing' , false);\n\
              xhr.send( null );\n\
              console.log('request sent');\n\
              if(xhr.readyState == 4 && xhr.status == 200){\n\
                console.log('listening');\n\
                if(xhr.responseText !== 'nothing') {\n\
                  $('#link').html(`";

String rootHTMLP2 = "`);\n\
                  listen(5000); \n\
                } else {\n\
                  listen(1000); \n\
                }\n\
              } else {\n\
                listen(1000);\n\
              }\n\
             }, time); \n\
            }\n\
      </script>\n\
   </head>\n\
   <body onLoad='listen(1000)'>\n\
      <span id='link'></span> \n\
   </body>\n\
</html>";

String interfaceHTML = "\
<html>\n\
   <head>\n\
      <script type='text/javascript'>\n\
        function sayHello() {\n\
          var xhr = new XMLHttpRequest();\n\
          var message = document.getElementById('msg').value\n\
          xhr.open('GET', 'http://' + window.location.hostname + '/setAudioUrl?url=' + message , false);\n\
          xhr.send( null );\n\
          console.log('request sent');\n\
          if(xhr.readyState == 4 && xhr.status == 200){\n\
            console.log('response received');\n\
            console.log(xhr.responseText);\n\
            alert('link changed');\n\
          }\n\
        }\n\
      </script>\n\
   </head>\n\
   <body>\n\
    new link : <input type='text' id='msg'/>\n\
    <input type='button' onclick='sayHello()' value='Go' />\n\
   </body>\n\
</html>";

String adminHTML = "\
<html>\n\
   <head>\n\
      <script type='text/javascript'>\n\
        function setUrl() {\n\
          var xhr = new XMLHttpRequest();\n\
          var url = document.getElementById('url').value\n\
          var type = document.getElementById('type').value\n\
          xhr.open('GET', 'http://' + window.location.hostname + '/setUrl?url=' + url + '&type=' + type  , false);\n\
          xhr.send( null );\n\
          console.log('request sent');\n\
          if(xhr.readyState == 4 && xhr.status == 200){\n\
            console.log('response received');\n\
            console.log(xhr.responseText);\n\
            alert('url changed');\n\
          }\n\
        }\n\
      </script>\n\
   </head>\n\
   <body>\n\
    new url : <input type='text' id='url'/>\n\
    new type : <select id='type'><option value='youtube'>youtube embed link</option><option value='other' selected>other direct link</option></select>\n\
    <input type='button' onclick='setUrl()' value='Go' />\n\
   </body>\n\
</html>";



String interfaceTypeHTML = "\
<html>\n\
   <head>\n\
      <script type='text/javascript'>\n\
        function sayHello() {\n\
          var xhr = new XMLHttpRequest();\n\
          var message = document.getElementById('msg').value\n\
          xhr.open('GET', 'http://' + window.location.hostname + '/setAudioTypeUrl?url=' + message , false);\n\
          xhr.send( null );\n\
          console.log('request sent');\n\
          if(xhr.readyState == 4 && xhr.status == 200){\n\
            console.log('response received');\n\
            console.log(xhr.responseText);\n\
            alert('link changed');\n\
          }\n\
        }\n\
      </script>\n\
   </head>\n\
   <body>\n\
    type link : <input type='text' id='msg'/>\n\
    <input type='button' onclick='sayHello()' value='Go' />\n\
   </body>\n\
</html>";

int calibrationTime = 30;

int ledPin = LED_BUILTIN;                // choose the pin for the LED
int ledPin2 = D13;                // choose the pin for the LED
int inputPin = D2;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status

String getHTML() {
    String toLink = "https://www.youtube.com/embed/0g1CaKuVEhc";
    File f = SPIFFS.open("/audiourl.txt", "r");
    if (!f) {
      Serial.println("file open failed");
    } else {
      String s = f.readStringUntil('\n');
      if(s[0] != '\0') {
        toLink = s;
      }
    }

    char type = 'y';
    File f2 = SPIFFS.open("/audiotypeurl.txt", "r");
    if (!f2) {
      Serial.println("file open failed");
    } else {
      char s2 = f2.readStringUntil('\n')[0];
      Serial.println("-------------------------------------------");
      Serial.println(s2);
      if(s2 != '\0') {
        type = s2;
      }
    }

    String updatedRootHTML = rootHTMLP1 + linkAudioP1 + toLink + linkAudioP2 + rootHTMLP2;

    if( type == 'y') {
      updatedRootHTML = rootHTMLP1 + linkP1 + toLink + linkP2 + rootHTMLP2;
      Serial.println("-------------------------------------------");
      Serial.println("-------------------------------------------");
    }
    return updatedRootHTML;
}

void handleRoot() {
    server.send(200, "text/html", getHTML());
}

void handleAdmin() {
    server.send(200, "text/html", adminHTML );
}

void handleInterfaceRoot() {
    server.send(200, "text/html", interfaceHTML);
}

void handleInterfaceTypeRoot() {
    server.send(200, "text/html", interfaceTypeHTML);
}

/****Setups****/
void setupWifi() {
    //WiFiManager
    WiFiManager wifiManager;

    //reset saved settings -- Flush flash
    //wifiManager.resetSettings();

    //fetches ssid and pass from eeprom and tries to connect
    //if it does not connect it starts an access point with the specified name
    //and goes into a blocking loop awaiting configuration
    wifiManager.autoConnect(ssid);

    // might seem redundant but it's not printed the 1st time:
    Serial.println("local ip");
    Serial.println(WiFi.localIP());
}

void setupServer() {
    server.on("/", handleRoot);
    server.on("/admin", handleAdmin);
    server.on("/set", handleInterfaceRoot);
    server.on("/setType", handleInterfaceTypeRoot);

    server.on("/setUrl", [](){
      String url = server.arg("url");
      String type = server.arg("type");
      //set url in a file
      File f = SPIFFS.open("/audiourl.txt", "w");
      if (!f) {
        Serial.println("file open failed");
      }
      f.println( url );
      f.close();

      //set type in a file
      f = SPIFFS.open("/audiotypeurl.txt", "w");
      if (!f) {
        Serial.println("file open failed");
      }
      f.println( url );
      f.close();

      server.send(200, "text/html", "done");
    });

    server.on("/setAudioUrl", [](){
      String myLink = server.arg("url");
      File f = SPIFFS.open("/audiourl.txt", "w");
      if (!f) {
        Serial.println("file open failed");
      }
      f.println( myLink );
      f.close();
    });



    server.on("/setAudioTypeUrl", [](){
      String myLink = server.arg("url");
      File f = SPIFFS.open("/audiotypeurl.txt", "w");
      if (!f) {
        Serial.println("file open failed");
      }
      f.println( myLink );
      f.close();
    });
    server.begin();
    Serial.println("HTTP server started");
}

void setupMDNS() {
    // Add service to MDNS-SD to access the ESP with the URL http://<ssid>.local
    if (MDNS.begin(ssid)) {
        Serial.print("MDNS responder started as http://");
        Serial.print(ssid);
        Serial.println(".local");
    }
    MDNS.addService("http", "tcp", 8080);
}

void setup() {
  Serial.begin(115200);
  SPIFFS.begin();
  pinMode(ledPin, OUTPUT);      // declare LED as output
  pinMode(ledPin2, OUTPUT);      // declare LED as output
  pinMode(inputPin, INPUT);     // declare sensor as input

  Serial.print("calibrating sensor ");
  for(int i = 0; i < calibrationTime; i++){
    Serial.print(i);
    Serial.println("s");
    delay(1000);
  }


    Serial.println("Starting WiFi.");
    setupWifi();
    setupServer();
    setupMDNS();
  server.begin();
}

/****Loop****/
void loop() {
  server.handleClient();
  val = digitalRead(inputPin);  // read input value
  if(val == HIGH) {
    Serial.println(val);
    digitalWrite(ledPin, LOW);
    digitalWrite(ledPin2, HIGH);
    String message = "capteur on";
    server.send(200, "text/html", " Received : "+ message);
  } else {
    digitalWrite(ledPin, HIGH);
    digitalWrite(ledPin2, LOW);
  }
}
