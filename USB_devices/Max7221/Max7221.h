/**
* @file Max7221.h
* @brief This file contains the class defnition of Max7221 and define statements related to the class, and max7221 device
* The methods in this class are derived from posts on the mbed forum written by Igor Skochinsky on October 2009
* 
* @author Dwayne S. Dilbeck
*
* @date 8/6/2013
*/
#ifndef Max7221_H
#define Max7221_H


// define max7219/max7221 registers
#define max7219_reg_noop         0x00
#define max7219_reg_digit0       0x01
#define max7219_reg_digit1       0x02
#define max7219_reg_digit2       0x03
#define max7219_reg_digit3       0x04
#define max7219_reg_digit4       0x05
#define max7219_reg_digit5       0x06
#define max7219_reg_digit6       0x07
#define max7219_reg_digit7       0x08
#define max7219_reg_decodeMode   0x09
#define max7219_reg_intensity    0x0a
#define max7219_reg_scanLimit    0x0b
#define max7219_reg_shutdown     0x0c
#define max7219_reg_displayTest  0x0f

#define LOW 0
#define HIGH 1
#define MHZ 1000000
#define NUMDIGITS 8

#ifdef NUMDIGITS 
#define UPPERBOUND 99999999
#define LOWERBOUND -9999999
#endif

/**
 * Max7221 Example
 * @code
 * #include "mbed.h"
 * #include "Max7221.h"
 *  
 * 
 * // p5: DIN, p7: CLK, p8: LOAD/CS
 * Max7221 max7221disp1(p5, p7, p8);
 * //Max7221 max7221disp2(p5, p7, p8);
 * //Max7221 max7221disp3(p11, p13, p14);
 * //Max7221 max7221disp4(p11, p13, p14);
 * 
 * int count=-99;
 * 
 * void loop(void) {
 *    max7221disp1.WriteInt(count);
 *    if (count < 100)
 *       count=count+1;
 *    else 
 *      count=-99; 
 * }
 * 
 * int main() {  
 *     max7221disp1.Setup();
 *     //Max7221::SetupALl();
 *     max7221disp1.WriteFloat(123.125);
 *     wait(1.0);  
 * 
 *     while (1) {
 *         loop(); 
 *         wait(1.0);
 *     }
 * }
 * @endcode
 */
class Max7221 {
public:
    /**
    *  Constructor. This is the default constructor
    *  @author Dwayne S. Dilbeck
    *  @param msoi  The SPI pin used as input to the device.       Valid values for LPC1768 are p5 or p11
    *  @param mclk  The SPI pin used as clock for the device.      Valid values for LPC1768 are p7 or p13
    *  @param load  The pin used to control load for the device.   Any pin capable for DigitalOut can be used, but the same load pin must be used for 
    *    device that share the same msoi and mclk pins
    *  @date 8/6/2013
    */
    Max7221(PinName msoi=p5, PinName mclk=p7, PinName load=p8);
   
    /**
    *  This method is used to write a byte of data to a specified register for only the device defined in this class instance
    *  @author Dwayne S. Dilbeck
    *  @param reg  The register to write to.
    *  @param data The value to be written.
    *  @date 8/6/2013   
    */
    void Write( unsigned int reg, unsigned int data);

    /**
    *  This method is used to display an integer to the specified device instance. Underflow and overflow result in '-' written to all digits
    *  @author Dwayne S. Dilbeck
    *  @param value  An integer value to display
    *  @date 8/6/2013   
    */   
    void WriteInt( int value );

    /**
    *  This method is used to display a floating point number to the specified device instance. 
    *   Underflow and overflow result in '-' written to all digits. The digits after the decimal 
    *   point are truncated to fit the display.
    *  @author Dwayne S. Dilbeck
    *  @param value  A float value to display
    *  @date 8/6/2013   
    */   
    void WriteFloat( float value);
   
    /**
    *  Overload of the EQUALS operator to provide easy use of the class.
    *  @author Dwayne S. Dilbeck
    *  @param value  An integer value to display
    *  @date 8/6/2013   
    */      
    Max7221& operator= (int value){ 
        WriteInt(value); 
        return *this;
        };
    /**
    *  Overload of the EQUALS operator to provide easy use of the class.
    *  @author Dwayne S. Dilbeck
    *  @param value  A float value to display
    *  @date 8/6/2013   
    */   
    Max7221& operator= (float value){ 
        WriteFloat(value); 
        return *this; 
        };

    /**
    *  This method is used to write an  intial set off values to the device to prepare it for use.
    *  @author Dwayne S. Dilbeck
    *  @date 8/6/2013   
    */   
    void Setup (void);

    /**
    *  This method is used to write a byte of data to a specified register for all the devices instantiated.
    *  @author Dwayne S. Dilbeck
    *  @param reg  The register to write to.
    *  @param data The value to be written.
    *  @date 8/6/2013   
    */   
    static void WriteAll (unsigned int reg, unsigned int data);

    /**
    *  This method is used to write an intial set off values to ALL device to prepare them for use.
    *  @author Dwayne S. Dilbeck
    *  @date 8/6/2013   
    */   
    static void SetupAll (void);
   
private:
   /// Pointer for the class instance to a particular SPI bus
   SPI *max72_spi;
   /// pointer for the class instance of a particular load signal
   DigitalOut *load;
   ///  id of the class instance
   int id;
   /// pointer to the number of devices connected to the SPI bus instance this device instance is connected to.
   int *maxInUse;
   
   ///For the class we have a static set of values.  There are 2 SPI buses, 2 load signals, and 2 counters of the number of devices connected to a SPI bus.
   static SPI *spi1;
   static SPI *spi2;
   static DigitalOut *load1;
   static DigitalOut *load2;
   static int maxInUseSPI1;
   static int maxInUseSPI2;
};

#endif