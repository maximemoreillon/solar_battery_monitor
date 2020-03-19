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

    
  }

  
}
