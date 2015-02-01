#include <Arduino.h>

#include "button.h"

Button::Button (int pin) {
  this->pin = pin;
  
  this->last_state = 1;
  this->state = 1;
  this->debounce = 0;
  
  pinMode (pin, INPUT);
  digitalWrite (pin, HIGH);
}

ButtonPress
Button::tick () {
  int dr = digitalRead (pin);
  if (dr != this->last_state) {
    debounce = millis ();
  } else {
    if (millis () - debounce > BUTTON_DEBOUNCE_DELAY) {
      if (state != dr) {
        state = dr;
        
        if (dr == LOW) {
          return BUTTON_PRESS_SINGLE;
        }
      } else {
        if (dr == LOW) {
          if (millis () - debounce > BUTTON_LONG_PRESS) {
            debounce = millis();
            return BUTTON_PRESS_LONG;
          }
        }
      }
    }
  }
  
  last_state = dr;
  return BUTTON_PRESS_NONE;
}

Switch::Switch (int pin) {
  this->pin = pin;
  
  pinMode (pin, INPUT);
  digitalWrite (pin, HIGH);
}

int
Switch::get_status () {
  return digitalRead (pin);
}
