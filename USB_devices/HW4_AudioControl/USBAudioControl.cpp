#include "stdint.h"
#include "USBAudioControl.h"

//Override the USBDevice suspendStateChange function to provide callback hook
void USBAudioControl::suspendStateChanged(unsigned int suspend){
    if(callbackSuspendChange!=NULL)
      (*callbackSuspendChange)(suspend);
}

//Provide a way to poll the configure state of USB device
unsigned int USBAudioControl::getConnectState(){
   if (device.state != CONFIGURED)
       return 0;
   return 1;
}

//Define the report descriptor
uint8_t * USBAudioControl::reportDesc() {
       static uint8_t reportDescriptor[] = {
            USAGE_PAGE(2),      0x0c,0x00,       // Consumer
            USAGE(1),           0x01,       // Remote Control
            COLLECTION(1),      0x01,       // Application
            LOGICAL_MINIMUM(1),     0x00,
            LOGICAL_MAXIMUM(1),     0x01,
    
            USAGE(2),           0xe9,0x00,       // Vol+
            USAGE(2),           0xea,0x00,       // Vol-
            USAGE(2),           0xb5,0x00,       // NextTrack
            USAGE(2),           0xb6,0x00,       // PrevTrack
            USAGE(2),           0xcd,0x00,       // Pause/play
            REPORT_COUNT(1),    0x05,
            REPORT_SIZE(1),     0x01,
            INPUT(1),           0x02,
            REPORT_COUNT(1),    0x03,
            REPORT_SIZE(1),     0x01,
            INPUT(1),           0x01,
            LOGICAL_MINIMUM(1), 0x00,                // logical minimum = 0
            LOGICAL_MAXIMUM(2), 0xFF, 0x00,          // logical maximum = 255
            REPORT_SIZE(1),     0x08,
            REPORT_COUNT(1),    0x01,                   // report count
            USAGE(1),           0x02,                          // usage
            OUTPUT(1),          0x02,                         // Output (array)
            REPORT_COUNT(1),    0x01,
            USAGE(1),           0x03,
            FEATURE(2),         0x02, 0x01,
            END_COLLECTION(0),
        };
        reportLength = sizeof(reportDescriptor);
        return reportDescriptor;
}



