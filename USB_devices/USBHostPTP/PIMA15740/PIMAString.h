/**
*  @file PIMAString.h
*  @brief PIMA String class definition
*  @author Dwayne Dilbeck
*  @date 8/23/2013
* 
* mbed USBHostPTP Library(PIMA15740 String definition)
* @par Copyright:
*  Copyright (c) 2013 Dwayne Dilbeck
* @par License:
*  This software is distributed under the terms of the GNU Lesser General Public License
*/

/**
*  Class PIMA String
*
*  The PIMA15740 standard defines a string as an unsigned 8bit length followed by a list
*   of 16bit unicode characters
*
*/
class PIMAString {
public:

  /**
  * Constructor
  *
  * By default the pointers are set to NULL and length set to ZERO.
  */
  PIMAString() {
     length=0;
     StringChars=NULL;
     vals=NULL;
  };
  
  /**
  * Desctructor
  *
  * Free the Memory allocated for character storage
  */
  ~PIMAString() {
     if( StringChars !=NULL)
         free(StringChars);
     if(vals!=NULL)
         free(vals);
   };

  /**
   *  Create and fill character storage from a supplied buffer pointer
   *
   *  @param currentPtr a unit8_t pointer to a buffer location where a PIMAString shoudl be read
   *  
   *  @return The number of bytes used from the buffer to create the PIMA string.
   */
 int FillString(uint8_t *currentPtr) {
     setLength(*currentPtr);
     setStringChars((uint16_t *)(currentPtr+1));
     return 2*length +1;
 };

  /**
  *  Truncate the unicode characters to make a string for printf.
  *
  *  @param none
  *  @return pointer to an array of characters that is null terminated.
  */
  char * getString() {
     if(vals!=NULL)
        free(vals);  
     vals=(char *) malloc(sizeof(char)*(this->length+1));
     for(int i=0;i<this->length;i++)
           vals[i]=StringChars[i];  //NOTE: Truncates the uint16_t value.
     vals[length]='\0';
     return vals;
  }

  /**
  * Get the number of characters stored in the PIMA String
  *
  * @param none
  * @return uint8_t value that is the number of chracters notstored in the PIMA String
  */
  uint8_t getLength() {
     return this->length;
  }
 
 private:
 /**
 * Function to allocate unicode cahracter storage space and set the length value
 *
 * @param uint8_t The number of characters to create storage space for.
 * @return none
 */
 void setLength(uint8_t length) {
     if(length > this->length) {
         if( StringChars !=NULL)
            free(StringChars);
         StringChars = (uint16_t *) malloc(sizeof(uint16_t)*length);
     }
     this->length=length;
 };
 
 /**
 * Function to read unicode charcaters form a uint8t buffer and store them.
 * @param pointer to a uint8_t buffer
 */
 void setStringChars(uint16_t *buffer){
     if(buffer!=NULL && StringChars !=NULL)
       for(int i=0;i<this->length;i++)
           StringChars[i]=buffer[i];
 };
 
  ///number of stored characters
  uint8_t length;
  ///Pointer to the character storage
  uint16_t *StringChars;
  ///Pointer to Character storge that are truncated unicode values.
  char *vals;
};