#include <GPGUI_3_MAIN_CPU.h>
GPGUI_3_MAIN_CPU cpu;

byte map_Width = 20;
byte map_Height = 20;
//
int   view_Port_Start_X   = 0;
int   view_Port_Start_Y   = 0;
int   view_Port_Width     = 320;
int   view_Port_Height    = 240;
int   camera_Clip_Dist    = 10;
bool  turn_Left           = true;
bool  turn_Right          = false;
bool  move_Fwd            = false;
bool  move_Back           = false;
bool  clipping_Enable     = false;
bool  camera_Up           = false;
bool  camera_Down         = false;
float FOV                 = 0.66;
int   current_Camera_PosX = 0;
int   current_Camera_PosY = 0;
float move_Speed          = 0.05;
float turn_Speed          = 1.5;
bool  crosshair_Enable    = true;
bool  debug_Enable        = false;
//
int  map_Pos = 0;
byte map_Mem_Start_Address = 10;
byte roof_Color = 0;
byte floor_Color = 42;
int bin;

//TEXTURE RAM ADDRESS VALUES 1-16
byte MAP[20][20] =
{ 
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,14,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,15,0,0,0,0,0,0,0},
  {0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0},
  {0,0,0,0,0,16,17,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,13,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,10,0,0,0,0,0,0,9,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,4,0,0,0,0,11,0,0,0,0,0,12,0,0,0,0,0,3,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};



void setup()
{
   cpu. GPGUI_INIT();

    //MAP SETTING DATA IN RAM
    map_Ram[0] = map_Width;
    map_Ram[1] = map_Height;
    map_Ram[2] = roof_Color;
    map_Ram[3] = floor_Color;
    map_Ram[4] = 8;
    map_Ram[5] = 8;
    //


    
    //map_Ram[map_Mem_Start_Address] = 0; //ADDRESS 0 OF MAP GRID
    //write map into raycast ram//////
    for(int y = 0; y<20; y++)
    {
       for(int x =0; x<20; x++)
      {
          map_Ram[map_Pos+map_Mem_Start_Address] = MAP[y][x];
          map_Pos++;    
      } 
    }

  //FILL ALL 16 TEXTURE SLOTS IN RAM WITH DEFAULT TEXTURES/
for(int i = 0; i<17; i++) cpu.write_Default_Wall_Texture_To_Ram(i, texture_Ram);
}



  
void loop()
{ 


//DRAW FRAME
  cpu.drawBackgroundColor(0);
  
    cpu. raycast_Map_From_Ram
     (
         map_Ram,
         texture_Ram,
         view_Port_Start_X,
         view_Port_Start_Y,
         view_Port_Width,
         view_Port_Height,
         turn_Left,
         turn_Right,
         move_Fwd,
         move_Back,
         clipping_Enable,
         camera_Clip_Dist,
         camera_Up,
         camera_Down,
         move_Speed,
         turn_Speed,
         &bin,
         &bin,
         &bin,
         &bin,
         &bin,
         &bin,
         &bin,
         &bin,
         &bin,
         &bin,
         crosshair_Enable,
         debug_Enable
     );
     
  cpu.refreshScreen();
  }
