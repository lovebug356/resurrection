#include "overview.h"

#include <Arduino.h>

int offset = 0;

Overview::Overview (Menu *menu) {
  this->menu = menu;
  
  this->last_air_temp = 0;
  this->last_ground_temp = 0;
  this->last_sand_temp = 0;
  this->last_heating_on = 3;
}

void write_temp_line (LiquidCrystal *lcd, int xpos, int ypos, char *name, double temp) {
  lcd->setCursor (xpos, ypos);
  lcd->print (name);
  lcd->print (": ");
  lcd->print (temp);
  lcd->print ((char)223);
  lcd->print ("C");
}

void write_temp (LiquidCrystal *lcd, int xpos, int ypos, double temp) {
  lcd->setCursor (xpos, ypos);
  lcd->print (temp);
  lcd->print ((char)223);
  lcd->print ("C ");
}

void
Overview::redraw () {
  menu->lcd->clear ();
  
  if (next) {
    delete next;
    next = 0x00;
  }
  
  // Write temperatures
  write_temp_line (menu->lcd, 0, 0, "Lucht", menu->get_temp (0));
  write_temp_line (menu->lcd, 0, 1, "Grond", menu->get_temp (2));
  write_temp_line (menu->lcd, 0, 2, "zand ", menu->get_temp (1));

  // Write Menu items
  menu->lcd->setCursor (0, 3);
  if (!next) 
    menu->lcd->print ("TEMP | LICHT  | VENT");
  
  update();
}

void
Overview::update () {
  double temp = menu->get_temp (0);
  if (temp != last_air_temp - offset) {
    write_temp (menu->lcd, 7, 0, temp + offset);
    last_air_temp = temp;
  }
  temp = menu->get_temp (1);
  if (temp != last_sand_temp) {
    write_temp (menu->lcd, 7, 2, temp);
    last_sand_temp = temp;
  }
  temp = menu->get_temp (2);
  if (temp != last_ground_temp) {
    write_temp (menu->lcd, 7, 1, temp);
    last_ground_temp = temp;
  }
  
  // Write heating on
  menu->lcd->setCursor (16, 2);
  if (menu->heating.get_status()) {
    menu->lcd->print ("AAN");
  } else {
    menu->lcd->print ("UIT");
  }
}

void
Overview::button_click(int id, ButtonPress button_press) {
  if (button_press == BUTTON_PRESS_NONE) return;
  
  if (id == BUTTON_1) {
    next = new ConfigTempView (menu);
    next->redraw();
  }
}

ConfigTempView::ConfigTempView (Menu *menu) {
  this->menu = menu;
  
  this->set_temp = menu->set_temp;
  this->temp_controller_active = menu->temp_controller_active;
  this->idx = 0;
}

byte char_right[8] = {
  B10000,
  B01100,
  B00010,
  B00001,
  B00010,
  B01100,
  B10000,
};

byte char_up[8] = {
  B00000,
  B00100,
  B01010,
  B10101,
  B00100,
  B00100,
  B00000,
};

byte char_down[8] = {
  B00000,
  B00100,
  B00100,
  B10101,
  B01010,
  B00100,
  B00000,
};

void
ConfigTempView::redraw () {
  menu->lcd->createChar (0, char_right);
  menu->lcd->createChar (1, char_up);
  menu->lcd->createChar (2, char_down);
  menu->lcd->clear ();
  // 1. Set temperatuur
  menu->lcd->setCursor (1, 0);
  menu->lcd->print ("Temperatuur:");
  write_temp (menu->lcd, 13, 0, set_temp);
  // 2. Controller aan/uit
  menu->lcd->setCursor (1, 1);
  menu->lcd->print ("Controller:");
  menu->lcd->setCursor (17, 1);
  if (temp_controller_active) {
    menu->lcd->print ("AAN");
  } else {
    menu->lcd->print ("UIT");
  }
  
  menu->lcd->setCursor (0, 3);
  menu->lcd->print ("TERUG |  ");
  menu->lcd->write (byte (1));
  menu->lcd->write (byte (2));  
  menu->lcd->print ("  |  OK");
  
  menu->lcd->setCursor (0, idx);
  menu->lcd->write (byte(0));
}

void
ConfigTempView::update () {
}

void
ConfigTempView::button_click(int id, ButtonPress button_press) {
  if (button_press == BUTTON_PRESS_NONE) return;
  
  if (id == BUTTON_UP) {
    if (idx == 0)
      if (set_temp < 30) set_temp += (button_press == BUTTON_PRESS_SINGLE ? 0.1 : 1.0);
    if (idx == 1)
      temp_controller_active = !temp_controller_active;
    redraw();
  }
  
  if (id == BUTTON_DOWN) {
    if (idx == 0)
      if (set_temp > 10) set_temp -= (button_press == BUTTON_PRESS_SINGLE ? 0.1 : 1.0);
    if (idx == 1)
      temp_controller_active = !temp_controller_active;
    redraw();
  }
  
  if (id == BUTTON_2) {
    if(++idx == 2) idx = 0;
    redraw();
  }
  
  if (id == BUTTON_1) {
    status = VIEW_STATUS_HIDE;
  }
  
  if (id == BUTTON_3) {
    menu->set_temp_set (set_temp);
    menu->set_temp_controller_active (temp_controller_active);
    status = VIEW_STATUS_HIDE;
  }
}
