#ifndef SELECTION_H
#define SELECTION_H

#include "Menu.h"

class Menu; 

class Selection {
    private:
        
    public:
        void (*fun)();   // pointer to a function to execute 
        uint8_t value;
        char* selText;   // selection text
        int pos;         // selection position
        Menu *childMenu; 
        
        Selection(void (*)(), int, Menu *, char *); 
        Selection(uint8_t, int, Menu *, char *); 
};

#endif 