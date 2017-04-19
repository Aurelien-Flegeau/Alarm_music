// Uses a PIR sensor to detect movement, buzzes a buzzer
// more info here: http://blog.makezine.com/projects/pir-sensor-arduino-alarm/
// email me, John Park, at jp@jpixl.net
// based upon:
// PIR sensor tester by Limor Fried of Adafruit
// tone code by michael@thegrebs.com

//the time we give the sensor to calibrate (10-60 secs according to the datasheet)
int calibrationTime = 30;  
 
int ledPin = LED_BUILTIN;                // choose the pin for the LED
int ledPin2 = D13;                // choose the pin for the LED
int inputPin = D2;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status

void setup() {
  pinMode(ledPin, OUTPUT);      // declare LED as output
  pinMode(ledPin2, OUTPUT);      // declare LED as output
  pinMode(inputPin, INPUT);     // declare sensor as input
  Serial.begin(9600);
  
  Serial.print("calibrating sensor ");
  for(int i = 0; i < calibrationTime; i++){
    Serial.print(i);
    Serial.println("s");
    delay(1000);
  }
}

void loop(){
  val = digitalRead(inputPin);  // read input value
  if(val == HIGH) {
    Serial.println(val);
    digitalWrite(ledPin, LOW);
    digitalWrite(ledPin2, HIGH);
    delay(3000);
  } else {
    digitalWrite(ledPin, HIGH);
    digitalWrite(ledPin2, LOW);
  }
}


