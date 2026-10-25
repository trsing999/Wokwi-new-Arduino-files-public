int irPin = 3;

void setup(){
  pinMode(irPin, INPUT);
  Serial.begin(9600);
}

void loop(){
  Serial.println(digitalRead(irPin));
  delay(500);
}