#ifndef USBaudiocontrol_H
#define USBaudiocontrol_H

#include "USBHID.h"


/* Common usage */

/**
 *
 * @endcode
 */

class USBAudioControl: public USBHID
{
    public:
        
        /**
        *   Constructor
        *
        * @param vendor_id Your vendor_id (default: 0x1234)
        * @param product_id Your product_id (default: 0x0001)
        * @param product_release Your preoduct_release (default: 0x0001)
        *
        */
        USBAudioControl( uint16_t vendor_id = 0x1234, uint16_t product_id = 0x0006, uint16_t product_release = 0x0001): 
            USBHID(1, 1, 1, vendor_id, product_id, product_release, false)
            {   
            // NO suspend callback by default.     
                callbackSuspendChange=NULL;
            };
        
        
        /*
        * To define the report descriptor. Warning: this method has to store the length of the report descriptor in reportLength.
        *
        * @returns pointer to the report descriptor
        */
        virtual uint8_t * reportDesc();
        
        /* 
        * USBDevice does not ptovide a callback function for connection state changes
        *    This functions will poll the state
        * 
        * @returns 0/1 for the Connected state
        */
        unsigned int getConnectState();
        
        /*
        * User accessable Function pointer for callbacks
        */
        void (*callbackSuspendChange)(unsigned int connected);
        
        /*
        *
        * Provide override function for USBDevice function suspendStateChanged 
        *    USBDevice version of the function does nothing, the HIDAudiControl
        *    provides a use callback hool.
        */
        virtual void suspendStateChanged(unsigned int suspend);

};

#endif
