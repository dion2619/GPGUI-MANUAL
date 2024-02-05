#include <GPGUI_3_MAIN_CPU.h>
GPGUI_3_MAIN_CPU cpu;
byte map_Ram[4096];
byte texture_Ram[4096];
byte map_Width = 20;
byte map_Height = 20;
//
int   view_Port_Start_X   = 60;
int   view_Port_Start_Y   = 20;
int   view_Port_Width     = 200;
int   view_Port_Height    = 200;
int   camera_Clip_Dist    = 10;
bool  turn_Left           = false;
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
for(int i = 0; i<17; i++) cpu.write_Default_Texture_To_Ram(i, texture_Ram);
}



  
void loop()
{ 
  //CHECK KEYBOARD FOR INPUT
    switch (cpu.read_Keyboard_Port())
    {
      case 279:   move_Fwd    = true;  move_Back = false;   break;   
      case 33047: move_Fwd    = false;   break;  
      case 280:   move_Back   = true;  move_Fwd = false;  break;   
      case 33048: move_Back   = false;   break;  
      case 277:   turn_Left   = true; turn_Right = false;  break; 
      case 33045: turn_Left   = false; break; 
      case 278:   turn_Right  = true; turn_Left  = false; break; 
      case 33046: turn_Right  = false; break;    
      case 275:   camera_Up   = true;  camera_Down = false; break;    
      case 33043: camera_Up   = false; break;   
      case 276:   camera_Down = true; camera_Up = false; break;    
      case 33044: camera_Down = false; break;   
    }


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
         FOV,
         &current_Camera_PosX,
         &current_Camera_PosY,
         crosshair_Enable,
         debug_Enable
     );
     
  cpu.refreshScreen();
  }
