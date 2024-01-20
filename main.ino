#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27,  16, 2);
unsigned long time;
const int trigPin = 7;
const int echoPin = 8;

const int ledInPin = 12;
const int ledFullPin = 5;

const int buttonPin = 10;
const int servoPin = 13;

const int park1LedPin = 2;
const int park2LedPin = 3;
const int park3LedPin = 4;

int vec[3]={-1};

int freeParks = 3;
int angle = 10;
float duration, distance;
Servo servo;

void setup() {
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(ledInPin, OUTPUT);
  pinMode(ledFullPin, OUTPUT);

  pinMode(park1LedPin, OUTPUT);
  pinMode(park2LedPin, OUTPUT);
  pinMode(park3LedPin, OUTPUT);

  pinMode(buttonPin, INPUT_PULLUP);
  

  servo.attach(servoPin);
  servo.write(angle);
  
  lcd.init();
  lcd.backlight(); 
  lcd.clear();
  Serial.begin(9600);
}

void loop() {
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration * .0343) / 2;
  
  Serial.println(distance);

  time = millis() / 1000;
  lcd.setCursor(0, 0);
  lcd.print("Parking");
  lcd.setCursor(12, 0);
  lcd.print(time); 

  byte buttonState = digitalRead(buttonPin);

  if ((distance <= 10 && freeParks != 0) || (buttonState == LOW && freeParks != 3 )) {
    digitalWrite(ledInPin, HIGH);
    digitalWrite(ledFullPin, LOW);

    for (angle = 0; angle <= 100; angle++) {
      servo.write(angle);
      delay(5);
    }
    delay(3000);
    for (angle = 100; angle > 10; angle--) {
      servo.write(angle);
      delay(5);
    }
    if (buttonState == LOW) {
      if (freeParks < 3) {
        freeParks++;
        lcd.clear();
        //afisez timpul cat a stat : lcd print: time- vec[LOCUL_parcarii]
      }
    } else {
      if (freeParks > 0) {
        freeParks--;
        vec[freeParks]=time;
      }
    }
  } else {
    
    if (freeParks == 3) {
      lcd.setCursor(0, 1);
      lcd.print("Free parks: 3");
      digitalWrite(park1LedPin, HIGH);
      digitalWrite(park2LedPin, HIGH);
      digitalWrite(park3LedPin, HIGH);
      digitalWrite(ledInPin, HIGH);
    } else if (freeParks == 2) {
      lcd.setCursor(0, 1);
      lcd.print("Free parks: 2");
      digitalWrite(park1LedPin, LOW);
      digitalWrite(park2LedPin, HIGH);
      digitalWrite(park3LedPin, HIGH);
      digitalWrite(ledInPin, HIGH);
    } else if (freeParks == 1) {
      lcd.setCursor(0, 1);
      lcd.print("Free parks: 1");
      digitalWrite(park1LedPin, LOW);
      digitalWrite(park2LedPin, LOW);
      digitalWrite(park3LedPin, HIGH);
      digitalWrite(ledInPin, HIGH);
    } else {
      lcd.setCursor(0, 1);
      lcd.print("Parking is FULL");
      Serial.print("FULL");
      digitalWrite(park1LedPin, LOW);
      digitalWrite(park2LedPin, LOW);
      digitalWrite(park3LedPin, LOW);
      digitalWrite(ledFullPin, HIGH);
      digitalWrite(ledInPin, LOW);
    }
  }

  delay(100);
}
