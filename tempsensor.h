#ifndef TEMP_SENSOR_H
#define TEMP_SENSOR_H

#include <Arduino.h>

#define WRONG_TEMPERATURE    999.99

class TempSensor {
  private:
    OneWire *ow;
    byte addr[8];
    byte type_s;
    float lastTemp;
  public:
    TempSensor (byte addr[8], OneWire *ow);
    void print_addr ();

    float getCelsius () {return lastTemp;};

    void requestTemperature ();
    void readTemperature ();
};

#endif
