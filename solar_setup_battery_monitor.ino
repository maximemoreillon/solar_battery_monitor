/*
 * SOLAR SETUP BATTERY VOLTAGE MONITOR
 * Maxime MOREILLON
 * 
 * Board type: ESP32 DEV Board
 * 
 */

// Libraries
#include <WiFi.h>
#include <WiFiClient.h> 
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include <WebServer.h>
#include "credentials.h";
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>

// Parameters
#define HOSTNAME "solarMonitor"
#define VERSION "1.0.1"

// Pin mapping
#define VOLTAGE_ADC_PIN 35
#define CURRENT_ADC_PIN 34

// MQTT
#define MQTT_BROKER_ADDRESS IPAddress(192, 168, 1, 2)
#define MQTT_PORT 1883
#define MQTT_STATUS_TOPIC "solar/status"
#define MQTT_LAST_WILL "{\"state\":\"disconnected\"}"
#define MQTT_QOS 1
#define MQTT_RETAIN true

// Web server
#define WWW_PORT 80
#define WS_PORT 81

// Wifi settings
#define WIFI_CONNECTION_TIMEOUT 5000

#define VCC 3.30
#define ADC_MAX 4095.00
#define VOLTAGE_DIVIDER_COEFFICIENT 5.70

// Timing settings
#define ADC_READ_PERIOD 100
#define WS_EMIT_PERIOD 1000
#define MQTT_PUBLISH_PERIOD 200000
#define FILTER_CONSTANT 0.05

class LowPassFilter {
  
  private:
    float filter_constant;
    
  public:
    float output;
    
    LowPassFilter(float filter_constant) {
      this->output = 0;
      this->filter_constant = filter_constant;
    }
    
    void feed(float input) {
      this->output = (this->output)*(1.00 - this->filter_constant) + input * (this->filter_constant);
    }
};

LowPassFilter voltage_lpf(FILTER_CONSTANT);
LowPassFilter current_lpf(FILTER_CONSTANT);


WiFiClient wifi_client;
PubSubClient MQTT_client(wifi_client);
WebServer web_server(WWW_PORT);
WebSocketsServer ws_server = WebSocketsServer(WS_PORT);

float battery_voltage;
float current;

void setup() {
  // Mandatory initial delay
  delay(10);

  // Serial init
  Serial.begin(115200);
  Serial.println(); // Separate serial stream from initial gibberish
  Serial.println(F(__FILE__ " " __DATE__ " " __TIME__));
  
  wifi_setup();
  MQTT_setup();
  ota_setup();
  web_server_setup();
  websocket_setup();
}

void loop() {
  wifi_connection_manager();
  MQTT_connection_manager();
  MQTT_client.loop();
  ArduinoOTA.handle();
  web_server.handleClient();
  ws_server.loop();
  adc_read();
  periodic_ws_emit();
  periodic_MQTT_publish();
}
