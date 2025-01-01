#include "sensor.h"
#include <Arduino.h>

void sensor_init() {
  
}

bool is_active() {
    return digitalRead(sensor_pin) == HIGH;
}