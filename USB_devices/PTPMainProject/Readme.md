= mbed PTP hosting library =
== USB Device Interface: Architecture, Protocols, and Programming Class Project ==
 {{/media/uploads/jakowisp/img_20130902_200043_188.jpg}} 
=== Goal ===
Provide a generic PTP library to allow the mBed to host a PTP device

=== Details ===
 * Main thread configures and displays the Menu system, as Commands are selected in the menu a command value is set.
 * MSD thread blocks until a MSd device is connected, If a device is removed it will try to reconnect
 * PTP thread blocks until a PTP device ois connected. After connection of device, a session is opened, device information is retriewved, and object handles are recieved.  As long as a PTP device is connected, the command value is checked for a non-zero value. when a command code is recieved, the statusFunction is changed from NULL, and the command executed. After execution the command code is cleared.


=== Project Hardware ===
 * mbed LPC1768 CPU board 
 * mbed DEV-11695 application board 
 * USB HUB
 * USB Mass Storage Device
 * Camera using PTP Protocol 
 * Three USB Cables (normal mbed connection, connection to USB HUB, and connection to camera)
 
=== Testing ===
||**Test Item**||**Description**||**Status**||
||**Unit Tests**||||||
||connected1||Test connected with  a device.||Pass||
||connected2||Test connected without  a device.||Pass||||
||connect1||Test Connect() with no device||Pass||||
||connect2||Test connect() with a ptp device||Pass||||
||connect3||Test connect() with multiple PTP devices||Not Tested||||
||connect4||Test Connect() with multiple PTP devices and, Classes||Not Tested||
||cancel1||Test CancelRequest() with invalid transactionID||Not Tested||||
||cancel2||Test CancelRequest() with valid transactionID||Not Tested||||
||reset1||Test DeviceReestRequest on Device that supports the call||Not Tested||||
||reset2||Test DeviceReestRequest on Device that does not supports the call||Not Tested||||
||status1||Test GetDeviceStatus returns status||Not Tested||||
||extended1||Test if GetExtendedEventData functions on device that does not support it.||Not Tested||||
||transaction1||Test transaction() with unsupported opcode||Pass||||
||transaction2||Test transaction() with supported opcode||Pass||||
||transaction3||Test transaction() with opcode other than OpenSession or GetDeviceInfo before a session is openned.||Pass||||
||transaction4||Test transaction() with getDeviceInfo before OpenSession||Pass||||
||transaction5||Test transaction() with max number parameter||Pass||||
||transaction6||Test transaction() with no parameters||Pass||||
||transaction7||Test transaction() with 1 parameter||Pass||||
||transaction8||Test transaction() with no data stage||Pass||||
||transaction9||Test transaction() with 1 input data stage||Pass||||
||transaction10||Test transaction() with data stage that spans max packet size||Pass||||
||transaction11||Test transaction() with Response container that has no paramerters||Pass||||
||transaction12||Test transaction() with response container that has parameters||Pass||||
||operation1||Test Operation() call without params||Not Tested||||
||operation2||Test Operation() call with parameters||Not Tested||||
||opensession1||Test OpenSession opens a session||Pass||||
||close1||Test Close Session closes a session||Pass||||
||close2||Test Close Session with an invlid sesison number||Not Tested||||
||deviceinfo1||Test getDeviceInfo obtains a device Info structure||Pass||||
||powerdown1||Test powerdown() on a device that does not support powerdown.||Not Tested||||
||selftest1||Test SelfTest on device that does not support selftest.||Not Tested||||
||getobjecthandkles1||GetObject handles for an empty device||Pass||||
||getobjecthandles2||GetObjectHandles for all types.||Pass||||
||getobjecthandles3||Get ObjectHandles filtered by type||Not Tested||||
||getobjecthandles4||GetObjectHandles filtered by association||Not Tested||||
||getnumobjects1||Test GetNumObjects for all types||||
||getnumobjects2||Test GetNumObjects for only image types||||
||getnumobjects3||Test GetNumObjects filtered by assoication||Not Tested||||
||getobject1||Test getObjectInfo for image types||||
||getobject2||Test getObjectInfo for non-image type||||
||getthumb1||Test getThumb for an object that has no Thumb||||
||getthumb2||Test getThumb for an image object||||
||getstorageids1||Test getStorageIDs gets the handles for the storage devices.||||
||copyobject1||Test CopyObject can copy an Object to a new location||Not Tested||||
||copyobject2||Test CopyObject fails for invalid handle||Not Tested||||
||copyobject3||Test CopyObject fails for invalid storageID||Not Tested||||
||copyobject4||Test CopyObject fails for invalid parent||Not Tested||||
||copyobject5||Test CopyObject returns a new handle in the varaible provided||Not Tested||||
||deleteobject1||Test DeleteObject can remove an object||Not Tested||||
||deleteobject2||Test DeleteObject fails for invalid handle||Not Tested||||
||deleteobject3||Test DeleteObject with the Format field||Not Tested||||
||protect1||Test SetObjectProtection on a device that does not support the action.||Not Tested||||
||prop1||Test GetDevicePropertyDesc functions||Not Tested||||
||prop2||Test GetDeviceProperty functions||Not Tested||||
||prop3||Test SetDeviceProperty Functions||Not Tested||||
||prop4||Test ResetDeviceProperty funxctions||Not Tested||||
||**Functional**||||||
||flow1||"Complete flow-poweron, connect msd, connect PTP, all thumbs and files transferred"||Pass||||
||**Stress**||||||
||stress1||Contious transfer of same image||Pass||||
||stress2||Transfer of all images on camera - Continous.||Fail||Buffer Overflow.||
||**Performance**||||||
||perf1||Image transfer time per image||||
||perf2||Thumb transfer time per image||||
||**Interoperability**||||||
||compat1||Motorola Droid4 in Camera mode||Pass||||
||compat2||Cannon Eos 10D||Fail||||
||compat3||Cannon Powershot a1400||Pass||||
||power up/ power down||||||
||power1||No devices present||Pass||||
||power2||MSD only present||Pass||||
||power3||PTP Only present||Pass||||
||power4||Both devices present||Pass||||
||**USB Compliance**||||||
||Compliance1||Beagle USB trace tool able to track packets and class data.||Fail||USBHost not compliant.||
||Compliance2||"Verify Control, Bulk_in, bulk_out and Interupt_in endpoints function."||Pass||||

=== Example of using the USBHostPTP class ===
<<program /users/jakowisp/code/class_project_main/docs/013bdb272a9c/main_8cpp_source.html>>

===Known Issues ===
 * Buffer overflow will halt the mbed device after ~6 hours of transfers
 * Method Transaction is unable to SEND Data Containers
 * MSD removal does not  change the Data container handler. This should become NULL, when the MSD is removed.

===Future Work ===
 * Find the buffer overflow
 * Implement a PTP Capture Test.
 * Implement send data in method Transaction
 * Implement an Opcode decoder
 * Add Canon extended PTP support.

=== References ===
 * PIMA15740
 * PTP Vedor code list
 * Arduino Camera Capture Library

