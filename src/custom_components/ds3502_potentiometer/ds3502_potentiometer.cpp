#include "ds3502_potentiometer.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace ds3502_potentiometer {

static const char *const TAG = "ds3502_potentiometer";

void DS3502Potentiometer::setup() {
  ESP_LOGCONFIG(TAG, "Setting up DS3502 Potentiometer...");
  // Der I2C-Bus sollte bereits von ESPHome initialisiert werden
  // Stelle sicher, dass die I2C-Kommunikation funktioniert
  if (!this->is_ready()) {
    ESP_LOGE(TAG, "I2C Device not ready. Check wiring and address.");
    this->mark_failed();
    return;
  }
  // Optional: Initialen Wert setzen oder auslesen
  int current_wiper = this->getCurrentWiperSetting();
  int current_amps = map(current_wiper, 127, 0, 8, 64);
  this->publish_state(current_amps); // Veröffentliche den aktuellen Ampere-Wert
}

void DS3502Potentiometer::dump_config() {
  LOG_NUMBER(TAG, "DS3502 Potentiometer", this);
  LOG_I2C_DEVICE(this);
}

void DS3502Potentiometer::control(float value) {
  int amps = static_cast<int>(roundf(value));
  byte wiper_value = ampsToWiper(amps);
  this->setWiper(wiper_value);
  this->publish_state(amps); // Aktualisiere den Zustand der Number-Komponente
  ESP_LOGD(TAG, "Set DS3502 wiper to %d for %d Amps", wiper_value, amps);
}

void DS3502Potentiometer::setWiper(byte value) {
  this->write_register(0x00, &value, 1);
}

byte DS3502Potentiometer::ampsToWiper(int amps) {
  switch (amps) {
    case 64: return 0;
    case 50: return 20;
    case 32: return 40;
    case 24: return 60;
    case 16: return 80;
    case 12: return 100;
    case 8:  return 127;
    default: return 127; // Standardwert für unbekannte Ampere-Werte
  }
}

int DS3502Potentiometer::getCurrentWiperSetting() {
  uint8_t value;
  if (this->read_register(0x00, &value, 1)) {
    return value;
  }
  return -1; // Fehler beim Lesen
}

}  // namespace ds3502_potentiometer
}  // namespace esphome