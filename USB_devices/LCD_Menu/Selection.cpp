#include "mbed.h"
#include "Selection.h"
//#include "SerialLCD.h"

Selection::Selection(void (*fun)(), int position, Menu *child, char* text) : 
    fun(fun), selText(text), pos(position), childMenu(child) {
    value=0x00;
}

Selection::Selection(uint8_t value, int position, Menu *child, char* text) : 
    value(value), selText(text), pos(position), childMenu(child) {
       fun=NULL;
}
    


