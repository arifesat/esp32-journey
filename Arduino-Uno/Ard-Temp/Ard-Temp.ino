int lm35_pin = A0;
int led_pin = 8;
#define buzzer 9
int time = 5000;
int read_value = 0;
int temp_res = 0;
int temp = 0;

void setup() {
  pinMode(led_pin, OUTPUT);
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  read_value = analogRead(lm35_pin);
  temp_res = (read_value / 1023.0) * 5000;
  temp = temp_res / 10.0;
  Serial.println(temp);
  if(temp > 20){
    digitalWrite(led_pin, HIGH);
    digitalWrite(buzzer, HIGH);
    delay(time);
    digitalWrite(led_pin, LOW);
    digitalWrite(buzzer, LOW);
    delay(time);
  }else{
    digitalWrite(led_pin, LOW);
    digitalWrite(buzzer, LOW);
  }
}
