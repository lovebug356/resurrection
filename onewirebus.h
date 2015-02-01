#ifndef ONE_WIRE_BUS_H
#define ONE_WIRE_BUS_H

#include <Arduino.h>
#include <OneWire.h>

#include "tempsensor.h"

#define ONE_WIRE_BUS_MAX_DEVICES           5
#define ONE_WIRE_DELAY_BEFORE_READ         1000
#define ONE_WIRE_DELAY_BETWEEN_DEVICES     1000
#define ONE_WIRE_DETECT_FINGER_DIFF        2

#define TICK_STATE_REQUEST   1
#define TICK_STATE_WAITING   2
#define TICK_STATE_READ      3
#define TICK_STATE_DONE      4

class OneWireBus {
private:
  OneWire *ow;
  byte addr[ONE_WIRE_BUS_MAX_DEVICES][8];
  byte addr_count;
  TempSensor *sensors[ONE_WIRE_BUS_MAX_DEVICES];

  // detect finger
  float finger_temp[ONE_WIRE_BUS_MAX_DEVICES];

  // tick management
  byte tick_id;
  byte tick_state;
  unsigned long tick_delay;
public:
  OneWireBus (uint8_t pin);

  void scan ();
  int getDeviceCount () {return addr_count;};
  TempSensor *getTempSensor (int id);
  TempSensor *getTempSensorByAddr (byte addr[8]);
  void dump ();

  // Detect a sensor by touching it with your finger
  void detect_finger_reset ();
  byte detect_finger_scan (TempSensor **sensor);

  void tick ();
};

#endif
