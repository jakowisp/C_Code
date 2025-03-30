/**
*  @file USBHostPTP.h
*  @brief USBHostPTP class definition
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


#ifndef USBHOSTPTP_H
#define USBHOSTPTP_H

#include "USBHostConf.h"
#include "USBHost.h"
#include "PIMA15740_types.h"

//#define USDBPTPDEBUG 1

 
    
/** 
 * USB PTP definitions
 *    
 */
#define USB_PTP_CLASS_CODE 0x06
#define USB_PTP_CLASS_REQUEST_CANCELREQEUST           0x64
#define USB_PTP_CLASS_REQUEST_GET_EXTENDED_EVENT_DATA 0x65
#define USB_PTP_CLASS_REQUEST_DEVICE_RESET            0x66
#define USB_PTP_CLASS_REQUEST_GET_DEVICE_STATUS       0x67

//Define a data handler function pointer type to allow handler function pointeers to be readable in function definitions.
typedef void (*DataHandler)(void *object, uint8_t *bufffer, uint16_t length);
typedef bool (*CodeDecoder)(uint16_t code, char *bufffer);

/**
 * USBHostPTP class
 *
 */        
class USBHostPTP : public IUSBEnumerator {
public:

   /**
    * Define a set of flags to make calls to execute Operations a simple wrapper.  
    * This structure is copied from the 'Circuits At Home' Arduino Library to match thier format.
    */
   struct OperFlags
        {
                uint16_t        opParams        :       3;                      // 7    - maximum number of operation parameters
                uint16_t        rsParams        :       3;                      // 7    - maximum number of response parameters
                uint16_t        txOperation     :       1;                      // I->R operation if the flag is set
                uint16_t        dataStage       :       1;                      // operation has data stage if the flag is set
                uint16_t        typeOfVoid      :       2;                      // NOT USED
                uint16_t        dataSize        :       6;                      // NOT USED - This is meant to be used when sending data to the PTP device               
        };     

    /**
    * Constructor
    *
    * @param None
    */
    USBHostPTP(void);

    /**
    * Check if a PTP device is connected
    *
    * @return true if a PTP device is connected
    */
    bool connected();

    /**
     * Try to connect to a PTP device
     *
     * @return true if connection was successful
     */
    bool connect();

    /**
     *  Cancel Request
     *  USB PTP class specific operation to allow cancelation of a bulk transaction
     *
     *  @param TransactionID  The idnumber of the transaction to cancel
     *
     *  @return true if the operation was succesful
     */
    bool CancelRequest(unsigned int TransactionID);
    
    /**
     *  DeviceResetRequest
     *  The Device Reset Request is used by the host to return the 
     *   Still Image Capture Device to the Idle state after the Bulk-pipe
     *   has stalled. The request may also be used to clear any vendor specified suspend conditions.
     *
     *  @param none
     *
     *  @return true if the operation was succesful
     */
    bool DeviceResetRequest(void);
    
    
    /**
     *  GetDeviceStatus
     *  This request is used by the host to retrieve information needed to 
     *  clear halted endpoints that result from a device initiated data transfer cancellation.
     *
     *  @param none
     *
     *  @return true if the operation was succesful
     */
    bool GetDeviceStatus(void);
    
    /**
     *  GetExtendedEventData
     *  The data stage transfers to the host extended information regarding
     *   an asynchronous event or vendor condition.
     *
     *  @param none
     *
     *  @return true if the operation was succesful
     */
    bool GetExtendedEventData(void);
    
    ///Provide external access to deviceInfo
    DeviceInfoStruct deviceInfo;
    ///provide external access to ObjectInfo
    ObjectInfoStruct objectInfo;
    ///provide external access to storageInfo
    StorageInfoStruct storageInfo;
    ///Provide a visible counter of bytes remaining during long transfers
    uint32_t dataLeftToTransfer;
    ///Provide a visible value of total data to transfer during long transfers
    uint32_t totalDataToTransfer;
    
    /**
     *  Simple PTP operation caller which has no data stage. Any number of uint32_t params can be supplied.
     *
     *  @param opcode The operation to send to the device for execeution
     *  @param nparams The number of parameters being supplied for the operation
     *  @param params  A pointer to the paratmers array to be used.
     *
     *  @return A PTP response code
     */
     uint16_t Operation(uint16_t opcode, uint8_t nparams = 0, uint32_t *params = NULL);
     
     /**
     * Open a new session on the PTP device, this is required before any other operations besides GetDeviceInfo can be called.
     *
     * @param none
     * @return  A PTP response code
     */
    uint16_t OpenSession();
    
    /**
     * Close the currently open session. The PIMA standard does not allow muliptl session to be open at a time.
     *
     * @param none
     * @return  A PTP response code
     */
     uint16_t CloseSession();
     
    /**
     * This command get the device configuration information, including the operations, porperties, and events the divce supports.
     * @param none
     * @return  A PTP response code
     */
    uint16_t GetDeviceInfo();
    
    /**
     * Request a device power down.
     *
     * @param none
     * @return  A PTP response code
     */
    uint16_t PowerDown();
    
    /**
     * Execute a self test
     *
     * @param type (Optional) Specify the test to execute.
     * @param none
     * @return  A PTP response code
     */
    uint16_t SelfTest(uint16_t type = 0);
    
    /**
     * Get the number of objects stored on the device
     *
     * @param retval a pointer to a uint32_t value , this will be set by the function to return the value
     * @param storage_id Provide a way to filter by storage ID.
     * @param format Provide a way to filter by format. PTP devices are not required to support filtering by format
     * @param assoc  Provide a way to filter by association. PTP devices are not required to support filtering by association
     * @return  A PTP response code
     */
    uint16_t GetNumObjects(uint32_t *retval, uint32_t storage_id=0xffffffff, uint16_t format=0x0000, uint32_t assoc=0x0000);
    
    /**
     * Get an array of file handles for the objects stored on the device
     *
     * @param storage_id Provide a way to filter by storage ID.
     * @param format Provide a way to filter by format. PTP devices are not required to support filtering by format
     * @param assoc  Provide a way to filter by association. PTP devices are not required to support filtering by association
     * @param parser A pointer to a function to handle the data returned from the device. If left in the default value of NULL the data is thrown away as it is recieved.
     * @return  A PTP response code
     */
    uint16_t GetObjectHandles(uint32_t storage_id=0xffffffff, uint16_t format=0x0000, uint16_t assoc=0x0000, void *parser=NULL);

    /**
     * Get information about an object
     *
     * @param handle the file handle of the object to get information about.
     * @return  A PTP response code
     */
    uint16_t GetObjectInfo(uint32_t handle);
    /**
     * Ge a thumbnail for an object
     *
     * @param handle the file handle of the object.
     * @param parser A pointer to a function to handle the data returned from the device. If left in the default value of NULL the data is thrown away as it is recieved.
     * @return  A PTP response code
     */
    uint16_t GetThumb(uint32_t handle, void *parser=NULL);
    /**
     *
     * @param handle
     * @param parser A pointer to a function to handle the data returned from the device. If left in the default value of NULL the data is thrown away as it is recieved.
     * @return  A PTP response code
     */
    uint16_t GetObject(uint32_t handle, void *parser=NULL);
    /**
     *
     * @param parser A pointer to a function to handle the data returned from the device. If left in the default value of NULL the data is thrown away as it is recieved.
     * @return  A PTP response code
     */
    uint16_t GetStorageIDs(void *parser=NULL);

    /**
     * Obtain inforamtion about a storge.
     *
     * @param storage_id
     * @return  A PTP response code
     */
    uint16_t GetStorageInfo(uint32_t storage_id);

    /**
     *
     * @param handle
     * @param storage_id
     * @param parent
     * @param new_handle
     * @return  A PTP response code
     */
    uint16_t CopyObject(uint32_t handle, uint32_t storage_id, uint32_t parent, uint32_t *new_handle);
    /**
     *
     * @param handle
     * @param format
     * @return  A PTP response code
     */
    uint16_t DeleteObject(uint32_t handle, uint16_t format);
    /**
     *
     * @param handle
     * @param attrib
     * @return  A PTP response code
     */
    uint16_t SetObjectProtection(uint32_t handle, uint16_t attrib);
    /**
     *
     * @param handle
     * @param storage_id
     * @param parent
     * @return  A PTP response code
     */
    uint16_t MoveObject(uint32_t handle, uint32_t storage_id, uint32_t parent);
    /**
     *
     * @param pcode
     * @param parser A pointer to a function to handle the data returned from the device. If left in the default value of NULL the data is thrown away as it is recieved.
     * @return  A PTP response code
     */
    uint16_t GetDevicePropDesc(const uint16_t pcode, void *parser=NULL);
    /**
     *
     * @param pcode
     * @param parser A pointer to a function to handle the data returned from the device. If left in the default value of NULL the data is thrown away as it is recieved.
     * @return  A PTP response code
     */
    uint16_t GetDevicePropValue(const uint16_t pcode, void *parser=NULL);
    /**
     *
     * @param pcode
     * @param val
     * @return  A PTP response code
     */
    uint16_t SetDevicePropValue(uint16_t pcode, uint32_t val);
    /**
     *
     * @param pcode
     * @return  A PTP response code
     */
    uint16_t ResetDevicePropValue(const uint16_t pcode);
    /**
     *
     * @param storage_id
     * @param fsformat
     * @return  A PTP response code
     */
    uint16_t FormatStore(uint32_t storage_id, uint32_t fsformat);
    /**
     *
     * @param storage_id
     * @param format
     * @return  A PTP response code
     */
    uint16_t InitiateCapture(uint32_t storage_id, uint16_t format);
    /**
     *
     * @param storage_id
     * @param format
     * @return  A PTP response code
     */
    uint16_t InitiateOpenCapture(uint32_t storage_id, uint16_t format);
    
    /**
     *
     * @param tran_id
     * @return  A PTP response code
     */
    uint16_t TerminateOpenCapture(uint32_t trans_id);
    
    /**
     * Send the contents of the deviceInfo variable via serial communication
     * @param none
     * @return  void
     */
    void DumpDeviceInfo(void);
    
    /**
     * Send the contents of the objectInfo variable via serial communication
     * @param none
     * @return  void
     */
    void DumpObjectInfo(void);
    PIMAContainer      dataContainer;
    
    //This is a generic function should belong to some other class or namespance.
    void DumpBuffer(uint8_t *buffer,uint32_t length);
    void (*LookupCodeStringFunctionPointer)(uint16_t value,char *stringValue);
 
    CodeDecoder CodeDecoderFunction;

//protected:
    //From IUSBEnumerator
    virtual void setVidPid(uint16_t vid, uint16_t pid);
    virtual bool parseInterface(uint8_t intf_nb, uint8_t intf_class, uint8_t intf_subclass, uint8_t intf_protocol); //Must return true if the interface should be parsed
    virtual bool useEndpoint(uint8_t intf_nb, ENDPOINT_TYPE type, ENDPOINT_DIRECTION dir); //Must return true if the endpoint will be used

    void DumpResponseContainer(void);
    void PrintCodeInformation(uint16_t value);
    
//private:
    USBHost            * host;
    USBDeviceConnected * pointerToDevice;
    bool               deviceFound;
    bool               deviceConnected;
    USBEndpoint        * bulk_in;
    USBEndpoint        * bulk_out;
    USBEndpoint        * int_in;
    uint8_t            numberOfEndpoints;
    
    PIMAContainer      commandContainer;
    PIMAContainer      responseContainer;
    //PIMAContainer      dataContainer;
    PIMAContainer      eventContainer;
    PIMAPropDesc       propertyDescription;
    uint32_t           transactionCnt;
    uint32_t           sessionID;
    uint8_t            buffer[1024];
    int                ptp_intf;
    
    void initializeClassData();
    int  checkResult(uint8_t res, USBEndpoint * ep);
    bool IsCommandSupported(uint16_t opcode);
    bool IsPropertySupported(uint16_t code);
    bool CheckEvent(void);
    static void ParseDeviceInfoDataBlock(void *ptp, uint8_t *buffer,uint16_t length); 
    static void ParseObjectInfoDataBlock(void *ptp, uint8_t *buffer,uint16_t length);
    static void ParseStorageInfoDataBlock(void *ptp, uint8_t *buffer,uint16_t length);
    static void ParseDevicePropDescrBlock(void *ptp,uint8_t *buffer,uint16_t length);
    void PrepareControlContainer(uint16_t operationCode, 
                                 OperFlags *operationFlags,
                                 uint32_t *parameters);
    bool RecieveDataContainer(void *dataHandlerFunctionCastToVoid, uint16_t *responseCode);
    bool SendDataContainer(void *dataGeneratorFunctionCastToVoid, uint16_t *responseCode);
    uint16_t Transaction(uint16_t operationCode, OperFlags *flags, uint32_t *params = NULL, void *pVoid = NULL);

};

#endif




