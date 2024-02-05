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
  cpu.drawBackgroundColor(black);//CLEAR VGA BUFFER WITH COLOR
  //
  cpu.setPixel(10,10,red);//(x,y,color)
  cpu.drawLine(10, 20, 100, 20, green);//(x1,y1,x2,y2,color)
  //
  cpu.drawHollowRectangle(10, 30, 50, 50, cyan);//(x,y,x size, y size, color)
  cpu.drawSolidRectangle (10, 90, 50, 50, yellow);//(x,y,x size, y size, color)
  //
  cpu.drawHollowCircle(35, 170, 10, grey);//(x,y,radius,color);
  cpu.drawSolidCircle (35, 200, 10, blue);//(x,y,radius,color);
  //
  cpu.drawTriangle(150,10,250,10,200,100,true,white);//(x1,y1,x2,y2,x3,y3,fill type,color)
  //
  cpu.refreshScreen();//UPDATE VGA BUFFER
}











