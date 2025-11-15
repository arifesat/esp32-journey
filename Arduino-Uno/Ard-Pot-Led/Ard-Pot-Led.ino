#define potpin A0
#define Led 3

int value = 0;

void setup() {
}

void loop() {
  value = analogRead(potpin);
  // Serial.println(value);
  value = map(value, 0, 1023, 0, 255);
  // Serial.println("Mapped: " + value);
  analogWrite(Led,value);
}
