int leds[] = {2, 3, 4, 5, 6, 7};
int delay_time = 200;
void setup() {
  for(int i = 0; i < 6; i++){
    pinMode(leds[i], OUTPUT);
  }

  Serial.begin(9600);
  Serial.println("New Delay Time(ms): ");
}

void loop() {
  if(Serial.available() > 0){
    int new_delay_time = Serial.parseInt();

    if(new_delay_time > 0){
      delay_time = new_delay_time;
      Serial.print("New delay time is set to: ");
      Serial.println(delay_time);
    }

    while(Serial.available() > 0){
      Serial.read();
    }
  }

  for(int i = 0; i < 6; i++){
    digitalWrite(leds[i], HIGH);
    delay(delay_time);
    digitalWrite(leds[i], LOW);
  }
  for(int j = 5; j > -1; j--){
    digitalWrite(leds[j], HIGH);
    delay(delay_time);
    digitalWrite(leds[j], LOW);
  }
}
