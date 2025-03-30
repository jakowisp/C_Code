/**
*  @file PIMAconst.h
*  @brief PIMA constants definitions
*  @author Dwayne Dilbeck
*  @date 8/23/2013
* 
* @par Copyright:
*  Copyright (c) 2013 Dwayne Dilbeck
* @par License:
*  This software is distributed under the terms of the GNU Lesser General Public License
*
* mbed USBHostPTP Library(PIMA15740 Constants definition)
 *
 * Special thanks to the 'Circuits at Home', 
 * Reviewing thier code helped in understanding what needed to be done in my code for the mbed.
 *
 * @par Reference
 * Circuits At Home, LTD
 * Web      :  http://www.circuitsathome.com 
 * e-mail   :  support@circuitsathome.com
 */
 

#ifndef __PIMACONST_H__
#define __PIMACONST_H__

#define PIMA_CONTAINER_HEADER_LENGTH  0xc

// PIMA container types
// PIMA 15740:2000 standard Appendix D.7.1.1 
#define PIMA_CONTAINER_UNDEFINED         0x0000
#define PIMA_CONTAINER_COMMAND           0x0001
#define PIMA_CONTAINER_DATA              0x0002
#define PIMA_CONTAINER_RESPONSE          0x0003
#define PIMA_CONTAINER_EVENT             0x0004

/* Vendor IDs */
//http://www.imaging.org/ist/resources/standards/files/Final_PTP_Vendor_Extension_Registry.pdf
#define PTP_VENDOR_EASTMAN_KODAK            0x00000001
#define PTP_VENDOR_SEIKO_EPSON              0x00000002
#define PTP_VENDOR_AGILENT                  0x00000003
#define PTP_VENDOR_POLAROID                 0x00000004
#define PTP_VENDOR_AGFA_GEVAERT             0x00000005
#define PTP_VENDOR_MICROSOFT                0x00000006
#define PTP_VENDOR_EQUINOX                  0x00000007
#define PTP_VENDOR_VIEWQUEST                0x00000008
#define PTP_VENDOR_STMICROELECTRONICS       0x00000009
#define PTP_VENDOR_NIKON                    0x0000000A
#define PTP_VENDOR_CANON                    0x0000000B
#define PTP_VENDOR_FOTONATION               0x0000000C
#define PTP_VENDOR_PENTAX                   0x0000000D
#define PTP_VENDOR_FUJI                     0x0000000E
#define PTP_VENDOR_SAMSUNG                  0x0000001A

/* Operation Codes */
// PIMA 15740:2000 standard 10.3 Table 18
#define PIMA_OPERATION_CODE_Undefined                    0x1000
#define PIMA_OPERATION_CODE_GetDeviceInfo                0x1001
#define PIMA_OPERATION_CODE_OpenSession                  0x1002
#define PIMA_OPERATION_CODE_CloseSession                 0x1003
#define PIMA_OPERATION_CODE_GetStorageIDs                0x1004
#define PIMA_OPERATION_CODE_GetStorageInfo               0x1005
#define PIMA_OPERATION_CODE_GetNumObjects                0x1006
#define PIMA_OPERATION_CODE_GetObjectHandles             0x1007
#define PIMA_OPERATION_CODE_GetObjectInfo                0x1008
#define PIMA_OPERATION_CODE_GetObject                    0x1009
#define PIMA_OPERATION_CODE_GetThumb                     0x100A
#define PIMA_OPERATION_CODE_DeleteObject                 0x100B
#define PIMA_OPERATION_CODE_SendObjectInfo               0x100C
#define PIMA_OPERATION_CODE_SendObject                   0x100D
#define PIMA_OPERATION_CODE_InitiateCapture              0x100E
#define PIMA_OPERATION_CODE_FormatStore                  0x100F
#define PIMA_OPERATION_CODE_ResetDevice                  0x1010
#define PIMA_OPERATION_CODE_SelfTest                     0x1011
#define PIMA_OPERATION_CODE_SetObjectProtection          0x1012
#define PIMA_OPERATION_CODE_PowerDown                    0x1013
#define PIMA_OPERATION_CODE_GetDevicePropDesc            0x1014
#define PIMA_OPERATION_CODE_GetDevicePropValue           0x1015
#define PIMA_OPERATION_CODE_SetDevicePropValue           0x1016
#define PIMA_OPERATION_CODE_ResetDevicePropValue         0x1017
#define PIMA_OPERATION_CODE_TerminateOpenCapture         0x1018
#define PIMA_OPERATION_CODE_MoveObject                   0x1019
#define PIMA_OPERATION_CODE_CopyObject                   0x101A
#define PIMA_OPERATION_CODE_GetPartialObject             0x101B
#define PIMA_OPERATION_CODE_InitiateOpenCapture          0x101C

/* Proprietary vendor extension operations mask */
// PIMA 15740:2000 standard 10.3 Table 18
#define PIMA_OPERATION_CODE_EXTENSION                    0x9000

/* Response Codes */
// PIMA 15740:2000 standard 11.2 Table 20
#define PIMA_RETURN_CODE_Undefined                              0x2000
#define PIMA_RETURN_CODE_OK                                     0x2001
#define PIMA_RETURN_CODE_GeneralError                           0x2002
#define PIMA_RETURN_CODE_SessionNotOpen                         0x2003
#define PIMA_RETURN_CODE_InvalidTransactionID                   0x2004
#define PIMA_RETURN_CODE_OperationNotSupported                  0x2005
#define PIMA_RETURN_CODE_ParameterNotSupported                  0x2006
#define PIMA_RETURN_CODE_IncompleteTransfer                     0x2007
#define PIMA_RETURN_CODE_InvalidStorageId                       0x2008
#define PIMA_RETURN_CODE_InvalidObjectHandle                    0x2009
#define PIMA_RETURN_CODE_DevicePropertyNotSupported             0x200A
#define PIMA_RETURN_CODE_InvalidObjectFormatCode                0x200B
#define PIMA_RETURN_CODE_StoreFull                              0x200C
#define PIMA_RETURN_CODE_ObjectWriteProtected                   0x200D
#define PIMA_RETURN_CODE_StoreReadOnly                          0x200E
#define PIMA_RETURN_CODE_AccessDenied                           0x200F
#define PIMA_RETURN_CODE_NoThumbnailPresent                     0x2010
#define PIMA_RETURN_CODE_SelfTestFailed                         0x2011
#define PIMA_RETURN_CODE_PartialDeletion                        0x2012
#define PIMA_RETURN_CODE_StoreNotAvailable                      0x2013
#define PIMA_RETURN_CODE_SpecificationByFormatUnsupported       0x2014
#define PIMA_RETURN_CODE_NoValidObjectInfo                      0x2015
#define PIMA_RETURN_CODE_InvalidCodeFormat                      0x2016
#define PIMA_RETURN_CODE_UnknownVendorCode                      0x2017
#define PIMA_RETURN_CODE_CaptureAlreadyTerminated               0x2018
#define PIMA_RETURN_CODE_DeviceBusy                             0x2019
#define PIMA_RETURN_CODE_InvalidParentObject                    0x201A
#define PIMA_RETURN_CODE_InvalidDevicePropFormat                0x201B
#define PIMA_RETURN_CODE_InvalidDevicePropValue                 0x201C
#define PIMA_RETURN_CODE_InvalidParameter                       0x201D
#define PIMA_RETURN_CODE_SessionAlreadyOpened                   0x201E
#define PIMA_RETURN_CODE_TransactionCanceled                    0x201F
#define PIMA_RETURN_CODE_SpecificationOfDestinationUnsupported  0x2020

/* Proprietary vendor extension response code mask */
// PIMA 15740:2000 standard 11.2 Table 20
#define PIMA_RETURN_CODE_EXTENSION                    0xA000

/* PIMA Event Codes */
// PIMA 15740:2000 standard 12.4  Table 22
#define PIMA_EVENT_CODE_Undefined                    0x4000
#define PIMA_EVENT_CODE_CancelTransaction            0x4001
#define PIMA_EVENT_CODE_ObjectAdded                  0x4002
#define PIMA_EVENT_CODE_ObjectRemoved                0x4003
#define PIMA_EVENT_CODE_StoreAdded                   0x4004
#define PIMA_EVENT_CODE_StoreRemoved                 0x4005
#define PIMA_EVENT_CODE_DevicePropChanged            0x4006
#define PIMA_EVENT_CODE_ObjectInfoChanged            0x4007
#define PIMA_EVENT_CODE_DeviceInfoChanged            0x4008
#define PIMA_EVENT_CODE_RequestObjectTransfer        0x4009
#define PIMA_EVENT_CODE_StoreFull                    0x400A
#define PIMA_EVENT_CODE_DeviceReset                  0x400B
#define PIMA_EVENT_CODE_StorageInfoChanged           0x400C
#define PIMA_EVENT_CODE_CaptureComplete              0x400D
#define PIMA_EVENT_CODE_UnreportedStatus             0x400E
// PIMA 15740:2000 standard 12.4 Table 22
#define PIMA_EVENT_CODE_VENDOR_EXTENSION             0xC000


/* PIMA15740 Object Format Codes */
// PIMA 15740:2000 standard 6.2 Table 14
#define PIMA_OBJECT_FORMAT_CODE_Undefined                   0x3000
#define PIMA_OBJECT_FORMAT_CODE_ASSOCIATION                 0x3001
#define PIMA_OBJECT_FORMAT_CODE_Script                      0x3002
#define PIMA_OBJECT_FORMAT_CODE_Executable                  0x3003
#define PIMA_OBJECT_FORMAT_CODE_Text                        0x3004
#define PIMA_OBJECT_FORMAT_CODE_HTML                        0x3005
#define PIMA_OBJECT_FORMAT_CODE_DPOF                        0x3006
#define PIMA_OBJECT_FORMAT_CODE_AIFF                        0x3007
#define PIMA_OBJECT_FORMAT_CODE_WAV                         0x3008
#define PIMA_OBJECT_FORMAT_CODE_MP3                         0x3009
#define PIMA_OBJECT_FORMAT_CODE_AVI                         0x300A
#define PIMA_OBJECT_FORMAT_CODE_MPEG                        0x300B
#define PIMA_OBJECT_FORMAT_CODE_ASF                         0x300C
/* image formats */
#define PIMA_OBJECT_FORMAT_CODE_EXIF_JPEG                   0x3801
#define PIMA_OBJECT_FORMAT_CODE_TIFF_EP                     0x3802
#define PIMA_OBJECT_FORMAT_CODE_FlashPix                    0x3803
#define PIMA_OBJECT_FORMAT_CODE_BMP                         0x3804
#define PIMA_OBJECT_FORMAT_CODE_CIFF                        0x3805
#define PIMA_OBJECT_FORMAT_CODE_Undefined_0x3806            0x3806
#define PIMA_OBJECT_FORMAT_CODE_GIF                         0x3807
#define PIMA_OBJECT_FORMAT_CODE_JFIF                        0x3808
#define PIMA_OBJECT_FORMAT_CODE_PCD                         0x3809
#define PIMA_OBJECT_FORMAT_CODE_PICT                        0x380A
#define PIMA_OBJECT_FORMAT_CODE_PNG                         0x380B
#define PIMA_OBJECT_FORMAT_CODE_Undefined_0x380C            0x380C
#define PIMA_OBJECT_FORMAT_CODE_TIFF                        0x380D
#define PIMA_OBJECT_FORMAT_CODE_TIFF_IT                     0x380E
#define PIMA_OBJECT_FORMAT_CODE_JP2                         0x380F
#define PIMA_OBJECT_FORMAT_CODE_JPX                         0x3810
// PIMA 15740:2000 standard 6.2 Table 14
#define PIMA_OBJECT_FORMAT_CODE_VENDOR_EXTENSION            0xB000


/* PIMA ASSOCIATION Types */
// PIMA 15740:2000 standard 6.4.1 Table 15
#define PIMA_ASSOCIATION_TYPE_Undefined                    0x0000
#define PIMA_ASSOCIATION_TYPE_GenericFolder                0x0001
#define PIMA_ASSOCIATION_TYPE_Album                        0x0002
#define PIMA_ASSOCIATION_TYPE_TimeSequence                 0x0003
#define PIMA_ASSOCIATION_TYPE_HorizontalPanoramic          0x0004
#define PIMA_ASSOCIATION_TYPE_VerticalPanoramic            0x0005
#define PIMA_ASSOCIATION_TYPE_2DPanoramic                  0x0006
#define PIMA_ASSOCIATION_TYPE_AncillaryData                0x0007
// PIMA 15740:2000 standard 6.4.1 Table 15
#define PIMA_ASSOCIATION_TYPE_VENDOR_EXTENSION            0xF000

/* PIMA Storage Types */
// PIMA 15740:2000 standard 5.5.3 Table 11 
#define PIMA_STORAGE_TYPE_Undefined                    0x0000
#define PIMA_STORAGE_TYPE_FixedROM                     0x0001
#define PIMA_STORAGE_TYPE_RemovableROM                 0x0002
#define PIMA_STORAGE_TYPE_FixedRAM                     0x0003
#define PIMA_STORAGE_TYPE_RemovableRAM                 0x0004

/* PIMA FilesystemType Values */
// PIMA 15740:2000 standard 5.5.3 Table 12
#define PIMA_FILESYSTEM_TYPE_Undefined                   0x0000
#define PIMA_FILESYSTEM_TYPE_GenericFlat                 0x0001
#define PIMA_FILESYSTEM_TYPE_GenericHierarchical         0x0002
#define PIMA_FILESYSTEM_TYPE_DCF                         0x0003
// PIMA 15740:2000 standard 5.5.3 Table 12
#define PIMA_VENDOR_Defined                              0xF000

/* PIMA StorageInfo AccessCapability Values */
// PIMA 15740:2000 standard 5.5.3 Table 13
#define PIMA_ACCESS_CAPABILITY_ReadWrite                        0x0000
#define PIMA_ACCESS_CAPABILITY_ReadOnl_without_Object_Deletion  0x0001
#define PIMA_ACCESS_CAPABILITY_ReadOnly_with_Object_Deletion    0x0002

/* DataType Codes */
// PIMA 15740:2000 standard 5.3 Table 3
#define PIMA_DATATYPE_CODE_UNDEF                       0x0000
#define PIMA_DATATYPE_CODE_INT8                        0x0001
#define PIMA_DATATYPE_CODE_UINT8                       0x0002
#define PIMA_DATATYPE_CODE_INT16                       0x0003
#define PIMA_DATATYPE_CODE_UINT16                      0x0004
#define PIMA_DATATYPE_CODE_INT32                       0x0005
#define PIMA_DATATYPE_CODE_UINT32                      0x0006
#define PIMA_DATATYPE_CODE_INT64                       0x0007
#define PIMA_DATATYPE_CODE_UINT64                      0x0008
#define PIMA_DATATYPE_CODE_INT128                      0x0009
#define PIMA_DATATYPE_CODE_UINT128                     0x000A
#define PIMA_DATATYPE_CODE_AINT8                       0x4001
#define PIMA_DATATYPE_CODE_AUINT8                      0x4002
#define PIMA_DATATYPE_CODE_AINT16                      0x4003
#define PIMA_DATATYPE_CODE_AUINT16                     0x4004
#define PIMA_DATATYPE_CODE_AINT32                      0x4005
#define PIMA_DATATYPE_CODE_AUINT32                     0x4006
#define PIMA_DATATYPE_CODE_AINT64                      0x4007
#define PIMA_DATATYPE_CODE_AUINT64                     0x4008
#define PIMA_DATATYPE_CODE_AINT128                     0x4009
#define PIMA_DATATYPE_CODE_AUINT128                    0x400A
#define PIMA_DATATYPE_CODE_STR                         0xFFFF

/* Device Properties Codes */
// PIMA 15740:2000 standard 13.3.5 Table 26
#define PIMA_DEVICE_PROPERTY_CODE_Undefined                   0x5000
#define PIMA_DEVICE_PROPERTY_CODE_BatteryLevel                0x5001
#define PIMA_DEVICE_PROPERTY_CODE_FunctionalMode              0x5002
#define PIMA_DEVICE_PROPERTY_CODE_ImageSize                   0x5003
#define PIMA_DEVICE_PROPERTY_CODE_CompressionSetting          0x5004
#define PIMA_DEVICE_PROPERTY_CODE_WhiteBalance                0x5005
#define PIMA_DEVICE_PROPERTY_CODE_RGBGain                     0x5006
#define PIMA_DEVICE_PROPERTY_CODE_FNumber                     0x5007
#define PIMA_DEVICE_PROPERTY_CODE_FocalLength                 0x5008
#define PIMA_DEVICE_PROPERTY_CODE_FocusDistance               0x5009
#define PIMA_DEVICE_PROPERTY_CODE_FocusMode                   0x500A
#define PIMA_DEVICE_PROPERTY_CODE_ExposureMeteringMode        0x500B
#define PIMA_DEVICE_PROPERTY_CODE_FlashMode                   0x500C
#define PIMA_DEVICE_PROPERTY_CODE_ExposureTime                0x500D
#define PIMA_DEVICE_PROPERTY_CODE_ExposureProgramMode         0x500E
#define PIMA_DEVICE_PROPERTY_CODE_ExposureIndex               0x500F
#define PIMA_DEVICE_PROPERTY_CODE_ExposureBiasCompensation    0x5010
#define PIMA_DEVICE_PROPERTY_CODE_DateTime                    0x5011
#define PIMA_DEVICE_PROPERTY_CODE_CaptureDelay                0x5012
#define PIMA_DEVICE_PROPERTY_CODE_StillCaptureMode            0x5013
#define PIMA_DEVICE_PROPERTY_CODE_Contrast                    0x5014
#define PIMA_DEVICE_PROPERTY_CODE_Sharpness                   0x5015
#define PIMA_DEVICE_PROPERTY_CODE_DigitalZoom                 0x5016
#define PIMA_DEVICE_PROPERTY_CODE_EffectMode                  0x5017
#define PIMA_DEVICE_PROPERTY_CODE_BurstNumber                 0x5018
#define PIMA_DEVICE_PROPERTY_CODE_BurstInterval               0x5019
#define PIMA_DEVICE_PROPERTY_CODE_TimelapseNumber             0x501A
#define PIMA_DEVICE_PROPERTY_CODE_TimelapseInterval           0x501B
#define PIMA_DEVICE_PROPERTY_CODE_FocusMeteringMode           0x501C
#define PIMA_DEVICE_PROPERTY_CODE_UploadURL                   0x501D
#define PIMA_DEVICE_PROPERTY_CODE_Artist                      0x501E
#define PIMA_DEVICE_PROPERTY_CODE_CopyrightInfo               0x501F

/* Proprietary vendor extension device property mask */
// PIMA 15740:2000 standard 13.3.5 Table 26
#define PIMA_DEVICE_PROPERTY_CODE_EXTENSION                   0xD000

/* Device Property Form Flag */
// PIMA 15740:2000 standard 13.3.3 Table 23
#define PIMA_DEVICE_PROPERTY_FORM_FLAG_None                       0x00
#define PIMA_DEVICE_PROPERTY_FORM_FLAG_Range                      0x01
#define PIMA_DEVICE_PROPERTY_FORM_FLAG_Enumeration                0x02

/* Device Property GetSet type */
// PIMA 15740:2000 standard 13.3.3 Table 23
#define PIMA_DEVICE_PROPERTY_GETSET_Get                        0x00
#define PIMA_DEVICE_PROPERTY_GETSET_GetSet                     0x01

#endif //__PIMACONST_H__ 
