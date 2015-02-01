#include <LiquidCrystal.h>
#include <OneWire.h>

#include "OneWireBus.h"
#include "menu.h"

#define PIN_LIGHT_1    5
#define PIN_LIGHT_2    6
#define PIN_VENT       3

#define PIN_ONE_WIRE  7

LiquidCrystal lcd  (13, 12, 8, 9, 10, 11);
OneWireBus bus     (PIN_ONE_WIRE);
Menu   menu        (&lcd, &bus);

void setup(void) {
  // start serial port
//  Serial.begin(9600);
  
  pinMode (PIN_LIGHT_1, OUTPUT);
  pinMode (PIN_LIGHT_2, OUTPUT);
  pinMode (PIN_VENT, OUTPUT);
  
  digitalWrite (PIN_LIGHT_1, HIGH);
  digitalWrite (PIN_LIGHT_2, HIGH);
  digitalWrite (PIN_VENT, HIGH);

  // Scan all devices on the one wire interface
  bus.scan ();
}

void loop(void) {
  bus.tick();
  menu.tick();
}
