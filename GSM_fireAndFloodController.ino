#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 2);

int inputToFireDetector = 13;
int inputToFloodDetector = 12;

void setup() {
  Serial.begin(9600);

  pinMode(inputToFireDetector, INPUT);
  pinMode(inputToFloodDetector, INPUT);
}

// For data transmission from Serial to Software Serial port & vice versa
void updateSerial() {
  delay(500);
  while (Serial.available()) {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }

  while (mySerial.available()) {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}

void sendSMS(String msg) {
  mySerial.begin(9600);
  Serial.println("Send SMS Initializing...");
  delay(1000);

  // Send attention command to check if all fine, it returns OK
  mySerial.println("AT");
  updateSerial();

  // Configuring module in TEXT mode
  mySerial.println("AT+CMGF=1");
  updateSerial();

  // to send message use these 3 statements, upto write(26)
  // change ZZ with country code and xxxxxxxxxxx with phone number to sms
  //mySerial.println("AT+CMGS=\"+639626483959\""); // 1)
  mySerial.println("AT+CMGS=\"+639985333809\""); // 1)
  updateSerial();
  mySerial.print(msg); // 2) text content 
  updateSerial();
  mySerial.write(26); // 3)
}

void loop() {
  mySerial.begin(9600);
  Serial.println("Initializing...");
  delay(1000);

  if (digitalRead(inputToFireDetector) == HIGH) {
    Serial.println("Looping FIRE");
    sendSMS("Fire Alert");
    delay(3000);
  } else if (digitalRead(inputToFloodDetector) == HIGH) {
     Serial.println("Looping FLOOD");
    sendSMS("Flood Alert");
    delay(3000);
  }
}