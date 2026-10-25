#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#define DHTPIN 13
#define DHTTYPE DHT22
DHT dht(DHTPIN,DHTTYPE);
LiquidCrystal_I2C lcd(0x27,16,2);

int ledPin = 11;
int trigPin = 9;
int echoPin = 10;
int irPin = 4;
int buzzerPin = 7;
int buttonPin = 8;

int clockTimer = 0;
int pressedTime = 0;
int workMode = 0;
bool isLongPressed = false;

void setup(){
  pinMode(ledPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(irPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  dht.begin();
  lcd.init();
  lcd.backlight();
}

void loop(){
  int buttonState = digitalRead(buttonPin);
  int irState = digitalRead(irPin);
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  clockTimer += 1;

  if(isnan(t) || isnan(h)){
    lcd.setCursor(0,0);
    lcd.print("Sensor Error!");
    lcd.setCursor(0,1);
    lcd.print("Check Wiring");
    delay(2000);
    return;
  }

  if(buttonState == LOW){
    if(pressedTime == 0){
      pressedTime = millis();
      isLongPressed = false;
    }else if(millis() - pressedTime > 500 && !isLongPressed){
      isLongPressed = true;
    }
    
    if(pressedTime !=0 && !isLongPressed){
      workMode = 1 - workMode;
      isLongPressed = true;
    }
    pressedTime = 0;
    isLongPressed = false;
  }

  digitalWrite(trigPin, LOW);
  delayMicroseconds(20);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  long duration = pulseIn(echoPin,HIGH);
  float d = duration*0.034/2;

    if(workMode == 0){
      lcd.setCursor(0,0);
      lcd.print("Temp:");lcd.print(t,1);
      lcd.print("C  ");
      lcd.setCursor(0,1);
      lcd.print("Humi:");lcd.print(h,1);
      lcd.print("%  ");
    }else{
      lcd.setCursor(0,0);
      lcd.print("Dist:");lcd.print(d,1);
      lcd.print("cm   ");
      lcd.setCursor(0,1);lcd.print("Human:");
      if(irState == HIGH){
        lcd.print("YES ");
      }else{
        lcd.print("No  ");
      }
    }

    if(irState == LOW || d > 20){
      analogWrite(ledPin,25);
      noTone(buzzerPin);
    }

    else if(irState == HIGH && t > 28.0){
      if(clockTimer > 20){
        clockTimer = 0;
      }
      if(clockTimer < 10){
        analogWrite(ledPin,255);
      }else{
        analogWrite(ledPin,0);
      }
      if(isLongPressed){
        noTone(buzzerPin);
      }else{
        if(clockTimer <10){
          tone(buzzerPin,1000);
        }else{
          noTone(buzzerPin);
        }
      }
    }

    else if(irState == HIGH && d <= 20){
      analogWrite(ledPin,255);
      if(clockTimer >50){
        clockTimer = 0;
      }
        if(!isLongPressed){
        if(clockTimer < 25){
          tone(buzzerPin,1000);
        }else{
          noTone(buzzerPin);
        }
      }else{
        noTone(buzzerPin);
      }
    }
    delay(20);
  }






