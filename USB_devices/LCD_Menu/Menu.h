#ifndef MENU_H
#define MENU_H

#include "mbed.h"
#include "Selection.h"
#include <vector>

class Selection;

class Menu {
    private:
               
    public:
        vector<Selection> selections;
        char *menuID;
        
        Menu(char *);
        
        void add(const Selection &toAdd);
        char *getText(int);
};
#endif