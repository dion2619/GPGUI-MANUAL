#include <GPGUI_3_MAIN_CPU.h>
GPGUI_3_MAIN_CPU cpu;

//LOAD MAP AND TEXTURE DATA FROM MEM CARD SLOT OR MAP EDITOR PROGRAM TO RAM
byte map_Ram[4096];     //4k SRAM
byte texture_Ram[4096]; //4k SRAM


//EXAMPLE MAP RAM DATA STRUCTURE
byte map_Width           = 10;   //RAM[0] LIMITED TO 10-60
byte map_Height          = 10;   //RAM[1] LIMITED TO 10-60
byte roof_Color          = 16;   //RAM[2] LIMITED TO 0-64
byte floor_Color         = 4;    //RAM[3] LIMITED TO 0-64
byte current_Camera_PosX = 3;    //RAM[4]  
byte current_Camera_PosY = 5;    //RAM[5]  
//


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
int tex_Index = 1; // 1 - 16







void setup()
{
    cpu. GPGUI_INIT();
    // setting data
    map_Ram[0] = map_Width;        
    map_Ram[1] = map_Height;         
    map_Ram[2] = roof_Color;         
    map_Ram[3] = floor_Color;        
    map_Ram[4] = current_Camera_PosX;  
    map_Ram[5] = current_Camera_PosY;  
    //
    map_Ram[10]   = 8;//START ADDRESS OF MAP GRID
    map_Ram[3609] = 8;//END ADDRESS OF MAX SIZE MAP GRID
    //

    
    for(int y = 0 ; y<16; y++)
{
  for(int x = 0; x<16; x++)
  {
    temp_Reg[temp_Reg_Address] = TEX[y][x];
    temp_Reg_Address++;
  }  
}
   
     cpu.write_Texture_To_Ram
     (
      tex_Index,
      texture_Ram,
      temp_Reg
     );
     
     cpu.write_Default_Map_To_Ram
     (
         map_Width, 
         map_Height,
         map_Ram,
         tex_Index
     );

       cpu.write_Default_Wall_Texture_To_Ram(16,texture_Ram);  
       map_Ram[66] = 16;
}


void loop()
  {    
    cpu.raycast_Map_Preview(map_Ram,texture_Ram); 
  }






  