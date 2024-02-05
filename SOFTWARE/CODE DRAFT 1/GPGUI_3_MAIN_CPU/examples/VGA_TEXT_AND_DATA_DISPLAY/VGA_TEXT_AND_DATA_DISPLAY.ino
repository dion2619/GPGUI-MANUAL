#include <GPGUI_3_MAIN_CPU.h>
# define black   0  
# define white   63 
# define red     3  
# define lime    12 
# define blue    48 
# define yellow  15 
# define cyan    60 
# define magenta 51 
# define grey    42 
# define maroon  2  
# define olive   10 
# define green   8  
# define purple  34 
# define teal    40 
# define navy    32 


GPGUI_3_MAIN_CPU cpu;

void setup() 
{
 cpu.GPGUI_INIT();//start DMA transfer loop from VGA ram to VGA port 
}



void loop()
{
  cpu.drawBackgroundColor(navy);//CLEAR VGA BUFFER WITH COLOR
  //
  cpu.printNumberInt     (10, 10, 10,    red,  black);//(input data,x start, y start, txt color, back color)
  cpu.printNumberHex     (255, 10, 18,  blue,   cyan);
  cpu.printNumberDouble(33.5, 10, 26,  green,  white);  //ALL TEXT IS 6X8 PIXELS 6 X, 8 Y IN SIZE
  cpu.print    ("SAMPLE TXT", 10, 34, yellow, purple);

  //
  cpu.refreshScreen();//UPDATE VGA BUFFER
}











