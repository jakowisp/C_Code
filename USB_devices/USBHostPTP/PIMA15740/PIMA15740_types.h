/**
*  @file PIMA15740_types.h
*  @brief PIMA type definitions
*  @author Dwayne Dilbeck
*  @date 8/23/2013
* 
* @par Copyright:
*  Copyright (c) 2013 Dwayne Dilbeck
* @par License:
*  This software is distributed under the terms of the GNU Lesser General Public License
*
*  mbed USBHostPTP Library(PIMA15740 Constants definition)
 */
 
#include "PIMAconst.h"
#include "PIMAArray.h"
#include "PIMAString.h"
#include "PIMAPropDescr.h"

/** @struct PIMAContainer
 *  PIMA 15740:2000 standard Appendix D.7.1.1   
*/      
typedef __packed struct {
     uint32_t length;
     uint16_t type;
     uint16_t code;
     uint32_t transactionID;
     uint32_t payload[5];
    } PIMAContainer;

/// PIMA 15740:2000 standard 5.5.1 Table 6        
typedef struct {
    uint16_t            standardVersion;
    uint32_t            vendorExtensionID;
    uint16_t            vendorExtensionVersion;
    PIMAString          vendorExtensionDesc;
    uint16_t            functionMode;
    PIMAArray<uint16_t> operationsSupported;
    PIMAArray<uint16_t> eventsSupported;
    PIMAArray<uint16_t> devicePropertiesSupported;
    PIMAArray<uint16_t> captureFormats;
    PIMAArray<uint16_t> imageFormats;
    PIMAString          manufacturer;
    PIMAString          model;
    PIMAString          deviceVersion;
    PIMAString          serialNumber;
} DeviceInfoStruct;

/// PIMA 15740:2000 standard 5.5.2 Table 8
typedef struct {
    uint32_t   storageID;            //0x0
    uint16_t   objectFormat;         //0x4
    uint16_t   protectionStatus;     //0x6
    uint32_t   objectCompressSize;   //0x8
    uint16_t   thumbFormat;          //0xc
    uint32_t   thumbCompressedSize;  //0xd
    uint32_t   thumbPixWidth;        //0x12
    uint32_t   thumbPixHeight;       //0x16
    uint32_t   imagePixWidth;        //0x1a
    uint32_t   imagePixHeight;       //0x1e
    uint32_t   imageBitDepth;        //0x22
    uint32_t   parentObject;         //0x26
    uint16_t   associationType;      //0x2a
    uint32_t   associationDesc;      //0x2c
    uint32_t   sequenceNumber;       //0x30
    PIMAString filename;             //0x34
    PIMAString captureDate; 
    PIMAString modificationDate;
    PIMAString keywords;
} ObjectInfoStruct;

/// PIMA 15740:2000 standard 5.5.3 Table 10
typedef struct {
    uint16_t   storageType;
    uint16_t   fileSystemType;
    uint16_t   accessCapability;
    uint64_t   maxCapacity;
    uint64_t   freeSpaceInBytes;
    uint32_t   freeSpaceInImages;
    PIMAString storageDescription;
    PIMAString volumeLabel;
} StorageInfoStruct;
