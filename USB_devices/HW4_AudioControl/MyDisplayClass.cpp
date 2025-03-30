#include "MyDisplayClass.h"
//Constructor
MyDisplayClass::MyDisplayClass(){
   //Create a new LCD instance
   lcd=new C12832_LCD();
   //Create a new bargraph instance
   volume=new bargraph(lcd,32);
   //Set the default feature modes. Volme Display off. Graphic mode off.
   volumeDisplayEnable=false;
   graphicModeEnable=false;
}

void MyDisplayClass::drawNoConnection(C12832_LCD *lcd) {
     lcd->fillrect(0, 31, 15,16, 0);
     lcd->rect(0, 31, 15,16, 1);
     lcd->line(2,29,13,18,1);
     lcd->circle(7,24,4,1);
}

void MyDisplayClass::drawConnection(C12832_LCD *lcd) {
     lcd->fillrect(0, 31, 15,16, 0);
     lcd->rect(0, 31, 15,16, 1);
     lcd->rect(5,26,10,21,1);
     lcd->line(6,23,7,25,1);
     lcd->line(7,25,14,17,1);
}

void MyDisplayClass::drawSuspend(C12832_LCD *lcd) {
     lcd->fillrect(0, 31, 15,16, 0);
     lcd->rect(0, 31, 15,16, 1);
     lcd->line(4,20,11,20,1);
     lcd->line(4,27,11,20,1);
     lcd->line(4,27,11,27,1);
}

void MyDisplayClass::setLevel(int level){
      volume->setLevel(level);
   }
   
void MyDisplayClass::setMaxLevel(int level){
      volume->setMaxLevel(level);
   }

//When in Graphic mode update USB status with pictures.
void MyDisplayClass::UpdateStatus(int state){
    switch(state) {
       case 2:  drawSuspend(lcd); break;
       case 3:  drawConnection(lcd); break;
      default:  drawNoConnection(lcd); break;
      }
}

//When in text mode update the USB status based on state
void MyDisplayClass::UpdateTextStatus(int state){
       lcd->locate(3,12);
       lcd->printf("         ");
       lcd->locate(3,12);
        switch(state) {
           case 2:  lcd->printf("Suspended"); break;
           case 3:  lcd->printf("Normal"); break;
          default:  lcd->printf("Offline"); break;
          }

}

//Update the volume control display bsed on feature enable state, USB state and volume
void MyDisplayClass::update(int state){
      //Is volume display enabled
      if(volumeDisplayEnable)
         //Volume:Is Graphic mode enabled or should messages be in text
         if(graphicModeEnable) {
           //Call Graphic mode update function
           volume->updateBargraph();
         } else {
           lcd->locate(3,3);
           lcd->printf("Volume: %3d PCT%",(volume->level * 100)/volume->maxlevels); 
         } 
         //USB:Is Graphic mode enabled or should messages be in text  
        if(graphicModeEnable)  
           UpdateStatus(state);
        else
           UpdateTextStatus(state);
}
