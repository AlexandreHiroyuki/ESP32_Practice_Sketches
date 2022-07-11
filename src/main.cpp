#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFi.h>

#include "./env.h"

#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

struct led_data {
  uint8_t pin;
  unsigned int delay;
};

struct server_address {
  String host_name;
};

static TaskHandle_t task_1 = NULL;
static TaskHandle_t task_2 = NULL;

static led_data led_1 = {2, 300};    // BUILT_IN LED
static led_data led_2 = {32, 1000};  // DIGITAL LED

static const server_address server = {.host_name = "http://192.168.0.138:3333"};

WiFiClient client;

unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

void toggleLED(void *parameter) {
  led_data led = *(led_data *)parameter;

  while (true) {
    digitalWrite(led.pin, HIGH);
    vTaskDelay(led.delay / portTICK_PERIOD_MS);
    digitalWrite(led.pin, LOW);
    vTaskDelay(led.delay / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(9600);
  vTaskDelay(1000 / portTICK_PERIOD_MS);

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    vTaskDelay(500 / portTICK_PERIOD_MS);
    Serial.println("Connecting to WiFi..");
  }
  WiFi.mode(WIFI_MODE_STA);
  Serial.println(WiFi.macAddress());
  Serial.println(WiFi.localIP());

  pinMode(led_1.pin, OUTPUT);
  pinMode(led_2.pin, OUTPUT);

  xTaskCreatePinnedToCore(toggleLED, "toggleLED1", 1024, &led_1, 1, &task_1,
                          app_cpu);
  xTaskCreatePinnedToCore(toggleLED, "toggleLED2", 1024, &led_2, 1, &task_2,
                          app_cpu);
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    // Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;

      String serverPath = server.host_name;
      Serial.println(serverPath);

      // Your Domain name with URL path or IP address with path
      http.begin(serverPath.c_str());

      // Send HTTP GET request
      int httpResponseCode = http.GET();

      if (httpResponseCode > 0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      } else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    } else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}
