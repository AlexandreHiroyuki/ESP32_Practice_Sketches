# ESP32 Practice Sketches

## Introduction

This is my first attempt at a sketch for the ESP32. I tried to explore the many features of the ESP32 and the PlatformIO ecosystem.

## Main Features

Using the ESP FreeRTOS, I created two tasks that switches the built-in LED and an external LED on and off.

Both tasks are running the same function at the same priority. The way it differs is which argument is passed to the function. The first task is passed the built-in LED pin and delay, the second task is passed the external LED pin and delay.

Also, on the main loop, the ESP32 makes HTTP requests to the server and prints the response.

## Environment Header

Inspired by the .env module used on node.js, I created a header file that contains the variables that are used in the sketch.

You can easily add new variables to the header file, just copy the `env-model.h` and rename it to `env.h`.

The variables `SSID` and `PASSWORD` are used to connect to the WiFi network.

## Practice Server

The `/server` folder contains the server that is used to make HTTP requests to the ESP32.

```
cd server
yarn
yarn start
```

## Unit Tests

The `/tests` folder contains the unit tests for the sketch.

[Unit Test Espressif Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/unit-tests.html)

[Unit Test PlatformIO Documentation](https://docs.platformio.org/en/latest/advanced/unit-testing/index.html)