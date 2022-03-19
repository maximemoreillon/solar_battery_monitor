/*
 * SOLAR SETUP BATTERY VOLTAGE MONITOR
 * Maxime MOREILLON
 * 
 * Board type: ESP32 DEV Board
 * 
 */

#include "IotKernel.h"

// Pin mapping
#define VOLTAGE_ADC_PIN 35
#define CURRENT_ADC_PIN 34

#define MQTT_STATUS_TOPIC "solar/status"


// Measurement parameters
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

IotKernel iot_kernel("solar","0.1.0"); 


float battery_voltage;
float current;

void setup() {
  iot_kernel.init();
  mqtt_config();
}

void loop() {
  iot_kernel.loop();
  adc_read();
  periodic_MQTT_publish();
}
