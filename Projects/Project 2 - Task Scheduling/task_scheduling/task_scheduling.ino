//Program 2
//Task Scheduling and preemption
//Write two tasks and schedule them to run. After one task
//is complete, delete it and run only the second task
//Written by Mike Yannuzzi

//Challenge:
//Create a user interface that changes the blink rate of the led
//Use one task for the led rate and the other task to listen to the 
//serial monitor

//Config FreeRTOS for only 1 core
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

//String to print
const char msg[] = "Robots are pretty cool.";

//Task handles
static TaskHandle_t task_1 = NULL;
static TaskHandle_t task_2 = NULL;

//--------------------------------
//Task definitions

//Task 1: Print to serial terminal with lower priority
void startTask1(void *parameter) {

  //Count the number of characters in the string
  int msg_len = strlen(msg);

  //Print string to terminal
  while(1) {
    Serial.println();
    for(int i=0;i<msg_len; i++){
      Serial.print(msg[i]);
    }
    Serial.println();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }  
}

//Task 2: Print to serial terminal with higher priority
void startTask2(void *parameter){
  while(1) {
    Serial.print('*');
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

//-------------------------------------------
// Main (runs as its own task with priority 1 on cor 1)

void setup() {
  //Setup and configure the serial port
  //A slower baud rate is used to watch output easier
  Serial.begin(300);

  //Wait so we don't miss the serial output
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS Challenge 2---");

  //Print self priority
  Serial.print("Setup and loop task running on core ");
  Serial.print(xPortGetCoreID());
  Serial.print(" with priority ");
  Serial.println(uxTaskPriorityGet(NULL));

  //Task to run forever
  //Task 1 creation
  xTaskCreatePinnedToCore(startTask1,
                          "Task 1",
                          1024,
                          NULL,
                          1,
                          &task_1,
                          app_cpu);

  //Task 2 creation
  xTaskCreatePinnedToCore(startTask2,
                          "Task 2",
                          1024,
                          NULL,
                          1,
                          &task_2,
                          app_cpu);
}

void loop() {
  //Suspend the higher priority task for some intervals
  for(int i=0;i<3;i++){
    vTaskSuspend(task_2);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    vTaskResume(task_2);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }

  //Delete the lower priority task
  if(task_1 != NULL) {
    vTaskDelete(task_1);
    task_1 = NULL;
  }
}
