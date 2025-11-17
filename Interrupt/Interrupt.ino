const int ledPin = 2;

//hw_timer_t * timer = NULL;
hw_timer_t * timer = NULL;


// ISR (Interrupt Service Routine)
// IRAM_ATTR: store function on RAM, not Flash storage
// reduces crash risk via decreasing access time
void IRAM_ATTR onTimer(){
  digitalWrite(ledPin, !digitalRead(ledPin)); // invert LED status
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  
  /*
    ESP32 has 4 hardware timers: 0, 1, 2, 3
    timerBegin(timerNumber, prescaler, count_up);
    0: Timer ID
    80: Prescaler. ESP32 works with 80MHz. Divide by 80 and we get 1 MHz (1ms) tick
    true: count upwards
  */

  timer = timerBegin(1000000);

  /*
    Attack Interrupt to Timer
    timerAttachInterrupt(timer, function, edge)
    timer: timer object
    &onTimer: function address to be called
    true: Edge trigger
  */

  timerAttachInterrupt(timer, &onTimer);

  /*
    Set Alarm
    timerAlarmWrite(timer, interrupt_at_ticks, auto_reload)
    timer: timer object
    1000000: 1 million mikrosecond = 1 second (due precscaler 1 tick = 1us)
    true: Auto-reload
  */

  timerAlarm(timer, 1000000, true, 0);

  Serial.println("Timer started. LED blinks every 1 second");

}

void loop() {}
