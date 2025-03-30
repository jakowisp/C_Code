#include "mbed.h"

#include "USBAudioControl.h"
#include "MyDisplayClass.h"

//Define Key words for the special disconnect/connect functionality
#define PAUSEKEY 0x10
#define NUMSECONDSTOHOLD 5

//Joystick array for the audio control functions
BusIn btnArray(p15,p12,p13,p16,p14);
//Display LED to provide feedback during the hold button sequence
DigitalOut toggle(LED1);

//Group all the display items in a class. This class creates a LCD object, and bargraph object
MyDisplayClass display;

//Set up the USB HID device
USBAudioControl hid(0x1234,0x0006,0x0001);
//temporary structure to hold data before filling the input report
HID_REPORT tempReport(1);


//Variable to hold USB state
// Bit 0: suspended=0,normal=1
// Bit 1: 1=USB configured, 0 USB not configured
unsigned int USBstate=0;

//Callback funciton: When an Output Report is received, set the current volume level.
void SetLevel(HID_REPORT *report){
    display.setLevel(report->data[0]);   
}

//Callback Function: When a feature report is sent, change the feature bits, and the scale
void SetFeatures(HID_REPORT *report){
    display.volumeDisplayEnable=((report->data[1]&0x10)>>4)==0x1;
    display.graphicModeEnable=((report->data[1]&0x20)>>5)==0x1;   
    display.setMaxLevel((report->data[1]&0x0f)<<2);
}

//Function to write the USB configured bit, this is polled.
void WriteConnectedBit(unsigned int connected){
    USBstate = (connected << 1) | (USBstate & 0x1);
}

//Callback function: When the Suspend bit is changed, update USBstate
void WriteSuspendBit(unsigned int suspend){
    USBstate = (suspend) | (USBstate & 0x2);
}

//Fill a temp report with the button array state 
void PollInputs(HID_REPORT *tempReport) {
   tempReport->data[0]=btnArray;
}

//Provide a check for a special button combination to connect and disconnect.
void CheckForHeldPauseKey(HID_REPORT * tempreport, USBAudioControl * hid, DigitalOut * toggle){
 static int holdCount=0;
     
     //Provide Feedback to user holding the key by flashing an LED when the key is held
     *toggle=(holdCount>0)?(holdCount /5)%2 :0;
     //Count how long the PAUSEKEY is held
     if(tempReport.data[0]==PAUSEKEY) {
        holdCount++;     
     } else
        holdCount=0;
     //If Key is held # of seconds, The hid will diconnect/connect the USB device
     if(holdCount>=NUMSECONDSTOHOLD*10) {
        holdCount=0;
       if(hid->getConnectState())  { 
         hid->disconnect();
       } else
         hid->connect();
     }
}


//Main Function
int main() {
   
   //Assign callback functions for USB events
    hid.callbackSetOutputReport=&SetLevel;
    hid.callbackSetFeatureReport=&SetFeatures;
    hid.callbackSuspendChange=&WriteSuspendBit;
    
    
    //Main Loop
    while(1) {
     //Poll Inputs
     PollInputs(&tempReport);
     //Poll USB Configured
     WriteConnectedBit(hid.getConnectState());
     //Hidden Configureation option
     CheckForHeldPauseKey(&tempReport,&hid,&toggle);
     //Set up the input data for a send
     hid.FillInputReport(&tempReport);
     //Update the display
     display.update(USBstate); 
     wait(0.1);
    }
}



