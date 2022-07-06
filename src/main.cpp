#include <Arduino.h>

// built in led: 2
// led digital: 32

#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

struct led_data {
  uint8_t pin;
  unsigned int delay;
};

// static const int led_pin = 2;

static TaskHandle_t task_1 = NULL;
static TaskHandle_t task_2 = NULL;

static led_data led_1 = {2, 300};
static led_data led_2 = {32, 1000};

void toggleLED(void *parameter) {
  led_data *led = (led_data *)parameter;

  while (true) {
    digitalWrite(led->pin, HIGH);
    vTaskDelay(led->delay / portTICK_PERIOD_MS);
    digitalWrite(led->pin, LOW);
    vTaskDelay(led->delay / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(9600);

  vTaskDelay(1000 / portTICK_PERIOD_MS);

  Serial.println();

  pinMode(led_1.pin, OUTPUT);
  pinMode(led_2.pin, OUTPUT);

  xTaskCreatePinnedToCore(toggleLED, "toggleLED1", 1024, &led_1, 1, &task_1,
                          app_cpu);
  xTaskCreatePinnedToCore(toggleLED, "toggleLED2", 1024, &led_2, 1, &task_2,
                          app_cpu);
}

void loop() {}