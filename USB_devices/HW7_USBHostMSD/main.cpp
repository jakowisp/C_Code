#include "mbed.h"
#include "USBHostMSD.h"

DigitalOut led(LED1);

int writeFile(int numFiles=10,int size=1000){
    char fname[80];
    uint32_t data;
    FILE *fp;
    
    for (int j=0;j<numFiles;j++) {
        sprintf(fname,"/usb/test%d.txt",j);
        fp = fopen(fname, "wb");
        if (fp != NULL) {
            for(int i=0; i<size;i++) {
              data=rand();
              fwrite(&data,sizeof(uint32_t),1,fp);
            }
            fclose(fp);
        } else {
            printf("FILE == NULL\r\n");
        }
    }
    return 0;
}

int readFile(int numFiles=10,int size=1000){
    char fname[80];
    uint32_t data;
    FILE *fp;
    
    for (int j=0;j<numFiles;j++) {
        sprintf(fname,"/usb/test%d.txt",j);
        fp = fopen(fname, "rb");
        if (fp != NULL) {
            
            for(int i=0; i<size;i++) {
              fread(&data,sizeof(uint32_t),1,fp);
            }
            fclose(fp);
        } else {
            printf("FILE == NULL\r\n");
        }
    }
     return 0;
}


void msd_task(void const *) {
    
    USBHostMSD msd("usb");
    Timer t;
    
    
        
    // try to connect a MSD device
    while(!msd.connect()) {
        Thread::wait(500);
    }
        
 
    printf("Start Writing\n\r");
    t.reset();
    t.start();
    writeFile(1,10000000);
    t.stop();
    printf("Large File Write: The time taken was %f seconds\n\r", t.read());
    
    printf("Start Writing\r\n");
    t.reset();
    t.start();
    readFile(1,10000000);
    t.stop();
    printf("Large File read: The time taken was %f seconds\r\n", t.read());

    printf("Start Writing\r\n");
    t.reset();
    t.start();
    writeFile(100,100000);
    t.stop();
    printf("Small Files Write: The time taken was %f seconds\r\n", t.read());
    
    printf("Start Writing\r\n");
    t.reset();
    t.start();
    readFile(100,100000);
    t.stop();
    printf("Small File read: The time taken was %f seconds\r\n", t.read());

}


int main() {
    Thread msdTask(msd_task, NULL, osPriorityNormal, 1024 * 4);
    while(1) {
        led=!led;
        Thread::wait(500);
    }
}

/*
[USB_INFO: /src/USBHost/USBHost/USBHost.cpp:158]New device connected: 100038ac [hub: 0 - port: 1]
[USB_INFO: /src/USBHost/USBHostMSD/USBHostMSD.cpp:81]New MSD device: VID:058f PID:6387 [dev: 100038ac - intf: 0]
Start Writing
[USB_INFO: /src/USBHost/USBHostMSD/USBHostMSD.cpp:164]MSD [dev: 100038ac] - Vendor ID: TRON:
[USB_INFO: /src/USBHost/USBHostMSD/USBHostMSD.cpp:168]MSD [dev: 100038ac] - Product ID: Legacy
[USB_INFO: /src/USBHost/USBHostMSD/USBHostMSD.cpp:172]MSD [dev: 100038ac] - Product rev: 8.07
[USB_INFO: /src/USBHost/USBHostMSD/USBHostMSD.cpp:139]MSD [dev: 100038ac] - blockCount: 8269823, blockSize: 512, Capacity: 4234149376

[USB_INFO: /src/USBHost/USBHost/USBHost.cpp:158]New device connected: 100038ac [hub: 0 - port: 1]
[USB_INFO: /src/USBHost/USBHostMSD/USBHostMSD.cpp:81]New MSD device: VID:058f PID:6387 [dev: 100038ac - intf: 0]
Start Writing
[USB_INFO: /src/USBHost/USBHostMSD/USBHostMSD.cpp:164]MSD [dev: 100038ac] - Vendor ID: TRON:
[USB_INFO: /src/USBHost/USBHostMSD/USBHostMSD.cpp:168]MSD [dev: 100038ac] - Product ID: Legacy
[USB_INFO: /src/USBHost/USBHostMSD/USBHostMSD.cpp:172]MSD [dev: 100038ac] - Product rev: 8.07
[USB_INFO: /src/USBHost/USBHostMSD/USBHostMSD.cpp:139]MSD [dev: 100038ac] - blockCount: 8269823, blockSize: 512, Capacity: 4234149376

Large File Write: The time taken was 740.766541 seconds
Start Writing
Large File read: The time taken was 470.112549 seconds
Start Writing
Small Files Write: The time taken was 962.334229 seconds
Start Writing
Small File read: The time taken was 562.649902 seconds
*/