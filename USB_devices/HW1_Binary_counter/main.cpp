//Load mbed basic function header
#include "mbed.h"

//Define the LEDs as a BUS 
BusOut myleds(LED1, LED2, LED3, LED4);
DigitalIn updown(p14);

//Define a global count variable
int cnt=0;
bool direction=false;

 
//Define a generic increment by X function. The functions to increment is passed as a pointer.  The increment amount is passed by value to the function
void IncrementValueByX(int *value, int x) {
    *value+=x;
} 

//Define a generic  wait function using the milliseconds, wait time is passed by value.
void WaitForTimeInMilliSeconds(int mseconds) {
    wait_ms(mseconds);
}

//Define a generic function to assign a nibble to a bus. The BUS is passed by reference and the value is passed 
//   by value.
void SetNibbleBusOut(BusOut *nibble,int value) {
       nibble->write(value & 0xF);
}

//Determine if a button has been released
bool CheckButtonRelease(int *buttonState,DigitalIn *inputPin) {
    int currentButton;
    bool retValue;
     
     currentButton=inputPin->read();
     if( *buttonState != currentButton && currentButton == 0) {
        retValue=true;
     } else {
        retValue=false;
     }
     *buttonState=currentButton; 
     return retValue;
}

//Invert a boolean based on a boolean value.
void InvertBoolBasedonValue(bool *direction, int buttonRelease){
   if(buttonRelease)
      *direction= !*direction;
}

//Change a boolean value based on button release
void ChangeDirectionOnButtonRelease(bool *direction, DigitalIn *inputPin){
    bool buttonRelease=0;
    static int buttonState;
    
    buttonRelease=CheckButtonRelease(&buttonState,inputPin);
    InvertBoolBasedonValue(direction,buttonRelease);
}


//Main loop
int main() {
    while(true) {
            ChangeDirectionOnButtonRelease(&direction,&updown);       
            if(direction) {
               IncrementValueByX(&cnt,1);
            } else {
               IncrementValueByX(&cnt,-1);
            }
            SetNibbleBusOut(&myleds,cnt);
            WaitForTimeInMilliSeconds(1000);
    }
}
