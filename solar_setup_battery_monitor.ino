/*
 * SOLAR SETUP BATTERY VOLTAGE MONITOR
 * Maxime MOREILLON
 * 
 * Board type: ESP32 DEV Board
 * 
 */

#include "IotKernel.h"
#include "LowPassFilter.cpp"
#include <WebSocketsServer.h>

// Pin mapping
#define VOLTAGE_ADC_PIN 35
#define CURRENT_ADC_PIN 34

// MQTT
#define MQTT_STATUS_TOPIC "solar/status"

// WS
#define WS_PORT 8087

// Measurement parameters
#define VCC 3.30
#define ADC_MAX 4095.00
#define VOLTAGE_DIVIDER_COEFFICIENT 5.70

// Timing settings
#define ADC_READ_PERIOD 100
#define WS_EMIT_PERIOD 500
#define MQTT_PUBLISH_PERIOD 200000
#define FILTER_CONSTANT 0.05



WebSocketsServer ws_server = WebSocketsServer(WS_PORT);


LowPassFilter voltage_lpf(FILTER_CONSTANT);
LowPassFilter current_lpf(FILTER_CONSTANT);

IotKernel iot_kernel("solar","0.1.1"); 


float battery_voltage;
float current;

void setup() {
  iot_kernel.init();
  mqtt_config();
  ws_setup();
}

void loop() {
  iot_kernel.loop();
  ws_server.loop();
  adc_read();
  periodic_MQTT_publish();
  periodic_ws_emit();
}
