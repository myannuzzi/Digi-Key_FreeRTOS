//Program uses a single task to blink the built in led
//Core 1 is used and a print statement shows the task completed
//Written by Mike Yannuzzi

//Challenge:
//Create a second task to blink the light at different rates

//Use only one core when creating tasks
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

//LED  rates
static const int rate_1 = 500; //ms
static const int rate_2 = 300; //ms

//Pins
static const int led_pin = LED_BUILTIN;

//Our task: Blink an LED
void toggleLED(void *parameter) {
  while(1) {
    Serial.print("Starting task 1\n");
    digitalWrite(led_pin, HIGH);
    vTaskDelay(rate_1 / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(rate_2 / portTICK_PERIOD_MS);
    //Adding in a print to the console
    Serial.print("Task 1 Completed!\n");
    Serial.print("---------------\n");
  }
}

//Create a second task to blink the led at a different rate
//Our task: Blink an LED
void toggleLED2(void *parameter) {
  while(1) {
    Serial.print("Starting task 2\n");
    digitalWrite(led_pin, HIGH);
    vTaskDelay(rate_2 / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(rate_2 / portTICK_PERIOD_MS);
    //Adding in a print to the console
    Serial.print("Task 2 Completed!\n");
    Serial.print("---------------\n");
  }
}

void setup() {
  //Setup the serial port
  Serial.begin(115200);
  Serial.print("Program started...");
  Serial.print(xPortGetCoreID());
  // put your setup code here, to run once:
  pinMode(led_pin, OUTPUT);

  // Task to run forever
  xTaskCreatePinnedToCore(      // Use xTaskCreate() in Vanilla FreeRTOS
              toggleLED,    // Function to be called
              "Toggle LED", // Name of task
              1024,         // Stack size (bytes in ESP32, words in FreeRTOS)
              NULL,         // Parameter to pass to function
              1,            // Task priority (0 to configMAX_PRIORITIES - 1)
              NULL,         // Task Handle
              app_cpu);     // Run on one core for demo purposes (ESP32 only)

  // Task to run forever
  xTaskCreatePinnedToCore(      // Use xTaskCreate() in Vanilla FreeRTOS
              toggleLED2,    // Function to be called
              "Toggle LED2", // Name of task
              1024,         // Stack size (bytes in ESP32, words in FreeRTOS)
              NULL,         // Parameter to pass to function
              1,            // Task priority (0 to configMAX_PRIORITIES - 1)
              NULL,         // Task Handle
              app_cpu);     // Run on one core for demo purposes (ESP32 only)

  // If this was vanilla FreeRTOS, you'd want to call vTaskStartScheduler() in
  // main after setting up your tasks.
                        
}

void loop() {
  // put your main code here, to run repeatedly:

}
