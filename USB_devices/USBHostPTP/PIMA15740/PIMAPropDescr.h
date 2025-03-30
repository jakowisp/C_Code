/**
*  @file PIMAPropDesc.h
*  @brief PIMA Property Description class definition
*  @author Dwayne Dilbeck
*  @date 9/16/2013
* 
* mbed USBHostPTP Library(PIMA15740 String definition)
* @par Copyright:
*  Copyright (c) 2013 Dwayne Dilbeck
* @par License:
*  This software is distributed under the terms of the GNU Lesser General Public License
*/
#include "PIMADTS.h"

/**
*  Class Property Description
*
*  The PIMA15740 standard defines 
*
*/
class PIMAPropDesc {
public:

    

   int FillPropDesc(uint8_t *initialPtr) {
     uint8_t *currentPtr = initialPtr;
     
     devicePropertyCode = *((uint16_t *)currentPtr);
     currentPtr+=sizeof(uint16_t);
     dataType = *((uint16_t *)(currentPtr));
     currentPtr+=sizeof(uint16_t);
     getSet=*((uint8_t *)(currentPtr));
     currentPtr+=sizeof(uint8_t);
     currentPtr+=ParseValue(currentPtr,&factoryDefault);
     currentPtr+=ParseValue(currentPtr,&currentValue);
     formFlag=*((uint8_t *)(currentPtr));
     currentPtr+=sizeof(uint8_t);
     
     return (currentPtr-initialPtr); //TODO: Finish Fill function.
   }

private:
     unsigned int ParseValue(uint8_t *currentPtr, DTSUnion *DTSUnionIn){
         unsigned int result;
         switch(dataType) {
             case PIMA_DATATYPE_CODE_INT8:
                  result=fill<int8_t > ((void *)&(DTSUnionIn->signed8bitInt),currentPtr);
                  break;
             case PIMA_DATATYPE_CODE_UINT8:
                  result=fill<uint8_t > ((void *)&(DTSUnionIn->unsigned8bitInt),currentPtr);
                  break;
             case PIMA_DATATYPE_CODE_INT16:
                  result=fill<int16_t > ((void *)&(DTSUnionIn->signed16bitInt),currentPtr);
                  break;
             case PIMA_DATATYPE_CODE_UINT16:
                  result=fill<uint16_t > ((void *)&(DTSUnionIn->unsigned16bitInt),currentPtr);
                  break;
             case PIMA_DATATYPE_CODE_INT32:
                  result=fill<int32_t > ((void *)&(DTSUnionIn->signed32bitInt),currentPtr);
                  break;
             case PIMA_DATATYPE_CODE_UINT32:
                  result=fill<uint32_t > ((void *)&(DTSUnionIn->signed32bitInt),currentPtr);
                  break;
             case PIMA_DATATYPE_CODE_INT64:
                  result=fill<int64_t > ((void *)&(DTSUnionIn->signed64bitInt),currentPtr);
                  break;
             case PIMA_DATATYPE_CODE_UINT64:
                  result=fill<uint64_t > ((void *)&(DTSUnionIn->signed64bitInt),currentPtr);
                  break;
             case PIMA_DATATYPE_CODE_INT128:
                  break;
             case PIMA_DATATYPE_CODE_UINT128:
                  break;
            case PIMA_DATATYPE_CODE_AINT8:
                  DTSUnionIn->arraysigned8bitInt = new PIMAArray<int8_t> ;
                  result=DTSUnionIn->arraysigned8bitInt->FillArray(currentPtr);
                  break;
             case PIMA_DATATYPE_CODE_AUINT8:
                  DTSUnionIn->arrayunsigned8bitInt = new PIMAArray<uint8_t> ;
                  result=DTSUnionIn->arrayunsigned8bitInt->FillArray(currentPtr);
                  break;
             case PIMA_DATATYPE_CODE_AINT16:
                  DTSUnionIn->arraysigned16bitInt = new PIMAArray<int16_t> ;
                  result=DTSUnionIn->arraysigned16bitInt->FillArray(currentPtr);
                  break;
             case PIMA_DATATYPE_CODE_AUINT16:
                  DTSUnionIn->arrayunsigned16bitInt = new PIMAArray<uint16_t> ;
                  result=DTSUnionIn->arrayunsigned16bitInt->FillArray(currentPtr);
                  break;
             case PIMA_DATATYPE_CODE_AINT32:
                  DTSUnionIn->arraysigned32bitInt = new PIMAArray<int32_t> ;
                  result=DTSUnionIn->arraysigned32bitInt->FillArray(currentPtr);
                  break;
             case PIMA_DATATYPE_CODE_AUINT32:
                  DTSUnionIn->arrayunsigned32bitInt = new PIMAArray<uint32_t> ;
                  result=DTSUnionIn->arrayunsigned32bitInt->FillArray(currentPtr);
                  break;
             case PIMA_DATATYPE_CODE_AINT64:
                  DTSUnionIn->arraysigned64bitInt = new PIMAArray<int64_t> ;
                  result=DTSUnionIn->arraysigned64bitInt->FillArray(currentPtr);
                  break;
             case PIMA_DATATYPE_CODE_AUINT64:
                  DTSUnionIn->arrayunsigned64bitInt = new PIMAArray<uint64_t> ;
                  result=DTSUnionIn->arrayunsigned64bitInt->FillArray(currentPtr);
                  break;
/*             case PIMA_DATATYPE_CODE_AINT128:
                  break;
             case PIMA_DATATYPE_CODE_AUINT128:
                  break; 
             case PIMA_DATATYPE_CODE_STR:
                  DTSUnionIn->ptri8 = new DTS<PIMAString>;
                  break; */
             default:
                  break;
         }
         return result;
     }

     template<class Type>
     unsigned int fill(void *targetPtr,uint8_t *ptr){
        Type temp;
        
        temp=*((Type *)ptr);
        *((Type *)targetPtr)=temp;
        
        return sizeof(temp);
     }
     
     ///PIMA 15740:2000 Section 13, table 23
     uint16_t   devicePropertyCode;
     uint16_t   dataType;
     uint8_t    getSet;
     DTSUnion   factoryDefault;
     DTSUnion   currentValue;
      
     uint8_t   formFlag;
     typedef union {
         struct {
           DTSUnion minimumValue;
           DTSUnion maximumValue;
           DTSUnion stepSize;
         } rangeForm;
         PIMAArray<DTSUnion> *enumerationForm;
      }  FORMS;
      FORMS FORM;     
};
