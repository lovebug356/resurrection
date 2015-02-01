#include "onewirebus.h"

OneWireBus::OneWireBus (uint8_t pin) {
  ow = new OneWire (pin);
  addr_count = 0;
  tick_id = 0;
  tick_state = TICK_STATE_REQUEST;
  for (int i=0; i<ONE_WIRE_BUS_MAX_DEVICES; i++)
    sensors[i] = NULL;
}

void OneWireBus::scan () {
  byte device_found;

  ow->reset_search ();
  addr_count = 0;

  do {
    device_found = ow->search (addr[addr_count]);
    if (device_found) {
      addr_count += 1;
    }
  } while (device_found);
}

void OneWireBus::dump () {
  Serial.println ("OneWireBus:");
  Serial.print ("Devices found:");
  Serial.println (addr_count);
  for (int j=0; j<addr_count; j++) {
    Serial.print("\t(");
    Serial.print(j);
    Serial.print(") R:");
    for(int i = 0; i < 8; i++) {
      Serial.print(addr[j][i], HEX);
      Serial.print(" ");
    }

    if ( OneWire::crc8( addr[j], 7) != addr[j][7]) {
        Serial.print("CRC is not valid!\n");
        return;
    }

    if ( addr[j][0] == 0x10) {
        Serial.print("Device is a DS18S20 family device.\n");
    }
    else if ( addr[j][0] == 0x28) {
        Serial.print("Device is a DS18B20 family device.\n");
    }
    else {
        Serial.print("Device family is not recognized: 0x");
        Serial.println(addr[j][0],HEX);
        return;
    }
  };
}

void OneWireBus::tick () {
  if (tick_state == TICK_STATE_REQUEST) {
    getTempSensor (tick_id)->requestTemperature();
    tick_delay = millis();
    tick_state = TICK_STATE_WAITING;
    if (tick_delay + ONE_WIRE_DELAY_BETWEEN_DEVICES < ONE_WIRE_DELAY_BETWEEN_DEVICES) {
      tick_delay = 0;
    }
  } else if (tick_state == TICK_STATE_WAITING) {
    if (millis () - tick_delay > ONE_WIRE_DELAY_BEFORE_READ) {
      tick_state = TICK_STATE_READ;
    }
  } else if (tick_state == TICK_STATE_READ) {
    getTempSensor (tick_id)->readTemperature();

    tick_id += 1;
    if (tick_id == addr_count) tick_id = 0;
    tick_state = TICK_STATE_DONE;
    tick_delay = millis ();
    if (tick_delay + ONE_WIRE_DELAY_BETWEEN_DEVICES < ONE_WIRE_DELAY_BETWEEN_DEVICES) {
      tick_delay = 0;
    }
  } else if (tick_state == TICK_STATE_DONE) {
    if (millis () - tick_delay > ONE_WIRE_DELAY_BETWEEN_DEVICES) {
      tick_state = TICK_STATE_REQUEST;
    }
  }
}

TempSensor *OneWireBus::getTempSensor (int id) {
  if (id >= addr_count)
    return NULL;

  if (sensors[id] != NULL)
    return sensors[id];

  sensors[id] = new TempSensor (addr[id], ow);
  return sensors[id];
}

TempSensor *OneWireBus::getTempSensorByAddr (byte addr[8]) {
  for (int id=0; id<addr_count; id++) {
    byte match = true;
    for (int i=0; i<8; i++) {
      if (addr[i] != this->addr[id][i]) {
        match = false;
        break;
      }
    }
    if (!match)
      continue;
    return getTempSensor (id);
  }
  return NULL;
}

void OneWireBus::detect_finger_reset () {
  for (int i=0; i<ONE_WIRE_BUS_MAX_DEVICES; i++)
      finger_temp[i] = WRONG_TEMPERATURE;
}

byte OneWireBus::detect_finger_scan (TempSensor **sensor) {
  for (int i=0; i<addr_count; i++) {
    TempSensor *ts = getTempSensor (i);

    if (finger_temp[i] == WRONG_TEMPERATURE) {
      finger_temp[i] = ts->getCelsius ();
    }

    if (ts->getCelsius () - finger_temp[i] > ONE_WIRE_DETECT_FINGER_DIFF) {
      *sensor = ts;
      return true;
    }
  }
  return false;
}
