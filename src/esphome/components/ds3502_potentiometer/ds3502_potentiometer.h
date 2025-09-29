#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/components/i2c/i2c.h"
#include "Adafruit_DS3502.h"
#include <map>

namespace esphome
{
  namespace ds3502_potentiometer
  {

    static const char *const TAG = "ds3502_potentiometer";

    class DS3502Potentiometer : public Component, public i2c::I2CDevice
    {
    public:
      void setup() override
      {
        ESP_LOGCONFIG(TAG, "Setting up DS3502 Potentiometer...");
        this->potentiometer_ = new Adafruit_DS3502();
        if (!this->potentiometer_->begin(this->address_))
        {
          ESP_LOGE(TAG, "Could not find DS3502 at address 0x%02X", this->address_);
          this->mark_failed();
          return;
        }
        // Setze den Initialwert auf 12A
        this->set_wiper(amps_to_wiper(12));
        ESP_LOGCONFIG(TAG, "DS3502 Potentiometer successfully set up.");
      }

      int amps_to_wiper(int amps)
      {
        if (amps >= 64)
          return 127; // Minimaler Widerstand für maximalen Strom
        if (amps >= 50)
          return 110;
        if (amps >= 48)
          return 105;
        if (amps >= 45)
          return 98;
        if (amps >= 40)
          return 90;
        if (amps >= 36)
          return 85;
        if (amps >= 32)
          return 80;
        if (amps >= 27)
          return 70;
        if (amps >= 24)
          return 60;
        if (amps >= 20)
          return 50;
        if (amps >= 16)
          return 40;
        if (amps >= 12)
          return 30;
        if (amps >= 10)
          return 20;
        return 0; // Standardwert für niedrigsten Strom
      }

      bool set_wiper(int value)
      {
        if (value < 0 || value > 127)
        {
          ESP_LOGE(TAG, "Wiper value %d is out of range (0-127).", value);
          return false;
        }
        this->potentiometer_->setWiper(value);
        this->last_wiper_ = value;
        ESP_LOGD(TAG, "Successfully set wiper to %d", value);
        return true;
      }

      int get_wiper_value()
      {
        return this->last_wiper_;
      }

      void set_address(uint8_t address) { this->address_ = address; }

    protected:
      Adafruit_DS3502 *potentiometer_;
      int last_wiper_ = -1;
    };

  } // namespace ds3502_potentiometer
} // namespace esphome
