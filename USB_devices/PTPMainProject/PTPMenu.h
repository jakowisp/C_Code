#include "mbed.h"
#include <vector>
#include "C12832_lcd.h"
#include "Selection.h"
#include "Menu.h"
#include "Navigator.h"

#define GETALLJPG       0x01
#define GETALLJPGTHUMB  0x02
#define GETNUMJPG       0x03
#define GETNUMOBJ       0x04
#define DUMPDEVICEINFO  0x05
#define CAPTUREMODEON   0x06
#define CAPTUREMODEOFF  0x07
#define VFINDERON       0x08
#define VFINDEROFF      0x09
#define TAKEPHOTO       0x0a
#define ENABLEDECODER   0x0b
#define CLOSESESSION    0xFF

class PTPMenu {
public:
    PTPMenu(C12832_LCD *lcdIn){
      menus.clear();
      navigator = NULL;
      command = 0x00;
      this->lcd=lcdIn;
      GenerateMenu();
    };

    uint8_t command;
    C12832_LCD *lcd;
    Navigator *navigator;
    void poll(void);

private:
    vector<Menu> menus;
    void GenerateMenu(void);

};