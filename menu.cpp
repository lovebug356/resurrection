#include "menu.h"

#include "overview.h"

Menu::Menu (LiquidCrystal *lcd, OneWireBus *bus):
  heating (PIN_HEATING),
  all_off (A0),
  button_1 (A1),
  button_2 (A2),
  button_3 (A3),
  button_up (A5),
  button_down (A4)
{
  this->lcd = lcd;
  this->bus = bus;
  this->state = MENU_STATE_STARTUP;
  this->time = 0;
  
  eeprom_read_block ((void *)&this->set_temp, (const void*) ADDR_TEMP_SET, sizeof (double));
  if (isnan (this->set_temp)) this->set_temp = DEFAULT_TEMP_SET;
  eeprom_read_block ((void *)&this->temp_controller_active, (const void*) ADDR_TEMP_ACTIVE, sizeof (int));
  
  // Initialize Display
  view = new Overview (this);
  init_lcd ();
}

void
Menu::init_lcd () {
  lcd->begin (20, 4);
  view->get_last_view ()->redraw ();
}

void
Menu::check_heating () {
  double temp;
  
  if (!temp_controller_active || all_off.get_status ()) {
    if (heating.get_status()) {
      if (heating.set_status (false)) {
        goto beach;
      }
    }
    return;
  }

  temp = bus->getTempSensor(TEMP_PROBE_SET)->getCelsius();
  if (heating.get_status() && temp > (set_temp + TEMP_VARIANCE)) {
    if (heating.set_status (false)) goto beach;
  }
  
  if (!heating.get_status() && temp < (set_temp - TEMP_VARIANCE)) {
    if (heating.set_status (true)) goto beach;
  }

  return;

beach:
  init_lcd ();
}

void
Menu::tick () {
  View *next;
  next = view->get_last_view (); // TODO cache?
  
  next->button_click (BUTTON_1, button_1.tick ());
  next->button_click (BUTTON_2, button_2.tick ());
  next->button_click (BUTTON_3, button_3.tick ());
  next->button_click (BUTTON_UP, button_up.tick ());
  next->button_click (BUTTON_DOWN, button_down.tick ());
  
  if (next->status == VIEW_STATUS_HIDE) {
    View *parent = view->get_parent (next);
    delete next;
    parent->next = 0x00;
    next = parent;
    next->redraw();
  }
  
  if (millis () - time < 1000) return;
  time = millis();

  check_heating();
  next->update(); 
}

double
Menu::get_temp (int id) {
  TempSensor *sensor = bus->getTempSensor (id);
  if (sensor) return sensor->getCelsius();
  return WRONG_TEMPERATURE;
}

void
Menu::set_temp_set (double new_temp_set) {
  if (new_temp_set != set_temp) {
    set_temp = new_temp_set;
    eeprom_write_block ((void *)&this->set_temp, (void*) ADDR_TEMP_SET, sizeof (double));
  }
}

void
Menu::set_temp_controller_active (int new_active) {
  if (temp_controller_active != new_active) {
    temp_controller_active = new_active;
    eeprom_write_block ((void *)&this->temp_controller_active, (void*) ADDR_TEMP_ACTIVE, sizeof (int));
  }
}
