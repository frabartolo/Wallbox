#pragma once

#include <cstdint>
#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/number/number.h"

namespace esphome {
namespace ds3502_potentiometer {

class DS3502Potentiometer : public number::Number, public i2c::I2CDevice {
 public:
  DS3502Potentiometer() : number::Number() {}

  void setup() override;
  void dump_config() override;
  void control(float value) override; // Setzt den Wert über ESPHome

 protected:
  // Wandelt Ampere in Wiper-Wert um
  uint8_t ampsToWiper(int amps);
  void setWiper(uint8_t value);
  int getCurrentWiperSetting();
};

}  // namespace ds3502_potentiometer
}  // namespace esphome