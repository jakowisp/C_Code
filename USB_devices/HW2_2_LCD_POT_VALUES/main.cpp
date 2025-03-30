
#include "mbed.h"
#include "C12832_lcd.h"
 
DigitalIn button(p21);
AnalogIn potA(p20);
AnalogIn potB(p19);
C12832_LCD lcd;
 
int main() {

    while(potA < 0.999 && potB < 0.999) {
        lcd.cls();
        lcd.locate(0,3);
        lcd.printf("Pot A:%d, Pot B:%d\n", potA.read_u16(),potB.read_u16());
        wait(0.1);
    }
    lcd.cls();
    lcd.locate(10,3);
    lcd.printf("Loop unexpectedly");
    lcd.locate(40,12);
    lcd.printf("terminated");
    error("Loop unexpectedly terminated");
}