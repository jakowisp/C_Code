/**
*  @file PIMAArray.h
*  @brief PIMA Array class definition
*  @author Dwayne Dilbeck
*  @date 9/16/2013
* 
* mbed USBHostPTP Library(PIMA15740 Array definition)
*
* @par Copyright:
*  Copyright (c) 2013 Dwayne Dilbeck
* @par License:
*  This software is distributed under the terms of the GNU Lesser General Public License
*/

/**
*  Class PIMA array
*
*  The PIMA15740 standard defines an array as an unsigned 32bit number of elements followed by a list
*   of insinged 16bit codes;
*/
template <class TYPE>
class PIMAArray {
public:
  /**
  *   Constructor
  *  @param None
  *
  *   Zeros the number of elements and sets codes pointer to NUll
  */
  PIMAArray() {
     numberOfElements=0;
     elements=NULL;
  };
  
  /**
  * Destructor
  *
  * Frees assigned storage for codes.
  * @param None
  */
  ~PIMAArray() {
    if( elements !=NULL)
        free(elements);
  };

  /**
   *  Create and fill array storage from a supplied buffer pointer
   *
   *  @param currentPtr a unit8_t pointer to a buffer location where a PIMAArray should be read
   *  
   *  @return The number of bytes used from the buffer to create the PIMA array.
   */
   
   unsigned int FillArray(uint8_t *initialPtr) {
     uint8_t *currentPtr=initialPtr;
     
     currentPtr+=SetNumberOfElements(*((uint32_t *)currentPtr));
     currentPtr+=SetElements((uint8_t *)(currentPtr));
     return (currentPtr-initialPtr); 
   }
   
   TYPE GetElement(uint32_t index) {
       return elements[index];
   }
   
   bool CheckValueInArray(TYPE value) {
    bool res=false;
    for(int i=0; i<numberOfElements;i++)
       if(value == elements[i])
          res=true;
    return res;
    }
   

   ///Number of elelments stored in the array
   uint32_t numberOfElements;
   ///Pointer to elements storage
   TYPE *elements;
      
private:
/**
 * Function to allocate array storage space and set the number of elements
 *
 * @param uint8_t The number of elements to create storage space for.
 * @return none
 */
 unsigned int SetNumberOfElements(uint8_t length) {
     this->numberOfElements=length;
     if( elements !=NULL)
        free(elements);
     elements = (TYPE *) malloc(sizeof(TYPE)*length);
     return sizeof(uint32_t);
 };
 
 /**
 * Function to read codes form a uint8t buffer and store them.
 * @param buffer pointer to a uint8_t buffer
 */
 unsigned int SetElements(uint8_t *buffer){
     TYPE *castPtr = (TYPE *)buffer;
     if(castPtr!=NULL && elements !=NULL)
       for(int i=0;i<this->numberOfElements;i++)
           elements[i]=castPtr[i];
     return (sizeof(elements[0])*numberOfElements);
 };
  
};
