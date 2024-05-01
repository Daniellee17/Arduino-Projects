#include <Servo.h>

int inputPinToDispense = 13;
int outputPinToBuzzer = 12;
int outputPintToServo = 11;
int outputPintToTriggerPin = 10;  // ultrasonic sensor pin
int inputPintToEchoPin = 9;      // ultrasonic sensor pin
int outputPinToEsp32 = 8;
int outputPinPillTaken = 7;

Servo myservo;
int pos = 0;  // variable to store the servo position
int wasPillTaken = 0;
float duration, distance;  // variables for ultrasonic sensor
float starttime, endtime;

void setup() {
  Serial.begin(9600);

  myservo.attach(outputPintToServo);
  pinMode(outputPinToEsp32, OUTPUT);
  pinMode(outputPinPillTaken, OUTPUT);
  pinMode(inputPinToDispense, INPUT);
  pinMode(inputPintToEchoPin, INPUT);
  pinMode(outputPintToTriggerPin, OUTPUT);
  pinMode(outputPinToBuzzer, OUTPUT);
  digitalWrite(outputPinToBuzzer, LOW);
  digitalWrite(outputPinToEsp32, LOW);
  digitalWrite(outputPinPillTaken, LOW);
}

void dispensePill() {
  Serial.println("Dispense Pill");
  moveServo();
  delay(3000);
}

void moveServo() {
  Serial.println("Move Servo");
  for (pos = 0; pos <= 90; pos += 1) {  // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);  // tell servo to go to position in variable 'pos'
    delay(15);           // waits 15ms for the servo to reach the position
  }
  for (pos = 90; pos >= 0; pos -= 1) {  // goes from 180 degrees to 0 degrees
    myservo.write(pos);                  // tell servo to go to position in variable 'pos'
    delay(15);                           // waits 15ms for the servo to reach the position
  }
}

void setBuzzer() {
  Serial.println("On Buzzer");
  digitalWrite(outputPinToBuzzer, HIGH);
  runSensor();
  Serial.println("Off Buzzer");
  digitalWrite(outputPinToBuzzer, LOW);
}

void runSensor() {
  Serial.println("Running Sensor");
  starttime = millis();
  endtime = starttime;
  while ((endtime - starttime) <= 5500)
  {
    digitalWrite(outputPintToTriggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(outputPintToTriggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(outputPintToTriggerPin, LOW);

    duration = pulseIn(inputPintToEchoPin, HIGH);
    distance = (duration * .0343) / 2;

    if(distance < 50) {
      wasPillTaken = 1;
      digitalWrite(outputPinPillTaken, HIGH);
      Serial.println();
      Serial.println("Pill taken!");
      Serial.println();
    }
    Serial.print("Distance: ");
    Serial.println(distance);
    delay(100);
    endtime = millis();
  }
}

void sendEmail() {
  Serial.println("Send Email");
  digitalWrite(outputPinToEsp32, HIGH);
  delay(4000);
  digitalWrite(outputPinToEsp32, LOW);
}

void loop() {
  if (digitalRead(inputPinToDispense) == HIGH) {
    Serial.println("Start Loop Process");
    dispensePill();
    setBuzzer();
    sendEmail();
    digitalWrite(outputPinPillTaken, LOW);
    delay(8000);
  }
}
