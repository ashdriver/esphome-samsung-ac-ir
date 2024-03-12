#include "samsung_ac.h"

namespace esphome {
namespace samsung_ac {

static const char *const TAG = "climate.samsung_ac";

void SamsungClimate::init(sensor::Sensor *sensor, uint16_t pin) {
  this->set_sensor(sensor);
  ac_ = new IRSamsungAc(pin);
  if (this->sensor_) {
    this->sensor_->add_on_state_callback([this](float state) {
      this->current_temperature = state;
      this->publish_state();
    });
    this->current_temperature = this->sensor_->state;
  } else {
    this->current_temperature = NAN;
  }

  auto restore = this->restore_state_();
  if (restore.has_value()) {
    restore->apply(this);
  } else {
    this->mode = climate::CLIMATE_MODE_OFF;
    this->target_temperature = 24;
    this->fan_mode = climate::CLIMATE_FAN_AUTO;
    this->swing_mode = climate::CLIMATE_SWING_OFF;
    this->preset = climate::CLIMATE_PRESET_NONE;
  }

  if (isnan(this->target_temperature)) {
    this->target_temperature = 25;
  }

  ac_->begin();
  this->setup_ir_cmd();

  ESP_LOGD("DEBUG", "Samsung A/C remote is in the following state:");
  ESP_LOGD("DEBUG", "  %s\n", ac_->toString().c_str());
}

void SamsungClimate::set_sensor(sensor::Sensor *sensor) { this->sensor_ = sensor; }

void SamsungClimate::setup_ir_cmd() {
  if (this->mode == climate::CLIMATE_MODE_OFF) {
    ac_->setPower(false);
  } else {
    ac_->setPower(true);
    if (this->mode == climate::CLIMATE_MODE_AUTO) {
      ac_->setMode(kSamsungAcAuto);
    } else if (this->mode == climate::CLIMATE_MODE_COOL) {
      ac_->setMode(kSamsungAcCool);
    } else if (this->mode == climate::CLIMATE_MODE_HEAT) {
      ac_->setMode(kSamsungAcHeat);
    } else if (this->mode == climate::CLIMATE_MODE_DRY) {
      ac_->setMode(kSamsungAcDry);
    } else if (this->mode == climate::CLIMATE_MODE_FAN_ONLY) {
      ac_->setMode(kSamsungAcFan);
    }

    ac_->setTemp((uint8_t)this->target_temperature);

    if (this->fan_mode == climate::CLIMATE_FAN_AUTO) {
      ac_->setFan(kSamsungAcFanAuto);
    } else if (this->fan_mode == climate::CLIMATE_FAN_LOW) {
      ac_->setFan(kSamsungAcFanLow);
    } else if (this->fan_mode == climate::CLIMATE_FAN_MEDIUM) {
      ac_->setFan(kSamsungAcFanMed);
    } else if (this->fan_mode == climate::CLIMATE_FAN_HIGH) {
      ac_->setFan(kSamsungAcFanHigh);
    }

    if (this->swing_mode == climate::CLIMATE_SWING_OFF) {
      ac_->setSwing(false);
    } else if (this->swing_mode == climate::CLIMATE_SWING_VERTICAL) {
      ac_->setSwing(true);
    }

 //   ac_->setSleep(this->preset == climate::CLIMATE_PRESET_SLEEP);
    //ac_->setHealth(this->preset == climate::CLIMATE_PRESET_COMFORT);
  }
}

climate::ClimateTraits SamsungClimate::traits() {
  auto traits = climate::ClimateTraits();
  traits.set_supported_modes({climate::CLIMATE_MODE_OFF, climate::CLIMATE_MODE_COOL, climate::CLIMATE_MODE_HEAT,
                              climate::CLIMATE_MODE_FAN_ONLY, climate::CLIMATE_MODE_DRY, climate::CLIMATE_MODE_AUTO});
  traits.set_supported_fan_modes(
      {climate::CLIMATE_FAN_LOW, climate::CLIMATE_FAN_MEDIUM, climate::CLIMATE_FAN_HIGH, climate::CLIMATE_FAN_AUTO});
  traits.set_supported_swing_modes({climate::CLIMATE_SWING_OFF, climate::CLIMATE_SWING_VERTICAL});
  traits.set_supported_presets({climate::CLIMATE_PRESET_NONE, /*climate::CLIMATE_PRESET_SLEEP,*/
                                /*climate::CLIMATE_PRESET_COMFORT,*/ climate::CLIMATE_PRESET_BOOST});

  traits.set_visual_max_temperature(SAMSUNG_AC_TEMP_MAX);
  traits.set_visual_min_temperature(SAMSUNG_AC_TEMP_MIN);
  traits.set_visual_temperature_step(1);
  traits.set_supports_current_temperature(true);

  return traits;
}

void SamsungClimate::control(const climate::ClimateCall &call) {
  if (call.get_mode().has_value()) 
    this->mode = *call.get_mode();

  if (call.get_target_temperature().has_value()) 
    this->target_temperature = *call.get_target_temperature();

  if (call.get_fan_mode().has_value())
    this->fan_mode = *call.get_fan_mode();

  if (call.get_swing_mode().has_value())
    this->swing_mode = *call.get_swing_mode();

  if (call.get_preset().has_value())
    this->preset = *call.get_preset();
  
  this->setup_ir_cmd();
  ac_->send();

  this->publish_state();

  ESP_LOGD("DEBUG", "Samsung A/C remote is in the following state:");
  ESP_LOGD("DEBUG", "  %s\n", ac_->toString().c_str());
}

}  // namespace samsung_ac
}  // namespace esphome
