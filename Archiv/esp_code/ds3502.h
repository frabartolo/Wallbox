#ifndef DS3502_H
#define DS3502_H

#include <Wire.h>
#include <cstdint>

class DS3502 {
  public:
    void begin();
    void setWiper(uint8_t value);
    void setWiperForAmps(int amps);
    int getCurrentSetting();
  private:
    static constexpr byte I2C_ADDRESS = 0x28;
    int ampsToWiper(int amps);
};

#endif
