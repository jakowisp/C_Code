 #include "bargraph.h"
 /*
 * Constructor
 */
 bargraph::bargraph(C12832_LCD *inlcd,int maxlevelsIn,int Xin,int Yin,int widthIn,int heightIn){
      this->lcd=inlcd;
      this->level=0;
      this->lastLevel=0;
      //Make sure maxlevel is a valid level
      if(maxlevelsIn<=32 && maxlevelsIn >=4)
         this->maxlevels=maxlevelsIn;
      else
         this->maxlevels=32;
      //make sure all initial values for size and location are valid   
      if(Xin>=0 && Xin<=127-this->maxlevels)
           this->x=Xin;
      else
           this->x=0;
      if(Yin>=0 && Yin<=31-this->maxlevels)
           this->y=Yin;
      else
           this->y=0;
      if(widthIn>maxlevels && widthIn<128)
        this->width=widthIn-1;
      else
        this->width=127;
      if(heightIn>=(maxlevels-1) && heightIn<31)  
         this->height=heightIn;
      else 
         this->height=31;
      //calculate how wide and tall each bar shall be.
      this->leveladjust=(this->height+1)/this->maxlevels;
      this->levelwidth=(this->width+1)/this->maxlevels;
   }
   
   void  bargraph::setMaxLevel(unsigned int maxlevelsIn) {
//Defensively set maxlevels to a new format level and clear the level/lastlevel values.
      if(maxlevelsIn<=32 && maxlevelsIn >=4)
         this->maxlevels=maxlevels;
      else
         this->maxlevels=32;
      this->lcd->fillrect(this->x, this->y, this->x+this->width,this->y+this->height, 0);
      this->lastLevel=0;
      this->level=0;    
      this->leveladjust=(this->height+1)/maxlevels;
      this->levelwidth=(this->width+1)/maxlevels;
   }
   
   void bargraph::setLevel(unsigned int level){       
   //Set level to a normalized value relative to maxlevels
      this->level=(this->maxlevels * level)/0xff;     
   }
   
   void  bargraph::updateBargraph(){  
   //Erase than draw the bars for the graph.
      if(this->lcd!=NULL) {
        if(this->level!=this->lastLevel) {
          for(int i = 0 ; i<this->lastLevel;i++) {
             this->lcd->fillrect(this->x+this->levelwidth*(i), 
                                 this->y, 
                                 this->x+this->levelwidth*(i+1)-1,
                                 this->y+i*this->leveladjust, 
                                 0);
          }
          if (this->level > this->maxlevels)
              this->level=this->maxlevels;
          for(int i = 0 ; i<level;i++) {
             this->lcd->fillrect(this->x+this->levelwidth*(i), 
                                 this->y, 
                                 this->x+this->levelwidth*(i+1)-1,
                                 this->y+i*this->leveladjust, 
                                 1);
          }
          this->lastLevel = this->level;
        }
      }
    };



