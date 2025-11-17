const int ledPin = 2;

void TaskBlink(void *pvParameters);
void TaskSerial(void * pvParameters);

void setup() {
  Serial.begin(115200);

  xTaskCreate(
    TaskBlink,   // function to call
    "BlinkTask", // task name for debugging
    1024,        // Stack size (word, not byte)
    NULL,        // parameters if exists, using with pointers
    1,           // priority, higher better
    NULL         // task handle to manage it afterwards if we wish
  );

  xTaskCreate(
    TaskSerial,
    "SerialTask",
    2048,        // more stack size for serial processes
    NULL,
    1,
    NULL
  );

  Serial.println("Setup finished, tasks started.");
}

void loop() {
  //we're using FreeRTOS schedules so we do not need loop section right now.
}

// Task Functions
// Task must not return anything, they should stay in forever loop

void TaskBlink(void *pvParameters){
  pinMode(ledPin, OUTPUT);
  // same as while(1); loops forever
  for(;;){
    digitalWrite(ledPin, HIGH);
    vTaskDelay(pdMS_TO_TICKS(500));
    // unsigned long start = millis();
    // while(millis() - start < 3000){}

    digitalWrite(ledPin, LOW);
    vTaskDelay(pdMS_TO_TICKS(500));
    // start = millis();
    // while(millis() - start < 1000){}
  }
}

void TaskSerial(void *pvParameters){
  int counter = 0;

  for(;;){
    Serial.print("Second: ");
    Serial.println(counter);
    counter++;

    vTaskDelay(pdMS_TO_TICKS(1000)); // 1000ms delay
  }
}