#include "C12832_lcd.h"

class bargraph {
public:
   /*
   * Constructor , Requires a pointer to the LCD
   */
   bargraph(C12832_LCD *inlcd,int maxlevelsIn=32,int Xin=0,int Yin=0,int widthIn=128,int heightIn=32);
   
   // This variable is used to draw the current volume level
   unsigned int level;
   // This variable represents the maximum volume  Volume leveles are percents of maxlevel when graphed.
   unsigned int maxlevels;

   //This function will redraw the Bar graph
   void updateBargraph();
   
   //this fuction will reset the maxlevels and clea the current level
   void setMaxLevel(unsigned int maxlevels);
   //This function will set the level value.  The input is 0x00 to 0xff and is normalized to a percent of maximum level.
   void setLevel(unsigned int level);
 
private:
   //How wide and tall the grpah is as where as where it should start.
   int x;
   int y;
   int width;
   int height;
   
   //When drawing bars how tall and wide should they be.
   int leveladjust;
   int levelwidth;
   
   //What was the last drawn state so we delete only what was drawn
   unsigned int lastLevel;
   
   C12832_LCD *lcd;
};
      

