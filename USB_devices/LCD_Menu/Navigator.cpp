#include "Navigator.h"

Navigator::Navigator(Menu *root, C12832_LCD *lcd) : activeMenu(root), lcd(lcd) 
{
    bottom = root->selections.size();
    cursorPos = 0;
    cursorLine = 1;
    lastButton[0] = 0;
    lastButton[1] = 0;
    lastButton[2] = 0;
    
    setButtons();
    printMenu();
    printCursor();
}

DigitalIn pbUp(p15);
DigitalIn pbDown(p12);
DigitalIn pbSelect(p14);


void Navigator::setButtons()
{
}

void Navigator::printMenu()
{ 
    lcd->cls();
    lcd->locate(10,0);
    if(bottom == 1){ // the current Menu only has one selection
        lcd->printf("%s", activeMenu->selections[0].selText);
    } else {
        if(cursorLine == 2){ 
            // moving down in menu
            lcd->printf("%s", activeMenu->selections[cursorPos-1].selText);
            lcd->locate(10,13); // Sparkfun Serial LCD based on PIC16LF88
            lcd->printf("%s", activeMenu->selections[cursorPos].selText);
        } else {
            // moving up in menu
            lcd->printf("%s", activeMenu->selections[cursorPos].selText);
            lcd->locate(10,13); // Sparkfun Serial LCD based on PIC16LF88
            lcd->printf("%s", activeMenu->selections[cursorPos+1].selText);
        }
    }
}

void Navigator::printCursor()
{   
     
    lcd->locate(0,0); // Sparkfun Serial LCD based on PIC16LF88
    if(cursorLine == 1){
        lcd->printf(">");
        lcd->locate(0,13); // Sparkfun Serial LCD based on PIC16LF88
        lcd->printf(" ");
    } else if(cursorLine == 2){
        lcd->printf(" ");
        lcd->locate(0,13); // Sparkfun Serial LCD based on PIC16LF88
        lcd->printf(">");
    }
}

void Navigator::poll(uint8_t *returnValue)
{
    button[0]=pbUp;
    button[1]=pbDown;
    button[2]=pbSelect;
    
    if(button[0] == 1 && button[0] != lastButton[0]){
        moveUp();
    }
    if(button[1] == 1 && button[1] != lastButton[1]){
        moveDown();
    }
    if(button[2] == 1 && button[2] != lastButton[2]){ 
        if(activeMenu->selections[cursorPos].fun != NULL){
            (activeMenu->selections[cursorPos].fun)();
        }
        
        if(returnValue != NULL){
           *returnValue = activeMenu->selections[cursorPos].value;
        }
        
        if(activeMenu->selections[cursorPos].childMenu != NULL){
            activeMenu = activeMenu->selections[cursorPos].childMenu;
            bottom = activeMenu->selections.size();
            cursorPos = 0;
            cursorLine = 1;
        }
    }
    printMenu();
    printCursor();
    lastButton[0]=button[0];
    lastButton[1]=button[1];
    lastButton[2]=button[2];      
}

void Navigator::moveUp()
{
    if(cursorLine == 2){
        cursorLine = 1;
    }
    if(cursorPos != 0){
        cursorPos--;
    }
}

void Navigator::moveDown()
{
    if(cursorLine == 1){
        cursorLine = 2;
    }     
    if(cursorPos != (bottom-1)){
        cursorPos++;
    }
}
