#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_Sensor.h>
LiquidCrystal_I2C lcd(0x27,16,2);

int redPin = 2;
int yellowPin = 4;
int greenPin = 7;
int trigPin = 3;
int echoPin = 5;
int buzzerPin = 8;
int buttonPin = 12;

int clockTimerA = 0;
int clockTimerB = 0;
int clockTimerC = 0;

int workMode = 0;

bool lastButtonState = HIGH;

void setup(){
  pinMode(redPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();
}

void loop(){
  clockTimerA += 1;
  clockTimerB += 1;
  clockTimerC += 1;

  if(clockTimerA > 20){
    clockTimerA = 0;
  }
  if(clockTimerB > 40){
    clockTimerB = 0;
  }

  digitalWrite(trigPin, LOW);
  delayMicroseconds(20);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  long duration = pulseIn(echoPin,HIGH);
  float d = duration*0.034/2;
  if(d == 0 || d > 400){
    d =999;
  }

  int currentButtonState = digitalRead(buttonPin);

  if(currentButtonState == LOW && lastButtonState == HIGH){
    if(20 <= d && d < 50  ||  d <= 20){
      workMode = 1 - workMode;
    }
  }
  lastButtonState = currentButtonState;

  if(workMode == 0){
    lcd.setCursor(11,1);
    lcd.print("ALARM");
  }else{
    lcd.setCursor(11,1);
    lcd.print(" MUTE");
  }

    lcd.setCursor(0,0);lcd.print("Dist:");
    lcd.print(d,0);lcd.print("cm  ");

  if(d > 100){
    lcd.setCursor(0,1);lcd.print("SAFE   ");
    digitalWrite(greenPin, HIGH);
    digitalWrite(redPin, LOW);
    digitalWrite(yellowPin, LOW);
    noTone(buzzerPin);
  }

  if(50 <= d && d <= 100){
    lcd.setCursor(0,1);lcd.print("CAUTION");
    digitalWrite(yellowPin, HIGH);
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, LOW);
    noTone(buzzerPin);
  }

  if(20 <= d && d < 50){
    lcd.setCursor(0,1);lcd.print("WARNING");
    digitalWrite(yellowPin, LOW);
    digitalWrite(greenPin, LOW);
    if(clockTimerA < 10){
      digitalWrite(redPin, HIGH);
    }else{
      digitalWrite(redPin, LOW);
    }
    if(workMode == 0){
    if(clockTimerB < 20){
      tone(buzzerPin,1000);
    }else{
      noTone(buzzerPin);
    }
  }else{
    noTone(buzzerPin);
  }
}

  if(d < 20){
    int dynamicMax = map(d,0,20,10,30);
    lcd.setCursor(0,1);lcd.print("STOP");
    digitalWrite(yellowPin, LOW);
    digitalWrite(greenPin, LOW);
    if(clockTimerC > dynamicMax){
      clockTimerC = 0;
    }
    if(clockTimerC < (dynamicMax/2)){
      digitalWrite(redPin, HIGH);
    }else{
      digitalWrite(redPin, LOW);
    }
    if(workMode == 0){
      if(clockTimerC % 10 < 5){
        tone(buzzerPin,1000);
        }else{
          noTone(buzzerPin);
      }
    }else{
      noTone(buzzerPin);
    }
  }
  delay(25);
}

