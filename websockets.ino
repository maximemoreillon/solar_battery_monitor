void websocket_setup(){
  ws_server.begin();
}

void periodic_ws_emit(){
  static long last_emit_time;

  long now = millis();
  
  if(now - last_emit_time > WS_EMIT_PERIOD){
    last_emit_time = now;
    
    StaticJsonDocument<200> outbound_JSON_message;
    
    outbound_JSON_message["battery_voltage"] = (String) battery_voltage;
    outbound_JSON_message["current"] = (String) current;
    
    char JSONmessageBuffer[200];
    serializeJson(outbound_JSON_message, JSONmessageBuffer, sizeof(JSONmessageBuffer));
    ws_server.broadcastTXT(JSONmessageBuffer);
    
  }
}
