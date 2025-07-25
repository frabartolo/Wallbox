#include "ds3502.h"

void DS3502::begin() {
  Wire.begin();
}

void DS3502::setWiper(byte value) {
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(0x00); // Wiper register
  Wire.write(value);
  Wire.endTransmission();
}

int DS3502::ampsToWiper(int amps) {
  switch (amps) {
    case 64: return 0;
    case 50: return 20;
    case 32: return 40;
    case 24: return 60;
    case 16: return 80;
    case 12: return 100;
    case 8:  return 127;
    default: return 127;
  }
}

void DS3502::setWiperForAmps(int amps) {
  setWiper(ampsToWiper(amps));
}

int DS3502::getCurrentSetting() {
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.requestFrom(I2C_ADDRESS, 1);
  return Wire.read();
}
