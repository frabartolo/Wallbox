#ifndef DS3502_H
#define DS3502_H

#include <Wire.h>

class DS3502 {
  public:
    void begin();
    void setWiper(byte value);
    void setWiperForAmps(int amps);
    int getCurrentSetting();
  private:
    const byte I2C_ADDRESS = 0x28;
    int ampsToWiper(int amps);
};

#endif
