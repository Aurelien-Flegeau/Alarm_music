/****Includes****/
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <WiFiClient.h>
#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the &onfiguration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <ESP8266mDNS.h>          //Allow custom URL

/*****Initialization*****/
ESP8266WebServer server(80);
const char *ssid = "iot_capteur";

/*****WebPage*****/
// Warning: only use simple quotes in the html (no double)

String link = "<iframe width='560' height='315' src='https://www.youtube.com/embed/0g1CaKuVEhc?autoplay=1' frameborder='0' allowfullscreen></iframe>";

String rootHTML = "\
<!doctype html> \n\
<html>\n\
   <head>\n\
      <script type='text/javascript' src='https://code.jquery.com/jquery-3.2.1.min.js'></script>\n\
      <script type='text/javascript'>\n\
            function listen(time = 1000) { \n\
              setTimeout(function(){ \n\
              var xhr = new XMLHttpRequest();\n\
              xhr.open('GET', 'http://10.33.2.35/msg?msg=' + 'nothing' , false);\n\
              xhr.send( null );\n\
              console.log('request sent');\n\
              if(xhr.readyState == 4 && xhr.status == 200){\n\
                console.log('listening');\n\
                if(xhr.responseText !== 'nothing') {\n\
                  $('#link').html(`" + link + "`);\n\
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

int calibrationTime = 30;  
 
int ledPin = LED_BUILTIN;                // choose the pin for the LED
int ledPin2 = D13;                // choose the pin for the LED
int inputPin = D2;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status

String getHTML() {
    String updatedRootHTML = rootHTML;
    String voltage = String(analogRead(A0) * 3. / 1024.);
    updatedRootHTML.replace("xxx", voltage);
    return updatedRootHTML;
}

void handleRoot() {
    server.send(200, "text/html", getHTML());
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
  pinMode(ledPin, OUTPUT);      // declare LED as output
  pinMode(ledPin2, OUTPUT);      // declare LED as output
  pinMode(inputPin, INPUT);     // declare sensor as input
  
  Serial.print("calibrating sensor ");
  for(int i = 0; i < calibrationTime; i++){
    Serial.print(i);
    Serial.println("s");
    delay(1000);
  }
    Serial.begin(115200);

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


