#define echoPin 6
#define trigPin 7
#define buzzerPin 8
#define redLed 11
#define greenLed 10
#define blueLed 9

int maxRange = 50;
int minRange = 4;
long duration, distance;

unsigned long prevBuzzMs = 0;
int buzzerState = LOW;
int buzzerDelay = 0;
void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(blueLed, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  measureDistance();

  updateLedColor();

  updateBuzzer();
}

void measureDistance(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distance = duration / 58.2;
  Serial.println(distance);
}

void updateLedColor(){
  int redValue;
  int greenValue;

  redValue = map(distance, minRange, maxRange, 255, 0);

  greenValue = map(distance, minRange, maxRange, 0, 255);

  redValue = constrain(redValue, 0, 255);
  greenValue = constrain(greenValue, 0, 255);

  analogWrite(redLed, 255 - redValue);
  analogWrite(greenLed, 255 - greenValue);
  analogWrite(blueLed, 255);
}

void updateBuzzer(){
  if(distance >= maxRange || distance <= minRange){
    buzzerDelay = 0;
    noTone(buzzerPin);
    buzzerState = LOW;
    return;
  }

  buzzerDelay = map(distance, minRange, maxRange, 50, 1000);

  unsigned long currentMs = millis();

  if(currentMs - prevBuzzMs >= buzzerDelay){
    prevBuzzMs = currentMs;

    if(buzzerState == LOW){
      buzzerState = HIGH;
      tone(buzzerPin, 2000);
    }else{
      buzzerState = LOW;
      noTone(buzzerPin);
    }
  }
}