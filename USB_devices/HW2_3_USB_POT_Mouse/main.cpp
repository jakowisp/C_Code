#include "mbed.h"
#include "USBMouse.h"

struct positionXY {
   int x;
   int y;
   }  mousePosition;

USBMouse mouse(ABS_MOUSE);
AnalogIn potA(p20);
AnalogIn potB(p19);

BusIn button(p12,p15,p13);
int lastButtonState=0,buttonChanged=0,buttonState=0;

void DetermineMousePressReleaseAndSend(int buttonState) {
    buttonChanged =  buttonState ^ lastButtonState;
    mouse.press(buttonState & buttonChanged);
    mouse.release(!buttonState & buttonChanged);
}

int ReadAnalogValueAndShift(AnalogIn *pot) {
      return  pot->read_u16() >> 1;
}   

void DetermineXAndY(struct positionXY *position) {
    position->x=  ReadAnalogValueAndShift(&potA);
    position->y=  ReadAnalogValueAndShift(&potB);     
}

int main() {
    while (1) {
        DetermineXAndY(&mousePosition);
        mouse.move(mousePosition.x, mousePosition.y);
        buttonState =  button.read() & 0x7;
        DetermineMousePressReleaseAndSend(buttonState);
        wait(0.001);
    }
}
