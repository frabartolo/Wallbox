#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/components/i2c/i2c.h"
#include "Adafruit_DS3502.h"

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
        this->set_wiper(0);
        ESP_LOGCONFIG(TAG, "DS3502 Potentiometer successfully set up.");
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
