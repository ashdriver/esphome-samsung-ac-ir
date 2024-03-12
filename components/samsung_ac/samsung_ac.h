#pragma once

#include "esphome/core/log.h"
#include "esphome/components/climate/climate.h"
#include "esphome/components/sensor/sensor.h"

#include "IRremoteESP8266.h"
#include "IRsend.h"
#include "ir_Samsung.h"


namespace esphome {
namespace samsung_ac {

const uint8_t SAMSUNG_AC_TEMP_MIN = 16;  // 16C
const uint8_t SAMSUNG_AC_TEMP_MAX = 30;  // 32C

class SamsungClimate : public climate::Climate {
 public:
  SamsungClimate() : climate::Climate() {}
 public:
  void set_sensor(sensor::Sensor *sensor);
  void init(sensor::Sensor *sensor, uint16_t pin);
 
 protected:
  IRSamsungAC *ac_{nullptr};
  sensor::Sensor *sensor_{nullptr};
  
  void setup_ir_cmd();

  climate::ClimateTraits traits() override;
  void control(const climate::ClimateCall &call) override;

};

}  // namespace samsung_ac
}  // namespace esphome
