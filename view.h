#ifndef __VIEW_H_
#define __VIEW_H_

#include "button.h"

enum ViewStatus {
  VIEW_STATUS_OK,
  VIEW_STATUS_HIDE
};

class View {
  private:
  public:
    View *next;
    ViewStatus status;

    View ();
    
    View* get_last_view ();
    View* get_parent (View *child);
    
    virtual void redraw();
    virtual void update();
    
    virtual void button_click(int id, ButtonPress button_press);
};

#endif
