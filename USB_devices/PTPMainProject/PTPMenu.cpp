#include "PTPMenu.h"


void PTPMenu::GenerateMenu(void) {
    menus.push_back(Menu("root")); 
    menus.push_back(Menu("Test menu")); 
    menus[1].add(Selection(DUMPDEVICEINFO,      0, NULL, "Dump Device Info")); // The function argument of selection can be added directly
    menus[1].add(Selection(GETALLJPG,           1, NULL, "Get All Images")); // The function argument of selection can be added directly
    menus[1].add(Selection(GETNUMOBJ,           2, NULL, "Get Number of Objects"));
    menus[1].add(Selection(GETNUMJPG,           3, NULL, "Get Number of Images"));
    menus[1].add(Selection(ENABLEDECODER,       4, NULL, "Enable Decoder"));
    //testMenu.add(Selection(NULL, 5, NULL, "Get Number of Thumbnails"));
    menus[1].add(Selection(CLOSESESSION,        5, NULL, "Close Session"));
    menus[1].add(Selection((void (*)())NULL,    6, &menus[0], "  Go back"));  // always add a Selection at the end to point to the parent
    
    menus.push_back(Menu("About Menu")); // about menu crediting us :)
    menus[2].add(Selection((void (*)())NULL,    0, NULL, "Author:"));
    menus[2].add(Selection((void (*)())NULL,    1, NULL, " Dwayne S Dilbeck"));
    menus[2].add(Selection((void (*)())NULL,    2, NULL, " 8/29/2013"));
    menus[2].add(Selection((void (*)())NULL,    3, NULL, " USB Device Interface:"));
    menus[2].add(Selection((void (*)())NULL,    4, NULL, " Architecture,"));
    menus[2].add(Selection((void (*)())NULL,    5, NULL, " Protocols,"));
    menus[2].add(Selection((void (*)())NULL,    6, NULL, " and programming."));
    menus[2].add(Selection((void (*)())NULL,    7, &menus[0], "  Go back"));
    
    menus.push_back(Menu("Capture Menu"));
    menus[3].add(Selection(CAPTUREMODEON,       0, NULL, "Enable Capture Mode"));
    menus[3].add(Selection(CAPTUREMODEOFF,      1, NULL, "Disable Capture Mode"));
    menus[3].add(Selection(VFINDERON,           2, NULL, "Enable View Finder"));
    menus[3].add(Selection(VFINDEROFF,          3, NULL, "Disable View Finder"));
    menus[3].add(Selection(TAKEPHOTO,           4, NULL, "Take Photo"));
    menus[3].add(Selection((void (*)())NULL,    5, &menus[0], "  Go back"));
    
    menus[0].add(Selection((void (*)())NULL,    0, &menus[1], "TEST MENU"));
    menus[0].add(Selection((void (*)())NULL,    1, &menus[3], "Capture menu"));
    menus[0].add(Selection((void (*)())NULL,    2, &menus[2], "About menu"));   
    navigator = new Navigator(&menus[0], lcd);
}


void PTPMenu::poll(void) {
    navigator->poll(&command);
}
