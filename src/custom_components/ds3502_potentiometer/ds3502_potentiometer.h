#include "esphome.h"

class DS3502Potentiometer : public Component, public Number {
 public:
  void setup() override {
    Wire.begin();
  }

  void control(float value) override {
    int wiper = amps_to_wiper((int)value);
    Wire.beginTransmission(0x28);
    Wire.write(0x00);  // Wiper register
    Wire.write(wiper);
    Wire.endTransmission();
    this->publish_state(value);
  }

 protected:
  int amps_to_wiper(int amps) {
    if (amps >= 64) return 0;
    if (amps >= 50) return 20;
    if (amps >= 32) return 40;
    if (amps >= 24) return 60;
    if (amps >= 16) return 80;
    if (amps >= 12) return 100;
    return 127; // niedrigster Strom
  }
};
