/**
* @file Max7221.cpp
* @brief This file defines the Max7221 class methods and the default values for static variables
* 
* @author Dwayne S. Dilbeck
*
* @date 8/6/2013
*/
#include "mbed.h"
#include "Max7221.h"

///Intialize the Class static members
///Devices used per SPI bus are set to ZERO
int Max7221::maxInUseSPI1 = 0;
int Max7221::maxInUseSPI2 = 0;
///Set the static pointers to the SPI busses to NULL
SPI *Max7221::spi1=NULL;
SPI *Max7221::spi2=NULL;
///Set the static pointers to the load signals to NULL
DigitalOut *Max7221::load1=NULL;
DigitalOut *Max7221::load2=NULL;

/**
*  Constructor. This is the default constructor
*  @author Dwayne S. Dilbeck
*  @param msoi  The SPI pin used as input to the device.       Valid values for LPC1768 are p5 or p11
*  @param mclk  The SPI pin used as clock for the device.      Valid values for LPC1768 are p7 or p13
*  @param load  The pin used to control load for the device.   Any pin capable for DigitalOut can be used, but the same load pin must be used for 
*    device that share the same msoi and mclk pins
*  @date 8/6/2013
*/
Max7221::Max7221(PinName msoi, PinName mclk, PinName load){
   ///Determine which bus we are connecting the device to based on the MSOI pin name
   switch (msoi) {
     ///If using SPI bus #1
     case p5: maxInUseSPI1++; 
              ///Set this insctance to pointers to the correct static pointers
              this->id=maxInUseSPI1;
              this->maxInUse=&maxInUseSPI1;
              ///If no devices have been assigned to the SPI bus it must be initialized
              if (spi1 ==NULL) {
                 spi1 = new SPI(msoi, NC, mclk);
                 load1 = new DigitalOut(load);
              } else {
                ///TODO: Check that load pin is the same for all SP2 
              }
              this->max72_spi=spi1;
              this->load = load2;
              break;
     ///If using SPI bus #2
     case p11: maxInUseSPI2++;  
              ///Set this insctance to pointers to the correct static pointers
              this->id=maxInUseSPI2;
              this->maxInUse=&maxInUseSPI2;
              ///If no devices have been assigned to the SPI bus it must be initialized
              if (spi2 ==NULL) {
                 spi2 = new SPI(msoi, NC, mclk);
                 load2 = new DigitalOut(load);
              } else {
                ///TODO: Check that load pin is the same for all SP2 
              }
              this->max72_spi=spi2;
              this->load = load2;
              break; 
     ///If a pin not belonging to a SPI bus is used, Throw error
     default: error("Not a SPI port");
   }
   
}

/**
*  This method is used to write a byte of data to a specified register for only the device defined in this class instance
*  @author Dwayne S. Dilbeck
*  @param reg  The register to write to.
*  @param data The value to be written.
*  @date 8/6/2013   
*//**
*  This method is used to write a byte of data to a specified register for only the device defined in this class instance
*  @author Dwayne S. Dilbeck
*  @param reg  The register to write to.
*  @param data The value to be written.
*  @date 8/6/2013   
*/
void Max7221::Write( unsigned int reg, unsigned int data) {
    int c = 0;
    *load = LOW;

    ///if there are multiple devices sharing the SPI buss and they preceed the current device in the cascade Write a NOOP to them
    for ( c = *maxInUse; c > this->id; c--) {
        max72_spi->write(0);  // no-op
        max72_spi->write(0);  // no-op
    }
    ///Write to this device registers
    max72_spi->write(reg);  // specify register
    max72_spi->write(data);  // put data

    ///if there are multiple devices sharing the SPI buss and they follow the current device in the cascade Write a NOOP to them
    for ( c=this->id-1; c >= 1; c--) {
        max72_spi->write(0);  // no-op
        max72_spi->write(0);  // no-op
    }
    *load = HIGH;
}

/**
*  This method is used to write an  intial set off values to the device to prepare it for use.
*  @author Dwayne S. Dilbeck
*  @date 8/6/2013   
*/   
void Max7221::Setup () {
    // initiation of the max 7221
    // SPI setup: 8 bits, mode 0
    max72_spi->format(8, 0);
    
    // going by the datasheet, min clk is 100ns so theoretically 10MHz should work...
    max72_spi->frequency(10*MHZ);
    
    Write(max7219_reg_scanLimit, 0x07);   ///ENABLE all 8 digits
    Write(max7219_reg_decodeMode, 0xff);  // Turn on Code B font decode for all digits
    Write(max7219_reg_shutdown, 0x01);    // Disable shutdown mode
    Write(max7219_reg_displayTest, 0x00); // Disable display test
    for (int e=1; e<=8; e++) {            // Write blank to all digits
        Write(e,0xf);
    }
    Write(max7219_reg_intensity, 0x01 );    // Set the display intensity to a low level. 
                                                  // range: 0x00 to 0x0f
}

/**
*  This method is used to write a byte of data to a specified register for all the devices instantiated.
*  @author Dwayne S. Dilbeck
*  @param reg  The register to write to.
*  @param data The value to be written.
*  @date 8/6/2013   
*/   
void Max7221::WriteAll (unsigned int reg, unsigned int data) {   
    ///Write to all the devices on SPI Bus #1 first
    if(load1 !=NULL) {
        *load1 = LOW;                    // begin
        for ( int c=1; c<= maxInUseSPI1; c++) {
            spi1->write(reg);  // specify register
            spi1->write(data);  // put data
        }
        *load1 = HIGH;
    }
    ///Write to all the devices on SPI Bus #2
    if(load2 !=NULL) {
        *load2 = LOW;
        for ( int c=1; c<= maxInUseSPI2; c++) {
            spi2->write(reg);  // specify register
            spi2->write(data);  // put data
        }
        *load2 = HIGH;
    }
}
/**
*  This method is used to display an integer to the specified device instance. Underflow and overflow result in '-' written to all digits
*  @author Dwayne S. Dilbeck
*  @param value  An integer value to display
*  @date 8/6/2013   
*/   
void Max7221::WriteInt( int value ){
  char buffer[16];
   
  ///TODO:SET UPPERBOUND AND LOWERBOUND based on NUMDIGITS
  ///Check the the INT value can be displayed and convert to a string.
  if (value <= UPPERBOUND && value >= LOWERBOUND) {
     sprintf(buffer,"%8d",value);
  } else {
     sprintf(buffer,"--------");
  } 
  ///In case a program changed the decode mode, set it again.
  Write(max7219_reg_decodeMode, 0xff);
  for (int i=0;i<NUMDIGITS;i++) {
       ///For each character of the string, convert the ASCII to the datacode needed by the device
       switch(buffer[i]){
           case 0x2d: buffer[i]=0xa; break;
           case 0x20: buffer[i]=0xf; break;
           default: buffer[i]= buffer[i] & 0x0f;
       }
       ///Call function to write the data for each character of the srting.
       Write(NUMDIGITS-i,buffer[i]);
  }
}

/**
*  This method is used to display a floating point number to the specified device instance. 
*   Underflow and overflow result in '-' written to all digits. The digits after the decimal 
*   point are truncated to fit the display.
*  @author Dwayne S. Dilbeck
*  @param value  A float value to display
*  @date 8/6/2013   
*/   
void Max7221::WriteFloat( float value ){
  char buffer[32];
  int ptr=-1,len;
  int i;
 
  sprintf(buffer,"%f",value);
  len=strlen(buffer);
  i=len-1;
  while(buffer[i]==0x30) {
     buffer[i]='\0';
     i--;
     len--;
  }
  for( i =0; i<=len; i++) {
      switch(buffer[i]){
               case 0x2d: buffer[i]=0xa; break;
               case 0x20: buffer[i]=0xf; break;
               case 0x2e: buffer[i]=buffer[i-1] | 0x80;
                           ptr = i-1;
                           break;
               default: buffer[i]= buffer[i];
      }       
      if (ptr != -1) {
        buffer[i-1]=buffer[i]; 
     }
  }
     
  len=strlen(buffer);
  Write(max7219_reg_decodeMode, 0xff);

  // If too large for display set to '-'
  if(len > NUMDIGITS && (ptr==-1 || ptr>NUMDIGITS))
        for (int i=0;i<NUMDIGITS;i++) {
           buffer[i]=0x0a;
        }
  //if number is smaller than display, fill with ' '      
  if (len<=NUMDIGITS) { 
      for (int i=1;i<=NUMDIGITS;i++) {
           if(len-i>=0) {
             Write(i,buffer[len-i]);
           } else {
             Write(i,0xf);   
           }
      }
  } else {
  //Write out the buffer, truncating the decimal digits if larger than display
      for (int i=0;i<NUMDIGITS;i++) {
          Write(NUMDIGITS-i,buffer[i]);
      }
  }
}

/**
*  This method is used to write an intial set off values to ALL device to prepare them for use.
*  @author Dwayne S. Dilbeck
*  @date 8/6/2013   
*/   
void Max7221::SetupAll () {
    // initiation of the max 7219
    // SPI setup: 8 bits, mode 0
    if(spi1!=NULL) {
       spi1->format(8, 0);
       spi1->frequency(10*MHZ);    
    }        
    if(spi2!=NULL) {
       spi2->format(8, 0);
       spi2->frequency(10*MHZ);    
    }        
    WriteAll(max7219_reg_scanLimit, 0x07);
    WriteAll(max7219_reg_decodeMode, 0xff);  // using an led matrix (not digits)
    WriteAll(max7219_reg_shutdown, 0x01);    // not in shutdown mode
    WriteAll(max7219_reg_displayTest, 0x00); // no display test
    for (int e=1; e<=8; e++) {            // empty registers, turn all LEDs off
        WriteAll(e,0xf);
    }
    WriteAll(max7219_reg_intensity, 0x01 & 0x0f);    // the first 0x0f is the value you can set
                                                  // range: 0x00 to 0x0f
}

