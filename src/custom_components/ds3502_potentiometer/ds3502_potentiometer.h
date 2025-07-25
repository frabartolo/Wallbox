#pragma once

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
  void control(float value) override; // Methode zum Setzen des Werts vom ESPHome Framework

 protected:
  // Hilfsfunktion zur Umwandlung von Ampere in Wiper-Wert
  byte ampsToWiper(int amps);
  void setWiper(byte value);
  int getCurrentWiperSetting();
};

}  // namespace ds3502_potentiometer
}  // namespace esphome