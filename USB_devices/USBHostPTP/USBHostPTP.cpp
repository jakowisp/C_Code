/**
*  @file USBHostPTP.cpp
*  @brief USBHostPTP class function definitions
*  @author Dwayne Dilbeck
*  @date 8/23/2013
* 
* mbed USBHostPTP Library
*
* @par Copyright:
*  Copyright (c) 2013 Dwayne Dilbeck
* @par License:
*  This software is distributed under the terms of the GNU Lesser General Public License
*/


#include "USBHostPTP.h"

#if 1
//USBHOST_PTP

#include "dbg.h"

#define DEVICE_TO_HOST  0x80
#define HOST_TO_DEVICE  0x00


USBHostPTP::USBHostPTP( void )
{
    host = USBHost::getHostInst();
    initializeClassData();
}

void USBHostPTP::initializeClassData() {
    pointerToDevice         = NULL;
    bulk_in                 = NULL;
    bulk_out                = NULL;
    int_in                  = NULL;
    deviceFound             = false;
    deviceConnected         = false;    
    ptp_intf                = -1;
    numberOfEndpoints       = 0;
    transactionCnt          = 0;
    commandContainer.type   = PIMA_CONTAINER_COMMAND;
    sessionID               = 1;
    dataLeftToTransfer      = 0;
    CodeDecoderFunction = NULL;
    memset(buffer,0,1024);
}


bool USBHostPTP::connected()
{
    return deviceConnected;
}

bool USBHostPTP::connect()
{

    if (deviceConnected) {
        return true;
    }
    
    for (uint8_t i = 0; i < MAX_DEVICE_CONNECTED; i++) {
        if ((pointerToDevice = host->getDevice(i)) != NULL) {
            
            USB_DBG("Trying to connect PTP device\r\n");
            
            if(host->enumerate(pointerToDevice, this))
                break;

            if (deviceFound) {
                bulk_in =  pointerToDevice->getEndpoint(ptp_intf, BULK_ENDPOINT, IN);
                bulk_out = pointerToDevice->getEndpoint(ptp_intf, BULK_ENDPOINT, OUT);
                int_in =   pointerToDevice->getEndpoint(ptp_intf, INTERRUPT_ENDPOINT, IN);
                
                if (!bulk_in || !bulk_out || !int_in)
                    continue;
                
                USB_INFO("New PTP device: VID:%04x PID:%04x [dev: %p - intf: %d]", pointerToDevice->getVid(),
                                                                                 pointerToDevice->getPid(),
                                                                                 pointerToDevice, ptp_intf);
                pointerToDevice->setName("PTP", ptp_intf);
                host->registerDriver(pointerToDevice, ptp_intf, this, &USBHostPTP::initializeClassData);

                this->deviceConnected = true;
                return true;
            }
        } //if()
    } //for()
    initializeClassData();
    return false;
}

/*virtual*/ void USBHostPTP::setVidPid(uint16_t vid, uint16_t pid)
{
    // we don't check VID/PID for PTP driver
}

/*virtual*/ bool USBHostPTP::parseInterface(uint8_t intf_nb, uint8_t intf_class, uint8_t intf_subclass, uint8_t intf_protocol) //Must return true if the interface should be parsed
{
    if ((ptp_intf == -1) &&
        (intf_class == USB_PTP_CLASS_CODE) &&
        (intf_subclass == 0x01) &&
        (intf_protocol == 0x01)) {
        ptp_intf = intf_nb;
        return true;
    }
    return false;
}

/*virtual*/ bool USBHostPTP::useEndpoint(uint8_t intf_nb, ENDPOINT_TYPE type, ENDPOINT_DIRECTION dir) //Must return true if the endpoint will be used
{

    if (intf_nb == ptp_intf) {
        if (type == INTERRUPT_ENDPOINT) {
            numberOfEndpoints++;  
        }
        if (type == BULK_ENDPOINT) {
            numberOfEndpoints++;
        }
        if (numberOfEndpoints == 3)
            deviceFound = true;
        return true;
    }
    return false;
}








bool USBHostPTP::CancelRequest(unsigned int TransactionID) {
    unsigned char buffer[6];
    int res = 0;

    buffer[0] =0x01;  //Cancellation Code 0x4001
    buffer[1] =0x40;
    buffer[2]= TransactionID & 0xff;
    buffer[3]= (TransactionID>>8) & 0xff;
    buffer[4]= (TransactionID>>16) & 0xff;;
    buffer[5]= (TransactionID>>24) & 0xff;;
    
    res = host->controlWrite(   pointerToDevice,
                                USB_RECIPIENT_ENDPOINT | USB_HOST_TO_DEVICE | USB_REQUEST_TYPE_STANDARD,
                                USB_PTP_CLASS_REQUEST_CANCELREQEUST,
                                0, 0, buffer, 6);
    return res==0?true:false;
}

bool USBHostPTP::DeviceResetRequest(void) {
   
    int res = 0;
    
    res = host->controlWrite(   pointerToDevice,
                                USB_RECIPIENT_ENDPOINT | USB_HOST_TO_DEVICE | USB_REQUEST_TYPE_STANDARD,
                                USB_PTP_CLASS_REQUEST_DEVICE_RESET,
                                0, 0, NULL, 0);
    return res==0?true:false;
}


bool USBHostPTP::GetDeviceStatus(void) {
    unsigned char buffer[6];
    int res = 0;

    //TODO: DetermineBuffer size
    
    res = host->controlWrite(   pointerToDevice,
                                USB_RECIPIENT_ENDPOINT | USB_HOST_TO_DEVICE | USB_REQUEST_TYPE_STANDARD,
                                USB_PTP_CLASS_REQUEST_GET_DEVICE_STATUS,
                                0, 0, buffer, sizeof(buffer));
    return res==0?true:false;
}

bool USBHostPTP::GetExtendedEventData(void) {
    unsigned char buffer[6];
    int res = 0;

    ///TODO: DetermineBuffer size
    
    res = host->controlWrite(   pointerToDevice,
                                USB_RECIPIENT_ENDPOINT | USB_HOST_TO_DEVICE | USB_REQUEST_TYPE_STANDARD,
                                USB_PTP_CLASS_REQUEST_GET_EXTENDED_EVENT_DATA,
                                0, 0, buffer, sizeof(buffer));
    return res==0?true:false;
}

bool USBHostPTP::CheckEvent(){
      int res=0;
      
      memset(&eventContainer, 0, sizeof(PIMAContainer));
      
      res = host->interruptRead(pointerToDevice, int_in,(uint8_t *)&eventContainer, sizeof(PIMAContainer));
      
      if(res!=USB_TYPE_OK)
         return false;
      
      return true;
}

int USBHostPTP::checkResult(uint8_t res, USBEndpoint * ep) {
    // if ep stalled: send clear feature
    if (res == USB_TYPE_STALL_ERROR) {
        res = host->controlWrite(   pointerToDevice,
                                    USB_RECIPIENT_ENDPOINT | USB_HOST_TO_DEVICE | USB_REQUEST_TYPE_STANDARD,
                                    CLEAR_FEATURE,
                                    0, ep->getAddress(), NULL, 0);
        // set state to IDLE if clear feature successful
        if (res == USB_TYPE_OK) {
            ep->setState(USB_TYPE_IDLE);
        }
    }

    if (res != USB_TYPE_OK)
        return -1;

    return 0;
}

void USBHostPTP::PrepareControlContainer(uint16_t operationCode, 
                                 OperFlags *operationFlags,
                                 uint32_t *parameters) {

  this->commandContainer.length        = PIMA_CONTAINER_HEADER_LENGTH + operationFlags->opParams*4;
  this->commandContainer.code        = operationCode;
  this->commandContainer.transactionID = this->transactionCnt++;
  for (int i=0; i<operationFlags->opParams; i++)
    this->commandContainer.payload[i]=parameters[i];

  #ifdef USDBPTPDEBUG
      printf("PTPCommandContainer:\r\n");
      DumpBuffer((uint8_t *)&(this->commandContainer),this->commandContainer.length);
  #endif

}

bool USBHostPTP::RecieveDataContainer(void *dataHandlerFunctionCastToVoid, uint16_t *responseCode){
  int         i=0;
  int         transferResult=0;
  bool        firstDataBlockReceived=true;
  uint8_t     *pointerToUint8tData;
  uint32_t    bytesReceived=0;
  uint32_t    bytesRemaining=0;
  uint32_t    dataToBeHandled=0;
   
  DataHandler dataHandler1 = (DataHandler) dataHandlerFunctionCastToVoid;
  
  if( responseCode==NULL)
     return false;
  
   *responseCode=PIMA_RETURN_CODE_Undefined;
    
   do {
       ///TODO: Magic number needs to be replaced with DEFINE
       transferResult = host->bulkRead(this->pointerToDevice, this->bulk_in, this->buffer, 1024);
       if ( checkResult(transferResult, this->bulk_in))
          return PIMA_RETURN_CODE_IncompleteTransfer;
       #ifdef USDBPTPDEBUG
       printf("Data stage bulkRead result: %d\r\n",transferResult);
       #endif
       bytesReceived = bulk_in->getLengthTransferred();   
       if(firstDataBlockReceived==true) {
          pointerToUint8tData=(uint8_t *)&dataContainer;
          for(i=0; i<PIMA_CONTAINER_HEADER_LENGTH; i++){
               pointerToUint8tData[i]=buffer[i];
          }
          #ifdef USDBPTPDEBUG
          DumpBuffer(pointerToUint8tData,PIMA_CONTAINER_HEADER_LENGTH);
          #endif
          firstDataBlockReceived = false;
          pointerToUint8tData    = buffer + PIMA_CONTAINER_HEADER_LENGTH;
          bytesRemaining         = dataContainer.length - PIMA_CONTAINER_HEADER_LENGTH;
          this->totalDataToTransfer = bytesRemaining;
          if(dataContainer.type==PIMA_CONTAINER_DATA)
              bytesReceived -= PIMA_CONTAINER_HEADER_LENGTH;
          #ifdef USDBPTPDEBUG
          DumpBuffer(pointerToUint8tData,bytesReceived);
          #endif
       } else {
          pointerToUint8tData=buffer;
       }
       
       if(bytesReceived<=bytesRemaining)
           dataToBeHandled=bytesReceived;
       else {
           dataToBeHandled=bytesRemaining;
       }
       
       #ifdef USDBPTPDEBUG
       printf("Calling Datahandler\r\n");
       printf("DataToBeHandled: %ld\r\n",dataToBeHandled);
       #endif   
       if( dataHandler1!=NULL && dataToBeHandled !=0 )
             (*dataHandler1)(this,pointerToUint8tData,dataToBeHandled);
             
       #ifdef USDBPTPDEBUG
       DumpBuffer(buffer,bytesReceived);
       #endif
       
       //IF the Response got appended to Data block            
       if(bytesReceived<=bytesRemaining) {
           bytesRemaining-=bytesReceived;
       } else {
           #ifdef USDBPTPDEBUG
           printf("Handling Response Packet in data container\r\n");
           #endif
           if(bytesReceived-bytesRemaining==responseContainer.length) {
               pointerToUint8tData=(uint8_t *)&responseContainer;
               for(i=0;i<responseContainer.length;i++){
                  pointerToUint8tData[i]=buffer[bytesRemaining+i];
               } 
               *responseCode = responseContainer.code;
               bytesRemaining=0;
               
           } else {
               return false; 
           }
       }
       this->dataLeftToTransfer = bytesRemaining;
       #ifdef USDBPTPDEBUG
       printf("Loopcheck: %ld\r\n",bytesRemaining);
       #endif 
  } while( bytesRemaining>0 );
  return true;
}

bool USBHostPTP::SendDataContainer(void *dataGeneratorFunctionCastToVoid, uint16_t *responseCode){  

   uint32_t *length;
   int         transferResult=0;
   
   length=(uint32_t *)dataGeneratorFunctionCastToVoid;
   
   *responseCode=PIMA_RETURN_CODE_Undefined;
   if (*length > 1024-0xc) {
     printf("Multiple block DataOut not coded\r\n");
   } else {
     transferResult = this->host->bulkWrite(this->pointerToDevice, 
                                            this->bulk_out,
                                            (uint8_t *)dataGeneratorFunctionCastToVoid,
                                            *length);
     if (checkResult(transferResult, this->bulk_out))
        *responseCode= PIMA_RETURN_CODE_GeneralError;
   }
   return true;
}


uint16_t USBHostPTP::Transaction(uint16_t operationCode, 
                                 OperFlags *operationFlags,
                                 uint32_t *parameters,
                                 void *dataHandlerFunctionCastToVoid){
  
  
  uint16_t    responseCode=0;
  int         transferResult=0;
  
  if(IsCommandSupported(operationCode)==false)
       return PIMA_RETURN_CODE_OperationNotSupported;
       
  responseContainer.length          = PIMA_CONTAINER_HEADER_LENGTH + operationFlags->rsParams*4;
  
  PrepareControlContainer(operationCode,operationFlags,parameters);  
  
  transferResult = this->host->bulkWrite(this->pointerToDevice, this->bulk_out,(uint8_t *)&(this->commandContainer), this->commandContainer.length);
  if (checkResult(transferResult, this->bulk_out))
     return PIMA_RETURN_CODE_GeneralError;
  #ifdef USDBPTPDEBUG
  printf("Command bulkWrite result: %d\r\n",transferResult);
  #endif
  
  if (operationFlags->dataStage==1) {
    if (operationFlags->txOperation==0) {        
       if ( RecieveDataContainer(dataHandlerFunctionCastToVoid,&responseCode) != true ) {
          return PIMA_RETURN_CODE_GeneralError; 
       } else {
          if (responseCode != PIMA_RETURN_CODE_Undefined)
             return responseCode;
       }
    } else {
       if ( SendDataContainer(dataHandlerFunctionCastToVoid,&responseCode) != true ) {
          return PIMA_RETURN_CODE_GeneralError; 
       } else {
          if (responseCode != PIMA_RETURN_CODE_Undefined)
             return responseCode;
       }
    }
  }
  transferResult = host->bulkRead(pointerToDevice, bulk_in,(uint8_t *)&responseContainer, sizeof(responseContainer));
  #ifdef USDBPTPDEBUG
     printf("Response bulkRead result: %d\r\n",transferResult);
     DumpResponseContainer();
  #endif
  if (checkResult(transferResult, bulk_in))
      return PIMA_RETURN_CODE_GeneralError;
  return responseContainer.code;
}





uint16_t USBHostPTP::Operation(uint16_t opcode, uint8_t nparams, uint32_t *params){
    OperFlags flags = {0,0,0,0,0,0};
    flags.opParams = nparams;
            
    return Transaction(opcode,&flags,params);
}

uint16_t  USBHostPTP::OpenSession(void) {
    OperFlags flags = {1,0,0,0,0,0};
            
    return Transaction(PIMA_OPERATION_CODE_OpenSession,&flags,&sessionID);
}

uint16_t  USBHostPTP::CloseSession(void) {
    OperFlags flags = {1,0,0,0,0,0};
    
    return Transaction(PIMA_OPERATION_CODE_CloseSession,&flags,&sessionID);
}

uint16_t USBHostPTP::GetDeviceInfo(void)
{
    OperFlags   flags = { 0, 0, 0, 1, 1, 0 };
    return Transaction(PIMA_OPERATION_CODE_GetDeviceInfo, &flags,NULL,(void*)&USBHostPTP::ParseDeviceInfoDataBlock);
}


uint16_t USBHostPTP::PowerDown()
{
    OperFlags   flags = { 0, 0, 0, 0, 0, 0 };
    return Transaction(PIMA_OPERATION_CODE_PowerDown, &flags);
}

uint16_t USBHostPTP::SelfTest(uint16_t type)
{
    OperFlags   flags = { 1, 0, 0, 0, 0, 0 };
    uint32_t    params[1];
    params[0]   = type;

    return Transaction(PIMA_OPERATION_CODE_SelfTest, &flags, params);
}

uint16_t USBHostPTP::GetObjectHandles(uint32_t storage_id, uint16_t format, uint16_t assoc, void *parser)
{
    OperFlags   flags = { 3, 0, 0, 1, 1, 0 };
    uint32_t    params[3];

    params[0] = storage_id;
    params[1] = (uint32_t)format;
    params[2] = (uint32_t)assoc;

    return Transaction(PIMA_OPERATION_CODE_GetObjectHandles, &flags, params, parser);
}

uint16_t USBHostPTP::GetNumObjects(uint32_t *retval, uint32_t storage_id, uint16_t format, uint32_t assoc)
{
    uint16_t    ptp_error = PIMA_RETURN_CODE_GeneralError;
    OperFlags   flags = { 3, 1, 0, 0, 0, 0 };
    uint32_t    params[3];

    params[0] = storage_id;
    params[1] = (uint32_t)format;
    params[2] = (uint32_t)assoc;

    if ( (ptp_error = Transaction(PIMA_OPERATION_CODE_GetNumObjects, &flags, params)) == PIMA_RETURN_CODE_OK)
        *retval = responseContainer.payload[0];

    return ptp_error;
}

uint16_t USBHostPTP::GetObjectInfo(uint32_t handle)
{
    OperFlags   flags = { 1, 0, 0, 1, 1, 0 };
    uint32_t    params[1];
    params[0] = handle;
    
    //Clear any previous data, incase a Reponse packet is returned instead of a data packet
    ///TODO:Magic number issue
    //No buffer overun issue: See ParseObjectInfoDataBlock
    memset((uint8_t *)&objectInfo,0,0x34);

    return Transaction(PIMA_OPERATION_CODE_GetObjectInfo, &flags, params, (void*)&USBHostPTP::ParseObjectInfoDataBlock);
}

uint16_t USBHostPTP::GetThumb(uint32_t handle, void *parser)
{
    OperFlags   flags = { 1, 0, 0, 1, 1, 0 };
    uint32_t    params[1];

    params[0] = handle;

    return Transaction(PIMA_OPERATION_CODE_GetThumb, &flags, params, parser);
}


uint16_t USBHostPTP::GetObject(uint32_t handle, void *parser)
{
    OperFlags   flags = { 1, 0, 0, 1, 1, 0 };
    uint32_t    params[1];

    params[0] = handle;

    return Transaction(PIMA_OPERATION_CODE_GetObject, &flags, params, parser);
}

uint16_t USBHostPTP::GetStorageIDs(void *parser)
{
    OperFlags   flags = { 0, 0, 0, 1, 1, 0 };
    return Transaction(PIMA_OPERATION_CODE_GetStorageIDs, &flags, NULL, parser);
}

uint16_t USBHostPTP::GetStorageInfo(uint32_t storage_id)
{
    OperFlags   flags       = { 1, 0, 0, 1, 1, 0 };

    uint32_t    params[1]; 
    params[0]   = storage_id;

    return Transaction(PIMA_OPERATION_CODE_GetStorageInfo, &flags, params,  (void*)&USBHostPTP::ParseStorageInfoDataBlock);
}

uint16_t USBHostPTP::CopyObject(uint32_t handle, uint32_t storage_id, uint32_t parent, uint32_t *new_handle)
{
    uint16_t    ptp_error = PIMA_RETURN_CODE_GeneralError;
    OperFlags   flags = { 3, 1, 0, 0, 0, 0 };
    uint32_t    params[3];

    params[0] = handle;
    params[1] = storage_id;
    params[2] = parent;

    if ( (ptp_error = Transaction(PIMA_OPERATION_CODE_CopyObject, &flags, params)) == PIMA_RETURN_CODE_OK)
        *new_handle = responseContainer.payload[0];

    return ptp_error;
}


uint16_t USBHostPTP::DeleteObject(uint32_t handle, uint16_t format)
{
    OperFlags   flags = { 2, 0, 0, 0, 0, 0 };
    uint32_t    params[2];

    params[0] = handle;
    params[1] = (uint32_t)format;

    return Transaction(PIMA_OPERATION_CODE_DeleteObject, &flags, params);
}


uint16_t USBHostPTP::SetObjectProtection(uint32_t handle, uint16_t attrib)
{
    OperFlags   flags = { 2, 0, 0, 0, 0, 0 };
    uint32_t    params[2];

    params[0] = handle;
    params[1] = (uint32_t)attrib;

    return Transaction(PIMA_OPERATION_CODE_SetObjectProtection, &flags, params);
}


uint16_t USBHostPTP::MoveObject(uint32_t handle, uint32_t storage_id, uint32_t parent)
{
    OperFlags   flags = { 3, 0, 0, 0, 0, 0 };
    uint32_t    params[3];

    params[0] = handle;
    params[1] = storage_id;
    params[2] = parent;

    return Transaction(PIMA_OPERATION_CODE_MoveObject, &flags, params);
}


uint16_t USBHostPTP::GetDevicePropDesc(const uint16_t pcode, void *parser)
{
    OperFlags   flags       = { 1, 0, 0, 1, 1, 0 };
    uint32_t    params[1];
    
    params[0] = (uint32_t)pcode;
    
    if(IsPropertySupported(pcode)==false)
       return PIMA_RETURN_CODE_DevicePropertyNotSupported;
       
    return Transaction(PIMA_OPERATION_CODE_GetDevicePropDesc, &flags, params, parser);
}


uint16_t USBHostPTP::GetDevicePropValue(const uint16_t pcode, void *parser)
{
    OperFlags   flags       = { 1, 0, 0, 1, 1, 0 };
    uint32_t    params[1];

    params[0] = (uint32_t)pcode;
    if(IsPropertySupported(pcode)==false)
       return PIMA_RETURN_CODE_DevicePropertyNotSupported;

    return Transaction(PIMA_OPERATION_CODE_GetDevicePropValue, &flags, params, parser);
}


uint16_t USBHostPTP::SetDevicePropValue(uint16_t pcode, uint32_t val)
{
    OperFlags   flags       = { 1, 0, 1, 1, 3, 4 };
    uint32_t    params[1];
    
    params[0]=pcode;

    this->dataContainer.length=0x10;
    this->dataContainer.type=0x0002;
    this->dataContainer.code=PIMA_OPERATION_CODE_SetDevicePropValue;
    this->dataContainer.transactionID=this->transactionCnt+1;
    this->dataContainer.payload[0]=val;
    

    if(IsPropertySupported(pcode)==false)
       return PIMA_RETURN_CODE_DevicePropertyNotSupported;

    return Transaction(PIMA_OPERATION_CODE_SetDevicePropValue, &flags, params, (void*)&(this->dataContainer));
}


uint16_t USBHostPTP::ResetDevicePropValue(const uint16_t pcode)
{
    OperFlags   flags       = { 1, 0, 0, 0 };
    uint32_t    params[1];

    params[0] = (uint32_t)pcode;

    if(IsPropertySupported(pcode)==false)
       return PIMA_RETURN_CODE_DevicePropertyNotSupported;

    return Transaction(PIMA_OPERATION_CODE_ResetDevicePropValue, &flags, params);
}


uint16_t USBHostPTP::FormatStore(uint32_t storage_id, uint32_t fsformat)
{
    OperFlags   flags       = { 2, 0, 0, 0, 0, 0 };

    uint32_t    params[2]; 
    params[0]   = storage_id;
    params[1]   = fsformat;

    return Transaction(PIMA_OPERATION_CODE_FormatStore, &flags, params);
}

uint16_t USBHostPTP::InitiateCapture(uint32_t storage_id, uint16_t format)
{
    uint16_t    ptp_error = PIMA_RETURN_CODE_GeneralError;
    OperFlags   flags = { 2, 0, 0, 0, 0, 0 };
    uint32_t    params[2];

    params[0] = storage_id;
    params[1] = (uint32_t)format;

    if ( (ptp_error = Transaction(PIMA_OPERATION_CODE_InitiateCapture, &flags, params)) == PIMA_RETURN_CODE_OK){
    }

    return ptp_error;
}

uint16_t USBHostPTP::InitiateOpenCapture(uint32_t storage_id, uint16_t format)
{
    uint16_t    ptp_error = PIMA_RETURN_CODE_GeneralError;
    OperFlags   flags = { 2, 0, 0, 0, 0, 0 };
    uint32_t    params[2];

    params[0] = storage_id;
    params[1] = (uint32_t)format;

    if ( (ptp_error = Transaction(PIMA_OPERATION_CODE_InitiateOpenCapture, &flags, params)) == PIMA_RETURN_CODE_OK){
    }

    return ptp_error;
}
    
uint16_t USBHostPTP::TerminateOpenCapture(uint32_t trans_id)
{
    OperFlags   flags = { 1, 0, 0, 0, 0, 0 };
    uint32_t    params[1];

    params[0] = trans_id;

    return Transaction(PIMA_OPERATION_CODE_TerminateOpenCapture, &flags, params);
}



void USBHostPTP::ParseDeviceInfoDataBlock(void *ptp,uint8_t *buffer,uint16_t length){
     uint8_t *currentPtr;
     USBHostPTP *dev=(USBHostPTP *)ptp;
     
     currentPtr=buffer;
     dev->deviceInfo.standardVersion=*((uint16_t *)currentPtr);
     currentPtr+=2;
     dev->deviceInfo.vendorExtensionID=*((uint32_t *)currentPtr);
     currentPtr+=4;
     dev->deviceInfo.vendorExtensionVersion=*((uint16_t *)currentPtr);
     currentPtr+=2;
     
     currentPtr+=dev->deviceInfo.vendorExtensionDesc.FillString(currentPtr);
          
     dev->deviceInfo.functionMode=*((uint16_t *)currentPtr);
     currentPtr+=2;
     
     currentPtr+=dev->deviceInfo.operationsSupported.FillArray(currentPtr);
     currentPtr+=dev->deviceInfo.eventsSupported.FillArray(currentPtr);
     currentPtr+=dev->deviceInfo.devicePropertiesSupported.FillArray(currentPtr);
     currentPtr+=dev->deviceInfo.captureFormats.FillArray(currentPtr);
     currentPtr+=dev->deviceInfo.imageFormats.FillArray(currentPtr);
 
     currentPtr+=dev->deviceInfo.manufacturer.FillString(currentPtr);
     currentPtr+=dev->deviceInfo.model.FillString(currentPtr);
     currentPtr+=dev->deviceInfo.deviceVersion.FillString(currentPtr);
     currentPtr+=dev->deviceInfo.serialNumber.FillString(currentPtr);
    
}

void USBHostPTP::ParseObjectInfoDataBlock(void *ptp,uint8_t *buffer,uint16_t length){
      //TEST: PASS
      //   iterated thru getObjectInfo for all 789 objects on phone in a loop 200 times
      //   memory dumps show no buffer over runs on objectInfo
      uint8_t *ptr,*structptr;
      USBHostPTP *obj=(USBHostPTP *)ptp; 
      
      structptr=(uint8_t *) &(obj->objectInfo);
      ptr=buffer;
      ////TODO: MAGIC number issue
      for(int i=0;i<0x34;i++)
         structptr[i]=ptr[i];
      ptr+=0x34;
      ptr+=obj->objectInfo.filename.FillString(ptr);
      ptr+=obj->objectInfo.captureDate.FillString(ptr);
      ptr+=obj->objectInfo.modificationDate.FillString(ptr);
      ptr+=obj->objectInfo.keywords.FillString(ptr);
      #ifdef USDBPTPDEBUG
      obj->DumpObjectInfo();
      #endif 
}

void USBHostPTP::ParseStorageInfoDataBlock(void *ptp,uint8_t *buffer,uint16_t length){
      uint8_t *ptr,*structptr;
      USBHostPTP *obj=(USBHostPTP *)ptp; 
      
      structptr=(uint8_t *) &(obj->storageInfo);
      ptr=buffer;
      for(int i=0;i<0x1a;i++)
         structptr[i]=ptr[i];
      ptr+=0x1a;
      ptr+=obj->storageInfo.storageDescription.FillString(ptr);
      obj->storageInfo.volumeLabel.FillString(ptr);
}

void USBHostPTP::ParseDevicePropDescrBlock(void *ptp,uint8_t *buffer,uint16_t length){
   //PIMAPropDescr *temp;
   
   //temp= new PIMAPropDescr;
   //temp.fill(buffer);
   
}

bool USBHostPTP::IsCommandSupported(uint16_t opcode){
   bool res=false;

   if(opcode==PIMA_OPERATION_CODE_OpenSession || opcode==PIMA_OPERATION_CODE_GetDeviceInfo)
       res=true;
   else
       res = deviceInfo.operationsSupported.CheckValueInArray(opcode);
   return res;
}

bool USBHostPTP::IsPropertySupported(uint16_t code){
  
   return deviceInfo.devicePropertiesSupported.CheckValueInArray(code);
   
}




void USBHostPTP::DumpBuffer(uint8_t *buffer,uint32_t length){
  for(int i=0;i<length;i++) {
     printf("%02x ",buffer[i]);
     if(i % 16 == 15 )
        printf("\r\n");   
  }
  printf("\r\n");
}

void USBHostPTP::PrintCodeInformation(uint16_t value){
    char stringValue[80];
    bool found=false;
    
    if( CodeDecoderFunction != NULL) {
       found = (*CodeDecoderFunction)(value,stringValue);
    }
    if (found) {
       printf("0x%04x:  %s\r\n",value,stringValue);
    } else {
       printf("0x%04x:  (Unknown)\r\n",value);
    }
}

void USBHostPTP::DumpDeviceInfo(){
     int i;
     
     printf("PIMA standard version:    %x\r\n",deviceInfo.standardVersion);
     printf("Vendor Extension ID:      %x\r\n",deviceInfo.vendorExtensionID);
     printf("Vendor Extension Version: %x\r\n",deviceInfo.vendorExtensionVersion);
     printf("Vendor Extension Description: \r\n");
     printf("%s\r\n",(deviceInfo.vendorExtensionDesc.getString()));
     printf("Functional Mode:          %x\r\n",deviceInfo.functionMode);
     printf("<<< Supported Operations: >>>\r\n");
     for(i=0;i<deviceInfo.operationsSupported.numberOfElements;i++) {
       PrintCodeInformation(deviceInfo.operationsSupported.GetElement(i));
    }
     printf("<<< Supported Events: >>>\r\n");
     for(i=0;i<deviceInfo.eventsSupported.numberOfElements;i++) {
       PrintCodeInformation(deviceInfo.eventsSupported.GetElement(i));
     }
     printf("<<< Supported Device Properties: >>>\r\n");
     for(i=0;i<deviceInfo.devicePropertiesSupported.numberOfElements;i++) {
       PrintCodeInformation(deviceInfo.devicePropertiesSupported.GetElement(i));
     }
     printf("<<< Supported Capture Formats: >>>\r\n");
     for(i=0;i<deviceInfo.captureFormats.numberOfElements;i++) {
       PrintCodeInformation(deviceInfo.captureFormats.GetElement(i));
     }
     printf("<<< Supported Image Formats: >>>\r\n");
     for(i=0;i<deviceInfo.imageFormats.numberOfElements;i++) {
       PrintCodeInformation(deviceInfo.imageFormats.GetElement(i));
    }
     
     printf("Manufacturer: \r\n");
     printf("%s\r\n",deviceInfo.manufacturer.getString());
     printf("Model: \r\n");
     printf("%s\r\n",deviceInfo.model.getString());
     printf("Device Version: \r\n");
     printf("%s\r\n",deviceInfo.deviceVersion.getString());
     printf("Serial Number: \r\n");
     printf("%s\r\n",deviceInfo.serialNumber.getString());
     
}

void USBHostPTP::DumpObjectInfo(){
     
     
     printf("-----------------------ObjectInfo--------------\r\n");
     printf("Storage ID:                %08x\r\n",objectInfo.storageID);
     printf("Object Format:             %04x\r\n",objectInfo.objectFormat);
     printf("Protection Status:         %04x\r\n",objectInfo.protectionStatus);
     printf("Compressed Size:           %ld\r\n",objectInfo.objectCompressSize);
     printf("Thumbnail format:          %04x\r\n",objectInfo.thumbFormat);
     printf("Thumbnail Compressed size: %04x\r\n",objectInfo.thumbCompressedSize);
     printf("Thumbnail Width            %ld\r\n",objectInfo.thumbPixWidth);
     printf("Thumbnail Height:          %ld\r\n",objectInfo.thumbPixHeight);
     printf("Image Width:               %ld\r\n",objectInfo.imagePixWidth);
     printf("Image Height:              %ld\r\n",objectInfo.imagePixHeight);
     printf("Image BitDepth:            %ld\r\n",objectInfo.imageBitDepth);
     printf("Parent Object:             %04x\r\n",objectInfo.parentObject);
     printf("Assosiaction Type:         %02x\r\n",objectInfo.associationType);
     printf("Assosiaction Description:  %04x\r\n",objectInfo.associationDesc);
     printf("Sequence Number:           %ld\r\n",objectInfo.sequenceNumber);          
     
     printf("Filename: \r\n");
     printf("%s\r\n",objectInfo.filename.getString());
     printf("Capture Date: \r\n");
     printf("%s\r\n",objectInfo.captureDate.getString());
     printf("Modification Date: \r\n");
     printf("%s\r\n",objectInfo.modificationDate.getString());
     printf("Keywords: \r\n");
     printf("%s\r\n",objectInfo.keywords.getString());
}

void USBHostPTP::DumpResponseContainer(){
      printf("PTPResponseContainer:\r\n");
      this->DumpBuffer((uint8_t *)&responseContainer,responseContainer.length);
      if (responseContainer.code==PIMA_RETURN_CODE_OK) {
         printf("PIMA_RETURN_CODE_OK\r\n");
      } else {
         printf("Response Length: %x\r\n",responseContainer.length);
         printf("Response type: %x\r\n",responseContainer.type);
         printf("Response code: %x\r\n",responseContainer.code);
         printf("Response ID: %d\r\n",responseContainer.transactionID);
      }

}

#endif




