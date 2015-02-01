#include <Arduino.h>

#include "output.h"

Output::Output (int p) {
  this->pin = p;
  this->status = false;

  pinMode (this->pin, OUTPUT);
  digitalWrite (this->pin, HIGH);
}

int
Output::set_status (int new_status) {
  if (status == new_status) {
    return false;
  }

  if (new_status == true) {
    digitalWrite (this->pin, LOW);
    this->status = true;
    return true;
  } else {
    digitalWrite (this->pin, HIGH);
    this->status = false;
    return false;
  }
}
