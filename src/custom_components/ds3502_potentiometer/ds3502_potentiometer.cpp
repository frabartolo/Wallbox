#include "ds3502_potentiometer.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"
#include <cmath>
#include <algorithm>

namespace esphome {
namespace ds3502_potentiometer {

static const char *const TAG = "ds3502_potentiometer";

// Simple map function (like Arduino's)
static int map_value(int x, int in_min, int in_max, int out_min, int out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void DS3502Potentiometer::setup() {
  ESP_LOGCONFIG(TAG, "Setting up DS3502 Potentiometer...");
  // The I2C bus should already be initialized by ESPHome
  // Ensure I2C communication works
  if (!this->is_ready()) {
    ESP_LOGE(TAG, "I2C Device not ready. Check wiring and address.");
    this->mark_failed();
    return;
  }
  // Optionally: Set or read initial value
  int current_wiper = this->getCurrentWiperSetting();
  int current_amps = map_value(current_wiper, 127, 0, 8, 64);
  this->publish_state(current_amps); // Publish current ampere value
}

void DS3502Potentiometer::dump_config() {
  LOG_NUMBER(TAG, "DS3502 Potentiometer", this);
  LOG_I2C_DEVICE(this);
}

void DS3502Potentiometer::control(float value) {
  int amps = static_cast<int>(std::round(value));
  uint8_t wiper_value = ampsToWiper(amps);
  this->setWiper(wiper_value);
  this->publish_state(amps); // Update the state of the Number component
  ESP_LOGD(TAG, "Set DS3502 wiper to %d for %d Amps", wiper_value, amps);
}

void DS3502Potentiometer::setWiper(uint8_t value) {
  this->write_register(0x00, &value, 1);
}

uint8_t DS3502Potentiometer::ampsToWiper(int amps) {
  switch (amps) {
    case 64: return 0;
    case 50: return 20;
    case 32: return 40;
    case 24: return 60;
    case 16: return 80;
    case 12: return 100;
    case 8:  return 127;
    default: return 127; // Default for unknown ampere values
  }
}

int DS3502Potentiometer::getCurrentWiperSetting() {
  uint8_t value;
  if (this->read_register(0x00, &value, 1)) {
    return value;
  }
  return -1; // Error reading
}

}  // namespace ds3502_potentiometer
}  // namespace esphome