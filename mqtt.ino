void mqtt_config(){
  Serial.println("[MQTT] configuring MQTT");
  iot_kernel.mqtt.setCallback(mqtt_message_callback);
}

void mqtt_message_callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("[MQTT] message received on ");
  Serial.print(topic);
  Serial.print(", payload: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void periodic_MQTT_publish(){
  static long last_publish_time;

  long now = millis();
  
  if(now - last_publish_time > MQTT_PUBLISH_PERIOD){
    last_publish_time = now;
    StaticJsonDocument<200> outbound_JSON_message;
  
    // Add the DHT reading to the JSON message
    outbound_JSON_message["battery_voltage"] = (String) battery_voltage;
    outbound_JSON_message["current"] = (String) current;
    
    // Serialize JSON into a char array
    char JSONmessageBuffer[200];
    serializeJson(outbound_JSON_message, JSONmessageBuffer, sizeof(JSONmessageBuffer));

    // Send the char array
    Serial.println(F("[MQTT] publish of measurement"));
    iot_kernel.mqtt.publish(MQTT_STATUS_TOPIC, JSONmessageBuffer, MQTT_RETAIN);
    
  }
}
