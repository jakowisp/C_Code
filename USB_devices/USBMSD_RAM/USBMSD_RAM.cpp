/** This library is based on the USBMSD_SD library.
 *  Modifications for using RAM were made by avnisha
 *  Only modifications doen by Dwayne Dilbeck were to remove the SD functions, and change the RAM location to the Ethernet RAM 
 */


#include "USBMSD_RAM.h"
#include "mbed_debug.h"

#define SD_COMMAND_TIMEOUT 5000
#define SD_DBG             0
#define BLOCKS  32
#define FS  BLOCKS*512

LocalFileSystem local("local"); 

char    s0[FS] __attribute__((section("AHBSRAM1")));
char    s1[FS] __attribute__((section("AHBSRAM0")));
int     status = 1;

USBMSD_RAM::USBMSD_RAM(PinName mosi, PinName miso, PinName sclk, PinName cs) :
    _spi(mosi, miso, sclk), _cs(cs) {
    _cs = 1;
    
    printf("cons\n");
    connect();
}

int USBMSD_RAM::disk_initialize() { 

    printf("ini\n");
    status--;
    return 0;
}

int USBMSD_RAM::disk_write(const uint8_t *buffer, uint64_t block_number) {

//
// find the correct block and write
//
    int offset;
    int i;
    char *s;
    
    //printf("write\n");
    if (block_number > (BLOCKS -1)) return 1;
    offset = 512 * block_number;   
    if (block_number < FS)
        s=(char *)&s0;
    else
        s=(char *)&s1;
    for (i = 0; i < 512; i++) {
        s[offset + i] = buffer[i];  
    }
    return 0;
}

int USBMSD_RAM::disk_read(uint8_t *buffer, uint64_t block_number) {

    int offset;
    int i;
    char *s;
    
    //printf("read\n");
    if (block_number > (BLOCKS -1)) return 1;
    offset = 512 * block_number;   
    if (block_number < FS)
        s=(char *)&s0;
    else
        s=(char *)&s1;
    for (i = 0; i < 512; i++) {
        buffer[i] = s[offset + i];  
    }
    return 0;
}

int USBMSD_RAM::disk_status() { 
    printf("status\n");
    return status; 
}


int USBMSD_RAM::disk_sync() { printf("sync\n"); return 0; }
uint64_t USBMSD_RAM::disk_size() { 
   printf("size\n"); 
   printf("Location s0 : %x\n", &s0);
   printf("Location s1 : %x\n", &s1);
   return 2*FS;
}
uint64_t USBMSD_RAM::disk_sectors() { printf("sectors\n"); return BLOCKS; }
