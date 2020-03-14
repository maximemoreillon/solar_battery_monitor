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
#define HOSTNAME "solar"

// Pin mapping
#define ADC_PIN 35

// MQTT
#define MQTT_BROKER_ADDRESS IPAddress(192, 168, 1, 2)
#define MQTT_PORT 1883
#define MQTT_STATUS_TOPIC "solar/status"
#define MQTT_LAST_WILL "{'state':'disconnected'}"
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
#define MQTT_PUBLISH_PERIOD 60000

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

#define FILTER_CONSTANT 0.1
LowPassFilter lpf(FILTER_CONSTANT);


WiFiClient wifi_client;
PubSubClient MQTT_client(wifi_client);
WebServer web_server(WWW_PORT);
WebSocketsServer ws_server = WebSocketsServer(WS_PORT);

float battery_voltage;

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
  ArduinoOTA.handle();
  web_server.handleClient();
  ws_server.loop();
  adc_read();
}
