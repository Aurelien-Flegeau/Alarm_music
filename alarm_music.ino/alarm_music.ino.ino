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

String value = "No move detected.";
String metaRefresh = "<meta http-equiv='refresh' content='0.1'>";

/*****WebPage*****/
// Warning: only use simple quotes in the html (no double)
String rootHTML = "\
<!doctype html> \
<html> \
<head>" + metaRefresh + " \
<title> IoT into </title> \
</head> \
<body>\
<br> "+ value+"\
</body> </html>\
";
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
}

/****Loop****/
void loop() {
  server.handleClient();
  val = digitalRead(inputPin);  // read input value
  if(val == HIGH) {
    metaRefresh = ""; // <meta http-equiv='refresh' content='10'>";
    value = "Move detected.";
    Serial.println(val);
    digitalWrite(ledPin, LOW);
    digitalWrite(ledPin2, HIGH);
    /*String answer = getHTML();
    answer.replace("No", "light on");
    server.send(200, "text/html", answer);*/
    // delay(10000);
  } else {
    metaRefresh = "<meta http-equiv='refresh' content='0.1'>";
    value = "No move detected.";
    digitalWrite(ledPin, HIGH);
    digitalWrite(ledPin2, LOW);
    /*String answer = getHTML();
    answer.replace("No", "light off");
    server.send(200, "text/html", answer);*/
  }
}


