#include "mbed.h"
#include "C12832_lcd.h"
#include "Selection.h"
#include "Menu.h"
#include "Navigator.h"
#include <vector>
#include <string>

DigitalOut led1(LED1), led2(LED2), led3(LED3), led4(LED4);
 
C12832_LCD lcd;
 
// some functions to tie to selections
void toggleLED1() { led1 = !led1; }
void toggleLED2() { led2 = !led2; }
void toggleLED3() { led3 = !led3; }
void toggleLED4() { led4 = !led4; }
void resetLED()
{
    led1 = 0;
    led2 = 0;
    led3 = 0;
    led4 = 0;
}
 
int main() {
    // In using this library, the user declares "Menus", which are essentially arrays of "Selections".
    // Selections describe each individual selectable item. 
    // Selections can be tied to functions. Functions must output void and take in no arguments.
    // Selections can also have references to child menus.
    
    // It makes sense to declare a root menu first, but you don't have to. 
    // Menus should have an identifier (the argument in constructor). 
    Menu rootMenu("root"); 
    
    // Selections are added to menus through the Menu's "add" method.
    // If a function is to be executed when the RPG is depressed, make a REFERENCE to it. Otherwise null.
    // The second argument is its position - the will be deprecated soon.
    // The last is the text to display. *INCLUDE A SPACE IN THE BEGINNING, this is where the cursor goes. 
    // *This means the text is limited to 14 characters (not counting the space) with this display. 
    // **It is 14 not 15 for reasons pertaining to the implementation of the TextLCD library. 
    Menu testMenu("Test menu"); 
    testMenu.add(Selection(&toggleLED1, 1, NULL, "Get All Images")); // The function argument of selection can be added directly
    testMenu.add(Selection(&toggleLED2, 0, NULL, "Get Number of Objects"));
    testMenu.add(Selection(&toggleLED3, 2, NULL, "Get Number of Images"));
    testMenu.add(Selection(&toggleLED4, 3, NULL, "Get Number of Thumbnails"));
    testMenu.add(Selection(&resetLED, 4, &rootMenu, "  Go back"));  // always add a Selection at the end to point to the parent
    
    Menu aboutMenu("About Menu"); // about menu crediting us :)
    aboutMenu.add(Selection(NULL, 0, NULL, "Author:"));
    aboutMenu.add(Selection(NULL, 1, NULL, " Dwayne S Dilbeck"));
    aboutMenu.add(Selection(NULL, 2, NULL, " 8/29/2013"));
    aboutMenu.add(Selection(NULL, 3, NULL, " USB Device Interface:"));
    aboutMenu.add(Selection(NULL, 4, NULL, " Architecture,"));
    aboutMenu.add(Selection(NULL, 5, NULL, " Protocols,"));
    aboutMenu.add(Selection(NULL, 6, NULL, " and programming."));
    aboutMenu.add(Selection(NULL, 7, &rootMenu, "  Go back"));
   
    // Selections to the root menu should be added last
    rootMenu.add(Selection(NULL, 0, &testMenu, "TEST MENU"));
    // rootMenu.add(Selection(NULL, 1, NULL, " Dummy menu 1"));  // a dummy menu, doesn't do anything
    rootMenu.add(Selection(NULL, 1, &aboutMenu, "About menu"));   
    
    // Here is the heart of the system: the navigator. 
    // The navigator takes in a reference to the root, an interface, and a reference to an lcd
    Navigator navigator(&rootMenu, &lcd);
    
    while(1){
        navigator.poll(); 
    }
}




