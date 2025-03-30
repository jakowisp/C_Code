/**
*  @file main.cpp
*  @brief Function to call USBHostPTP Library
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
#include "mbed.h"
#include "C12832_lcd.h"
#include "USBHostMSD.h"
#include "USBHostPTP.h"

#include "PTPMenu.h"



/**
* Initiate Global variables
*/
DigitalOut led(LED1);
FILE * fp2;
C12832_LCD lcd;
USBHostPTP *ptpdev = NULL;
USBHostMSD *msddev = NULL;
char fname[256];
PTPMenu *menuobj = NULL;
bool commandActive=false;
void (*statusFunction)(void);


bool CheckForCode(uint16_t code, char *buffer){
    FILE *fp;
    struct {
     char str[60];
     uint16_t code;
    } temp;
    bool  found=false;
    
    if( code < 0x9000) {
        fp = fopen("/usb2/PIMACODE.bin", "rb");
    } else {
        fp = fopen("/usb2/canon.bin", "rb");
    }
    if (fp != NULL) {
       while( !found && fread(&temp.code,sizeof(uint16_t),1,fp) >0 ) {
          fread(&temp.str,sizeof(char),60,fp);
          if (code == temp.code) {
             found=true;
             strcpy(buffer,temp.str);
          }
        }
        fclose(fp);
    } 
    
    return found;
}
/**
* This function is used to handle the raw data recieved via the bulk pipes
*
* @param ptp Pointer to the PTP device
* @param buffer Pointer to the data recieved
* @param length Total data received to be processed
* 
* @return Void
*/
void WriteObjectHandles(void *ptp,uint8_t *buffer,uint16_t length){
   int writeResult,errorcode;
   uint16_t transferLength=length;
   uint8_t *dataPtr=buffer;
   
   writeResult=fwrite(dataPtr,sizeof(uint8_t),transferLength,fp2);
   if( writeResult != transferLength) {
       errorcode=ferror(fp2);
       if( errorcode )
       {
          printf("\r\nError in writing to file %d\n",errorcode);
          error("Yucky@!");
       }
   }
}

/**
* Test function 0x01
*
* @param numberOfImages A pointer to where to write the images on the device
* @param numberOfThumbs A pointer to where to write the images on the device
* @return void
*/
void GetNumberOfThumbsAndImages(int *numberOfImages,int *numberOfThumbs){
    uint32_t fileHandle=0,numImages=0;
    int internalNumberOfImages=0;
    int internalNumberOfThumbs=0;
    
    FILE *fp = fopen("/usb2/objHandles.bin", "rb");
    fread(&numImages,sizeof(uint32_t),1,fp);
    while(numImages>0){
       fread(&fileHandle,sizeof(uint32_t),1,fp);
       ptpdev->GetObjectInfo(fileHandle);
       
       if (ptpdev->objectInfo.thumbFormat == 0x3801 ) {
          internalNumberOfThumbs++;
       }
       if (ptpdev->objectInfo.objectFormat == 0x3801 ) {
          internalNumberOfImages++;
       }
        numImages--;
    }
    fclose(fp);
    *numberOfImages = internalNumberOfImages;
    *numberOfThumbs = internalNumberOfThumbs;

}

/**
* Retrieve all thumbnail images and real images that are JPG files.
*/
void GetAllImagesAndThumbs(void) {
    FILE *fp;
        
    uint32_t fileHandle=0,numImages=0;
    
            printf("objectid,objectinfosize,imagetype,thumbtype\r\n");
            fp = fopen("/usb2/objHandles.bin", "rb");
            
            fread(&numImages,sizeof(uint32_t),1,fp);
            while(numImages>0){
               fread(&fileHandle,sizeof(uint32_t),1,fp);
               ptpdev->GetObjectInfo(fileHandle);
               printf("%ld,%x,%x,%x\r\n",numImages,fileHandle,ptpdev->objectInfo.objectFormat,ptpdev->objectInfo.thumbFormat);

               if (ptpdev->objectInfo.thumbFormat == 0x3801 && numImages < 864 ) {
                   sprintf(fname,"/usb2/thumb_%s",ptpdev->objectInfo.filename.getString());
                    fp2 = fopen(fname, "wb");                    
                    printf("Starting transfer of %s\r\n",fname);
                    ptpdev->GetThumb(fileHandle,(void *)&WriteObjectHandles);
                    fclose(fp2);
                    printf("GetThumb Transaction Complete\r\n");
               }
                           
               if (ptpdev->objectInfo.objectFormat == 0x3801 && numImages < 864 ) {
                   sprintf(fname,"/usb2/%s",ptpdev->objectInfo.filename.getString());
                   printf("%s -Type: 0x%04x\r\n",fname,ptpdev->objectInfo.objectFormat);
                    printf("Starting transfer of %s\r\n",fname);
                    fp2 = fopen(fname, "wb");   
                    ptpdev->GetObject(fileHandle,(void *)&WriteObjectHandles);
                    fclose(fp2);
                    printf("GetObject Transaction Complete\r\n");
                }
                numImages--;
            }
            fclose(fp);
}

/**
* Thread to Watch for MSD image
*/
void msd_task(void const *) {
    USBHostMSD msd2("usb2");
    
    msddev=&msd2;
    while(true) {
        while(!msd2.connect()) {
                Thread::wait(500);
            }

        Thread::wait(200);
        DIR *dp;
        struct dirent *ep;     
        dp = opendir ("/usb2");

        if (dp != NULL)
        {
            ep = readdir (dp);
            while (ep!=NULL) {
              printf("%s\r\n",ep->d_name);
              ep = readdir (dp);
            }
           (void) closedir (dp);
         }
         
         while(msd2.connect()) {
                Thread::wait(250);
                
         }
         if(ptpdev!=NULL)
             ptpdev->CodeDecoderFunction=NULL;
                
    }
}


void genericStatus(void) {
    led=!led;
    lcd.cls();
    lcd.locate(10,0);
    lcd.printf("Please wait....");
}

void getAllObjectHandles(void) {
        fp2 = fopen("/usb2/objHandles.bin", "wb");
        ptpdev->GetObjectHandles(0xffffffff,0x0000,0x0000,(void *)&WriteObjectHandles);
        fclose(fp2);
        printf("GetObjecthandles Transaction Complete\r\n");   
}


/**
* Function to display status during image transfers
*/
void GetAllJPGStatus(void) {
    led=!led;
    lcd.cls();
    if(ptpdev->dataLeftToTransfer>0) {
        lcd.locate(10,0);
        lcd.printf("%ld/%ld", ptpdev->dataLeftToTransfer, ptpdev->totalDataToTransfer);
        lcd.locate(0,10);
        lcd.printf("%s",fname); 
    }               
}

/**
* Thread to watch for the PTP device connected, and commands that need to be executed.
*/void ptp_task2(void const *) {
    
    
    USBHostPTP ptp;
    ptpdev=&ptp;
        
    int numi,numt;
    uint32_t objCount=0;
    
    while(true) {
        while(!ptpdev->connect()) {
            Thread::wait(500);
        }
        
        while(!msddev->connected()){
            Thread::wait(500);
        }
            
        ptp.OpenSession();
        printf("OpenSession Transaction Complete\r\n\r\n");
        ptp.GetDeviceInfo();
        printf("GetDeviceInfo Transaction Complete\r\n");
        
        while(ptpdev->connected()) {
            if(menuobj->command!=0x00)
               commandActive=true;
            switch(menuobj->command) {
            case GETALLJPG:
                statusFunction=&GetAllJPGStatus;
                getAllObjectHandles();
                GetAllImagesAndThumbs();
                break;
            case GETALLJPGTHUMB:
                break;
            case GETNUMJPG:
                getAllObjectHandles();
                GetNumberOfThumbsAndImages(&numi,&numt);
                printf("images: %d, thumbs:%d\r\n",numi,numt);
                break;
            case GETNUMOBJ:
                ptp.GetNumObjects(&objCount);
                printf("GetNumObjects Transaction Complete - Count:%ld\r\n",objCount);
                break;
            case DUMPDEVICEINFO:
                statusFunction=&genericStatus;
                ptp.GetDeviceInfo();
                ptp.DumpDeviceInfo();
                break;
            case CAPTUREMODEON:
                statusFunction=&genericStatus;
                ptp.Operation(0x9008);
                ptp.GetDeviceInfo();
                break;
            case CAPTUREMODEOFF:
                statusFunction=&genericStatus;
                ptp.Operation(0x9009);
                ptp.GetDeviceInfo();
                break;
            case VFINDERON:
                ptp.Operation(0x900b);
                break;
            case VFINDEROFF:
                ptp.Operation(0x900c);
                break;
            case TAKEPHOTO:
                statusFunction=&genericStatus;
                printf("Set Prop: %x\r\n",ptp.SetDevicePropValue(0xd029,0xd));
                printf("Focus Lock: %x\r\n",ptp.Operation(0x9014));
                printf("Image Capture: %x\r\n",ptp.Operation(0x901a));
                break;
            case CLOSESESSION:
                ptpdev->CloseSession();
                printf("CloseSession Transaction Complete\r\n");
                break;
            case ENABLEDECODER :
                if(msddev!=NULL) {
                    ptpdev->CodeDecoderFunction=&CheckForCode;
                }
                printf("Decoder Enabled\r\n");
                break;
            default:
                commandActive=false;
                break;
            }
            statusFunction=NULL;
            menuobj->command=0x00;
            Thread::wait(100);
        }
    }
}

///Main fuction to display the Status of the application.
int main() {
    lcd.cls();
    lcd.locate(10,3);
    lcd.printf("Initializing");
    Thread ptpTask2(ptp_task2, NULL, osPriorityNormal, 1024 * 4);
    Thread msdTask2(msd_task, NULL, osPriorityNormal,  1024 * 4);
    menuobj = new PTPMenu(&lcd);
    
    while(1) {
        if(ptpdev!=NULL) {
            if(ptpdev->connected()) {
                if(!commandActive) {
                    menuobj->poll(); 
                } else {
                    if(statusFunction!=NULL)
                       (*statusFunction)();                 
                }
            } else {
               lcd.cls();
               lcd.locate(10,3);
               lcd.printf("Please connect PTP device.");
            }            
        }
        Thread::wait(20);
    }
}

