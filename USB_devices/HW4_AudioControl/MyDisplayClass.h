#ifndef MyDisplayClass_H
#define MyDisplayClass_H

#include "C12832_lcd.h"
#include "bargraph.h"

class MyDisplayClass {
public:
   MyDisplayClass(void);
   void update(int state);
   void UpdateTextStatus(int state);
   void UpdateStatus(int state);
   void setLevel(int level);
   void setMaxLevel(int level);
   bool graphicModeEnable;
   bool volumeDisplayEnable;
    
private:
    void drawSuspend(C12832_LCD *lcd);
    void drawConnection(C12832_LCD *lcd);
    void drawNoConnection(C12832_LCD *lcd);
    
    C12832_LCD  *lcd;
    bargraph   *volume;
};

#endif