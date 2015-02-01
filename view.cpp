#include "view.h"

#include <Arduino.h>

View::View () {
  this->next = 0x00;
  this->status = VIEW_STATUS_OK;
}

View *
View::get_last_view () {
  if (this->next == 0x00) return this;
  return this->next->get_last_view ();
}

View *
View::get_parent (View *child) {
  if (this->next == child) return this;
  if (this->next) return this->next->get_parent (child);
  return 0x00;
}

void
View::redraw () {
}

void
View::update () {
  redraw();
}

void
View::button_click(int id,  ButtonPress button_press) {
}
