#include "esphome.h"
#include <Adafruit_DS3502.h>

Adafruit_DS3502 ds3502 = Adafruit_DS3502();
#define WIPER_VALUE_PIN A0

class DS3502_Component : public Component, public FloatOutput
{
public:

    int I2C_Address;

    DS3502_Component(int address = DS3502_I2CADDR_DEFAULT) {
        I2C_Address = address;
    }

    void setup() override
    {
        Serial.begin(115200);
        // Wait until serial port is opened
        while (!Serial)
        {
            delay(1);
        }

        if (!ds3502.begin(I2C_Address))
        {
            ESP_LOGI("custom", "Couldn't find DS3502 chip");
            while (1);
        }
        ESP_LOGI("custom", "Found DS3502 chip");
    }

    void write_state(float state) override {
        int mappedState = map(int(state*100), 0, 100, 0, 127);
        ESP_LOGI("custom", "Setting wiper to <%i>", mappedState);
        ds3502.setWiper(mappedState);
    }
};