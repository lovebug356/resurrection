#ifndef __BUTTON_H_
#define __BUTTON_H_

#define BUTTON_DEBOUNCE_DELAY    50
#define BUTTON_LONG_PRESS        500

enum ButtonType {
  BUTTON_1,
  BUTTON_2,
  BUTTON_3,
  BUTTON_UP,
  BUTTON_DOWN,
};

enum ButtonPress {
  BUTTON_PRESS_NONE,
  BUTTON_PRESS_SINGLE,
  BUTTON_PRESS_LONG
};

class Button {
  private:
    int last_state;
    int state;
    unsigned long debounce;
    
    int pin;
    
  public:
    Button (int pin);
    
    ButtonPress tick();
};

class Switch {
  private:
    int pin;

  public:
    Switch (int pin);
    int get_status ();
};

#endif
