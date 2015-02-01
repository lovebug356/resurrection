#include "tempsensor.h"

TempSensor::TempSensor (byte addr[8], OneWire *ow) {
  for (int i=0; i<8; i++)
    this->addr[i] = addr[i];
  this->ow = ow;
  this->lastTemp = WRONG_TEMPERATURE;

  switch (addr[0]) {
    case 0x10:
//      Serial.println("  Chip = DS18S20");  // or old DS1820
      type_s = 1;
      break;
    case 0x28:
//      Serial.println("  Chip = DS18B20");
      type_s = 0;
      break;
    case 0x22:
//      Serial.println("  Chip = DS1822");
      type_s = 0;
      break;
    default:
      Serial.println("Device is not a DS18x20 family device.");
      lastTemp = WRONG_TEMPERATURE;
  }
};

void TempSensor::print_addr () {
  for (int i=0; i<8; i++) {
    Serial.print (addr[i], HEX);
    Serial.print (" ");
  }
}

void TempSensor::requestTemperature () {
  ow->reset();
  ow->select(addr);
  ow->write(0x44, 1);        // start conversion, with parasite power on at the end
}

void TempSensor::readTemperature () {
  byte i;
  byte present = 0;
  byte data[12];

  present = ow->reset();
  ow->select(addr);
  ow->write(0xBE);         // Read Scratchpad

  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ow->read();
  }

  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }

  lastTemp = (float)raw / 16.0;
}
