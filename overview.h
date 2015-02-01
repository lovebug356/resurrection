#ifndef __OVERVIEW_H_
#define __OVERVIEW_H_

#include "view.h"
#include "menu.h"

class Overview: public View {
  private:
    Menu* menu;
    
    double last_air_temp;
    double last_ground_temp;
    double last_sand_temp;
    double last_heating_on;

  public:
    Overview (Menu *menu);
    
    virtual void redraw();
    virtual void update();
    virtual void button_click(int id, ButtonPress button_press);
};

class ConfigTempView: public View {
  private:
    Menu *menu;
    
    int idx;
    double set_temp;
    int temp_controller_active;
    
  public:
    ConfigTempView (Menu *menu);
    
    virtual void redraw();
    virtual void update();
    virtual void button_click(int id, ButtonPress button_press);
};

#endif
