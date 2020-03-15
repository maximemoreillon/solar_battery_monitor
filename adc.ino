void adc_read(){

  static long last_read_time;

  if(millis()- last_read_time> ADC_READ_PERIOD){
    last_read_time = millis();

    // Reading ADCs
    float adc_reading = analogRead(ADC_PIN);
    lpf.feed(adc_reading);
    float adc_reading_filtered = lpf.output;
    
    float adc_voltage = VCC * adc_reading_filtered / ADC_MAX;
    battery_voltage = VOLTAGE_DIVIDER_COEFFICIENT * adc_voltage;

    // THIS SHOULD BE IN A SEPARATE FUNCTION
    StaticJsonDocument<300> outbound_JSON_message;
    
    outbound_JSON_message["adc_reading"] = (String) adc_reading;
    outbound_JSON_message["adc_reading_filtered"] = (String) adc_reading_filtered;
    outbound_JSON_message["adc_voltage"] = (String) adc_voltage;
    outbound_JSON_message["battery_voltage"] = (String) battery_voltage;
    
    char JSONmessageBuffer[300];
    serializeJson(outbound_JSON_message, JSONmessageBuffer, sizeof(JSONmessageBuffer));
    ws_server.broadcastTXT(JSONmessageBuffer);
  }

  static long last_publish_time;

  long now = millis();
  
  if(now - last_publish_time > MQTT_PUBLISH_PERIOD){
    last_publish_time = now;
    StaticJsonDocument<200> outbound_JSON_message;
  
    // Add the DHT reading to the JSON message
    outbound_JSON_message["battery_voltage"] = (String) battery_voltage;
    
    // Serialize JSON into a char array
    char JSONmessageBuffer[100];
    serializeJson(outbound_JSON_message, JSONmessageBuffer, sizeof(JSONmessageBuffer));

    // Send the char array
    Serial.println(F("[MQTT] publish of measurement"));
    MQTT_client.publish(MQTT_STATUS_TOPIC, JSONmessageBuffer, MQTT_RETAIN);
    
  }
}
