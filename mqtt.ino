void MQTT_setup(){
  Serial.println(F("[MQTT] MQTT setup"));
  MQTT_client.setServer(MQTT_BROKER_ADDRESS, MQTT_PORT);
  MQTT_client.setCallback(MQTT_message_callback);
}

void MQTT_connection_manager(){

  static int MQTT_connected = -1; // 1: connected, 0: disconnected, -1: unknown
  static long last_MQTT_connection_attempt;
  
  if(!MQTT_client.connected()) {
    if(MQTT_connected != 0){
      // MQTT connection status changed to "disconnected"
      MQTT_connected = 0;
      Serial.print(F("[MQTT] Disconnected: "));
      Serial.println(MQTT_client.state());
    }
        
    if(millis() - last_MQTT_connection_attempt > 1000){
      last_MQTT_connection_attempt = millis();
      MQTT_client.connect(HOSTNAME, MQTT_USERNAME, MQTT_PASSWORD, MQTT_STATUS_TOPIC, MQTT_QOS, MQTT_RETAIN, MQTT_LAST_WILL);
    }
        
  }
  else {
    if(MQTT_connected != 1){
      // MQTT connection status changed to "connected"
      MQTT_connected = 1;
      Serial.println(F("[MQTT] Connected"));
    }
  }
}

void MQTT_message_callback(char* topic, byte* payload, unsigned int length) {
  Serial.print(F("[MQTTT] message received on "));
  Serial.print(topic);
  Serial.print(F(", payload: "));
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
    MQTT_client.publish(MQTT_STATUS_TOPIC, JSONmessageBuffer, MQTT_RETAIN);
    
  }
}
