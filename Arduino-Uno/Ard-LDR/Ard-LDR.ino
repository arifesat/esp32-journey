#define Led 3

void setup() {
  pinMode(Led, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int light = analogRead(A0);
  Serial.println(light);
  delay(50);
  if(light > 450){
    digitalWrite(Led, LOW);
  }else{
    digitalWrite(Led, HIGH);
  }
}
