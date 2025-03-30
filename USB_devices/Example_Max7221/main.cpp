 #include "mbed.h"
 #include "Max7221.h"
 

// p5: DIN, p7: CLK, p8: LOAD/CS
Max7221 max7221disp1(p5, p7, p8);
//Max7221 max7221disp2(p5, p7, p8);
//Max7221 max7221disp3(p11, p13, p14);
//Max7221 max7221disp4(p11, p13, p14);

int count=-99;

void loop(void) {
   max7221disp1=count;
   if (count < 100)
      count=count+1;
   else 
      count=-99; 
}

int main() {  
    max7221disp1.Setup();
    //Max7221::SetupALl();
    max7221disp1.WriteFloat(123.125);
    wait(1.0);  

    while (1) {
        loop();
        wait(1.0);
    }
}


