#include <GPGUI_3_MAIN_CPU.h>
GPGUI_3_MAIN_CPU cpu;

byte texture_Ram[4096];
int tex_Index = 1;

byte TEX[16][16] =
{ 
  {4,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {4,4,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
  {1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,1},
  {1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1},
  {1,0,0,0,1,0,0,0,0,0,0,1,0,0,0,1},
  {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1},
  {1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1},
  {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1},
  {1,0,0,0,1,0,0,0,0,0,0,1,0,0,0,1},
  {1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1},
  {1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,1},
  {1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
  
};
byte temp_Reg[255];
int temp_Reg_Address = 0;



void setup()
{
   cpu. GPGUI_INIT();

for(int y = 0 ; y<16; y++)
{
  for(int x = 0; x<16; x++)
  {
    temp_Reg[temp_Reg_Address] = TEX[y][x];
    temp_Reg_Address++;
  }  
}
   
   cpu.write_Texture_To_Ram(tex_Index, texture_Ram,temp_Reg);
}






void loop()
 { 
  cpu.drawBackgroundColor(0);
  cpu.draw_2D_Tile(10,10,tex_Index,texture_Ram);   
  cpu.refreshScreen();
  }






  
