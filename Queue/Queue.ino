#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

QueueHandle_t sensorQueue;

void TaskSender(void *pvParameters);
void TaskReceiveer(void *QueuepvParameters);

void setup() {
  Serial.begin(115200);

  sensorQueue = xQueueCreate(10, sizeof(int));

  if(sensorQueue == NULL){
    Serial.println("Queue can not created");
    while(1); // stop if there is error
  }
  xTaskCreate(TaskSender, "Sender", 2048, NULL, 1, NULL);
  xTaskCreate(TaskReceiver, "Receiver", 2048, NULL, 1, NULL);
}

void loop() {}

void TaskSender(void *pvParameters){
  int sensorValue = 0;

  for(;;){
    sensorValue = random(0, 100);

    xQueueSend(sensorQueue, &sensorValue, portMAX_DELAY);
    // portMAX_DELAY: if queue is full, wait until there is an empty space
    Serial.println("Data sent.");

    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void TaskReceiver(void *pvParameters){
  int receivedValue = 0;

  for(;;){
    if(xQueueReceive(sensorQueue, &receivedValue, portMAX_DELAY) == pdTRUE){
      Serial.print("Read value from queue: ");
      Serial.println(receivedValue);
    }
  }
}