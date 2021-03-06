void adc_read(){

  static long last_read_time;

  if(millis()- last_read_time> ADC_READ_PERIOD){
    last_read_time = millis();

    // Reading voltage ADC
    float voltage_adc_reading = analogRead(VOLTAGE_ADC_PIN);
    voltage_lpf.feed(voltage_adc_reading);    
    battery_voltage = VOLTAGE_DIVIDER_COEFFICIENT * (VCC * voltage_lpf.output / ADC_MAX);

    // Reading current ADC
    float current_adc_reading = analogRead(CURRENT_ADC_PIN);
    current_lpf.feed(current_adc_reading); 
    float filtered_current_adc_voltage =  VCC * current_lpf.output / ADC_MAX;

    // Conversion from voltage to current
    // Apparently not very accurate at the moment
    const float current_bias = 0.40;
    current = ((filtered_current_adc_voltage - 1.50) / (3.50 - 1.50)) * 10.00 - 5.00 + current_bias;
        
  }

 
}
