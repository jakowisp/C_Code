#include "mbed.h"
#include "Menu.h"
#include "Selection.h"

Menu::Menu(char *id) : menuID(id)
{
    selections.clear();
}

void Menu::add(const Selection &toAdd)
{
    selections.push_back(toAdd);
}


