#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include "mbed.h"
#include "Menu.h"
#include "C12832_lcd.h"

class Navigator {   
    private: 
               
    public:
        Navigator(Menu *, C12832_LCD *);
        Menu *activeMenu; // the current menu - can change when RPG is pushed on selection with child menu
        
        C12832_LCD *lcd; 
        bool lastButton[3], button[3];
        
        
        int bottom;     // the index of the last item of current menu
        int cursorPos;  // what selection the cursor points to
        int cursorLine; // what line of the lcd the cursor is on. 1 = first line, 2 = second line
        
        void poll(uint8_t *returnValue=NULL);    // repeatedly call this function to determine if RPG is being used
        void moveUp();
        void moveDown();
        void printMenu();
        void printCursor();
        void setButtons();
};

#endif 