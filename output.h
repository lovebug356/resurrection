#ifndef __HEATING_H_
#define __HEATING_H_

class Output {
  private:
    int pin;
    int status;

  public:
    Output (int pin);
    int get_status () {return status;};
    int set_status (int new_status);
};

#endif
