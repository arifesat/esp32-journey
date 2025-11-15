int pot_pin = A0;
int pot_val = 0;

int red_pin = 11;
int green_pin = 10;
int blue_pin = 9;

int red_val = 0;
int green_val = 0;
int blue_val = 0;


void setup() {
  pinMode(red_pin, OUTPUT);
  pinMode(green_pin, OUTPUT);
  pinMode(blue_pin, OUTPUT);
}

void loop() {
  pot_val = analogRead(pot_pin);

  if(pot_val < 341){
    pot_val = (pot_val * 3) / 4;

    red_val = 255 - pot_val;
    green_val = pot_val;
    blue_val = 0;
  }else if(pot_val < 682){
    pot_val = ((pot_val - 341) * 3) / 4;

    red_val = 0;
    green_val = 255 - pot_val;
    blue_val = pot_val;
  }else{
    pot_val = ((pot_val - 683) * 3) / 4;
    red_val = pot_val;
    green_val = 0;
    blue_val = 255 - pot_val;
  }

  analogWrite(red_pin, 255 - red_val);
  analogWrite(green_pin, 255 - green_val);
  analogWrite(blue_pin, 255 - blue_val);
}
