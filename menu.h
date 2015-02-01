#ifndef __MENU_H__
#define __MENU_H__

#include <LiquidCrystal.h>
#include <avr/eeprom.h>

#include "onewirebus.h"
#include "output.h"
#include "button.h"
#include "view.h"

// Output Pins
#define PIN_HEATING    4
#define PIN_LIGHT_1    5
#define PIN_LIGHT_2    6
#define PIN_VENT       3

// Input Pins
#define PIN_SWITCH    A0
#define PIN_BUTTON_1  A1
#define PIN_BUTTON_2  A2
#define PIN_BUTTON_3  A3
#define PIN_BUTTON_4  A4
#define PIN_BUTTON_5  A5

// Temperature control
#define DEBOUNCE_DELAY    50
#define DEFAULT_TEMP_SET          (22.2)
#define TEMP_PROBE_SET            (1)
#define TEMP_VARIANCE             (0.3)

// EEPROM Settings
#define ADDR_TEMP_SET             0x0
#define ADDR_TEMP_SET_SIZE        (sizeof (double))
#define ADDR_TEMP_ACTIVE          (ADDR_TEMP_SET + ADDR_TEMP_SET_SIZE)
#define ADDR_TEMP_ACTIVE_SIZE     (sizeof (int))

enum MenuState {
  MENU_STATE_STARTUP,
  MENU_STATE_OVERVIEW,
  MENU_STATE_CONFIG_TEMP
};

class Menu {
  public:
    LiquidCrystal *lcd;
    OneWireBus *bus;
    int state;
    unsigned long time;

    // Buttons
    Switch all_off;
    Button button_1;
    Button button_2;
    Button button_3;
    Button button_up;
    Button button_down;

    void check_button_1();
    void check_button_2();
    void check_button_3();
    void check_button_up();
    void check_button_down();

    // Output
    Output heating;

    // View
    View *view;

    // Temp controller
    int temp_controller_active;
    double set_temp;
    void check_heating ();

  public:
    Menu (LiquidCrystal *lc, OneWireBus *bus);

    double get_temp (int id);

    void set_temp_set (double new_temp_set);
    void set_temp_controller_active (int new_active);

    void init_lcd ();
    void tick ();
};

#endif
