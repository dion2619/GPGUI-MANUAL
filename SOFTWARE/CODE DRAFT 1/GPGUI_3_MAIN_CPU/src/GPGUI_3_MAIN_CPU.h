using namespace std;



#include <vector>
#include <string>
#include <Arduino.h>
#include "Wire.h"
#include "WirePacker.h"
#include "UART/HardwareSerial.h"
#include "VGA6Bit.h"
#include "Font6x8.h"
#include "Tone32.h"
#include "KEYBOARD/PS2KeyAdvanced.h"
#include "EEPROM.h"
#include "digits7Seg.h"
#include "MESH_ARRAYS/UNIT_CUBE_MESH.h"
#include "MESH_ARRAYS/UNIT_ICOSAHEDRON.h"
#include "MESH_ARRAYS/UNIT_SPHARE_MESH.h"
#include "GLOBAL_3D_CAMERA_SETTINGS.h"
#include "icon sprites.h"
#include "MIDI_CALC/MIDI_CALC.h"
#include "key_Codes.h"
#include "debug_Flags.h"
#include "shaded_Colors.h"
#include "GLOBAL_3D_CAMERA_SETTINGS.h"
#include "MIDI_TAGS.h"
#include "MATH/GRAPHICS_MATH.h"
#include "DATA_STRUCTURES.h"
#include "I_O_PINS.h"
#include "6_BIT_COLORS.h"
#include "COM_ADDRESSES.h"





#define EEPROM_SIZE     4096
#define BUZZER_CHANNEL  0
#define ScreenWidth   320
#define ScreenHeight  240






//SYSTEM FLAGS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
bool serialDebugEnabledFlag  = false;
bool start_Menu_Enable       = false;
bool set_Menu_Enter_Note_From_Midi_Keyboard = false;
bool set_window_Close_Note_From_Midi_Keyboard = false;


//PROGRAM RUN FLAGS
bool PROGRAM_1_Enable = false;
bool PROGRAM_control_Panel_Enable = false;
bool PROGRAM_midi_Monitor_Emulator_Enable = false;
bool PROGRAM_hardware_Enable = false;
bool PROGRAM_appearance_Enable = false;
bool PROGRAM_3D_Model_Enable = false;
bool PROGRAM_NEW_PROGRAM_NAME = false;
bool PROGRAM_3D_Model_Editor_Enable = false;


//GLOBAL SETTINGS IN EEPROM////////////
int menu_Down_Pitch  = 200;
int menu_Up_Pitch    = 100;
int enter_Beep_Pitch = 500;
int exit_Beep_Pitch  =  50;
char button_Click_Bounce_Time = 80;
bool midi_Thru_To_Soundcard = true;
bool system_Sounds_Beep_Enable = false;
bool screen_Saver_Enable = false;
bool system_Sounds_Midi_En = true;

//System sounds midi
char button_Inst = button_Inst_Map[13];
char button_Vel = 127;
char button_Vol = 127;
char menu_Enter_Note = 67;
char window_Close_Note = 60;
char button_Hover_Note = 48;


//System colors
char desktop_Color = navy;
char bar_Color = grey;
char window_Bar_Color = blue;
char window_Bar_Text_Color = black;
char border_Line_Color = grey;
char FPS_Bar_Color = green;
char mouse_Cursor_Color = red;
char button_Text_Color = black;
////////////////////////////////////////











//SYSTEM GLOBALS
byte COLORS[15] = { 0,63,3,12,48,15,60,51,42,2,10,8,34,40,32 };
int keyboard_Data;



//MOUSE************************************************************************
int   controller = 1;
bool  mouse_Left_Make = false;
bool  mouse_Right_Make = false;
bool  mouse_Left_Break = false;
bool  mouse_Right_Break = false;
bool  mouse_Middle_Make = false;
bool  mouse_Middle_Break = false;
bool  mouse_Scroll_Up = false;
bool  mouse_Scroll_Down = false;
float mouseX = 160;
float mouseY = 120;
bool  button_Active = false;
bool Cursor_Enable = true;
bool Cursor_Enable_X = true;
bool Cursor_Enable_Y = true;
float mouse_Scroll_Speed = 0.5;
bool mouse_Dir_X;
bool mouse_Dir_Y;
bool mouse_Still;
byte x_Delta;
byte y_Delta;

//*****************************************************************************








byte map_Ram[4096];
byte texture_Ram[4096];

//OBJECTS
HardwareSerial CONTROL_PORT(1);
HardwareSerial DEVICE_PORT(2);
VGA6Bit vga;
PS2KeyAdvanced keyboard;
GRAPHICS_MATH m;
shaded_Colors cs;
MIDI_CALC mid;



class  GPGUI_3_MAIN_CPU
{

public:


     GPGUI_3_MAIN_CPU()//CONSTRUCTOR
   {
  /*  Serial.begin(115200);
    pinMode(testLed,OUTPUT);
    delay(10);*/
    
  // 
    }

void GPGUI_INIT()
{
    Serial.begin(115200);
    pinMode(testLed, OUTPUT);
    delay(100);
    enable_Vga_Port();
    enable_Keyboard();
    enable_I2c_Port();
    enable_Device_Port();
    enable_Control_Port();

    
    
    Serial.println("WAITING FOR SYSTEM TO START");
    delay(2000);
    set_HDD_Led(1);
    beep(100, 200);
    Serial.println("SYSTEM SETUP");
    set_Control_Mux_To_Mouse_Port();
}

void enable_Keyboard()
{
    keyboard.begin(DATA_PIN, CLK_PIN);
    keyboard.setNoBreak(false);          // No break codes for keys (when key released)
    keyboard.setNoRepeat(true);         // Don't repeat shift ctrl etc
    Serial.println("Keyboard Enabled");
}

void enable_Vga_Port()
{
    vga.setFrameBufferCount(2);
    vga.init(vga.MODE320x240, redPins, greenPins, bluePins, hsyncPin, vsyncPin);
    Serial.println("VGA OK");
}

void enable_Midi_Port()
{
    DEVICE_PORT.end();
    DEVICE_PORT.begin(31250, SERIAL_8N1, 16, 17);//rx2 tx2
    set_Device_Mux_To_Midi_Ports();
    Serial.println("MIDI Port Enabled");
}

void enable_Device_Port()
{
    DEVICE_PORT.end();
    DEVICE_PORT.begin(115200, SERIAL_8N1, 16, 17);//rx2 tx2
    Serial.println("Device Port Enabled");
}

void enable_Control_Port()
{
    CONTROL_PORT.end();
    CONTROL_PORT.begin(115200, SERIAL_8N1, 5, 18);//rx1 tx1
    Serial.println("Control Port Enabled");
}

void enable_I2c_Port()
{
    Wire.begin(SDA_1, SCL_1);
    Serial.println("I2c Port Enabled");
}

void set_HDD_Led(bool state)
{
    digitalWrite(testLed, state);
}

void beep(int pitch, int  dur)
{
    if (system_Sounds_Beep_Enable)
    {
        tone(BUZZER_PIN, pitch);
        delay(dur);
        noTone(BUZZER_PIN);
    }
}

void beep_On(int note)
{
    switch (note)
    {
    //case 60:  tone(BUZZER_PIN, 261); break;//C4 mid c
    //case 61:  tone(BUZZER_PIN, 277); break;//C#
    //case 62:  tone(BUZZER_PIN, 293); break;//D
    //case 63:  tone(BUZZER_PIN, 311); break;//D#
    //case 64:  tone(BUZZER_PIN, 329); break;//E
    //case 65:  tone(BUZZER_PIN, 349); break;//F
    //case 66:  tone(BUZZER_PIN, 369); break;//F#
    //case 67:  tone(BUZZER_PIN, 392); break;//G
    //case 68:  tone(BUZZER_PIN, 415); break;//G#
    //case 69:  tone(BUZZER_PIN, 440); break;//A
    //case 70:  tone(BUZZER_PIN, 466); break;//A#
    //case 71:  tone(BUZZER_PIN, 493); break;//B
    default: break;
    } 
}

void beep_Off()
{
   // noTone(BUZZER_PIN);
}

void clear_EEPROM()
{
    for (int i = 0; i < EEPROM_SIZE; i++)
    {
        EEPROM.writeByte(i, 0);
    }
}

void send_Midi_Message_To_Soundcard(byte status, byte V1, byte V2 )
{
    Wire.beginTransmission(i2cAddress_Soundcard);
    Wire.write(status);
    Wire.write(V1);
    Wire.write(V2);
    Wire.endTransmission();
}

void set_Device_Mux_To_Midi_Ports()
{
    SendCommandI2c(i2cAddress_System_A, 8, 5, 255, 255);
}

void set_Control_Mux_To_Mouse_Port()
{
    SendCommandI2c(i2cAddress_System_A, 9, mouse_Com_Port, 255, 255);
}

void reset_Mouse_Cpu()
{
    SendCommandI2c(i2cAddress_System_A, 1,255, 255, 255);
}

void reset_Slots()
{
    SendCommandI2c(i2cAddress_System_A, 2, 255, 255, 255);
}

void reset_Sys_A_Cpu()
{
    SendCommandI2c(i2cAddress_System_A, 3, 255, 255, 255);
}

void reset_Device_Plug_Cpu()
{
    SendCommandI2c(i2cAddress_System_A, 4, 255, 255, 255);
}

void clear_Lcd()
{
    SendCommandI2c(i2cAddress_System_A, 5, 255, 255, 255);
}

void enable_Lcd_Backlight()
{
    SendCommandI2c(i2cAddress_System_A, 5, 1, 255, 255);
}

void disable_Lcd_Backlight()
{
    SendCommandI2c(i2cAddress_System_A, 5, 0, 255, 255);
}

void send_Com_Test_To_Sys_A_I2c()
{
    SendCommandI2c(i2cAddress_System_A, 255, 0, 255, 255);
}



//COM PORTS#########################################################################

void SendCommandI2c(byte i2cAddress,byte command,byte A,byte B,byte C)
{
    Wire.beginTransmission(i2cAddress);
 
        Wire.write(command);
        Wire.write(A);
        Wire.write(B);
        Wire.write(C); 
    Wire.endTransmission();
}

void send_Midi_Message(byte control,byte data1,byte data2)
{
    DEVICE_PORT.write(control);
    DEVICE_PORT.write(data1);
    DEVICE_PORT.write(data2);
}

void send_Byte_To_Control_Port(byte data)
{
    CONTROL_PORT.write(data);
}

void send_Byte_To_Device_Port(byte data)
{
    DEVICE_PORT.write(data);
}

void read_Midi_Port(byte *status, byte *d1, byte *d2)
{
    static byte STATUS;
    static byte D1;
    static byte D2;
    if (DEVICE_PORT.available())
    {         
        STATUS = DEVICE_PORT.read();
 

        if (STATUS > 127)
        {
            D1 = DEVICE_PORT.read();
            D2 = DEVICE_PORT.read();
            *status = STATUS;
            *d1 = D1;
            *d2 = D2;

          if(midi_Thru_To_Soundcard) send_Midi_Message_To_Soundcard(STATUS, D1, D2);
        } 
    }

}

int read_Keyboard_Port()
{
    while (keyboard.available())
    {
        screen_Saver_Enable = false;
        keyboard_Data = keyboard.read() & 0xffff;
        Serial.println(keyboard_Data);
    }

    return keyboard_Data;
}

//MOUSE
void read_Mouse_Cpu()
{
    byte sync;
    byte com_Id;
    byte command;
    byte x;
    byte y;
    bool sign_X;
    bool sign_Y;

    static unsigned long start_Time = 0;


    if (CONTROL_PORT.available())
    {
       
       
        while (CONTROL_PORT.available())
        {
            sync = CONTROL_PORT.read();
            if (sync == 255)
            {
                com_Id = CONTROL_PORT.read();
                {
                    command = CONTROL_PORT.read();
                    switch (command)
                    {
                    case 1:  mouse_Left_Make    = true;   screen_Saver_Enable = false; break;
                    case 2:  mouse_Left_Break   = true;   mouse_Left_Make = false; break;
                    case 3:  mouse_Middle_Make  = true;                              break;
                    case 4:  mouse_Middle_Break = true;     mouse_Middle_Make = false; break;
                    case 5:  mouse_Right_Make   = true;                              break;
                    case 6:  mouse_Right_Break  = true;    break;
                    case 8:  mouse_Scroll_Up    = true;    break;
                    case 9:  mouse_Scroll_Down  = true;   break;
        
                     
                    case 7://CURSOR X AND Y CHANGE
                        x = CONTROL_PORT.read();
                        y = CONTROL_PORT.read();
                        if (Cursor_Enable)
                        {
                            if (Cursor_Enable_X)
                            {
                                //decode x 7 bits for pos 1 for sign
                                for (int i = 0; i < 7; i++) bitWrite(x_Delta, i, bitRead(x, i));

                                if (bitRead(x, 7))   sign_X = true;
                                if (!bitRead(x, 7))  sign_X = false;

                                if (sign_X)  mouse_Dir_X = 1;
                                if (!sign_X) mouse_Dir_X = 0;

                                if (sign_X) { if ((int)mouseX - x_Delta > 0)    mouseX -= x_Delta * mouse_Scroll_Speed; }
                                if (!sign_X) { if ((int)mouseX + x_Delta < 320)  mouseX += x_Delta * mouse_Scroll_Speed; }
                            }

                            if (Cursor_Enable_Y)
                            {
                                //decode y 7 bits for pos 1 for sign
                                for (int i = 0; i < 7; i++) bitWrite(y_Delta, i, bitRead(y, i));

                                if (bitRead(y, 7))   sign_Y = true;
                                if (!bitRead(y, 7))  sign_Y = false;

                                if (sign_Y)  mouse_Dir_Y = 1;
                                if (!sign_Y) mouse_Dir_Y = 0;

                                if (!sign_Y) { if ((int)mouseY - y_Delta > 0)    mouseY -= y_Delta * mouse_Scroll_Speed; }
                                if (sign_Y) { if ((int)mouseY + y_Delta < 240)  mouseY += y_Delta * mouse_Scroll_Speed; }
                            }
                        }

                        start_Time = millis();
                        mouse_Still = false;
                 

                        
                    break;
                    }
                }
            }
        }
   
     
    }

    //read time since last mouse move
    if (millis() - start_Time > 50)
    {
        mouse_Still = true; //Serial.println("still");
  //  else Serial.println("move");

    }

}


//MIDI CORE SOUND CARD
void set_Soundcard_Max_Volume(byte vol)
{
    Wire.beginTransmission(i2cAddress_Soundcard);
    Wire.write(251);
    Wire.write(vol);
    Wire.write(0);
    Wire.endTransmission();
    

}

void set_Soundcard_Max_Velocity(byte vel)
{
    Wire.beginTransmission(i2cAddress_Soundcard);
    Wire.write(250);
    Wire.write(vel);
    Wire.write(0);
    Wire.endTransmission();

}

void enable_Midi_To_Soundcard()
{
    midi_Thru_To_Soundcard = true;
}

void disable_Midi_To_Soundcard()
{
    midi_Thru_To_Soundcard = false;
}

void set_Patch(byte ch, byte patch)
{
    switch (ch)
    {

    case 1: send_Midi_Message_To_Soundcard(0xC0, patch, 0); break;

    }

}

void note_On(byte ch, byte note, byte velocity)
{
    switch (ch)
    {
    case 1:send_Midi_Message_To_Soundcard(144, note, velocity); break;
    }
}

void note_Off(byte ch, byte note)
{
    switch (ch)
    {
    case 1:send_Midi_Message_To_Soundcard(128, note, 0); break;
    }
}








//##############################################################################################################
//#//////////////////////////////////////////VGA BUFFER CONTROL////////////////////////////////////////////////#
//##############################################################################################################

int get_Color_Shade(int p_Color, int s_Color)
{
    int color = cs.shaded_Color[p_Color][s_Color];
    return color;

}

void setPixel(int x,int y,byte color)//https://www.rapidtables.com/web/color/RGB_Color.html
{
 

    vga.dotFast(x, y, color);
}

void drawLine(int x1,int y1,int x2,int y2,byte color)
{
    DDA_Line(x1, y1, x2, y2, color);
}

void draw_Vector(int PX1,int PY1,int R,float AN,bool rotation_Dir,int* px2,int* py2,int color)
{
    int PX2 = R;
    int PY2 = 0;

    m.rotate_Point(&PX2, &PY2, AN, rotation_Dir);
    PX2 += PX1;
    PY2 += PY1;

    *px2 = PX2;//return end point of vector
    *py2 = PY2;

    DDA_Line(PX1, PY1, PX2, PY2, color);
}

void drawBackgroundColor(byte color)
{
    for (int y = 0; y < ScreenHeight; y++)
    {
        for (int x = 0; x < ScreenWidth; x++)
        {
            vga.dotFast(x, y, color);
        }
    }
}

void drawHollowRectangle(int xStart, int yStart,int xSize, int ySize,byte color)
{
        for (int y = yStart; y < (yStart + ySize)-1; y++)//left vert line
        {
            vga.dotFast(xStart, y, color);
        }

        for (int y = yStart; y < (yStart + ySize)-1; y++)//RIGHT vert line
        {
            vga.dotFast((xStart + xSize) -1, y, color);

        }

        for (int x = xStart; x < (xStart + xSize) -1; x++)//top horizontal line
        {
            vga.dotFast(x, yStart, color);
        }

        for (int x = xStart; x < (xStart + xSize); x++)//bottom horizontal line
        {
            vga.dotFast(x, (yStart + ySize )-1 , color);
        }
}

void drawHollowCircle(int x,int y,int radius,byte color)
{
    vga.circle(x, y, radius, color);
}

void drawSolidCircle(int x,int y,int radius,byte color)
{
    vga.fillCircle(x, y, radius, color);
}

void drawSolidRectangle(int xStart,int yStart,int xSize,int ySize, byte color)
{
    if (xStart <= ScreenWidth - xSize && yStart <= ScreenHeight - ySize)//only draw if within the screen size
    {
        for (int y = yStart; y < (yStart + ySize); y++)
        {
            for (int x = xStart; x < (xStart + xSize); x++)
            {
                setPixel(x, y, color);
            }
        }
    }
}

void drawTriangle(int x1,int y1, int x2,int y2,int x3, int y3,byte fill_Type,byte color)
{
    byte color_a;
    byte color_b;
    byte color_c;



    if (triangle_Debug)
    {
        color_a = red;
        color_b = green;
        color_c = blue;

    }
    else
    {
        color_a = color;
        color_b = color;
        color_c = color;
    }

    m.triangle_CW_Sort(x1, y1, x2, y2, x3, y3);






    bool line_Debug_Color = false;
    byte triangle_1_Color = color;
    byte triangle_2_Color = color;
    byte debug_Color_1 = red;
    byte debug_Color_2 = green;

    byte side;
    byte n_Triangles = 0;
    float DX_Left = x1 - x3;
    float DY_Left = y3 - y1;
    float Xinc_Left = DX_Left / DY_Left;
    float XL = x1;
    //
    float DX_Right = x2 - x1;
    float DY_Right = y2 - y1;
    float Xinc_Right = DX_Right / DY_Right;//SLOPE
    float XR = x1;
    int top_Lines_N = DY_Right;
    float lines_To_Draw_Buffer[250][2];//left x right x
    float x4 = x3;//x3
    float y4 = y2;
  





    if (fill_Type == 0)//WIRE FRAME MODE
    {
        drawLine(x1, y1, x2, y2, color_a);
        drawLine(x2, y2, x3, y3, color_b);
        drawLine(x3, y3, x1, y1, color_c);
    }





    if (triangle_Debug)
    {
        triangle_1_Color = debug_Color_1;
        triangle_2_Color = debug_Color_2;
    }
    else
    {
        triangle_1_Color = color;
        triangle_2_Color = color;
    }



    //X LINE FILL FLAT BOTTOM//////////////////////////////////////////////////////////////////////////////////////////

    if (fill_Type == 1)
    {


        drawLine(x1, y1, x2, y2, color_a);
        drawLine(x2, y2, x3, y3, color_b);
        drawLine(x3, y3, x1, y1, color_c);


        //left line points in quad 2 from p1
        for (int y = 0; y < DY_Left; y++)
        {
            lines_To_Draw_Buffer[y][0] = round(XL);
            XL -= Xinc_Left;
        }



        //right line points in quad 1 from p1
        for (int y = 0; y < DY_Right; y++)
        {
            lines_To_Draw_Buffer[y][1] = round(XR);
            XR += Xinc_Right;
        }

        x4 = lines_To_Draw_Buffer[top_Lines_N][0];//b_Line left start

        if (x2 <= x4) side = 1;
        if (x2 >= x4) side = 2;

   
     
   
        //DRAW X LINES LEFT TO RIGHT down to flat bottom
        for (int y = 0; y < top_Lines_N; y++)
        {

        
       
           float step_Size = ((lines_To_Draw_Buffer[y][1] - lines_To_Draw_Buffer[y][0])/100) /100;

                   //DRAW FILL LINES LEFT TO RIGHT
            if (side == 2)
            {
               for (float x = lines_To_Draw_Buffer[y][0]; x < lines_To_Draw_Buffer[y][1]; x++)
                {
                    setPixel((int)x, y + y1, triangle_1_Color);
                

                   float step_Size = (lines_To_Draw_Buffer[y][1] - lines_To_Draw_Buffer[y][0]) /100;

              
                               
                }
                y4 = y2;
            }

            //DRAW FILL LINES RIGHT TO LEFT
            if (side == 1)
            {
                for (float x = lines_To_Draw_Buffer[y][0]; x > lines_To_Draw_Buffer[y][1]; x--)
                {
                    setPixel((int)x, y + y1, triangle_1_Color);
                }
                y4 = y3;
            }
        }




        if (y2 == y3 || y2 == y1 || y3 == y1)   n_Triangles = 1;
        else n_Triangles = 2;

        //END DRAW DOWN TO FLAT BOTTOM






        ////DRAW FROM FLAT TOP DOWN

        float DX_Left_B;
        float DY_Bottom;
        float Xinc_Left_B;
        float XL_B;
        ////

        //LEFT LINE POINTS
        if (side == 2)
        {
            DX_Left_B = x4 - x3;
            DY_Bottom = y3 - y4;
            Xinc_Left_B = DX_Left_B / DY_Bottom;
            XL_B = x4;


            for (int y = 0; y < DY_Bottom; y++)
            {

                 //setPixel(round(XL_B), y + y4, random(1, 64));
                lines_To_Draw_Buffer[y][0] = round(XL_B);
                XL_B -= Xinc_Left_B;
            }
        }


        if (side == 1)
        {
            DX_Left_B = x2 - x3;
            DY_Bottom = y2 - y3;
            XL_B = x3;
            Xinc_Left_B = DX_Left_B / DY_Bottom;

            for (int y = 0; y < DY_Bottom; y++)
            {
                 //setPixel(round(XL_B), y + y3, random(1, 64));
                lines_To_Draw_Buffer[y][0] = round(XL_B);
                XL_B += Xinc_Left_B;
            }
        }
        //////END LEFT LINE PONTS


          //  Right LINE POINTS
        if (side == 2)
        {
            DX_Left_B = x2 - x3;
            DY_Bottom = y3 - y2;
            Xinc_Left_B = DX_Left_B / DY_Bottom;
            XL_B = x2;

            for (int y = 0; y < DY_Bottom; y++)
            {
                 // setPixel(round(XL_B), y + y2, random(1, 64));
                lines_To_Draw_Buffer[y][1] = round(XL_B);
                XL_B -= Xinc_Left_B;
            }
        }



        if (side == 1)
        {
            DX_Left_B = x2 - x4;
            DY_Bottom = y2 - y4;
            XL_B = x4;
            Xinc_Left_B = DX_Left_B / DY_Bottom;

            for (int y = 0; y < DY_Bottom; y++)
            {
                 // setPixel(round(XL_B), y + y4, random(1, 64));
                lines_To_Draw_Buffer[y][1] = round(XL_B);
                XL_B += Xinc_Left_B;
            }
        }
        //END Right LINE PONTS



        if (side == 1)
        {
            //DRAW X LINES LEFT TO RIGHT down to flat bottom
            for (int y = 0; y < DY_Bottom; y++)
            {
                //DRAW FILL LINES LEFT TO RIGHT

                for (float x = lines_To_Draw_Buffer[y][0]; x < lines_To_Draw_Buffer[y][1]; x++)
                {
                   // setPixel((int)x, y + y4, triangle_2_Color);
                }
            }
        }

        if (side == 2)
        {
            //DRAW X LINES LEFT TO RIGHT down to flat bottom
            for (int y = 0; y < DY_Bottom; y++)
            {
                //DRAW FILL LINES LEFT TO RIGHT

                for (float x = lines_To_Draw_Buffer[y][0]; x < lines_To_Draw_Buffer[y][1]; x++)
                {
                   // setPixel((int)x, y + y2, triangle_2_Color);
                }
            }
        }
    }







    if (triangle_Debug)
    {
        print("TRIANGLES = ", 0, 220, red, black); printNumberInt(n_Triangles, 75, 220, green, black);
        print("P2 Side   = ", 0, 230, red, black); printNumberInt(side, 75, 230, green, black);
        setPixel(x1, y1, random(1, 64));
        setPixel(x2, y2, green);
        setPixel(x3, y3, blue);
        // if (y2 != y3) setPixel(x4, y4, yellow);


         print("P1", x1 + 5, y1 + 5, red, black);
         print("P2", x2 + 5, y2 + 5, green, black);
         print("P3", x3 + 5, y3 + 5, blue, black);

    }







    ////print lines buffer
  /*  for (int i = 0; i < DY_Bottom; i++)
    {
        Serial.print(i); Serial.print("    ");
        Serial.print(lines_To_Draw_Buffer[i][0]);
        Serial.print("    ");
        Serial.println(lines_To_Draw_Buffer[i][1]);
        delay(1);
    }*/


}

void drawTriangle_Pipeline(int x1,int y1,int x2,int y2,int x3,int y3,byte fill_Type,byte color)
{
    byte color_a;
    byte color_b;
    byte color_c;



    if (triangle_Debug)
    {
        color_a = red;
        color_b = green;
        color_c = blue;

    }
    else
    {
        color_a = color;
        color_b = color;
        color_c = color;
    }

    m.triangle_CW_Sort(x1, y1, x2, y2, x3, y3);


    bool line_Debug_Color = false;
    byte triangle_1_Color = color;
    byte triangle_2_Color = color;
    byte debug_Color_1 = red;
    byte debug_Color_2 = green;



    float DXL;
    float DYL;
    float DXR;
    float DYR;
    float slope_Left;
    float slope_Right;
    float slope_Top;
    float slope_Bottom;

    float left_Point_X;
    float right_Point_X;
    int Y;

    if (fill_Type == 0)//WIRE FRAME MODE
    {
        drawLine(x1, y1, x2, y2, color_a);
        drawLine(x2, y2, x3, y3, color_b);
        drawLine(x3, y3, x1, y1, color_c);
    }


    int t_Type;


    if (triangle_Debug)
    {
        triangle_1_Color = debug_Color_1;
        triangle_2_Color = debug_Color_2;
    }
    else
    {
        triangle_1_Color = color;
        triangle_2_Color = color;
    }



    //X LINE FILL FLAT BOTTOM//////////////////////////////////////////////////////////////////////////////////////////
    byte trig_Tri_Color;
    if (fill_Type == 1)
    {


     //   drawLine(x1, y1, x2, y2, color_a);
      //  drawLine(x2, y2, x3, y3, color_b);
       // drawLine(x3, y3, x1, y1, color_c);



        if (y2 == y3 && x1 > x2 && x1 < x3 || y2 == y3 && x1 > x3 && x1 < x2)// flat bottom 2>>3 type 1a   3>>2 type 1b center 
        {
            if (x2 < x3)//TYPE A
            {
                DXL = x1 - x2;
                DYL = y2 - y1;
                DXR = x3 - x1;
                DYR = y3 - y1;
                trig_Tri_Color = 1;
                t_Type = 1;
            }
            if (x3 < x2)//TYPE B
            {
                DXL = x1 - x3;
                DYL = y3 - y1;
                DXR = x2 - x1;
                DYR = y2 - y1;
                trig_Tri_Color = 2;
                t_Type = 2;
            }
            slope_Left = DXL / DYL;      
            slope_Right = DXR / DYR;                       
            left_Point_X  = x1;
            right_Point_X = x1;
       
            for (int i = 0; i < DYL; i++)
            {
                Y = i + y1;
                for (int x = left_Point_X; x < right_Point_X; x++)   setPixel(x, Y, color);
                right_Point_X += slope_Right;
                left_Point_X -= slope_Left;
            }       
        }




        if (y2 == y3 && x1 <= x2 && x2 < x3 || y2 == y3 && x1 <= x3 && x2 > x3)// flat bottom 2>>3 type 2a   3>>2 type 2b LEFT OFFSET
        {
            if (x2 < x3)//TYPE A
            {
                DXL = x1 - x2;
                DYL = y2 - y1;
                DXR = x3 - x1;
                DYR = y3 - y1;
                trig_Tri_Color = 3;
            }
            if (x3 < x2)//TYPE B
            {
                DXL = x1 - x3;
                DYL = y3 - y1;
                DXR = x2 - x1;
                DYR = y2 - y1;
                trig_Tri_Color = 4;
            }
            slope_Left = DXL / DYL;
            slope_Right = DXR / DYR;
            left_Point_X = x1;
            right_Point_X = x1;
          
            for (int i = 0; i < DYL; i++)
            {
                Y = i + y1;
                for (int x = left_Point_X; x < right_Point_X; x++)   setPixel(x, Y, color);
                right_Point_X += slope_Right;
                left_Point_X -= slope_Left;
            }
        }


        if (y2 == y3 && x1 >= x2 && x2 < x3 || y2 == y3 && x1 >= x3 && x2 > x3)// flat bottom 2>>3  3>>2  RIGHT OFFSET
        {
            if (x2 < x3)//TYPE A
            {
                DXL = x1 - x2;
                DYL = y2 - y1;
                DXR = x3 - x1;
                DYR = y3 - y1;
                trig_Tri_Color = 3;
            }
            if (x3 < x2)//TYPE B
            {
                DXL = x1 - x3;
                DYL = y3 - y1;
                DXR = x2 - x1;
                DYR = y2 - y1;
                trig_Tri_Color = 4;
            }
            slope_Left = DXL / DYL;
            slope_Right = DXR / DYR;
            left_Point_X = x1;
            right_Point_X = x1;
            for (int i = 0; i < DYL; i++)
            {
                Y = i + y1;
                for (int x = left_Point_X; x < right_Point_X; x++)   setPixel(x, Y, color);
                right_Point_X += slope_Right;
                left_Point_X -= slope_Left;
            }
        }//end type 3












        //FLAT TOPS//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


        if (y1 == y2 && x3 > x2 && x3 < x1 && x2 < x1|| y1 == y2 && x3 > x1 && x3 < x2 && x1 < x2)// flat top 1>>2 type 4a   2>>1 type 4b right offset
        {
            if (x1 < x2)//TYPE A
            {
                DXL = x3 - x1;
                DYL = y3 - y1;
                DXR = x2 - x3;
                DYR = y3 - y1;
                t_Type = 1;
            }
            if (x2 < x1)//TYPE B
            {
                DXL = x3 - x2;
                DYL = y3 - y1;
                DXR = x1 - x3;
                DYR = y3 - y1;
                t_Type = 2;
            }
            slope_Left = DXL / DYL;
            slope_Right = DXR / DYR;
            left_Point_X = x3;
            right_Point_X = x3;
            for (int i = 0; i < DYL; i++)
            {
                Y =  y1+DYL - i;
                for (int x = left_Point_X; x < right_Point_X; x++)   setPixel(x, Y, color);
                left_Point_X -= slope_Left;
                right_Point_X += slope_Right;
            }
        }



        if (y1 == y2 && x3 <= x1 && x1 < x2 || y1 == y2 && x3 <= x2 && x2 < x1)// flat top left offest
        {
            if (x1 < x2)//TYPE A
            {
                DXL = x3 - x1;
                DYL = y3 - y1;
                DXR = x2 - x3;
                DYR = y3 - y1;
                t_Type = 1;
            }
            if (x2 < x1)//TYPE B
            {
                DXL = x3 - x2;
                DYL = y3 - y1;
                DXR = x1 - x3;
                DYR = y3 - y1;
                t_Type = 2;
            }
            slope_Left = DXL / DYL;
            slope_Right = DXR / DYR;
            left_Point_X = x3;
            right_Point_X = x3;
            for (int i = 0; i < DYL; i++)
            {
                Y = y1 + DYL - i;
                for (int x = left_Point_X; x < right_Point_X; x++)   setPixel(x, Y, color);
                right_Point_X += slope_Right;
                left_Point_X -= slope_Left;
            }
        }


        if (y1 == y2 && x3 >= x2 && x1 < x2 || y1 == y2 && x3 >= x1 && x2 < x1)// flat top right offset
        {
            if (x1 < x2)//TYPE A
            {
                DXL = x3 - x1;
                DYL = y3 - y1;
                DXR = x2 - x3;
                DYR = y3 - y1;
                t_Type = 1;
            }
            if (x2 < x1)//TYPE B
            {
                DXL = x3 - x2;
                DYL = y3 - y1;
                DXR = x1 - x3;
                DYR = y3 - y1;
                t_Type = 2;
            }
            slope_Left = DXL / DYL;
            slope_Right = DXR / DYR;
            left_Point_X = x3;
            right_Point_X = x3;
            for (int i = 0; i < DYL; i++)
            {
                Y = y1 + DYL - i;
                for (int x = left_Point_X; x < right_Point_X; x++)   setPixel(x, Y, color);
                left_Point_X -= slope_Left;
                right_Point_X += slope_Right;
            }
        }//end type 5


        //END OF SINGLE TRIANGLES


























        if (y2 < y3)//SPLIT INTO TO TRIANGLES/////////////////////////////////////////////////////////////////////
        {
            if (x1 == x3 && y2 > y1&& y2 < y3 && x2 > x3 )//flat left  CENTER
            {             
                DXL = x2 - x1;
                DYL = y2 - y1;
                DXR = x2 - x1;
                DYR = y3 - y2;                             
                slope_Top =    DXL / DYL;
                slope_Bottom = DXR / DYR;               
                right_Point_X = x1;
                left_Point_X = x1;
                for (int i = 0; i < (DYL+DYR); i++)
                {                  
                    Y = y1 + i;
                    if (Y < y2 )
                    {
                        for (int x = left_Point_X; x < right_Point_X; x++)   setPixel(x, Y, color);//top 
                        right_Point_X += slope_Top;
                    }

                    if (Y >= y2 )
                    {
                        for (int x = left_Point_X; x < right_Point_X; x++)   setPixel(x, Y,color);//bottom
                        right_Point_X -= slope_Bottom;
                    }
                }
            }


            if (x1 == x3 && y2 > y1 && y2 < y3 && x2 < x3)//flat right CENTER
            {
                DXL = x1 - x2;
                DYL = y2 - y1;
                DXR = x1 - x2;
                DYR = y3 - y2;
                slope_Top = DXL / DYL;
                slope_Bottom = DXR / DYR;
                right_Point_X = x1;
                left_Point_X = x1;
                for (int i = 0; i < (DYL + DYR); i++)
                {                  
                    Y = y1 + i;
                    if (Y < y2 )
                    {
                        for (int x = left_Point_X; x < right_Point_X; x++)   setPixel(x, Y, color);
                        left_Point_X -= slope_Top;
                    }
                    if (Y >= y2)
                    {
                        for (int x = left_Point_X; x < right_Point_X; x++)   setPixel(x, Y, color);//bottom
                        left_Point_X += slope_Bottom;
                    }
                }
            }






            if (x2 == x3 && y2 < y3 && y1 < y2 && y1 < y3 && x1 > x2)//flat left upper offset 
            {
                DXL = x1 - x2;
                DYL = y2 - y1;
                DXR = x1 - x2;
                DYR = y3 - y1;
                slope_Top = DXL / DYL;
                slope_Bottom = DXR / DYR;
                right_Point_X = x1;
                left_Point_X = x1;
                for (int i = 0; i <  DYR; i++)
                {
                    Y = y1 + i;
                    if (Y < y2)
                    {
                        for (int x = left_Point_X ; x < right_Point_X; x++)   setPixel(x, Y, color);
                        left_Point_X -= slope_Top;
                        right_Point_X -= slope_Bottom;
                    }
                    if (Y >= y2)
                    {
                        left_Point_X = x2;
                        for (int x = left_Point_X; x < right_Point_X; x++)   setPixel(x, Y, color);//bottom                     
                        right_Point_X -= slope_Bottom;
                    }
                }
            }





            if (x2 == x3 && y2 < y3 && y1 < y2 && y1 < y3 && x1 < x2)//flat right upper offset LEFT
            {
                DXL = x2 - x1;
                DYL = y2 - y1;
                DXR = x2 - x1;
                DYR = y3 - y1;
                slope_Top = DXL / DYL;
                slope_Bottom = DXR / DYR;
                right_Point_X = x1;
                left_Point_X = x1;
                for (int i = 0; i < DYR; i++)
                {
                    Y = y1 + i;
                    if (Y < y2)
                    {
                        for (int x = left_Point_X; x < right_Point_X; x++)   setPixel(x, Y, color);
                        left_Point_X += slope_Bottom;
                        right_Point_X += slope_Top;
                    }
                    if (Y >= y2)
                    {
                        right_Point_X = x2;
                        for (int x = left_Point_X; x < right_Point_X; x++)   setPixel(x, Y, color);//bottom
                        left_Point_X += slope_Bottom;
                    }
                }
            }



            if (x1 == x2 && y2 < y3 && y1 < y2 && y1 < y3 && x3 > x1)//flat right lower offset RIGHT
            {
                DXL = x3 - x1;
                DYL = y3 - y1;
                DXR = x3 - x1;
                DYR = y3 - y2;
                slope_Top = DXL / DYL;
                slope_Bottom = DXR / DYR;
                right_Point_X = x1;
                left_Point_X = x1;
                for (int i = 0; i < DYL; i++)
                {
                    Y = y1 + i;
                    if (Y < y2)
                    {
                        left_Point_X = x1;
                        for (int x = left_Point_X; x < right_Point_X; x++)   setPixel(x, Y, color);
                        right_Point_X += slope_Top;
                    }
                    if (Y >= y2)
                    {
                        for (int x = left_Point_X; x < right_Point_X; x++)   setPixel(x, Y, color);//bottom
                        left_Point_X += slope_Bottom;
                        right_Point_X += slope_Top;
                    }
                }
            }



            if (x1 == x2 && y2 < y3 && y1 < y2 && y1 < y3 && x3 < x1)//flat right lower offset  LEFT
            {
                DXL = x2 - x3;
                DYL = y3 - y1;
                DXR = x2 - x3;
                DYR = y3 - y2;
                slope_Top = DXL / DYL;
                slope_Bottom = DXR / DYR;
                right_Point_X = x1;
                left_Point_X = x1;
              
                for (int i = 0; i < DYL; i++)
                {
                    Y = y1 + i;
                    if (Y < y2)
                    {
                        right_Point_X = x1;
                        for (int x = left_Point_X; x < right_Point_X; x++)   setPixel(x, Y, color);
                        left_Point_X -= slope_Top;
                    }
                    if (Y >= y2)
                    {
                        for (int x = left_Point_X; x < right_Point_X; x++)   setPixel(x, Y, color);//bottom
                        left_Point_X -= slope_Top;
                        right_Point_X -= slope_Bottom;
                    }
                }
            }














        }

    }

    print("P1", x1 + 5, y1 + 5, red, black);
    print("P2", x2 + 5, y2 + 5, green, black);
    print("P3", x3 + 5, y3 + 5, blue, black);
}

void draw_Quad(float p1_X,float p1_Y,float p2_X,float p2_Y,float p3_X,float p3_Y,float p4_X,float p4_Y,bool fill_Type,byte color)
{

    drawTriangle_Pipeline((int)p1_X, (int)p1_Y, (int)p2_X, (int)p2_Y, (int)p3_X, (int)p3_Y, fill_Type, green);
  //  drawTriangle((int)p1_X, (int)p1_Y, (int)p3_X, (int)p3_Y, (int)p4_X, (int)p4_Y, fill_Type, red);

}

void map_Scanline_From_Texture(int x1,int x2,int yi,float texXstart,float texXend,float texYstart,byte tex[32][32])

{
    int texture_Width = 32;
    float un = 0;
    int texX = 0;
    int texY = 0;
    int texX_Index = 0;
    // float z = 0.8;
    // float zi = 1 / z;

    if (texXstart == 0 && texXend == 0) texXend = 1;

    m.interp(x1, x2, texXstart, &texX_Index);

    m.interp(0, texture_Width, texYstart, &texY);

    for (int i = x1; i < x2; i++)
    {
        m.normalize(i, x1, x2, &un);
        //un = un / z;//perspective divide
        //float uc = un * 1/zi;//perspective corect


        if (un + texXstart < 1 && un + texXstart < texXend)
        {
            m.interp(0, texture_Width, un + texXstart, &texX);//get texture x pos
            setPixel((i + texX_Index) - x1, yi, tex[texY][texX]);//sample texture for color og
        }
        else { break; }
    }


}

void draw_Perspective_Plane(int xStart,int yStart,int screen_Width,int screen_Height,int zFar_Offset,int zNear_Offset,int zFar_Plane_Length,int zNear_Plane_Length,byte tex[32][32])
{
    bool  debug = false;
    int zi = 0;
    int zFar = yStart + zFar_Offset;
    int zNear = (yStart + screen_Height) - zNear_Offset;
    int p1x = xStart + (screen_Width - zFar_Plane_Length) / 2;
    int p1y = zFar;
    int p2x = xStart + (screen_Width - ((screen_Width - zFar_Plane_Length) / 2));
    int p2y = zFar;
    int p3x = xStart + (screen_Width - ((screen_Width - zNear_Plane_Length) / 2));
    int p3y = zNear;
    int p4x = xStart + (screen_Width - zNear_Plane_Length) / 2;
    int p4y = zNear;
    float  incXL_Float_Counter = 0;
    float dxL = p1x - p4x;
    float dyL = p4y - p1y;
    float dxR = p3x - p2x;
    float dyR = p3y - p2y;
    float mL = dxL / dyL;
    float mR = dxR / dyR;
    int yInc = 1;
    int n_Lines = zNear - zFar;
    float xL_Counter = p1x;
    float xR_Counter = p2x;
    int xL = p1x;
    int xR = p2x;
    int yL = p1y;



    //DRAW BACKGROUND WINDOW
    drawSolidRectangle(xStart, yStart, screen_Width, screen_Height, black);



    if (debug)
    {
        //DRAW WIRE FRAME
        drawLine(p1x, p1y, p2x, p2y, red);
        drawLine(p2x, p2y, p3x, p3y, green);
        drawLine(p3x, p3y, p4x, p4y, blue);
        drawLine(p4x, p4y, p1x, p1y, yellow);

        //DRAW POINTS
        setPixel(p1x, p1y, random(1, 63));
        setPixel(p2x, p2y, random(1, 63));
        setPixel(p3x, p3y, random(1, 63));
        setPixel(p4x, p4y, random(1, 63));
    }


    //GET LEFT AND RIGHT END POINTS OF SCANLINE/////////////////////

    for (int y = 0; y < n_Lines; y++)
    {
        xL_Counter -= mL;
        yL += yInc;
        xL = round(xL_Counter);
        //
        xR_Counter += mR;
        xR = round(xR_Counter);
        //
        float vi;//texture v space
        m.normalize(y + zFar, zFar, zNear, &vi);//CONVERT SCREEN SPACE DEPTH BETWEEN PLANES TO TEX V SPACE 0.0f -1.0f
        map_Scanline_From_Texture(xL, xR, yL, 0.0f, 1.0f, vi, tex);
    }
}

void DDA_Line(int X1,int Y1,int X2,int Y2,byte color)//(P1   P2)
{
    int xPos = X1;
    int yPos = Y1;
    double DX;//DELTA X
    double DY;//DELTA Y
    double ySlope_Counter = yPos;//FLOATING POINT COUNTER THAT GETS ROUNDED TO THE CLOSEST INT
    double xSlope_Counter = xPos;
    int steps; //THE AMOUNT OF POINTS ALONG THE LINE
    double Xinc;//X,Y STEP AMOUNT
    double Yinc;
    double M; //SLOPE OF THE LINE 
    int quad; //HOLDS WHICH 90 DEG SEGMENT OF THE COMPASS P2 IS IN RELATION TO P1



    //FIND QUAD OR QUAD LINE OF P2 /////////////////////////
    //S/E - QUAD 1
    if (X2 > X1 && Y2 > Y1)
    {
        DX = X2 - X1;
        DY = Y2 - Y1;
        quad = 1;
    }

    //S/W - QUAD 2
    if (X2 <= X1 && Y2 > Y1)
    {
        DX = X1 - X2;
        DY = Y2 - Y1;
        quad = 2;
    }


    //N/W - QUAD 3
    if (X2 <= X1 && Y2 <= Y1)
    {
        DX = X1 - X2;// find quad
        DY = Y1 - Y2;
        quad = 3;
    }

    //N/E - QUAD 4
    if (X2 > X1 && Y2 <= Y1)
    {
        DX = X2 - X1;
        DY = Y1 - Y2;
        quad = 4;
    }





    //FIND THE SMALLEST  DELTA AND DIVIDE BY THE BIGGEST DELTA TO GET M-SLOPE THEN SET STEP TO LONGEST DELTA

    if (DY <= DX)
    {
        steps = DX;
        M = DY / DX;
        //FIND  INCREMENT SIZE
        if (M < 1) { Xinc = 1; Yinc = M; }   //0-45 deg in one quad
        if (M == 1) { Xinc = 1; Yinc = 1; }   //normal
        if (M > 1) { Xinc = M; Yinc = 1; }   //45-90 deg
    }

    if (DX <= DY)
    {
        steps = DY;
        M = DX / DY;
        if (M > 1) { Xinc = 1; Yinc = M; }   //0-45 deg in one quad
        if (M == 1) { Xinc = 1; Yinc = 1; }   //normal
        if (M < 1) { Xinc = M; Yinc = 1; }   //45-90 deg
    }





    if (DDA_Line_Debug)
    {
        color = random(0, 64);
        Serial.println();
        Serial.println();
        Serial.println();
        Serial.print("DY = ");            Serial.println(DY);
        Serial.print("DX = ");            Serial.println(DX);
        Serial.print("STEPS = ");         Serial.println(steps);
        Serial.print("X increment = ");   Serial.println(Xinc);
        Serial.print("Y increment = ");   Serial.println(Yinc);
        Serial.print("QUAD = ");          Serial.println(quad);
        Serial.print("SLOPE = ");         Serial.println(M);
        Serial.println();
    }



    //CALCULATE POINTS FROM P1 - P2
    setPixel(xPos, yPos, color);
    //PICK QUAD TO FIND STEP DIRECTION
    //S/E Q1
    if (quad == 1)
    {
        for (int i = 0; i < steps - 1; i++)
        {
            xSlope_Counter += Xinc;
            xPos = round(xSlope_Counter);
            //
            ySlope_Counter += Yinc; //add float values
            yPos = round(ySlope_Counter); //round to ints
            //  
            setPixel(xPos, yPos, color);

            if (DDA_Line_Debug)
            {
                Serial.print("x pos = ");    Serial.print(xPos);
                Serial.print("   y pos = "); Serial.println(yPos);
            }
        }
    }////////////////////////////////////////////////////


    //S/W Q2
    if (quad == 2)
    {
        for (int i = 0; i < steps - 1; i++)
        {
            xSlope_Counter -= Xinc;
            xPos = round(xSlope_Counter);
            //
            ySlope_Counter += Yinc; //add float values
            yPos = round(ySlope_Counter); //round to ints
            //  
            setPixel(xPos, yPos, color);
            if (DDA_Line_Debug)
            {
                Serial.print("x pos = ");    Serial.print(xPos);
                Serial.print("   y pos = "); Serial.println(yPos);
            }
        }
    }////////////////////////////////////////////////////



    //S/W Q3
    if (quad == 3)
    {
        for (int i = 0; i < steps - 1; i++)
        {
            xSlope_Counter -= Xinc;
            xPos = round(xSlope_Counter);
            //
            ySlope_Counter -= Yinc; //add float values
            yPos = round(ySlope_Counter); //round to ints
            setPixel(xPos, yPos, color);
            if (DDA_Line_Debug)
            {
                Serial.print("x pos = ");    Serial.print(xPos);
                Serial.print("   y pos = "); Serial.println(yPos);
            }
        }
    }///////////////////////////////////////////////////



    // S/W Q4
    if (quad == 4)
    {
        for (int i = 0; i < steps - 1; i++)
        {
            xSlope_Counter += Xinc;
            xPos = round(xSlope_Counter);
            //
            ySlope_Counter -= Yinc; //add float values
            yPos = round(ySlope_Counter); //round to ints
            setPixel(xPos, yPos, color);

            if (DDA_Line_Debug)
            {
                Serial.print("x pos = ");    Serial.print(xPos);
                Serial.print("   y pos = "); Serial.println(yPos);
            }
        }
    }
}

void sine_Wave_Bounce(float* Xpos,float* Ypos,bool dir,int height,float X_Distance,float start_Speed,float velocity,unsigned long currentMillis,bool enable,bool stop,bool* state)
{

    static int angle;
    int amp = height;//height
    static float Yinc;
    float Xinc = X_Distance / 180;
    static float Pos_speed = start_Speed;
    static unsigned long previousMillis = 0;
    static int x_Start = *Xpos;/////static
    static int y_Start = *Ypos;
    static bool run = false;


    if (enable == true)
    {
        run = true;
        *state = run;
        x_Start = *Xpos;//get starting x point of jump
    }

    if (stop == true)
    {
        run = false;
        angle = 0;
        Pos_speed = start_Speed;
        *state = run;
        if (x_Start > 320) x_Start = 320;
        if (x_Start < 0)   x_Start = 0;
        x_Start = *Xpos;//reset starting x and y to current point
        y_Start = *Ypos;
    }



    ///////////////////////////////////////////////////////////////
    if (currentMillis - previousMillis >= Pos_speed && run)
    {
        previousMillis = currentMillis;


        //CALCULATE X AND Y POSITIONS
        Yinc = sin(angle * 3.144 / 180) * amp;
        if (*Ypos > 20) *Ypos = y_Start - Yinc;
        // else stop = true;



         // CHANGE DIRECTION
        if (dir) { if (*Xpos < 320)*Xpos += Xinc; }
        if (!dir) { if (*Xpos > 0) *Xpos -= Xinc; }
        //

        angle++;
        //SLOW DOWN ON THE UP SLOPE AND DECRESS ON THE WAY DOWN

        if (angle < 90) Pos_speed += velocity;
        if (angle > 90) Pos_speed -= velocity;



        //RESET SPEED AND START POINT
        if (angle > 179) //end of arc
        {
            angle = 0;
            Pos_speed = start_Speed;
            run = false;
            *state = false;
            x_Start = *Xpos;//reset jump origin
        }

    }///////////////////////////////////////////////////////




    if (sine_Jump_Debug)
    {
        float Yinc_Line;
        float Xpos_Line = x_Start;
        float Ypos_Line = y_Start;


        //DRAW SINE LINE
        for (int i = 0; i < 180; i++)
        {
            //CALCULATE X AND Y POSITIONS
            Yinc_Line = sin(i * 3.144 / 180) * amp;
            if (Ypos_Line > 0) Ypos_Line = y_Start - Yinc_Line;
            if (Xpos_Line < 320 - Xinc && dir)              Xpos_Line += Xinc;
            if (Xpos_Line > Xinc && Xpos_Line > 0 && !dir)  Xpos_Line -= Xinc;
            if (Xpos_Line > 1 && Xpos_Line < 320 && Ypos_Line >1 && Ypos_Line < 239)
                setPixel(Xpos_Line, Ypos_Line, random(1, 64));
        }



        print("Pos_speed = ", 0, 0, red, black);  printNumberDouble(Pos_speed, 70, 0, green, black);
        print("DIR       = ", 0, 8, red, black);  printNumberInt(dir, 70, 8, green, black);
        print("X Pos     = ", 0, 16, red, black);  printNumberInt(*Xpos, 70, 16, green, black);
        print("Y Pos     = ", 0, 24, red, black);  printNumberInt(*Ypos, 70, 24, green, black);
        print("Sine angle= ", 0, 32, red, black);  printNumberInt(angle, 70, 32, green, black);
        print("X Inc     = ", 0, 40, red, black);  printNumberDouble(Xinc, 70, 40, green, black);
        print("Y Inc     = ", 0, 48, red, black);  printNumberInt(Yinc, 70, 48, green, black);
        print("X Distance= ", 0, 56, red, black);  printNumberInt(X_Distance, 70, 56, green, black);
        print("Amplitude = ", 0, 64, red, black);  printNumberInt(amp, 70, 64, green, black);
        print("X Start   = ", 0, 72, red, black);  printNumberInt(x_Start, 70, 72, green, black);
        print("Y Start   = ", 0, 80, red, black);  printNumberInt(y_Start, 70, 80, green, black);

    }
}

void DDA_Line_Clip(int X1,int Y1, int X2,int Y2,byte color,int xStart,int yStart,int window_Size_X,int window_Size_Y)//(P1   P2)
{
    int xPos = X1;
    int yPos = Y1;
    double DX;//DELTA X
    double DY;//DELTA Y
    double ySlope_Counter = yPos;//FLOATING POINT COUNTER THAT GETS ROUNDED TO THE CLOSEST INT
    double xSlope_Counter = xPos;
    int steps; //THE AMOUNT OF POINTS ALONG THE LINE
    double Xinc;//X,Y STEP AMOUNT
    double Yinc;
    double M; //SLOPE OF THE LINE 
    int quad; //HOLDS WHICH 90 DEG SEGMENT OF THE COMPASS P2 IS IN RELATION TO P1



    //FIND QUAD OR QUAD LINE OF P2 /////////////////////////
    //S/E - QUAD 1
    if (X2 > X1 && Y2 > Y1)
    {
        DX = X2 - X1;
        DY = Y2 - Y1;
        quad = 1;
    }

    //S/W - QUAD 2
    if (X2 <= X1 && Y2 > Y1)
    {
        DX = X1 - X2;
        DY = Y2 - Y1;
        quad = 2;
    }


    //N/W - QUAD 3
    if (X2 <= X1 && Y2 <= Y1)
    {
        DX = X1 - X2;// find quad
        DY = Y1 - Y2;
        quad = 3;
    }

    //N/E - QUAD 4
    if (X2 > X1 && Y2 <= Y1)
    {
        DX = X2 - X1;
        DY = Y1 - Y2;
        quad = 4;
    }





    //FIND THE SMALLEST  DELTA AND DIVIDE BY THE BIGGEST DELTA TO GET M-SLOPE THEN SET STEP TO LONGEST DELTA

    if (DY <= DX)
    {
        steps = DX;
        M = DY / DX;
        //FIND  INCREMENT SIZE
        if (M < 1) { Xinc = 1; Yinc = M; }   //0-45 deg in one quad
        if (M == 1) { Xinc = 1; Yinc = 1; }   //normal
        if (M > 1) { Xinc = M; Yinc = 1; }   //45-90 deg
    }

    if (DX <= DY)
    {
        steps = DY;
        M = DX / DY;
        if (M > 1) { Xinc = 1; Yinc = M; }   //0-45 deg in one quad
        if (M == 1) { Xinc = 1; Yinc = 1; }   //normal
        if (M < 1) { Xinc = M; Yinc = 1; }   //45-90 deg
    }





    //CALCULATE POINTS FROM P1 - P2
 
    if (xPos > xStart && xPos < xStart + (window_Size_X-1) && yPos > yStart && yPos < yStart + window_Size_Y)  setPixel(xPos, yPos, color);
  
    //PICK QUAD TO FIND STEP DIRECTION
    //S/E Q1
    if (quad == 1)
    {
        for (int i = 0; i < steps - 1; i++)
        {
            xSlope_Counter += Xinc;
            xPos = round(xSlope_Counter);
            //
            ySlope_Counter += Yinc; //add float values
            yPos = round(ySlope_Counter); //round to ints
            //  
            if (xPos > xStart && xPos < xStart + (window_Size_X - 1) && yPos > yStart && yPos < yStart + window_Size_Y)   setPixel(xPos, yPos, color);
           // else { break; }
        }
    }////////////////////////////////////////////////////


    //S/W Q2
    if (quad == 2)
    {
        for (int i = 0; i < steps - 1; i++)
        {
            xSlope_Counter -= Xinc;
            xPos = round(xSlope_Counter);
            //
            ySlope_Counter += Yinc; //add float values
            yPos = round(ySlope_Counter); //round to ints
            //  
            if (xPos > xStart && xPos < xStart + (window_Size_X-1) && yPos > yStart && yPos < yStart + window_Size_Y) setPixel(xPos, yPos, color);
          //  else { break; }
        }
    }////////////////////////////////////////////////////



    //S/W Q3
    if (quad == 3)
    {
        for (int i = 0; i < steps - 1; i++)
        {
            xSlope_Counter -= Xinc;
            xPos = round(xSlope_Counter);
            //
            ySlope_Counter -= Yinc; //add float values
            yPos = round(ySlope_Counter); //round to ints
            if (xPos > xStart && xPos < xStart + (window_Size_X-1) && yPos > yStart && yPos < yStart + window_Size_Y) setPixel(xPos, yPos, color);
           // else { break; }
        }
    }///////////////////////////////////////////////////



    // S/W Q4
    if (quad == 4)
    {
        for (int i = 0; i < steps - 1; i++)
        {
            xSlope_Counter += Xinc;
            xPos = round(xSlope_Counter);
            //
            ySlope_Counter -= Yinc; //add float values
            yPos = round(ySlope_Counter); //round to ints
            if (xPos > xStart && xPos < xStart + (window_Size_X-1) && yPos > yStart && yPos < yStart + window_Size_Y) setPixel(xPos, yPos, color);
           // else { break; }
        }
    }
}





//TXT AND DATA DISPLAY#########################################################################

void printNumberInt(int inputData,int x, int y,byte textColor,byte backColor)
{
    static byte r;
    static byte g;
    static byte b;
    static byte rB;
    static byte gB;
    static byte bB;

    switch (textColor)//base 16 colors
    {
    case  0:      r = 0;   g = 0;   b = 0;   break;//BLACK
    case white:   r = 255; g = 255; b = 255; break;//WHITE
    case red:     r = 255; g = 0;   b = 0;   break;//RED
    case lime:    r = 0;   g = 255; b = 0;   break;//LIME
    case blue:    r = 0;   g = 0;   b = 255; break;//BLUE
    case yellow:  r = 255; g = 255; b = 0;   break;//YELLOW
    case cyan:    r = 0;   g = 255; b = 255; break;//CYAN/AQUA
    case magenta: r = 255; g = 0;   b = 255; break;//MAGENTA/FUCHSIA
    case grey:    r = 128; g = 128; b = 128; break;//GREY
    case maroon:  r = 128; g = 0;   b = 0;   break;//MAROON
    case olive:   r = 128; g = 128; b = 0;   break;//OLIVE
    case green:   r = 0;   g = 128; b = 0;   break;//GREEN
    case purple:  r = 128; g = 0;   b = 128; break;//PURPLE
    case teal:    r = 0;   g = 128; b = 128; break;//TEAL
    case navy:    r = 0;   g = 0;   b = 128; break;//NAVY
    //END BASE 16 COLORS
    }


    switch (backColor)//base 16 colors
    {
    case  0:      rB = 0; gB = 0; bB = 0; break;//BLACK
    case white:   rB = 255; gB = 255; bB = 255; break;//WHITE
    case red:     rB = 255; gB = 0; bB = 0; break;//RED
    case lime:    rB = 0; gB = 255; bB = 0; break;//LIME
    case blue:    rB = 0; gB = 0; bB = 255; break;//BLUE
    case yellow:  rB = 255; gB = 255; bB = 0; break;//YELLOW
    case cyan:    rB = 0; gB = 255; bB = 255; break;//CYAN/AQUA
    case magenta: rB = 255; gB = 0; bB = 255; break;//MAGENTA/FUCHSIA
    case grey:    rB = 128; gB = 128; bB = 128; break;//GREY
    case maroon:  rB = 128; gB = 0; bB = 0; break;//MAROON
    case olive:   rB = 128; gB = 128; bB = 0; break;//OLIVE
    case green:   rB = 0; gB = 128; bB = 0; break;//GREEN
    case purple:  rB = 128; gB = 0; bB = 128; break;//PURPLE
    case teal:    rB = 0; gB = 128; bB = 128; break;//TEAL
    case navy:    rB = 0; gB = 0; bB = 128; break;//NAVY
    //END BASE 16 COLORS
    }

    vga.setFont(Font6x8);
    vga.setTextColor(vga.RGB(r, g, b), vga.RGB(rB, gB, bB));
    vga.setCursor(x, y);
    vga.print(inputData, 10, 1);
}

void printNumberHex(int inputData, int x,int y,byte textColor,byte backColor)
{
    static byte r;
    static byte g;
    static byte b;
    static byte rB;
    static byte gB;
    static byte bB;

    switch (textColor)//base 16 colors
    {
    case  0:      r = 0;   g = 0;   b = 0;   break;//BLACK
    case white:   r = 255; g = 255; b = 255; break;//WHITE
    case red:     r = 255; g = 0;   b = 0;   break;//RED
    case lime:    r = 0;   g = 255; b = 0;   break;//LIME
    case blue:    r = 0;   g = 0;   b = 255; break;//BLUE
    case yellow:  r = 255; g = 255; b = 0;   break;//YELLOW
    case cyan:    r = 0;   g = 255; b = 255; break;//CYAN/AQUA
    case magenta: r = 255; g = 0;   b = 255; break;//MAGENTA/FUCHSIA
    case grey:    r = 128; g = 128; b = 128; break;//GREY
    case maroon:  r = 128; g = 0;   b = 0;   break;//MAROON
    case olive:   r = 128; g = 128; b = 0;   break;//OLIVE
    case green:   r = 0;   g = 128; b = 0;   break;//GREEN
    case purple:  r = 128; g = 0;   b = 128; break;//PURPLE
    case teal:    r = 0;   g = 128; b = 128; break;//TEAL
    case navy:    r = 0;   g = 0;   b = 128; break;//NAVY
    //END BASE 16 COLORS
    }


    switch (backColor)//base 16 colors
    {
    case  0:      rB = 0;   gB = 0;   bB = 0;   break;//BLACK
    case white:   rB = 255; gB = 255; bB = 255; break;//WHITE
    case red:     rB = 255; gB = 0;   bB = 0;   break;//RED
    case lime:    rB = 0;   gB = 255; bB = 0;   break;//LIME
    case blue:    rB = 0;   gB = 0;   bB = 255; break;//BLUE
    case yellow:  rB = 255; gB = 255; bB = 0;   break;//YELLOW
    case cyan:    rB = 0;   gB = 255; bB = 255; break;//CYAN/AQUA
    case magenta: rB = 255; gB = 0;   bB = 255; break;//MAGENTA/FUCHSIA
    case grey:    rB = 128; gB = 128; bB = 128; break;//GREY
    case maroon:  rB = 128; gB = 0;   bB = 0;   break;//MAROON
    case olive:   rB = 128; gB = 128; bB = 0;   break;//OLIVE
    case green:   rB = 0;   gB = 128; bB = 0;   break;//GREEN
    case purple:  rB = 128; gB = 0;   bB = 128; break;//PURPLE
    case teal:    rB = 0;   gB = 128; bB = 128; break;//TEAL
    case navy:    rB = 0;   gB = 0;   bB = 128; break;//NAVY
    //END BASE 16 COLORS
    }

    vga.setFont(Font6x8);
    vga.setTextColor(vga.RGB(r, g, b), vga.RGB(rB, gB, bB));
    vga.setCursor(x, y);
    vga.print(inputData, 16, 1);
}

void printNumberDouble(double inputData,int x,int y,byte textColor,byte backColor)
{
    static byte r;
    static byte g;
    static byte b;
    static byte rB;
    static byte gB;
    static byte bB;

    switch (textColor)//base 16 colors
    {
    case  0:      r = 0; g = 0; b = 0; break;//BLACK
    case white:   r = 255; g = 255; b = 255; break;//WHITE
    case red:     r = 255; g = 0; b = 0; break;//RED
    case lime:    r = 0; g = 255; b = 0; break;//LIME
    case blue:    r = 0; g = 0; b = 255; break;//BLUE
    case yellow:  r = 255; g = 255; b = 0; break;//YELLOW
    case cyan:    r = 0; g = 255; b = 255; break;//CYAN/AQUA
    case magenta: r = 255; g = 0; b = 255; break;//MAGENTA/FUCHSIA
    case grey:    r = 128; g = 128; b = 128; break;//GREY
    case maroon:  r = 128; g = 0; b = 0; break;//MAROON
    case olive:   r = 128; g = 128; b = 0; break;//OLIVE
    case green:   r = 0; g = 128; b = 0; break;//GREEN
    case purple:  r = 128; g = 0; b = 128; break;//PURPLE
    case teal:    r = 0; g = 128; b = 128; break;//TEAL
    case navy:    r = 0; g = 0; b = 128; break;//NAVY
    //END BASE 16 COLORS
    }


    switch (backColor)//base 16 colors
    {
    case  0:      rB = 0; gB = 0; bB = 0; break;//BLACK
    case white:   rB = 255; gB = 255; bB = 255; break;//WHITE
    case red:     rB = 255; gB = 0; bB = 0; break;//RED
    case lime:    rB = 0; gB = 255; bB = 0; break;//LIME
    case blue:    rB = 0; gB = 0; bB = 255; break;//BLUE
    case yellow:  rB = 255; gB = 255; bB = 0; break;//YELLOW
    case cyan:    rB = 0; gB = 255; bB = 255; break;//CYAN/AQUA
    case magenta: rB = 255; gB = 0; bB = 255; break;//MAGENTA/FUCHSIA
    case grey:    rB = 128; gB = 128; bB = 128; break;//GREY
    case maroon:  rB = 128; gB = 0; bB = 0; break;//MAROON
    case olive:   rB = 128; gB = 128; bB = 0; break;//OLIVE
    case green:   rB = 0; gB = 128; bB = 0; break;//GREEN
    case purple:  rB = 128; gB = 0; bB = 128; break;//PURPLE
    case teal:    rB = 0; gB = 128; bB = 128; break;//TEAL
    case navy:    rB = 0; gB = 0; bB = 128; break;//NAVY
    //END BASE 16 COLORS
    }

    vga.setFont(Font6x8);
    vga.setTextColor(vga.RGB(r, g, b), vga.RGB(rB, gB, bB));
    vga.setCursor(x, y);
    vga.print(inputData, 2, 1);
}

void print(const char* inputData,int x,int y,byte textColor,byte backColor)
{

    static byte r;
    static byte g;
    static byte b;
    static byte rB;
    static byte gB;
    static byte bB;

    switch (backColor)//base 16 colors
    {
    case  0:      rB = 0; gB = 0; bB = 0; break;//BLACK
    case white:   rB = 255; gB = 255; bB = 255; break;//WHITE
    case red:     rB = 255; gB = 0; bB = 0; break;//RED
    case lime:    rB = 0; gB = 255; bB = 0; break;//LIME
    case blue:    rB = 0; gB = 0; bB = 255; break;//BLUE
    case yellow:  rB = 255; gB = 255; bB = 0; break;//YELLOW
    case cyan:    rB = 0; gB = 255; bB = 255; break;//CYAN/AQUA
    case magenta: rB = 255; gB = 0; bB = 255; break;//MAGENTA/FUCHSIA
    case grey:    rB = 128; gB = 128; bB = 128; break;//GREY
    case maroon:  rB = 128; gB = 0; bB = 0; break;//MAROON
    case olive:   rB = 128; gB = 128; bB = 0; break;//OLIVE
    case green:   rB = 0; gB = 128; bB = 0; break;//GREEN
    case purple:  rB = 128; gB = 0; bB = 128; break;//PURPLE
    case teal:    rB = 0; gB = 128; bB = 128; break;//TEAL
    case navy:    rB = 0; gB = 0; bB = 128; break;//NAVY
    //END BASE 16 COLORS
    }


    switch (textColor)//base 16 colors
    {
    case   0:       r = 0; g = 0; b = 0; break;//BLACK
    case  white:    r = 255; g = 255; b = 255; break;//WHITE
    case  red:      r = 255; g = 0; b = 0; break;//RED
    case  lime:     r = 0; g = 255; b = 0; break;//LIME
    case  blue:     r = 0; g = 0; b = 255; break;//BLUE
    case  yellow:   r = 255; g = 255; b = 0; break;//YELLOW
    case  cyan:     r = 0; g = 255; b = 255; break;//CYAN/AQUA
    case  magenta:  r = 255; g = 0; b = 255; break;//MAGENTA/FUCHSIA
    case  grey:     r = 128; g = 128; b = 128; break;//GREY
    case  maroon:   r = 128; g = 0; b = 0; break;//MAROON
    case  olive:    r = 128; g = 128; b = 0; break;//OLIVE
    case  green:    r = 0; g = 128; b = 0; break;//GREEN
    case  purple:   r = 128; g = 0; b = 128; break;//PURPLE
    case  teal:     r = 0; g = 128; b = 128; break;//TEAL
    case  navy:     r = 0; g = 0; b = 128; break;//NAVY
    //END BASE 16 COLORS
    }

    vga.setFont(Font6x8);
    vga.setTextColor(vga.RGB(r, g, b), vga.RGB(rB, gB, bB));

    vga.setCursor(x, y);
    vga.print(inputData);
}

//void printNumberInt_7Seg(int number, int xStart, int yStart, int txt_color)
//{
//    string str = to_string(number);
//
//    static int num_Of_Chars = 0;
//
//    if (number < 10)                     num_Of_Chars = 1;
//    if (number > 10 && number < 100)     num_Of_Chars = 2;
//    if (number > 99 && number < 1000)    num_Of_Chars = 3;
//    if (number > 999 && number < 10000)  num_Of_Chars = 4;
//    if (number > 9999 && number < 32767) num_Of_Chars = 5;
//
//
//
//
//    for (int i = 0; i < num_Of_Chars; i++)
//    {
//        switch (str[i])
//        {
//        case '0':
//
//            for (int y = 0; y < 7; y++)
//            {
//                for (int x = 0; x < 5; x++)
//                {
//                    if (digit_0_S1[y][x])
//                    {
//                        setPixel(x + xStart, y + yStart, txt_color);
//                    }
//                }
//            }
//
//
//
//            break;
//
//        case '1':
//            for (int y = 0; y < 7; y++)
//            {
//                for (int x = 0; x < 5; x++)
//                {
//                    if (digit_1_S1[y][x])
//                    {
//                        setPixel(x + xStart, y + yStart, txt_color);
//                    }
//                }
//            }
//            break;
//
//        case '2':
//            for (int y = 0; y < 7; y++)
//            {
//                for (int x = 0; x < 5; x++)
//                {
//                    if (digit_2_S1[y][x])
//                    {
//                        setPixel(x + xStart, y + yStart, txt_color);
//                    }
//                }
//            }
//            break;
//
//        case '3':
//            for (int y = 0; y < 7; y++)
//            {
//                for (int x = 0; x < 5; x++)
//                {
//                    if (digit_3_S1[y][x])
//                    {
//                        setPixel(x + xStart, y + yStart, txt_color);
//                    }
//                }
//            }
//            break;
//
//        case '4':
//            for (int y = 0; y < 7; y++)
//            {
//                for (int x = 0; x < 5; x++)
//                {
//                    if (digit_4_S1[y][x])
//                    {
//                        setPixel(x + xStart, y + yStart, txt_color);
//                    }
//                }
//            }
//            break;
//
//        case '5':
//            for (int y = 0; y < 7; y++)
//            {
//                for (int x = 0; x < 5; x++)
//                {
//                    if (digit_5_S1[y][x])
//                    {
//                        setPixel(x + xStart, y + yStart, txt_color);
//                    }
//                }
//            }
//            break;
//
//        case '6':
//            for (int y = 0; y < 7; y++)
//            {
//                for (int x = 0; x < 5; x++)
//                {
//                    if (digit_6_S1[y][x])
//                    {
//                        setPixel(x + xStart, y + yStart, txt_color);
//                    }
//                }
//            }
//            break;
//
//        case '7':
//            for (int y = 0; y < 7; y++)
//            {
//                for (int x = 0; x < 5; x++)
//                {
//                    if (digit_7_S1[y][x])
//                    {
//                        setPixel(x + xStart, y + yStart, txt_color);
//                    }
//                }
//            }
//            break;
//
//        case '8':
//            for (int y = 0; y < 7; y++)
//            {
//                for (int x = 0; x < 5; x++)
//                {
//                    if (digit_8_S1[y][x])
//                    {
//                        setPixel(x + xStart, y + yStart, txt_color);
//                    }
//                }
//            }
//            break;
//
//        case '9':
//            for (int y = 0; y < 7; y++)
//            {
//                for (int x = 0; x < 5; x++)
//                {
//                    if (digit_9_S1[y][x])
//                    {
//                        setPixel(x + xStart, y + yStart, txt_color);
//                    }
//                }
//            }
//            break;
//
//        case '.':
//            Serial.print(".,");
//            break;
//
//
//        }
//        xStart += 6;
//    }
//    Serial.println();
//
//
//}
//


//ICONS
void byte_Data_Bar(int xPos, int yPos, byte data, byte bar_Color, byte back_Color)
{


    bool a = bitRead(data, 0);
    bool b = bitRead(data, 1);
    bool c = bitRead(data, 2);
    bool d = bitRead(data, 3);
    bool e = bitRead(data, 4);
    bool f = bitRead(data, 5);
    bool g = bitRead(data, 6);
    bool h = bitRead(data, 7);

    binary_Bar_8_Bit(xPos, yPos, h, g, f, e, d, c, b, a, bar_Color, back_Color);

}

void int_Data_Bar(int xPos, int yPos, int data, byte color)
{


    bool a = bitRead(data, 0);
    bool b = bitRead(data, 1);
    bool c = bitRead(data, 2);
    bool d = bitRead(data, 3);
    bool e = bitRead(data, 4);
    bool f = bitRead(data, 5);
    bool g = bitRead(data, 6);
    bool h = bitRead(data, 7);
    bool i = bitRead(data, 8);
    bool j = bitRead(data, 9);
    bool k = bitRead(data, 10);
    bool l = bitRead(data, 11);
    bool m = bitRead(data, 12);
    bool n = bitRead(data, 13);
    bool o = bitRead(data, 14);
    bool p = bitRead(data, 15);

    binary_Bar_8_Bit(xPos + 24, yPos, h, g, f, e, d, c, b, a, color, black);//msb
    binary_Bar_8_Bit(xPos, yPos, p, o, n, m, l, k, j, i, color, black);//lsb

}

void binary_Bar_8_Bit(int x_pos, int y_pos, bool A, bool C, bool D, bool E, bool F, bool G, bool H, bool I, byte bar_Color, byte b)
{

    int X;
    int Y;

    byte a;
    byte c;
    byte d;
    byte e;
    byte f;
    byte g;
    byte h;
    byte i;


    if (A) { a = bar_Color; }
    else { a = b; }
    if (C) { c = bar_Color; }
    else { c = b; }
    if (D) { d = bar_Color; }
    else { d = b; }
    if (E) { e = bar_Color; }
    else { e = b; }
    if (F) { f = bar_Color; }
    else { f = b; }
    if (G) { g = bar_Color; }
    else { g = b; }
    if (H) { h = bar_Color; }
    else { h = b; }
    if (I) { i = bar_Color; }
    else { i = b; }






    byte color;
    byte block_1[8][24] =
    {
        {a,a,b,c,c,b,d,d,b,e,e,b,f,f,b,g,g,b,h,h,b,i,i,b},
        {a,a,b,c,c,b,d,d,b,e,e,b,f,f,b,g,g,b,h,h,b,i,i,b},
        {a,a,b,c,c,b,d,d,b,e,e,b,f,f,b,g,g,b,h,h,b,i,i,b},
        {a,a,b,c,c,b,d,d,b,e,e,b,f,f,b,g,g,b,h,h,b,i,i,b},
        {a,a,b,c,c,b,d,d,b,e,e,b,f,f,b,g,g,b,h,h,b,i,i,b},
        {a,a,b,c,c,b,d,d,b,e,e,b,f,f,b,g,g,b,h,h,b,i,i,b},
        {a,a,b,c,c,b,d,d,b,e,e,b,f,f,b,g,g,b,h,h,b,i,i,b},
        {a,a,b,c,c,b,d,d,b,e,e,b,f,f,b,g,g,b,h,h,b,i,i,b}
    };

    for (int y = y_pos; y < y_pos + 8; y++)
    {
        for (int x = x_pos; x < x_pos + 24; x++)
        {
            color = block_1[Y][X];
            setPixel(x, y, color);
            X++;
        }
        X = 0;
        Y++;
    }
    Y = 0;
}

void binary_Counter_Bar(int x, int y, byte bar_Color, byte back_Color, byte number)
{




    bool NUMBER[8];
    for (int i = 0; i < 8; i++)
    {
        NUMBER[i] = bitRead(number, i);

    }

    bool a = NUMBER[7];
    bool b = NUMBER[6];
    bool c = NUMBER[5];
    bool d = NUMBER[4];
    bool e = NUMBER[3];
    bool f = NUMBER[2];
    bool g = NUMBER[1];
    bool h = NUMBER[0];

    binary_Bar_8_Bit(x, y, a, b, c, d, e, f, g, h, bar_Color, back_Color);


}

void FPS_Counter(unsigned long current_Time, byte back_Color)
{
    const byte average_Count_Sample = 40;

    static double old_Time;
    static byte counter;
    static byte print_Counter;
    static double update_Time;
    static double av_Counts[average_Count_Sample];
    static double av_Total = 0;
    static double time_Past;
    static double frame_Rate;


    time_Past = current_Time - old_Time;
    old_Time = current_Time;
    frame_Rate = time_Past / 1000; //store frame time in seconds
    //
    av_Counts[print_Counter] = frame_Rate;


    if (print_Counter >= average_Count_Sample) {

        for (int i = 0; i < average_Count_Sample; i++)//calc average fps
        {
            av_Total += av_Counts[i];
        }

        update_Time = av_Total / average_Count_Sample;
        print_Counter = 0;
        av_Total = 0;
        if (counter <= 255) { counter++; }
        else { counter = 0; }

    }
    //

    if (1 / update_Time < 10 || 1 / update_Time < 100)
    {
        drawSolidRectangle(ScreenWidth - 36, ScreenHeight - 8, 36, 8, back_Color);//clear behind fps counter if less than 2 or 3 didgits
    }

   // print("FPS", ScreenWidth - 60, ScreenHeight - 8, green, black);
   // printNumberInt(1 / update_Time, ScreenWidth - 42, ScreenHeight - 8, red, black);


    //

    binary_Counter_Bar(ScreenWidth - 24, ScreenHeight - 8, FPS_Bar_Color, black, counter);
    print_Counter++;
}

void draw_Axis_Dials(int x_Start,int y_Start,float X,float Y,float Z)
{
    int bin;






    drawSolidCircle(x_Start, y_Start, 12, black);
    print("N", x_Start - 2, y_Start - 10, yellow, black);
    print("S", x_Start - 1, y_Start + 4, yellow, black);
    print("E", x_Start + 5, y_Start - 3, yellow, black);
    print("W", x_Start - 9, y_Start - 3, yellow, black);
    drawHollowCircle(x_Start, y_Start, 12, red);
    draw_Vector(x_Start, y_Start, 10, X, 0, &bin, &bin, green);






    drawSolidCircle(x_Start, y_Start + 27, 12, black);
    print("N", x_Start - 2, (y_Start + 27) - 10, yellow, black);
    print("S", x_Start - 1, (y_Start + 27) + 4, yellow, black);
    print("E", x_Start + 5, (y_Start + 27) - 3, yellow, black);
    print("W", x_Start - 9, (y_Start + 27) - 3, yellow, black);
    drawHollowCircle(x_Start, y_Start + 27, 12, green);
    draw_Vector(x_Start, y_Start + 27, 10, Y, 0, &bin, &bin, green);







    drawSolidCircle(x_Start, y_Start + 54, 12, black);
    print("N", x_Start - 2, (y_Start + 54) - 10, yellow, black);
    print("S", x_Start - 1, (y_Start + 54) + 4, yellow, black);
    print("E", x_Start + 5, (y_Start + 54) - 3, yellow, black);
    print("W", x_Start - 9, (y_Start + 54) - 3, yellow, black);
    drawHollowCircle(x_Start, y_Start + 54, 12, blue);
    draw_Vector(x_Start, y_Start + 54, 10, Z, 0, &bin, &bin, green);
}

void draw_Vertical_Data_Bar(int x_Start, int y_Start, int range, int value, int xSize, int ySize)
{
    //map value to pixel space
    int bar_Max_Y = range;
    int bar_Height = ySize;
    int bar_Width = xSize;
    int maxY = map(value, 0, range, 1, bar_Height);
    int color;

    //draw bar
    for (int i = 0; i < maxY; i++)
    {
        //get color from a percentage of value
        if (i >= bar_Height * .8)                        color = red;    //above 90% of range
        if (i >  bar_Height * .6 && i < bar_Height * .8) color = yellow; //above 75%  and less than 90% of range
        if (i <= bar_Height * .6)                        color = green;  //under 75% of range

        for (int k = 0; k < bar_Width; k++)//fast pixel line
        {
            setPixel(x_Start + k, y_Start - i, color);
        }
    }
}

void vertical_16_Value_Bar_Grapgh(int xStart, int yStart, int range, int xSize, int ySize, int values[16])
{

    //DRAW VELOCITY BARS
    int velocity_Bars_xStart = xStart+1;
    int velocity_Bars_yStart = yStart + ySize;
    int velocity_Bars_X_Size = xSize;
    int velocity_Bars_Y_Size = ySize;
    int x_Step = velocity_Bars_X_Size + 1;

    int back_Xsize = (x_Step * 16)+1;
    int back_Ysize = ySize+2;
    drawSolidRectangle(xStart, yStart, back_Xsize, back_Ysize, black);
    

    draw_Vertical_Data_Bar(velocity_Bars_xStart, velocity_Bars_yStart,                 range, values[0],  velocity_Bars_X_Size, velocity_Bars_Y_Size);
    draw_Vertical_Data_Bar(velocity_Bars_xStart + x_Step, velocity_Bars_yStart,        range, values[1],  velocity_Bars_X_Size, velocity_Bars_Y_Size);
    draw_Vertical_Data_Bar(velocity_Bars_xStart + (x_Step * 2), velocity_Bars_yStart,  range, values[2],  velocity_Bars_X_Size, velocity_Bars_Y_Size);
    draw_Vertical_Data_Bar(velocity_Bars_xStart + (x_Step * 3), velocity_Bars_yStart,  range, values[3],  velocity_Bars_X_Size, velocity_Bars_Y_Size);
    draw_Vertical_Data_Bar(velocity_Bars_xStart + (x_Step * 4), velocity_Bars_yStart,  range, values[4],  velocity_Bars_X_Size, velocity_Bars_Y_Size);
    draw_Vertical_Data_Bar(velocity_Bars_xStart + (x_Step * 5), velocity_Bars_yStart,  range, values[5],  velocity_Bars_X_Size, velocity_Bars_Y_Size);
    draw_Vertical_Data_Bar(velocity_Bars_xStart + (x_Step * 6), velocity_Bars_yStart,  range, values[6],  velocity_Bars_X_Size, velocity_Bars_Y_Size);
    draw_Vertical_Data_Bar(velocity_Bars_xStart + (x_Step * 7), velocity_Bars_yStart,  range, values[7],  velocity_Bars_X_Size, velocity_Bars_Y_Size);
    draw_Vertical_Data_Bar(velocity_Bars_xStart + (x_Step * 8), velocity_Bars_yStart,  range, values[8],  velocity_Bars_X_Size, velocity_Bars_Y_Size);
    draw_Vertical_Data_Bar(velocity_Bars_xStart + (x_Step * 9), velocity_Bars_yStart,  range, values[9],  velocity_Bars_X_Size, velocity_Bars_Y_Size);
    draw_Vertical_Data_Bar(velocity_Bars_xStart + (x_Step * 10), velocity_Bars_yStart, range, values[10], velocity_Bars_X_Size, velocity_Bars_Y_Size);
    draw_Vertical_Data_Bar(velocity_Bars_xStart + (x_Step * 11), velocity_Bars_yStart, range, values[11], velocity_Bars_X_Size, velocity_Bars_Y_Size);
    draw_Vertical_Data_Bar(velocity_Bars_xStart + (x_Step * 12), velocity_Bars_yStart, range, values[12], velocity_Bars_X_Size, velocity_Bars_Y_Size);
    draw_Vertical_Data_Bar(velocity_Bars_xStart + (x_Step * 13), velocity_Bars_yStart, range, values[13], velocity_Bars_X_Size, velocity_Bars_Y_Size);
    draw_Vertical_Data_Bar(velocity_Bars_xStart + (x_Step * 14), velocity_Bars_yStart, range, values[14], velocity_Bars_X_Size, velocity_Bars_Y_Size);
    draw_Vertical_Data_Bar(velocity_Bars_xStart + (x_Step * 15), velocity_Bars_yStart, range, values[15], velocity_Bars_X_Size, velocity_Bars_Y_Size);




}

void vertical_Slide_Bar(slide_Bar_Int& bar, float rangeMax, float* output, bool* en_Out, int start_pos)
{
    static int Boarder_Color = bar.boarder_Color;
    static int bar_Pos = start_pos;
    static bool scroll_Enable = false;
    int y_Start = (bar.yStart + 53) - 3;
    int bar_Max = 10;//0-20
    int bar_Graph_Pos = map(bar_Pos, 0, 100, 1, 20);
    int slide_Pos_Min = bar.yStart + 48;
    int slide_Pos_Max = bar.yStart + 11;
    int slide_Pos = map(bar_Pos, 0, 100, slide_Pos_Min, slide_Pos_Max);

    //background
    drawHollowRectangle(bar.xStart, bar.yStart, 20, 53, Boarder_Color);
    drawSolidRectangle(bar.xStart + 1, bar.yStart + 1, 18, 51, bar.back_Color);

    //slider
    drawLine(bar.xStart + 14, bar.yStart + 12, bar.xStart + 14, bar.yStart + 51, white);
    drawLine(bar.xStart + 13, bar.yStart + 12, bar.xStart + 13, bar.yStart + 51, white);
    drawSolidRectangle(bar.xStart + 10, slide_Pos, 8, 3, bar.slide_Color);

    //pos
    printNumberInt(bar_Pos, bar.xStart + 1, bar.yStart + 1, window_Bar_Text_Color, bar.back_Color);

    if (mouse_Middle_Make && check_Button_Mouse(bar.xStart, bar.yStart, 20, 53, mouseX, mouseY) && !scroll_Enable)
    {
        scroll_Enable = true;
        mouse_Middle_Make = false;
        Boarder_Color = green;
        beep(200, 100);
    }

    if (scroll_Enable)
    {
        if (mouse_Scroll_Up)
        {
            if (bar_Pos < 100) bar_Pos++;
            else beep(1000, 20);
            mouse_Scroll_Up = false;
        }

        if (mouse_Scroll_Down)
        {
            if (bar_Pos > 0) bar_Pos--;
            else beep(100, 20);
            mouse_Scroll_Down = false;
        }
    }

   
    if (mouse_Middle_Make && check_Button_Mouse(bar.xStart, bar.yStart, 20, 53, mouseX, mouseY))
    {
        scroll_Enable = false;
        mouse_Middle_Make = false;
        Boarder_Color = bar.boarder_Color;
        beep(50, 100);
    }

    //DRAW BAR GRAPH
    int bar_Graph_Color = green;
    for (int i = 0; i < bar_Graph_Pos; i++)
    {
        if (i >= 12 && i <= 16) bar_Graph_Color = yellow;
        if (i >= 17 && i <= 20) bar_Graph_Color = red;
        drawSolidRectangle(bar.xStart + 2, y_Start, 6, 1, bar_Graph_Color);
        y_Start -= 2;
    }


    //CLICK AND DRAG SLIDER
    static bool slide_Drag_En;
    static int current_Slide_Mouse_Y;

    if (scroll_Enable)
    {
        if (mouse_Left_Make && check_Button_Mouse(bar.xStart + 10, slide_Pos_Max, 8, 40, mouseX, mouseY))
        {
            slide_Drag_En = true;
            Cursor_Enable_X = false;
            current_Slide_Mouse_Y = slide_Pos_Min - (int)mouseY;
            bar_Pos = map(current_Slide_Mouse_Y, 0, 36, 0, 100);
        }

        if (slide_Drag_En)
        {
            if (mouseY < slide_Pos_Max) mouseY = slide_Pos_Max;
            if (mouseY > slide_Pos_Min) mouseY = slide_Pos_Min;
        }

        if (mouse_Left_Break)
        {
            Cursor_Enable_X = true;
            mouse_Left_Break = false;
            slide_Drag_En = false;
        }
    }
    float bar_Pos_Norm;
    m.normalize_Range_To_0_1(bar_Pos, 0, 100, &bar_Pos_Norm);
    *output = rangeMax * bar_Pos_Norm;
    *en_Out = scroll_Enable;
}


//SYSTEM VGA FUNCTIONS##########################################################################

void refreshScreen()
{
    vga.show();//MUST CALL AFTER EVERY FRAME LOAD  
}

byte readVGAscreenBufferCell(int x,int y,bool debugPrint)
{
    static byte cellColor;//rgb value 0-256.
    unsigned long int mCellData = vga.get(x, y);//read rgb data directly from the screen buffer.

    if (debugPrint == true && serialDebugEnabledFlag == false) { serialDebugEnabledFlag = true; }
    if (debugPrint) { Serial.print("CELL RGB DATA = "); Serial.print(mCellData); }
    //
    switch (mCellData)//convert to single byte per cell.
    {
    case 0:      cellColor = 0; break;//black
    case 63:     cellColor = 1; break;//white
    case 3:      cellColor = 2; break;//red
    case 12:     cellColor = 3; break;//lime
    case 48:     cellColor = 4; break;//blue
    case 15:     cellColor = 5; break;//yellow
    case 60:     cellColor = 6; break;//cyan
    case 51:     cellColor = 7; break;//magenta
    case 42:     cellColor = 8; break;//grey
    case 2:      cellColor = 9; break;//maroon
    case 10:     cellColor = 10; break;//olive
    case 8:      cellColor = 11; break;//green
    case 34:     cellColor = 12; break;//purple
    case 40:     cellColor = 13; break;//teal
    case 32:     cellColor = 14; break;//navy
    }
    //
    if (debugPrint) { Serial.print("    CELL COLOR REF # = "); Serial.print(cellColor); Serial.println(); }
    if (debugPrint == false && serialDebugEnabledFlag == true) { serialDebugEnabledFlag = false; }
    return cellColor;//return cell contents as a color ref 0-255.
}

void draw_Color_Map(bool run)
{
    int box_Size_X = 30;
    int box_Size_Y = 30;
    int XP;
    int YP;
    int color = 16;
    int space = 1;
    int xStart = 10;
    int yStart = 12;


    if (run)
    {
        for (int Y = 0; Y < 4; Y++)//DRAW CUBE MAP
        {
            for (int X = 0; X < 8; X++)
            {

                drawSolidRectangle(XP + xStart, YP + yStart, box_Size_X, box_Size_Y,color);
                printNumberInt(color, XP + xStart, YP + yStart, black, grey);
                XP += space + box_Size_X;
                color++;
            }
            YP += space + box_Size_Y;
            XP = xStart;
        }
        YP = yStart;
    }

}

void draw_Color_Map_Shaded(bool run)
{
    int box_Size_X = 25;
    int box_Size_Y = 25;
    int XP;
    int YP;
    int color;
    int space = 1;
    int xStart = 10;

    int yStart = 12;



    //violet 19



    if (run)
    {
        for (int y = 0; y < 7; y++)//DRAW CUBE MAP
        {
            for (int x = 0; x < 4; x++)
            {

                drawSolidRectangle(XP + xStart, YP + yStart, box_Size_X, box_Size_Y, get_Color_Shade(y,x));
                printNumberInt(get_Color_Shade(y, x), XP + xStart, YP + yStart, black, grey);
                XP += space + box_Size_X;
            }
            YP += space + box_Size_Y;
            XP = xStart;
        }
        YP = yStart;
    }


}

















///##############################################################################################################
///                                               VERTEX PIPELINE                 
///##############################################################################################################


void vertex_Rastor_Pipeline
(
    int xStart,
    int yStart,
    int screen_Width,
    int screen_Height,
    float cameraZ,
    float FOV,
    int projection_Type,
    int space_Color,
    int *v_Count,
    int *z_Cliped,
    vector <vertex3D> & world_Vertex_Buffer
)
{
    vector<vertex3D> screen_Space_pixel_Buffer;   
    static bool pixel_Bypass_Enable         = true;
    static bool pixel_Bypass_Z_Clip_Enable  = true;
    static bool line_Decoder_Enable         = true;
    float near_Plane = global_Near_Plane;
    float far_Plane = global_Far_Plane;
                                                           
        translate_XY_Veiw_Plane_To_Screen_Plane             
        (                                                   
            xStart,
            yStart,
            screen_Width,
            screen_Height,
            FOV,
            near_Plane,
            far_Plane,
            cameraZ,
            projection_Type,
            space_Color,
            &*v_Count,
            world_Vertex_Buffer,
            screen_Space_pixel_Buffer
        );
                                                       



        line_Decoder                                        
        (                                                    
            line_Decoder_Enable,                           
            screen_Width,                         
            screen_Height,
            xStart,
            yStart,
            screen_Space_pixel_Buffer
        );


                                                          
        pixel_Bypass                                     
        (
            pixel_Bypass_Enable,
            pixel_Bypass_Z_Clip_Enable,
            &*z_Cliped,
            screen_Width,                     
            screen_Height,
            xStart,
            yStart,
            screen_Space_pixel_Buffer
        );

        screen_Space_pixel_Buffer.clear();
    
}

void translate_XY_Veiw_Plane_To_Screen_Plane
(
    int xStart,
    int yStart,
    int window_Width,
    int window_Height, 
    float FOV,
    float z_Near,
    float z_Far,
    float camera_Z,
    int projection_Type,
    byte space_Color,
    int* vertex_Counter,
    vector <vertex3D>& to_Be_Translated_Buffer, 
    vector <vertex3D>& Translated_Buffer
)

{



    int v_Count = 0;
    float fov = m.deg_To_Rad(FOV);
    float camera_Plane_X = tan(fov / 2) * z_Far;
  

    //DEFINE WORLD VIEW PLANE
    //
    float vp1_X = camera_Plane_X * -1;
    float vp1_Y = camera_Plane_X * -1;
    //
    float vp2_X = camera_Plane_X;
    float vp2_Y = camera_Plane_X;
    //


    //DEFINE SCREEN PLANE
    float sp1_X = xStart;
    float sp1_Y = yStart;
    //
    float sp2_X = xStart + window_Width;
    float sp2_Y = yStart + window_Height;
    //



    float vertex[3];//TEMP BUFFER TO HOLD WORLD VERTEX TO BE TRANSLATED TO SCREEN SPACE
    int XN;//TRANSLATED SCREEN SPACE VERTEX
    int YN;

  

    //DRAW WORLD VOID COLOR BACKGROUND
   // drawSolidRectangle(sp1_X, sp1_Y, window_Width, window_Height, space_Color);

    //GO THROUGH BUFFER OF VERTICIES TRANSLATING FROM WORLD SPACE TO SCREEN SPACE
    for (int i = 0; i != to_Be_Translated_Buffer.size(); i++)
    {
        vertex3D vertex_Vi = to_Be_Translated_Buffer[i];
        vertex3D vertex_Vi_Translated;
        float temp[3];

        temp[0] = vertex_Vi.x;
        temp[1] = vertex_Vi.y;
        temp[2] = vertex_Vi.z;
        
        m.world_plane_To_Screen_Plane_Vertex_Translate
        (
            vp1_X,
            vp1_Y,
            vp2_X,
            vp2_Y,
            sp1_X,
            sp1_Y,
            sp2_X,
            sp2_Y,
            temp,
            projection_Type,
            window_Height,
            window_Width,
            z_Far,
            z_Near,
            FOV,
            &XN,
            &YN
        );

        vertex_Vi_Translated.x = (float)XN;
        vertex_Vi_Translated.y = (float)YN;
        vertex_Vi_Translated.z = 0;
        vertex_Vi_Translated.u = vertex_Vi.u;
        vertex_Vi_Translated.v = vertex_Vi.v;
        vertex_Vi_Translated.c = vertex_Vi.c;
        vertex_Vi_Translated.id = vertex_Vi.id;
        vertex_Vi_Translated.mesh_Type = vertex_Vi.mesh_Type;
        if (XN > 0 && XN < 320 && YN > 0 && YN < 240)
        {
            Translated_Buffer.push_back(vertex_Vi_Translated);
            v_Count++;
        }
       
    }
    *vertex_Counter = v_Count;
};

void pixel_Bypass
(
    bool enable,
    bool z_Clip,
    int *z_Cliped,
    int window_Clip_Size_X,
    int window_Clip_Size_Y,
    int window_Start_X,
    int window_Start_Y,
    vector<vertex3D> &buffer
)
{
    float x_Old = 0;
    float y_Old = 0;
    float z_Old = 0;
    int pc = 0;
    if (enable)
    {
        for (int i = 0; i != buffer.size(); i++)
        {
            vertex3D Vi = buffer[i];
            if (z_Clip)
            {
                if (Vi.x == x_Old && Vi.y == y_Old)
                {
                    if (Vi.z > z_Old)
                    {
                        //CLIP PIXELS AROUND SCREEN WINDOW
                        if (Vi.x > window_Start_X && Vi.x < window_Clip_Size_X + (window_Start_X-1) && Vi.y > window_Start_Y && Vi.y < window_Clip_Size_Y + window_Start_Y)
                        {
                            setPixel(Vi.x, Vi.y, Vi.c);//send to vga back buffer
                            z_Old = Vi.z;
                            pc++;
                        }
                    }
                }
                else
                {
                    //CLIP PIXELS AROUND SCREEN WINDOW
                    if (Vi.x > window_Start_X && Vi.x < window_Clip_Size_X + (window_Start_X-1) && Vi.y > window_Start_Y && Vi.y < window_Clip_Size_Y + window_Start_Y)
                    {
                        setPixel(Vi.x, Vi.y, Vi.c);//send to vga back buffer
                        x_Old = Vi.x,
                        y_Old = Vi.y;
                        z_Old = Vi.z;
                        pc++;
                    }
                }
            }
            else
            {
                if (Vi.x > window_Start_X && Vi.x < window_Clip_Size_X + (window_Start_X - 1) && Vi.y > window_Start_Y && Vi.y < window_Clip_Size_Y + window_Start_Y)
                    setPixel(Vi.x, Vi.y, Vi.c);//send to vga back buffer
            }
        }
        *z_Cliped = pc;
    }
}

void line_Decoder
(
    bool enable,
    int window_Clip_Size_X,
    int window_Clip_Size_Y,
    int xStart,
    int yStart,
    vector<vertex3D>& screen_Space_pixel_Buffer
)

{
    static int p1x;
    static int p1y;
    static int p2x;
    static int p2y;
    int line_Color;
    if (enable)
    {
        for (int Vi = 0; Vi != screen_Space_pixel_Buffer.size(); Vi++)
        {
            vertex3D vertex_a = screen_Space_pixel_Buffer[Vi];
            vertex3D vertex_b = screen_Space_pixel_Buffer[Vi + 1];

            if (vertex_a.mesh_Type == 1 && vertex_b.mesh_Type == 1 && vertex_a.id == vertex_b.id)
            {
                p1x = (int)vertex_a.x;
                p1y = (int)vertex_a.y;
                line_Color = vertex_a.c;

                p2x = (int)vertex_b.x;
                p2y = (int)vertex_b.y;
                DDA_Line_Clip(p1x, p1y, p2x, p2y, line_Color, xStart, yStart, window_Clip_Size_X , window_Clip_Size_Y);
            }
        }
    }
}

void write_Origin_Cross_Mesh_To_Vector
(
    vector<vertex3D>& mesh,
    float  origin_Cross_Size,
    bool full_Cross,
    int *vc
)


{
    vertex3D vi; 
    int vertex_Counter = 0;
    float origin_Cross_Step_Size = .5;
    if (origin_Cross_Size > 50) origin_Cross_Size = 50;

    //PLACE ORIGIN VECTOR VERTICIES INTO WORLD SPACE
    for (float i = 0; i < origin_Cross_Size; i += origin_Cross_Step_Size)
    {
        vi.x = i;
        vi.y = 0;
        vi.z = 0;
        vi.c = red;
        vi.mesh_Type = 0;
        vi.id = 0;
        if (i == origin_Cross_Size - origin_Cross_Step_Size) vi.c = purple;
        mesh.push_back(vi);
        vertex_Counter++;
    }

    for (float i = 0; i < origin_Cross_Size; i += origin_Cross_Step_Size)
    {
        vi.x = 0;
        vi.y = i;
        vi.z = 0;
        vi.c = green;
        vi.mesh_Type = 0;
        vi.id = 0;
        if (i == origin_Cross_Size - origin_Cross_Step_Size) vi.c = purple;
        mesh.push_back(vi);
        vertex_Counter++;
    }

    for (float i = 0; i < origin_Cross_Size; i += origin_Cross_Step_Size)
    {
        vi.x = 0;
        vi.y = 0;
        vi.z = i;
        vi.c = blue;
        vi.mesh_Type = 0;
        vi.id = 0;
        if (i == origin_Cross_Size - origin_Cross_Step_Size) vi.c = purple;
        mesh.push_back(vi);
        vertex_Counter++;
    }
    //////////////////////////////////////////////

    if (full_Cross)
    {
        for (float i = 0; i < origin_Cross_Size; i += origin_Cross_Step_Size)
        {
            vi.x = i*-1;
            vi.y = 0*-1;
            vi.z = 0*-1;
            vi.c = red;
            vi.mesh_Type = 0;
            vi.id = 0;
            if (i == origin_Cross_Size - 1) vi.c = purple;
            mesh.push_back(vi);
            vertex_Counter++;
        }

        for (float i = 0; i < origin_Cross_Size; i += origin_Cross_Step_Size)
        {
            vi.x = 0*-1;
            vi.y = i*-1;
            vi.z = 0*-1;
            vi.c = green;
            vi.mesh_Type = 0;
            vi.id = 0;
            if (i == origin_Cross_Size - 1) vi.c = purple;
            mesh.push_back(vi);
            vertex_Counter++;
        }

        for (float i = 0; i < origin_Cross_Size; i += origin_Cross_Step_Size)
        {
            vi.x = 0*-1;
            vi.y = 0*-1;
            vi.z = i*-1;
            vi.c = blue;
            vi.mesh_Type = 0;
            vi.id = 0;
            if (i == origin_Cross_Size - 1) vi.c = purple;
            mesh.push_back(vi);
            vertex_Counter++;
        }
        //////////////////////////////////////////////
    }

    *vc = vertex_Counter;



}

void write_Grid_Plane_To_Vector_XZ
(
    float paX,
    float paZ,
    float pbX,
    float pbZ,
    float pcX,
    float pcZ,
    float pdX,
    float pdZ,
    float y_Offset,
    int n_Lines,
    byte color,
    bool axis_Highlight,
    int start_Id,
    int* end_Id,
    vector<vertex3D>& buffer
)

    {
  

  

        //Z GRID LINES
        //float n_Lines = 10;
        float grid_Width = pbX - paX;
        float grid_Height = pcZ - paZ;
        float grid_Space_Height = grid_Height / (n_Lines - 1);
        float grid_Space = grid_Width / (n_Lines-1);
        int id = start_Id;
       // bool axis_Highlight = false;

    
        for (int i = 0; i < n_Lines; i++)
        {
            if (axis_Highlight)
            {
                if (i == 0 || i == (n_Lines-1) / 2 || i == n_Lines-1) color = blue;
                else color = grey;
            }




            vertex3D gn1, gn2;

            gn1.x = paX + (grid_Space * i);
            gn1.z = paZ;
            gn1.id = id;
            gn1.mesh_Type = 1;
            gn1.c = color;
            gn1.y = y_Offset;
            //
            gn2.x = paX + (grid_Space * i);
            gn2.z = pcZ;
            gn2.id = id;
            gn2.mesh_Type = 1;
            gn2.c = color;
            gn2.y = y_Offset;

            buffer.push_back(gn1);
            buffer.push_back(gn2);
            id++;
        }


      
        for (int i = 0; i < n_Lines; i++)
        {
            if (axis_Highlight)
            {
                if (i == 0 || i == (n_Lines-1) / 2 || i == n_Lines-1) color = red;
                else color = grey;
            }
            vertex3D gn3, gn4;

            gn3.z = paZ + (grid_Space_Height * i);
            gn3.x = paX;
            gn3.id = id;
            gn3.mesh_Type = 1;
            gn3.c = color;
            gn3.y = y_Offset;
            //
            gn4.z = pbZ + (grid_Space_Height * i);
            gn4.x = pbX;
            gn4.id = id;
            gn4.mesh_Type = 1;
            gn4.c = color;
            gn4.y = y_Offset;
            id++;
            buffer.push_back(gn3);
            buffer.push_back(gn4);
        }
        *end_Id = id;
    }

void write_Grid_Plane_To_Vector_XY
(
    float paX, 
    float paY, 
    float pbX, 
    float pbY, 
    float pcX, 
    float pcY, 
    float pdX,
    float pdY, 
    float Z_Offset, 
    int n_Lines, 
    byte color,
    bool axis_Highlight,
    int start_Id,
    int* end_Id,
    vector<vertex3D>& buffer
)
{


    
    //float n_Lines = 10;
    float grid_Width = pbX - paX;
    float grid_Height = pcY - paY;
    float grid_Space_Height = grid_Height / (n_Lines - 1);
    float grid_Space = grid_Width / (n_Lines - 1);
    int id = start_Id;
    


    
    for (int i = 0; i < n_Lines; i++)
    {
        if (axis_Highlight)
        {
            if (i == 0 || i == (n_Lines - 1) / 2 || i == n_Lines - 1) color = green;
            else color = grey;
        }
        vertex3D gn1, gn2;

        gn1.x = paX + (grid_Space * i);
        gn1.y = paY;
        gn1.id = id;
        gn1.mesh_Type = 1;
        gn1.c = color;
        gn1.z = Z_Offset;
        //
        gn2.x = paX + (grid_Space * i);
        gn2.y = pcY;
        gn2.id = id;
        gn2.mesh_Type = 1;
        gn2.c = color;
        gn2.z = Z_Offset;

        buffer.push_back(gn1);
        buffer.push_back(gn2);
        id++;
    }


 
    for (int i = 0; i < n_Lines; i++)
    {
        if (axis_Highlight)
        {
            if (i == 0 || i == (n_Lines - 1) / 2 || i == n_Lines - 1) color = red;
            else color = grey;
        }
        vertex3D gn3, gn4;

        gn3.y = paY + (grid_Space_Height * i);
        gn3.x = paX;
        gn3.id = id;
        gn3.mesh_Type = 1;
        gn3.c = color;
        gn3.z = Z_Offset;
        //
        gn4.y = pbY + (grid_Space_Height * i);
        gn4.x = pbX;
        gn4.id = id;
        gn4.mesh_Type = 1;
        gn4.c = color;
        gn4.z = Z_Offset;
        id++;
        buffer.push_back(gn3);
        buffer.push_back(gn4);
    }
    *end_Id = id;
}

void write_Grid_Plane_To_Vector_ZY
(
    float paZ, 
    float paY,
    float pbZ,
    float pbY,
    float pcZ, 
    float pcY, 
    float pdZ,
    float pdY,
    float X_Offset,
    int n_Lines,
    byte color,
    bool axis_Highlight,
    int start_Id,
    int* end_Id,
    vector<vertex3D>& buffer
)

{



    //float n_Lines = 10;
    float grid_Width = pbZ - paZ;
    float grid_Height = pcY - paY;
    float grid_Space_Height = grid_Height / (n_Lines - 1);
    float grid_Space = grid_Width / (n_Lines - 1);
    int id = start_Id;
    

    
    for (int i = 0; i < n_Lines; i++)
    {
        if (axis_Highlight)
        {
            if (i == 0 || i == (n_Lines - 1) / 2 || i == n_Lines - 1) color = green;
            else color = grey;
        }
        vertex3D gn1, gn2;

        gn1.z = paZ + (grid_Space * i);
        gn1.y = paY;
        gn1.id = id;
        gn1.mesh_Type = 1;
        gn1.c = color;
        gn1.x = X_Offset;
        //
        gn2.z = paZ + (grid_Space * i);
        gn2.y = pcY;
        gn2.id = id;
        gn2.mesh_Type = 1;
        gn2.c = color;
        gn2.x = X_Offset;

        buffer.push_back(gn1);
        buffer.push_back(gn2);
        id++;
    }

  
    for (int i = 0; i < n_Lines; i++)
    {
        if (axis_Highlight)
        {
            if (i == 0 || i == (n_Lines - 1) / 2 || i == n_Lines - 1) color = blue;
            else color = grey;
        }
        vertex3D gn3, gn4;

        gn3.y = paY + (grid_Space_Height * i);
        gn3.z = paZ;
        gn3.id = id;
        gn3.mesh_Type = 1;
        gn3.c = color;
        gn3.x = X_Offset;
        //
        gn4.y = pbY + (grid_Space_Height * i);
        gn4.z = pbZ;
        gn4.id = id;
        gn4.mesh_Type = 1;
        gn4.c = color;
        gn4.x = X_Offset;
        id++;
        buffer.push_back(gn3);
        buffer.push_back(gn4);
    }
    *end_Id;
}

void rotate_Mesh
(
    vector<vertex3D>& buffer, 
    int axis,
    float angle_Delta
)

{


    for (int i = 0; i != buffer.size(); i++)
    {
        vertex3D p = buffer[i];
        float xyz[3];
        xyz[0] = p.x;
        xyz[1] = p.y;
        xyz[2] = p.z;
       if(axis == 0) m.x_Rotation_Matrix(angle_Delta, xyz);
       if(axis == 1) m.y_Rotation_Matrix(angle_Delta, xyz);
       if(axis == 2) m.z_Rotation_Matrix(angle_Delta, xyz);
        p.x = xyz[0];
        p.y = xyz[1];
        p.z = xyz[2];
        buffer[i] = p;
    }
}


void write_Clusteter_Cube_Mesh_To_Vector
(
    int Q,
    float cube_Size,
    float Xo,
    float Yo,
    float Zo,
    int fill_Type,
    vector<vertex3D>& buffer
)

{
    vertex3D vertex_a;
    vertex3D vertex_b;
   

    //X Z PLANE
    for (float Vz = 0; Vz < cube_Size/2; Vz++)
    {     
            for (float Vx = 0; Vx < cube_Size / 2; Vx++)
            {              
                    vertex_a.x = Vx +Xo;
                    vertex_a.z = Vz +Zo;
                    vertex_a.y = Yo;
                    vertex_a.c = red;
                    //
                    vertex_b.x = Vx * -1 +Xo;
                    vertex_b.z = Vz + Zo;
                    vertex_b.y = Yo;
                    vertex_b.c = red;
               

                    //
                    buffer.push_back(vertex_a);
                    buffer.push_back(vertex_b);              
            }

            for (float Vx = 0; Vx < cube_Size / 2; Vx++)
            {            
                    vertex_a.x = Vx+Xo;
                    vertex_a.z = Vz * -1+Zo;
                    vertex_a.y = Yo;
                    vertex_a.c = red;
                    //
                    vertex_b.x = Vx * -1+Xo;
                    vertex_b.z = Vz * -1+Zo;
                    vertex_b.y = Yo;
                    vertex_b.c = red;
                    //
                    buffer.push_back(vertex_a);
                    buffer.push_back(vertex_b);               
            }
    }




    //X Y PLANE
    for (float Vy = 0; Vy < cube_Size / 2; Vy++)
    {
        for (float Vx = 0; Vx < cube_Size / 2; Vx++)
        {
            vertex_a.x = Vx + Xo;
            vertex_a.z = Zo;
            vertex_a.y = Vy + Yo;
            vertex_a.c = green;
            //
            vertex_b.x = Vx * -1 + Xo;
            vertex_b.z = Zo;
            vertex_b.y = Vy + Yo;
            vertex_b.c = green;
            //
            buffer.push_back(vertex_a);
            buffer.push_back(vertex_b);
        }

        for (float Vx = 0; Vx < cube_Size / 2; Vx++)
        {
            vertex_a.x = Vx + Xo;
            vertex_a.z = Zo;
            vertex_a.y = Vy*-1 + Yo;
            vertex_a.c = green;
            //
            vertex_b.x = Vx * -1 + Xo;
            vertex_b.z = Zo;
            vertex_b.y = Vy*-1 + Yo;
            vertex_b.c = green;
            //
            buffer.push_back(vertex_a);
            buffer.push_back(vertex_b);
        }
    }









}

void line_Gen_3D
(
    float p1x,
    float p1y,
    float p1z,
    float p2x,
    float p2y,
    float p2z,
    int color,
    int id,
    vector<vertex3D>& world_Space_Vertex
)

{
    int line_Id = id;
    //MANUALY CREATE LINE
    vertex3D v1,v2;
  

    v1.mesh_Type = 1;
    v2.mesh_Type = 1;
    v1.id = line_Id;
    v2.id = line_Id;
    v1.c = color;
    v2.c = color;
    
    v1.x = p1x;
    v1.y = p1y;
    v1.z = p1z;
    
    v2.x = p2x;
    v2.y = p2y;
    v2.z = p2z;


    //PUSH LINE INTO LINE BUFFER
    world_Space_Vertex.push_back(v1);
    world_Space_Vertex.push_back(v2);
}



void rotate_World_Vertex
(
    float x_Angle_Delta,
    float y_Angle_Delta,
    float z_Angle_Delta,
    vector <vertex3D>& buff
)

{
    rotate_Mesh(buff, 0, x_Angle_Delta);
    rotate_Mesh(buff, 1, y_Angle_Delta);
    rotate_Mesh(buff, 2, z_Angle_Delta);
}



void translate_Move_Vertex_Buffer(float xPos, float yPos, float zPos, vector<vertex3D>& vertex_Buffer)
{

    for (int i = 0; i != vertex_Buffer.size(); i++)
    {
        vertex3D pi = vertex_Buffer[i];
        float temp_P1[3];
  
        temp_P1[0] = pi.x;
        temp_P1[1] = pi.y;
        temp_P1[2] = pi.z;

        m.translate_Matrix(xPos, yPos, zPos, temp_P1);
     
        pi.x = temp_P1[0];
        pi.y = temp_P1[1];
        pi.z = temp_P1[2];
       


        vertex_Buffer[i] = pi;
    }

}




 

















//#################################################################################################################
//                                             TRIANGLE PIPE LINE
//#################################################################################################################


void translate_Triangle_Buffer_To_Pixel_Buffer
(
    int xStart,
    int yStart,
    int window_Width,
    int window_Height,
    float FOV,
    float z_Near,
    float z_Far,
    float camera_Z,
    int projection_Type,
    bool back_Face_Cliping_Enable,
    vector<triangle>& triangle_Buffer,
    vector<triangle>& pixel_Buffer
)

{
    static bool debug = false;
    float fov = m.deg_To_Rad(FOV);
    float camera_Plane_X = 2 * ((tan(fov / 2)) * z_Far);

    int v_Count = 0;




    //DEFINE WORLD VIEW PLANE
    //
    float vp1_X = (camera_Plane_X / 2) * -1;
    float vp1_Y = (camera_Plane_X / 2) * -1;
    //
    float vp2_X = camera_Plane_X / 2;
    float vp2_Y = camera_Plane_X / 2;
    //
    float view_Plane_Width = (vp2_X - vp1_X) / 2;

    //DEFINE SCREEN PLANE
    float sp1_X = xStart;
    float sp1_Y = yStart;
    //
    float sp2_X = xStart + window_Width;
    float sp2_Y = yStart + window_Height;
    //

 

    float vertex[3];//TEMP BUFFER TO HOLD WORLD VERTEX TO BE TRANSLATED TO SCREEN SPACE
    int Tx1;//TRANSLATED SCREEN SPACE VERTEX
    int Ty1;
    int Tx2;//TRANSLATED SCREEN SPACE VERTEX
    int Ty2;
    int Tx3;//TRANSLATED SCREEN SPACE VERTEX
    int Ty3;


    //CLIP ALL -NORMAL TRIANGLES







    //LOOP THROUGH TRIANGLE BUFFER TRANSLATING TO PIXEL SPACE
    for (int i = 0; i != triangle_Buffer.size(); i++)
    {
            //CREATE 3 TEMP VERTICIES AND A TRIANGLE FOR TRANSLATING 
            triangle Ti = triangle_Buffer[i];
            triangle Tt;

            float vertex_To_Translate_1[3];
            float vertex_To_Translate_2[3];
            float vertex_To_Translate_3[3];

  

            //TRANSLATE P0
            vertex_To_Translate_1[0] = Ti.p1.x;
            vertex_To_Translate_1[1] = Ti.p1.y;
            vertex_To_Translate_1[2] = Ti.p1.z;
            m.world_plane_To_Screen_Plane_Vertex_Translate
            (
                vp1_X,
                vp1_Y,
                vp2_X,
                vp2_Y,
                sp1_X,
                sp1_Y,
                sp2_X,
                sp2_Y,
                vertex_To_Translate_1,
                projection_Type,
                window_Height,
                window_Width,
                z_Far,
                z_Near,
                FOV,
                &Tx1,
                &Ty1
            );

            //TRANSLATE P1
            vertex_To_Translate_2[0] = Ti.p2.x;
            vertex_To_Translate_2[1] = Ti.p2.y;
            vertex_To_Translate_2[2] = Ti.p2.z;
            m.world_plane_To_Screen_Plane_Vertex_Translate
            (
                vp1_X,
                vp1_Y,
                vp2_X,
                vp2_Y,
                sp1_X,
                sp1_Y,
                sp2_X,
                sp2_Y,
                vertex_To_Translate_2,
                projection_Type,
                window_Height,
                window_Width,
                z_Far,
                z_Near,
                FOV,
                &Tx2,
                &Ty2
            );

            //TRANSLATE P2
            vertex_To_Translate_3[0] = Ti.p3.x;
            vertex_To_Translate_3[1] = Ti.p3.y;
            vertex_To_Translate_3[2] = Ti.p3.z;
            m.world_plane_To_Screen_Plane_Vertex_Translate
            (
                vp1_X,
                vp1_Y,
                vp2_X,
                vp2_Y,
                sp1_X,
                sp1_Y,
                sp2_X,
                sp2_Y,
                vertex_To_Translate_3,
                projection_Type,
                window_Height,
                window_Width,
                z_Far,
                z_Near,
                FOV,
                &Tx3,
                &Ty3
            );





            //GET NORMAL VECTOR TO Ti 
            vec3D V1, V2;

            //GET A AND B VECTOR
            V1.x = Ti.p2.x - Ti.p1.x;
            V1.y = Ti.p2.y - Ti.p1.y;
            V1.z = Ti.p2.z - Ti.p1.z;
            //
            V2.x = Ti.p3.x - Ti.p1.x;
            V2.y = Ti.p3.y - Ti.p1.y;
            V2.z = Ti.p3.z - Ti.p1.z;
            vec3D triangle_Normal = m.vector_Cross_Product(V1, V2);



        

            float X, Y, Z;

            //NORMALISE RAY FROM CAMERA TO TRIANGLE NORMAL VECTOR
            m.normalise_Range_To_1_0_1(Ti.p1.x, -view_Plane_Width, view_Plane_Width, &X);
            m.normalise_Range_To_1_0_1(Ti.p1.y, -view_Plane_Width, view_Plane_Width, &Y);
            m.normalise_Range_To_1_0_1(Ti.p1.z, -view_Plane_Width, view_Plane_Width, &Z);

            //CAMERA ORIGIN
            float camera_Pos_X = 0;
            float camera_Pos_Y = 0;
            float camera_Pos_Z = -1;   

            vec3D camera_Direction;
            camera_Direction.x = X - camera_Pos_X;
            camera_Direction.y = Y - camera_Pos_Y;
            camera_Direction.z = Z - camera_Pos_Z;

            //GET DOT PRODUCT OF CAMERA RAY AND Ti NORMAL
            float camera_Dot_Product = m.vector_Dot_Product(camera_Direction,triangle_Normal);
           
            //WRITE CURRENT TRIANGLE CROSS and dot PRODUCT VECTOR TO TRIANGLE
            Tt.Dp_Camera = camera_Dot_Product;
            Tt.Dp_Light = 0;  
            
            Tt.p1.x = Tx1;
            Tt.p1.y = Ty1;
            Tt.p1.z = 0;

            Tt.p2.x = Tx2;
            Tt.p2.y = Ty2;
            Tt.p2.z = 0;

            Tt.p3.x = Tx3;
            Tt.p3.y = Ty3;
            Tt.p3.z = 0;
            Tt.face_Color =  Ti.face_Color ;
            Tt.id = i;
            Tt.Nx = triangle_Normal.x;
            Tt.Ny = triangle_Normal.y;
            Tt.Nz = triangle_Normal.z;

                   
            if (camera_Dot_Product < 0 && back_Face_Cliping_Enable)

            {
                //CLIP AROUND WINDOW HERE

                //TRIANGLE BOX CLIPPING FUNCTION(INPUT Ti, OUTPUT BUFF TRIS_CLIPPED

                if (Tt.p3.y < Tt.p1.y)
                {
             
                    m.Swap_XY_Float(Tt.p3.x, Tt.p3.y, Tt.p1.x, Tt.p1.y, &Tt.p3.x, &Tt.p3.y, &Tt.p1.x, &Tt.p1.y);
                }




                pixel_Buffer.push_back(Tt);

            }





            if (!back_Face_Cliping_Enable)
            {
                //CLIP AROUND WINDOW HERE
                pixel_Buffer.push_back(Tt);
            }
    }
}


void rotate_Triangle_Vector
(
    vector<triangle>& triangle_Buffer,
    int axis,
    float An
)
{
   

    for (int i = 0; i != triangle_Buffer.size(); i++)
    {
        float p1[3];
        float p2[3];
        float p3[3];
        triangle Ti = triangle_Buffer[i];
        triangle Tit;

        p1[0] = Ti.p1.x;
        p1[1] = Ti.p1.y;
        p1[2] = Ti.p1.z;

        p2[0] = Ti.p2.x;
        p2[1] = Ti.p2.y;
        p2[2] = Ti.p2.z;

        p3[0] = Ti.p3.x;
        p3[1] = Ti.p3.y;
        p3[2] = Ti.p3.z;


        switch (axis)
        {
        case 0:
            m.x_Rotation_Matrix(An, p1);
            m.x_Rotation_Matrix(An, p2);
            m.x_Rotation_Matrix(An, p3);
            break;

        case 1:
            m.y_Rotation_Matrix(An, p1);
            m.y_Rotation_Matrix(An, p2);
            m.y_Rotation_Matrix(An, p3);
            break;


        case 2:
            m.z_Rotation_Matrix(An, p1);
            m.z_Rotation_Matrix(An, p2);
            m.z_Rotation_Matrix(An, p3);
            break;



        }

        Tit.p1.x = p1[0];
        Tit.p1.y = p1[1];
        Tit.p1.z = p1[2];

        Tit.p2.x = p2[0];
        Tit.p2.y = p2[1];
        Tit.p2.z = p2[2];

        Tit.p3.x = p3[0];
        Tit.p3.y = p3[1];
        Tit.p3.z = p3[2];
        Tit.face_Color = Ti.face_Color;
        Tit.id = Ti.id;
        triangle_Buffer[i] = Tit;
    }

}


void rotate_Obj_Triangle
(
    float x_Angle_Delta,
    float y_Angle_Delta,
    float z_Angle_Delta,
    vector <triangle>& buff
)

{
        rotate_Triangle_Vector(buff, 0, x_Angle_Delta);
        rotate_Triangle_Vector(buff, 1, y_Angle_Delta);
        rotate_Triangle_Vector(buff, 2, z_Angle_Delta);
}


void move_Background_Vertex_Buffer_To_World_Vertex_Buffer
(
    vector<vertex3D>& back_Buff,
    vector<vertex3D>& world_Buff

)
{
    for (int i = 0; i != back_Buff.size(); i++)
    {
        world_Buff.push_back(back_Buff[i]);
    }
}

void move_Obj_Triangle_Buffer_To_World_Triangle_Buffer
(
    vector<triangle>& back_Buff,
    vector<triangle>& world_Buff

)
{
    for (int i = 0; i != back_Buff.size(); i++)
    {
        world_Buff.push_back(back_Buff[i]);
    }
}



void triangle_Rastor_Pipeline
(
    int window_Start_X,
    int window_Start_Y,
    int window_Size_X,
    int window_Size_Y,
    float camera_z,
    float FOV,
    int projection_Type,
    float light_Vector_X,
    float light_Vector_Y,
    float light_Vector_Z,
    bool back_Face_Culling_Enable,
    bool light_Enable,
    bool wire_Frame_Enable,
    int* triangle_Count,
    vector<triangle>& triangle_Buffer
)
{
    vector<triangle> screen_Space_Triangle_Buffer;
    float z_Far = global_Far_Plane;
    float z_Near = global_Near_Plane;
 
   // drawSolidRectangle(window_Start_X, window_Start_Y, window_Size_X, window_Size_Y, black);

    translate_Triangle_Buffer_To_Pixel_Buffer
    (
        window_Start_X,
        window_Start_Y,
        window_Size_X,
        window_Size_Y,
        FOV,
        z_Near,
        z_Far,
        camera_z,
        projection_Type,
        back_Face_Culling_Enable,
        triangle_Buffer,
        screen_Space_Triangle_Buffer
    );
    




    //draw triangles in buffer
    for (int i = 0; i != screen_Space_Triangle_Buffer.size(); i++)
    {
        triangle Ti = screen_Space_Triangle_Buffer[i];
        drawTriangle //SEND TO VGA BACK BUFFER
        (
            (int)Ti.p1.x,
            (int)Ti.p1.y,
            (int)Ti.p2.x,
            (int)Ti.p2.y,
            (int)Ti.p3.x,
            (int)Ti.p3.y,
            wire_Frame_Enable,
            Ti.face_Color
        );

        //if (Ti.p2.x <= 70 || Ti.p3.x <= 70)
        //    drawLine(70, 0, 70, 230, red);

        //if (Ti.p2.y <= 30 || Ti.p3.y <= 30)
        //    drawLine(0, 30, 300, 30, red);




        //print("P1", Ti.p1.x, Ti.p1.y, white, blue);
        //print("P2", Ti.p2.x, Ti.p2.y, white, blue);
        //print("P3", Ti.p3.x, Ti.p3.y, white, blue);
    }


    *triangle_Count = screen_Space_Triangle_Buffer.size();
    triangle_Buffer.clear();
    screen_Space_Triangle_Buffer.clear();

}


void translate_Vertex_Array_To_Triangle_Buff_UNIT_CUBE(float vertex_Buff[8][3], int face_Buff[12][3], vector<triangle>& buff)
{
    buff.clear();

    for (int i = 0; i < 12; i++)
    {
        triangle ti;

        ti.p1.x = vertex_Buff[face_Buff[i][0] -1 ] [0];
        ti.p1.y = vertex_Buff[face_Buff[i][0] - 1] [1];
        ti.p1.z = vertex_Buff[face_Buff[i][0] - 1] [2];

        ti.p2.x = vertex_Buff[face_Buff[i][1] - 1][0];
        ti.p2.y = vertex_Buff[face_Buff[i][1] - 1][1];
        ti.p2.z = vertex_Buff[face_Buff[i][1] - 1][2];

        ti.p3.x = vertex_Buff[face_Buff[i][2] - 1][0];
        ti.p3.y = vertex_Buff[face_Buff[i][2] - 1][1];
        ti.p3.z = vertex_Buff[face_Buff[i][2] - 1][2];
    
        ti.id = i;
        ti.face_Color = i+1;
        buff.push_back(ti);
    };
}

void translate_Vertex_Array_To_Triangle_Buff_UNIT_ICOSAHEDRON(float vertex_Buff[12][3], int face_Buff[20][3], vector<triangle>& buff)
{
    buff.clear();

    for (int i = 0; i < 20; i++)
    {
        triangle ti;

        ti.p1.x = vertex_Buff[face_Buff[i][0] - 1][0];
        ti.p1.y = vertex_Buff[face_Buff[i][0] - 1][1];
        ti.p1.z = vertex_Buff[face_Buff[i][0] - 1][2];

        ti.p2.x = vertex_Buff[face_Buff[i][1] - 1][0];
        ti.p2.y = vertex_Buff[face_Buff[i][1] - 1][1];
        ti.p2.z = vertex_Buff[face_Buff[i][1] - 1][2];

        ti.p3.x = vertex_Buff[face_Buff[i][2] - 1][0];
        ti.p3.y = vertex_Buff[face_Buff[i][2] - 1][1];
        ti.p3.z = vertex_Buff[face_Buff[i][2] - 1][2];

        ti.id = i;
        ti.face_Color = i+1;
        buff.push_back(ti);
    };
}

void translate_Move_Triangle_Buffer(float xPos,float yPos, float zPos, vector<triangle>& triangle_Buffer)
{

    for (int i = 0; i != triangle_Buffer.size(); i++)
    {
        triangle ti = triangle_Buffer[i];

        float temp_P1[3];
        float temp_P2[3];
        float temp_P3[3];

        temp_P1[0] = ti.p1.x;
        temp_P1[1] = ti.p1.y;
        temp_P1[2] = ti.p1.z;

        temp_P2[0] = ti.p2.x;
        temp_P2[1] = ti.p2.y;
        temp_P2[2] = ti.p2.z;

        temp_P3[0] = ti.p3.x;
        temp_P3[1] = ti.p3.y;
        temp_P3[2] = ti.p3.z;


        m.translate_Matrix(xPos, yPos, zPos, temp_P1);
        m.translate_Matrix(xPos, yPos, zPos, temp_P2);
        m.translate_Matrix(xPos, yPos, zPos, temp_P3);

        ti.p1.x = temp_P1[0];
        ti.p1.y = temp_P1[1];
        ti.p1.z = temp_P1[2];

        ti.p2.x = temp_P2[0];
        ti.p2.y = temp_P2[1];
        ti.p2.z = temp_P2[2];

        ti.p3.x = temp_P3[0];
        ti.p3.y = temp_P3[1];
        ti.p3.z = temp_P3[2];

        triangle_Buffer[i] = ti;

    }

}

void scale_Triangle_Buffer(float xSf, float ySf, float zSf, vector<triangle>& triangle_Buffer)
{

    for (int i = 0; i != triangle_Buffer.size(); i++)
    {
        triangle ti = triangle_Buffer[i];

        float temp_P1[3];
        float temp_P2[3];
        float temp_P3[3];

        temp_P1[0] = ti.p1.x;
        temp_P1[1] = ti.p1.y;
        temp_P1[2] = ti.p1.z;

        temp_P2[0] = ti.p2.x;
        temp_P2[1] = ti.p2.y;
        temp_P2[2] = ti.p2.z;

        temp_P3[0] = ti.p3.x;
        temp_P3[1] = ti.p3.y;
        temp_P3[2] = ti.p3.z;


        m.scaling_Matrix(xSf, temp_P1);
        m.scaling_Matrix(ySf, temp_P2);
        m.scaling_Matrix(zSf, temp_P3);

        ti.p1.x = temp_P1[0];
        ti.p1.y = temp_P1[1];
        ti.p1.z = temp_P1[2];

        ti.p2.x = temp_P2[0];
        ti.p2.y = temp_P2[1];
        ti.p2.z = temp_P2[2];

        ti.p3.x = temp_P3[0];
        ti.p3.y = temp_P3[1];
        ti.p3.z = temp_P3[2];

        triangle_Buffer[i] = ti;

    }

}

void scale_Vertex_Buffer(float xSf, float ySf, float zSf, vector<vertex3D>& vertex_Buffer)
{
    for (int i = 0; i != vertex_Buffer.size(); i++)
    {
        vertex3D vi = vertex_Buffer[i];
        float temp_P1[3];     
        temp_P1[0] = vi.x;
        temp_P1[1] = vi.y;
        temp_P1[2] = vi.z;
        m.scaling_Matrix(xSf, temp_P1);
        vi.x = temp_P1[0];
        vi.y = temp_P1[1];
        vi.z = temp_P1[2];
        vertex_Buffer[i] = vi;
    }
}

void set_Light_Vector_Dir(float Xan,float Yan,float Zan,float Vlv[3])
{
    Vlv[0] = 0;
    Vlv[1] = 0;
    Vlv[2] = 1;
    m.z_Rotation_Matrix(Zan, Vlv);
    m.y_Rotation_Matrix(Yan, Vlv);
    m.x_Rotation_Matrix(Xan, Vlv);
}

void world_Triangle_Buffer_Face_Normal_Calc(vector<triangle>& triangle_Buff, vector<vertex3D> &vertex_Buff)
{

    for (int i = 0; i != triangle_Buff.size(); i++)
    {
        triangle Ti = triangle_Buff[i];
        vec3D P1;
        P1.x = Ti.p1.x;
        P1.y = Ti.p1.y;
        P1.z = Ti.p1.z;

        vec3D P2;
        P2.x = Ti.p2.x;
        P2.y = Ti.p2.y;
        P2.z = Ti.p2.z;

        vec3D P3;
        P3.x = Ti.p3.x;
        P3.y = Ti.p3.y;
        P3.z = Ti.p3.z;
        vec3D plane_Norm = m.get_Plane_Normal_Vector(P1, P2, P3);

        Ti.Nx = plane_Norm.x;
        Ti.Ny = plane_Norm.y;
        Ti.Nz = plane_Norm.z;

        int color = red;
      

        int normal_vector_Lenght = 5;
        line_Gen_3D(0, 0, 0, plane_Norm.x * normal_vector_Lenght, plane_Norm.y * normal_vector_Lenght, plane_Norm.z * normal_vector_Lenght, color, 1, vertex_Buff);


        triangle_Buff[i] = Ti;

    }

}

void world_Triangle_Buffer_Serial_Dump(vector<triangle>& triangle_Buff)
{

    for (int i = 0; i != triangle_Buff.size(); i++)
    {
        triangle Ti = triangle_Buff[i];
        Serial.println();
        Serial.println(i);
        Serial.print(" P1 X = "); Serial.println(Ti.p1.x);
        Serial.print(" P1 Y = "); Serial.println(Ti.p1.y);
        Serial.print(" P1 Z = "); Serial.println(Ti.p1.z);

        Serial.print(" P2 X = "); Serial.println(Ti.p2.x);
        Serial.print(" P2 Y = "); Serial.println(Ti.p2.y);
        Serial.print(" P2 Z = "); Serial.println(Ti.p2.z);

        Serial.print(" P3 X = "); Serial.println(Ti.p3.x);
        Serial.print(" P3 Y = "); Serial.println(Ti.p3.x);
        Serial.print(" P3 Z = "); Serial.println(Ti.p3.x);


        Serial.print(" NX = "); Serial.println(Ti.Nx);
        Serial.print(" NY = "); Serial.println(Ti.Ny);
        Serial.print(" NZ = "); Serial.println(Ti.Nz);



        Serial.print(" Id = "); Serial.println(Ti.id);
        Serial.print(" color = "); Serial.println(Ti.face_Color);
        Serial.print(" dp Light = "); Serial.println(Ti.Dp_Light);
        Serial.print(" dp camera = "); Serial.println(Ti.Dp_Camera);
        Serial.println();
        Serial.println();





    }

}


















//#################################################################################################################
//                                                  2D RAYCASTER
//#################################################################################################################

void raycast_Map_From_Ram
(
    byte ram[4096],
    byte ram_Texture[4096],
    int xStart,
    int yStart,
    int screen_Width,
    int screen_Height,
    bool rotate_Left,
    bool rotate_Right,
    bool move_Foward,
    bool move_Back,
    bool cliping_En,
    int  ray_Clip_Point,
    bool camera_Up,
    bool camera_Down,
    float move_Speed,
    float turn_Speed,
    int* camera_Map_Pos_X,
    int* camera_Map_Pos_Y,
    int* camera_Angle_Out,
    int* hit_Cell_X_Left,
    int* hit_Cell_Y_Left,
    int* hit_Cell_X_Center,
    int* hit_Cell_Y_Center,
    int* hit_Cell_X_Right,
    int* hit_Cell_Y_Right,
    int* target_Cell_Data,
    bool camera_Cross_Hair_Enable,
    bool debug

)

{
    //RUN ERROR CHECK ON MAP RAM
    ram_Data_Limit_Clip(ram);
    //if (FOV < 40) FOV = 40;
    //if (FOV > 90) FOV = 90;
    //
    static byte texture_Size = 16;
    static bool left;
    static bool right;
    static bool up;
    static bool down;
    static double rotationStep = 1;
    static double dirX = 1, dirY = 0; //initial direction vector -1 = left to right, +1 = right to left
    static double planeX = 0, planeY = 0.66;       // * 3.144 / 180;; //the 2d GPGUI version of camera plane 0.66 - 0.90
    static double time;
    static double oldTime;
    static double cameraX;
    static double rayDirX;
    static double rayDirY;
    static double moveSpeed;
    static double rotSpeed;
    static double frameTime;
    static int pos;
    byte texWidth = texture_Size;
    byte texHeight = texture_Size;
    int tex_Size = texture_Size * texture_Size;
    int map_Grid_Mem_Start = 10;
    static int mid_Screen = screen_Height / 2;
    float camera_UP_Down_Step = screen_Height * 0.02;
    static float camera_Angle = 0; // STARTING CAMERA ANGLE

    //CLIP VIEW PORT TO SCREEN SIZE
    if (screen_Height > 240) screen_Height = 240;
    if (screen_Width > 320) screen_Width = 320;


    //READ MAP WIDTH AND HEIGHT FROM MEMORY START LOCATION FIRST TWO ADDRESSES
    byte mapWidth = ram[0];
    byte mapHeight = ram[1];
    byte roof_Color = ram[2];
    byte floor_Color = ram[3];
    static byte posX_Start = ram[4];
    static byte posY_Start = ram[5];
    static float posX = posX_Start;
    static float posY = posY_Start;
    static float angle_Step = 0;//ROTATION DELTA IN DEGREES

    if (mapWidth > 60) mapWidth = 60;
    if (mapWidth < 0)  mapWidth = 0;

    if (mapHeight > 60) mapHeight = 60;
    if (mapHeight < 0)  mapHeight = 0;


    //LIMIT TO MAP AREA
    if (posX > (float)mapWidth - 1) { posX = (float)mapWidth - 1.05; beep(500, 10); }
    if (posY > (float)mapHeight - 1) { posY = (float)mapHeight - 1.05; beep(500, 10); }
    if (posX < 1) { posX = 1.05; beep(500, 10); }
    if (posY < 1) { posY = 1.05; beep(500, 10); }
    //

    //LOOK UP AND DOWN
    if (camera_Up && mid_Screen < screen_Height - 10) mid_Screen += camera_UP_Down_Step;//CAMERA UP
    if (camera_Down && mid_Screen > 10) mid_Screen -= camera_UP_Down_Step;//CAMERA DOWN



    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    for (int x = 0; x < screen_Width; x++)// start of raycast loop
    {
        //calculate ray position and direction
        cameraX = 2 * x / double(screen_Width) - 1; //x-coordinate in camera space
        rayDirX = dirX + planeX * cameraX;
        rayDirY = dirY + planeY * cameraX;
        double perpWallDist;//ray length
        double ray_Length;
        byte color;
        int side; //was a NS or a EW wall hit?
        unsigned int mapX = int(posX);//which box of the map we're in
        unsigned int mapY = int(posY);
        double sideDistX;  //length of ray from current position to next x or y-side
        double sideDistY;
        double deltaDistX = (rayDirX == 0) ? 1e30 : std::abs(1 / rayDirX); //length of ray from one x or y- side to next x or y- side
        double deltaDistY = (rayDirY == 0) ? 1e30 : std::abs(1 / rayDirY);
        int stepX; //what direction to step in x or y- direction (either +1 or -1)
        int stepY;
        int hit = 0; //was there a wall hit?


            //calculate step and initial sideDist
        if (rayDirX < 0) { stepX = -1; sideDistX = (posX - mapX) * deltaDistX; }
        else { stepX = 1;  sideDistX = (mapX + 1.0 - posX) * deltaDistX; }
        if (rayDirY < 0) { stepY = -1; sideDistY = (posY - mapY) * deltaDistY; }
        else { stepY = 1;  sideDistY = (mapY + 1.0 - posY) * deltaDistY; }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////

            //perform DDA Cast out ray
        while (hit == 0)
        {
            //jump to next map square, either in x- direction, or in y- direction
            if (sideDistX < sideDistY)
            {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            }

            else
            {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }


            //INDEX INTO MAP RAM
            pos = mapY * mapWidth + mapX;//calculate memory position
                  //limit ray to map boundry


            //STOP RAY AT MAP BOUNDRY
            if (mapX >= mapWidth - 1 || mapY >= mapHeight - 1 || mapX <= 0 || mapY <= 0)
            {
                hit = 1;
                color = roof_Color;
            }

            //STOP RAY AT ANY CELL ABOVE ZERO
            if (ram[pos + map_Grid_Mem_Start] > 0)
            {
                hit = 1;
            }

            //RETURN LEFT RAY CELL HIT DATA
            if (x == 0)
            {
                *hit_Cell_X_Left = mapX;
                *hit_Cell_Y_Left = mapY;
            }

            //RETURN CENTER RAY CEL HIT DATA 
            if (x == screen_Width / 2)
            {
                *hit_Cell_X_Center = mapX;
                *hit_Cell_Y_Center = mapY;
                *target_Cell_Data = ram[pos + map_Grid_Mem_Start];
            }


            //RETURN RIGHT RAY CELL HIT DATA
            if (x == screen_Width - 1)
            {
                *hit_Cell_X_Right = mapX;
                *hit_Cell_Y_Right = mapY;
            }
        }//END OF ONE RAY/////////////////////////////////////////////////////////////////////



            //read current ram cell data for wall color
        byte cell_Data = ram[pos + map_Grid_Mem_Start];

        //!!!!!!!!!!!!!!!!!RENDER WALLS AND ROOF!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        if (side == 0) { perpWallDist = (sideDistX - deltaDistX); }//Calculate distance projected on camera direction to wall
        else { perpWallDist = (sideDistY - deltaDistY); }
        int lineHeight = screen_Height / perpWallDist;//Calculate height of line to draw on screeN                   
        int drawStart = -lineHeight / 2 + mid_Screen;//Calculate lowest y and highest y pixel of the vertical x line
        if (drawStart < 0) drawStart = 0;

        int drawEnd = lineHeight / 2 + mid_Screen;
        ray_Length = perpWallDist;


        //DRAW ROOF NO TEXTURE                                                                                                                                           
        drawLine(x + xStart, yStart, x + xStart, drawStart + yStart, ram[2]);


        //DRAW FLOOR NO TEXTURE
        if (drawEnd < screen_Height) drawLine(x + xStart, drawEnd + yStart, x + xStart, screen_Height + yStart, floor_Color);



        // wall texturing calculations/////////////////////////////////////////////////////////

                 //calculate value of wallX
        double wallX; //where exactly the wall was hit
        if (side == 0) wallX = posY + perpWallDist * rayDirY;
        else           wallX = posX + perpWallDist * rayDirX;
        wallX -= floor((wallX));

        //x coordinate on the texture
        int texX = 15 - int(wallX * double(texWidth));
        if (side == 0 && rayDirX > 0) texX = texWidth - texX - 1;
        if (side == 1 && rayDirY < 0) texX = texWidth - texX - 1;

        // How much to increase the texture coordinate per screen pixel
        double step = 1.0 * texHeight / lineHeight;

        // Starting texture coordinate
        double texPos = (drawStart - mid_Screen + lineHeight / 2) * step;
        //


            //draw wall line from texture buffer
        for (int y = drawStart; y < drawEnd; y++)
        {
            // Cast the texture coordinate to integer, and mask with (texHeight - 1) in case of overflow
            int texY = (int)texPos & (texHeight - 1);
            texPos += step;

            if (cell_Data >= 1 && cell_Data <= 16)
                color = ram_Texture[texHeight * texY + (texX + tex_Size * (cell_Data - 1))];

            //clip pixels from the top and bottom of view the port
            if (y + yStart < screen_Height + yStart && y + yStart >= yStart)
                setPixel(x + xStart, y + yStart, color);

        }//END OF WALL COLLUM TEXTURE RENDER LOOP   
    }//end OF FOV SCAN
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!



    int cell_Address = ((int)posY * mapWidth + (int)posX) + map_Grid_Mem_Start;
    int cell_DATA = ram[cell_Address];


    //timing for input 
    oldTime = time;
    time = millis();
    frameTime = (time - oldTime) / 1000.0; //frameTime is the time this frame has taken, in seconds
   // move_Speed = screen_Height * .02;
    moveSpeed = frameTime * move_Speed; //the constant value is in squares/second
    rotSpeed = frameTime * turn_Speed;

    angle_Step = rotSpeed * 180 / 3.142;// rotate angle delta from radians to degrees


    if (rotate_Right)//right
    {
        rotate_Left = false;
        //both camera direction and camera plane must be rotated
        double oldDirX = dirX;
        dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
        dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);

        double oldPlaneX = planeX;
        planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
        planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
        camera_Angle += angle_Step;

    }

    //ADD OPOSITE CLEAR FLAGS
    if (rotate_Left)//left
    {
        rotate_Right = false;
        //both camera direction and camera plane must be rotated
        double oldDirX = dirX;
        dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
        dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);

        double oldPlaneX = planeX;
        planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
        planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
        camera_Angle -= angle_Step;

    }

    if (move_Back)
    {
        posX -= dirX * moveSpeed;
        posY -= dirY * moveSpeed;
    }

    if (move_Foward)
    {
        if (cell_DATA == 0)
        {
            posX += dirX * moveSpeed;
            posY += dirY * moveSpeed;
        }
        if (cell_DATA > 0)
        {
            posX -= dirX * moveSpeed;
            posY -= dirY * moveSpeed;
            beep(1000, 5);
        }
    }

    //RETURN THE CURRNT MAP X AND Y POS
    *camera_Map_Pos_X = (int)posX;
    *camera_Map_Pos_Y = (int)posY;
    //

    if (camera_Angle > 359) camera_Angle = 0;
    if (camera_Angle < 0)   camera_Angle = 359;

    *camera_Angle_Out = camera_Angle;





    if (camera_Cross_Hair_Enable)
    {
        drawLine(xStart + 5, yStart + screen_Height / 2, xStart + screen_Width - 5, yStart + screen_Height / 2, red);//X LINE
        drawLine(xStart + screen_Width / 2, yStart + 5, xStart + screen_Width / 2, yStart + screen_Height - 5, green);//Y LINE

    }


    if (debug)
    {
        print("posX", 0, 10, red, black);             printNumberDouble(posX, 80, 10, green, black);
        print("posY", 0, 20, red, black);             printNumberDouble(posY, 80, 20, green, black);
        print("mapX", 0, 30, red, black);             printNumberInt((int)posX, 80, 30, green, black);
        print("mapY", 0, 40, red, black);             printNumberInt((int)posY, 80, 40, green, black);
        print("CELL ADDRESS", 0, 50, red, black);     printNumberInt(cell_Address, 80, 50, green, black);
        print("CELL DATA", 0, 60, red, black);        printNumberInt(cell_DATA, 80, 60, green, black);
        print("MAP WIDTH", 0, 80, red, black);        printNumberInt(ram[0], 80, 80, green, black);
        print("MAP HEIGHT", 0, 90, red, black);       printNumberInt(ram[1], 80, 90, green, black);
        print("CAMERA ANGLE", 0, 100, red, black);    printNumberInt(camera_Angle, 80, 100, green, black);
    }

}


void write_Default_Map_To_Ram(byte width, byte height, byte map_Ram[4096], byte wall)
{
    //INPUT ERROR HANDLE
    if (width < 4) width = 4;
    if (width > 60) width = 60;

    if (height < 4)  height = 4;
    if (height > 60) height = 60;





    int mem_Address = 10;
    map_Ram[0] = width;
    map_Ram[1] = height;

 
        map_Ram[2] = blue;
        map_Ram[3] = grey;
        map_Ram[4] = width / 2;
        map_Ram[5] = height / 2;


    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            //DRAW WALL AROUND BOARDER
            if (y == 0 || y == height - 1 || x == 0 || x == width - 1) { map_Ram[mem_Address] = wall; }
            mem_Address++;
        }
    }
}

void write_Default_Sprite_Texture_To_Ram(int mem_Address, byte tex_Ram[4096])
{
    static byte texSize = 16;
    static bool bit = 0;
    int mem_Start = mem_Address;
    //write texture to texture ram location 1
    int tex_Size = texSize * texSize;
    byte tex_Color = mem_Address;

    if (mem_Start < 1) { mem_Start = 1;  mem_Address = 1; }
    if (mem_Start > 16) { mem_Start = 16; mem_Address = 16; }

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






    //index to start address
    if (mem_Address == 1)  mem_Start = 0;
    else if (mem_Address > 1 && mem_Address <= 16)  mem_Start = tex_Size * (mem_Address - 1);





    for (int y = 0; y < texSize; y++)//DEFAULT TEXTURE CROSS 
    {
        for (int x = 0; x < texSize; x++)
        {
            tex_Ram[mem_Start] = TEX[y][x];
            mem_Start++;
        }
    }


}

void write_Default_Wall_Texture_To_Ram(int mem_Address, byte tex_Ram[4096])
{
    static byte texSize = 16;
    static bool bit = 0;
    int mem_Start = mem_Address;

    if (mem_Start < 1) { mem_Start = 1;  mem_Address = 1; }
    if (mem_Start > 16) { mem_Start = 16; mem_Address = 16; }

    //write texture to texture ram location 1
    int tex_Size = texSize * texSize;
    byte tex_Color = mem_Address;


    //index to start address
    if (mem_Address == 1)  mem_Start = 0;
    else if (mem_Address > 1 && mem_Address <= 16)  mem_Start = tex_Size * (mem_Address - 1);





    for (int y = 0; y < texSize; y++)//DEFAULT TEXTURE CROSS HATCH BLACK AND WHITE
    {
        for (int x = 0; x < texSize; x++)
        {
            if (bit)
            {
                if (y == 0 || x == 0) { tex_Ram[mem_Start] = red;  bit = 0; }
                else { tex_Ram[mem_Start] = white; bit = 0; }
            }
            else { tex_Ram[mem_Start] = tex_Color;  bit = 1; }
            mem_Start++;
        }




        if (!bit)  bit = 1;
        else      bit = 0;
    }

    //set start of texture to green
    if (mem_Address == 1)  tex_Ram[0] = green;
    else if (mem_Address > 1 && mem_Address < 17)  tex_Ram[tex_Size * (mem_Address - 1)] = green;
}

void write_Texture_To_Ram(int mem_Address, byte tex_Ram[4096], byte tex_Buffer[255])
{
    const byte texture_Size = 16;
    int mem_Start = mem_Address;
    //write texture to texture ram location 1
    int tex_Size = texture_Size * texture_Size;

    //index to start address
    if (mem_Address == 1)  mem_Start = 0;
    else if (mem_Address > 1 && mem_Address < 17)  mem_Start = tex_Size * (mem_Address - 1);

    int tex_Address = 0;



    for (int y = 0; y < texture_Size; y++)
    {
        for (int x = 0; x < texture_Size; x++)
        {
            tex_Ram[mem_Start] = tex_Buffer[tex_Address];
            tex_Address++;
            mem_Start++;
        }
    }
}

void draw_Tile_Map
(
    int xStart,
    int yStart,
    byte map[4096],
    byte tex[4096],
    int cell_X, //camera cell pos
    int cell_Y,
    int cell_HitX_Left,//left ray hit point cell 
    int cell_HitY_Left,
    int cell_HitX_Center,//center ray hit point cell
    int cell_HitY_Center,
    int cell_HitX_Right,//right ray hit point cell
    int cell_HitY_Right,
    int* posX,//screen space camera and ray hit cell points
    int* posY,
    int* hitX_Left,
    int* hitY_Left,
    int* hitX_Center,
    int* hitY_Center,
    int* hitX_Right,
    int* hitY_Right
)
{

    ram_Data_Limit_Clip(map);
    byte texture_Size = 16;
    byte mapWidth = map[0];
    byte mapHeight = map[1];
    int xpos = xStart;
    int ypos = yStart;
    int mem_Address = 10;

    //limit camera pos
    if (cell_X < 1 || cell_X > mapWidth)  cell_X = mapWidth / 2;
    if (cell_Y < 1 || cell_Y > mapHeight) cell_Y = mapHeight / 2;




    static byte grid_Window_Square_Size = 150;
    static byte tile_Size = grid_Window_Square_Size / map[0];

    //SCALE GRID TO WINDOW AREA
    if (xStart + grid_Window_Square_Size > 320) xStart = 320 - grid_Window_Square_Size;
    if (yStart + grid_Window_Square_Size > 240) yStart = 240 - grid_Window_Square_Size;
    //
    if (tile_Size * map[0] > grid_Window_Square_Size) tile_Size--;
    if (tile_Size * map[1] > grid_Window_Square_Size) tile_Size--;

    //DRAW BACKGROUND
    drawSolidRectangle(xStart - 1, yStart - 1, 181, 181, map[2]);
    // drawHollowRectangle(xStart-1, yStart-1, 181, 181, red);


    for (int y = 0; y < mapHeight; y++)
    {
        for (int x = 0; x < mapWidth; x++)
        {





            //DRAW FLOOR TILE
            if (map[mem_Address] == 0)
            {
                drawSolidRectangle(x + xpos, y + ypos, tile_Size, tile_Size, map[3]);
            }

            //DRAW OBJECT TILE
            if (map[mem_Address] > 16)
            {
                drawSolidRectangle(x + xpos, y + ypos, tile_Size, tile_Size, map[mem_Address]);
                //  printNumberInt(map[mem_Address],x + xpos, y + ypos, red, black);
            }

            //DRAW TEXTURE TILE
            if (map[mem_Address] <= 16 && map[mem_Address] > 0)
            {
                drawSolidRectangle(x + xpos, y + ypos, tile_Size, tile_Size, map[mem_Address]);
                if (tile_Size > 16) draw_2D_Tile(x + xpos, y + ypos, map[mem_Address], tex);
            }

            //HIGHLIGHT CURRENT CAMERA CELL 
            if (cell_X == x && cell_Y == y)
            {
                drawSolidRectangle(x + xpos, y + ypos, tile_Size, tile_Size, random(1, 64));

                *posX = x + xpos + (tile_Size / 2);
                *posY = y + ypos + (tile_Size / 2);
                //return *posX_Center  *posY_Center 
            }

            //return ray hit cells

            if (x == cell_HitX_Left && cell_HitY_Left == y)
            {
                *hitX_Left = x + xpos + (tile_Size / 2);//center of cell in screen space
                *hitY_Left = y + ypos + (tile_Size / 2);
                drawSolidRectangle(x + xpos, y + ypos, tile_Size, tile_Size, random(1, 64));
            }



            if (x == cell_HitX_Center && cell_HitY_Center == y)
            {
                *hitX_Center = x + xpos + (tile_Size / 2);//center of cell in screen space
                *hitY_Center = y + ypos + (tile_Size / 2);
                drawSolidRectangle(x + xpos, y + ypos, tile_Size, tile_Size, random(1, 64));
            }



            if (x == cell_HitX_Right && cell_HitY_Right == y)
            {
                *hitX_Right = x + xpos + (tile_Size / 2);//center of cell in screen space
                *hitY_Right = y + ypos + (tile_Size / 2);
                drawSolidRectangle(x + xpos, y + ypos, tile_Size, tile_Size, random(1, 64));
            }



            xpos += tile_Size;
            mem_Address++;
        }
        xpos = xStart;
        ypos += tile_Size;
    }
}

void draw_2D_Tile(int xStart, int yStart, int tile_Index, byte tex_Ram[4096])
{
    static int mem_Address = 0;
    static byte tex_Width = 16;
    switch (tile_Index)
    {
    case 1:  mem_Address = 0;       break;
    case 2:  mem_Address = 256 * 1;   break;
    case 3:  mem_Address = 256 * 2;   break;
    case 4:  mem_Address = 256 * 3;   break;
    case 5:  mem_Address = 256 * 4;   break;
    case 6:  mem_Address = 256 * 5;   break;
    case 7:  mem_Address = 256 * 6;   break;
    case 8:  mem_Address = 256 * 7;   break;
    case 9:  mem_Address = 256 * 8;   break;
    case 10: mem_Address = 256 * 9;   break;
    case 11: mem_Address = 256 * 10;  break;
    case 12: mem_Address = 256 * 11;  break;
    case 13: mem_Address = 256 * 12;  break;
    case 14: mem_Address = 256 * 13;  break;
    case 15: mem_Address = 256 * 14;  break;
    case 16: mem_Address = 256 * 15;  break;
    }


    for (int y = 0; y < tex_Width; y++)
    {
        for (int x = 0; x < tex_Width; x++)
        {
            setPixel(x + xStart, y + yStart, tex_Ram[mem_Address]);
            mem_Address++;
        }
    }
}

void raycast_Map_Preview(byte map_Ram[4096], byte tex_Ram[4096])
{
    static int ray_Hit_Cell_X_Left = 0;
    static int ray_Hit_Cell_Y_Left = 0;
    static int ray_Hit_Cell_X_Center = 0;
    static int ray_Hit_Cell_Y_Center = 0;
    static int ray_Hit_Cell_X_Right = 0;
    static int ray_Hit_Cell_Y_Right = 0;
    static int target_Cell_Data = 0;
    static int hitx_Left = 0;
    static int hity_Left = 0;
    static int hitx_Center = 0;
    static int hity_Center = 0;
    static int hitx_Right = 0;
    static int hity_Right = 0;
    static int posX = 0;
    static int posY = 0;
    static int camera_Map_Pos_Y = map_Ram[4];
    static int camera_Map_Pos_X = map_Ram[5];
    static int view_Port_Start_X = 183;
    static int view_Port_Start_Y = 1;
    static int view_Port_Width = 136;
    static int view_Port_Height = 182;
    static int camera_Clip_Dist = 15;
    static bool turn_Left = true;
    static bool turn_Right = false;
    static bool move_Fwd = false;
    static bool move_Back = false;
    static bool clipping_Enable = false;
    static bool camera_Up = false;
    static bool camera_Down = false;
    static float move_Speed = 0.05;
    static float turn_Speed = 1.5;
    unsigned long current_Time = millis();
    static int camera_Angle = 0;
    static bool full_Screen = false;
    static bool crossHair_Enable = false;
    ////////////////////////////////////////////////////////////////////////////////////////////////////


        //RUN ERROR CHECK ON RAM
    ram_Data_Limit_Clip(map_Ram);
    //

    if (full_Screen)
    {
        view_Port_Start_X = 0;
        view_Port_Start_Y = 0;
        view_Port_Width = 320;
        view_Port_Height = 240;



    }
    else
    {
        view_Port_Start_X = 183;
        view_Port_Start_Y = 1;
        view_Port_Width = 136;
        view_Port_Height = 182;



    }


    //READ PS2 KEYBOAD
    switch (read_Keyboard_Port())
    {
    case UP_KEY_MAKE_CODE:           move_Fwd = true;   move_Back = false;   break;
    case UP_KEY_BREAK_CODE:          move_Fwd = false;                       break;
    case DOWN_KEY_MAKE_CODE:         move_Back = true;    move_Fwd = false;   break;
    case DOWN_KEY_BREAK_CODE:        move_Back = false;                       break;
    case LEFT_KEY_MAKE_CODE:         turn_Left = true;  turn_Right = false;   break;
    case LEFT_KEY_BREAK_CODE:        turn_Left = false;                       break;
    case RIGHT_KEY_MAKE_CODE:        turn_Right = true;   turn_Left = false;   break;
    case RIGHT_KEY_BREAK_CODE:       turn_Right = false;                       break;
    case Page_Up_Key_Make_Code:      camera_Up = true; camera_Down = false;   break;
    case Page_Up_Key_Break_Code:     camera_Up = false;                       break;
    case Page_Down_Key_Make_Code:    camera_Down = true;   camera_Up = false;   break;
    case Page_Down_Key_Break_Code:   camera_Down = false;                       break;
    case F1_KEY_MAKE_CODE: full_Screen = true; break;
    case F2_KEY_MAKE_CODE: full_Screen = false; break;
    case F3_KEY_MAKE_CODE: crossHair_Enable = true; break;
    case F4_KEY_MAKE_CODE: crossHair_Enable = false; break;
    }


    if (!full_Screen)
    {
        drawBackgroundColor(16);
        draw_Tile_Map // add error check for xstart < 1
        (
            2,                     //X START
            2,                     //Y START
            map_Ram,               //TILE MAP PALET
            tex_Ram,               //TEXTURE PALET
            camera_Map_Pos_X,      //CURRENT CAMERA MAP POS X
            camera_Map_Pos_Y,      //CURRENT CAMERA MAP POS Y
            ray_Hit_Cell_X_Left,   //X END MAP CELL OF RAY, X 0
            ray_Hit_Cell_Y_Left,   //Y END MAP CELL OF RAY, X 0
            ray_Hit_Cell_X_Center, //X END MAP CELL OF RAY, X SCREEN WIDTH/2
            ray_Hit_Cell_Y_Center, //Y END MAP CELL OF RAY, X SCREEN WIDTH/2
            ray_Hit_Cell_X_Right,  //X END MAP CELL OF RAY, X XSCREEN WIDTH
            ray_Hit_Cell_Y_Right,  //Y END MAP CELL OF RAY, X XSCREEN WIDTH
            &posX,                 //X CAMERA PIXEL
            &posY,                 //Y CAMERA PIXEL
            &hitx_Left,            //X END PIXEL OF RAY,X 0
            &hity_Left,            //Y END POINT OF RAY, X0
            &hitx_Center,          //X END POINT OF RAY,X SCREEN WIDTH/2
            &hity_Center,          //Y END POINT OF RAY, X SCREEN WIDTH/2
            &hitx_Right,           //X END POINT OF RAY, XSCREEN WIDTH
            &hity_Right            //Y END POINT OF RAY, XSCREEN WIDTH

        );
    }


    raycast_Map_From_Ram
    (
        map_Ram,                   //MAP DATA BUFFER (BYTE BUFF[4096])
        tex_Ram,                   //TEXTURE BUFFER (BYTE BUFF[4097])
        view_Port_Start_X,         //CAMERA VIEW PORT START X
        view_Port_Start_Y,         //CAMERA VIEW PORT START y
        view_Port_Width,           //CAMERA VIEW PORT WIDTH
        view_Port_Height,          //CAMERA VIEW PORT HEIGHT
        turn_Left,                 //CAMERA TURN LEFT ENABLE
        turn_Right,                //CAMERA TURN RIGHT ENABLE
        move_Fwd,                  //CAMERA MOVE FWD ENABLE
        move_Back,                 //CAMERA MOVE BACK ENABLE
        clipping_Enable,           //CAMERA DISTANCE CLIPING ENABLE
        camera_Clip_Dist,          //CAMERA CLIP DISTANCE
        camera_Up,                 //CAMERA UP ENABLE
        camera_Down,               //CAMERA DOWN ENABLE
        move_Speed,                //CAMERA FOWRD/BACK SPEED
        turn_Speed,                //CAMERA ROTATION SPEED
        &camera_Map_Pos_X,         //ADDRESS OF CAMERA MAP POS X
        &camera_Map_Pos_Y,         //ADDRESS OF CAMERA MAP POS y    
        &camera_Angle,             //WORLD MAP ABSOLUTE ANGLE
        &ray_Hit_Cell_X_Left,      //END OF LEFT RAY X
        &ray_Hit_Cell_Y_Left,      //END OF LEFT RAY Y
        &ray_Hit_Cell_X_Center,    //END OF CENTER RAY X
        &ray_Hit_Cell_Y_Center,    //END OF CENTER RAY Y
        &ray_Hit_Cell_X_Right,     //END OF RIGHT RAY X
        &ray_Hit_Cell_Y_Right,     //END OF RIGHT RAY Y
        &target_Cell_Data,         //DATA OF MEM ADDRESS AT CENTER RAY HIT POINT
        crossHair_Enable,          //CAMERA CROSSHAIR ENABLE
        false                      //DEBUG DATA ENABLE
    );
    //DRAW COMPASS GRAPH
    camera_Compass(267, 1, camera_Map_Pos_X, camera_Map_Pos_Y, camera_Angle, black);

    if (!full_Screen)
    {
        //DRAW FOV TRIANGLE
        drawLine(posX, posY, hitx_Left, hity_Left, red);
        drawLine(posX, posY, hitx_Center, hity_Center, green);
        drawLine(posX, posY, hitx_Right, hity_Right, red);


        //DRAW TEXTURES
        int index = 1;
        for (int x = 20; x < 308; x += 18)
        {
            draw_2D_Tile(x, 190, index, tex_Ram);
            index++;
        }

        //HIGHLIGHT TARGET CELL
        printNumberInt(target_Cell_Data, 1, 190, white, black);
        switch (target_Cell_Data)
        {
            //WALL TEXTURES
        case 1:  drawHollowRectangle(19, 189, 18, 18, random(1, 63)); break;
        case 2:  drawHollowRectangle((18 * 2) + 1, 189, 18, 18, random(1, 63)); break;
        case 3:  drawHollowRectangle((18 * 3) + 1, 189, 18, 18, random(1, 63)); break;
        case 4:  drawHollowRectangle((18 * 4) + 1, 189, 18, 18, random(1, 63)); break;
        case 5:  drawHollowRectangle((18 * 5) + 1, 189, 18, 18, random(1, 63)); break;
        case 6:  drawHollowRectangle((18 * 6) + 1, 189, 18, 18, random(1, 63)); break;
        case 7:  drawHollowRectangle((18 * 7) + 1, 189, 18, 18, random(1, 63)); break;
        case 8:  drawHollowRectangle((18 * 8) + 1, 189, 18, 18, random(1, 63)); break;
        case 9:  drawHollowRectangle((18 * 9) + 1, 189, 18, 18, random(1, 63)); break;
        case 10: drawHollowRectangle((18 * 10) + 1, 189, 18, 18, random(1, 63)); break;
        case 11: drawHollowRectangle((18 * 11) + 1, 189, 18, 18, random(1, 63)); break;
        case 12: drawHollowRectangle((18 * 12) + 1, 189, 18, 18, random(1, 63)); break;
        case 13: drawHollowRectangle((18 * 13) + 1, 189, 18, 18, random(1, 63)); break;
        case 14: drawHollowRectangle((18 * 14) + 1, 189, 18, 18, random(1, 63)); break;
        case 15: drawHollowRectangle((18 * 15) + 1, 189, 18, 18, random(1, 63)); break;
        case 16: drawHollowRectangle((18 * 16) + 1, 189, 18, 18, random(1, 63)); break;


            //SPRITE TEXTURES
        case 17: drawHollowRectangle(19, 209, 18, 18, random(1, 63)); break;
        case 18: drawHollowRectangle((18 * 2) + 1, 209, 18, 18, random(1, 63)); break;
        case 19: drawHollowRectangle((18 * 3) + 1, 209, 18, 18, random(1, 63)); break;
        case 20: drawHollowRectangle((18 * 4) + 1, 209, 18, 18, random(1, 63)); break;
        case 21: drawHollowRectangle((18 * 5) + 1, 209, 18, 18, random(1, 63)); break;
        case 22: drawHollowRectangle((18 * 6) + 1, 209, 18, 18, random(1, 63)); break;
        case 23: drawHollowRectangle((18 * 7) + 1, 209, 18, 18, random(1, 63)); break;
        case 24: drawHollowRectangle((18 * 8) + 1, 209, 18, 18, random(1, 63)); break;
        case 25: drawHollowRectangle((18 * 9) + 1, 209, 18, 18, random(1, 63)); break;
        case 26: drawHollowRectangle((18 * 10) + 1, 209, 18, 18, random(1, 63)); break;
        case 28: drawHollowRectangle((18 * 11) + 1, 209, 18, 18, random(1, 63)); break;
        case 29: drawHollowRectangle((18 * 12) + 1, 209, 18, 18, random(1, 63)); break;
        case 30: drawHollowRectangle((18 * 13) + 1, 209, 18, 18, random(1, 63)); break;
        case 31: drawHollowRectangle((18 * 14) + 1, 209, 18, 18, random(1, 63)); break;
        case 32: drawHollowRectangle((18 * 15) + 1, 209, 18, 18, random(1, 63)); break;
        case 33: drawHollowRectangle((18 * 16) + 1, 209, 18, 18, random(1, 63)); break;
        };

        //DRAW BLACK BAR BEHIND DATA READ OUT
        drawSolidRectangle(0, 230, 320, 10, black);


        //PRINT RAM DATA
        print("SIZE X", 1, 231, red, black); printNumberInt(map_Ram[0], 40, 231, green, black);
        print("Y", 60, 231, red, black);     printNumberInt(map_Ram[1], 70, 231, green, black);
    }

    // FPS_Counter(current_Time, black);
    refreshScreen();
}

void ram_Data_Limit_Clip(byte map_Ram[4096])
{
    byte map_Width_Min = 4;
    byte map_Width_Max = 60;
    byte map_Height_Min = 4;
    byte map_Height_Max = 60;


    if (map_Ram[0] < map_Width_Min)  map_Ram[0] = map_Width_Min;
    if (map_Ram[0] > map_Width_Max)  map_Ram[0] = map_Width_Max;
    //
    if (map_Ram[1] < map_Height_Min) map_Ram[1] = map_Height_Min;
    if (map_Ram[1] > map_Height_Max) map_Ram[1] = map_Height_Max;
    //
    if (map_Ram[2] < 0)              map_Ram[2] = 0;   //roof color 0 - 63
    if (map_Ram[2] > 63)             map_Ram[2] = 63;  //roof color 0 - 63
    //
    if (map_Ram[3] < 0)              map_Ram[3] = 0;   //floor color 0 - 63
    if (map_Ram[3] > 63)             map_Ram[3] = 63;  //floor color 0 - 63
    //
    //if (map_Ram[4] < 1)              map_Ram[4] = 2;//camera pos x start
    //if (map_Ram[4] > map_Ram[0])     map_Ram[4] = map_Ram[0] - 2;//camera pos x start
    ////
    //if (map_Ram[5] < 1)              map_Ram[5] = 2;//camera pos y start
    //if (map_Ram[5] > map_Ram[1])     map_Ram[5] = map_Ram[1] - 2;//camera pos y start
    //
    map_Ram[6] = 0;//UNDEFINED
    map_Ram[7] = 0;//UNDEFINED
    map_Ram[8] = 0;//UNDEFINED
    map_Ram[9] = 0;//UNDEFINED


    //set roof and floor to default if they match
    if (map_Ram[2] == map_Ram[3]) { map_Ram[2] = black; map_Ram[3] = 21; }

}

void camera_Compass(int xStart, int yStart, int xPos, int yPos, int angle, byte back_Color)
{
    static int bin;
    static int end1_x;
    static int end1_y;
    static int end2_x;
    static int end2_y;
    static int end3_x;
    static int end3_y;
    static int grid_StartX = xStart + 1;
    static int grid_StartY = yStart + 1;
    static int color = green;

    //DRAW BACKGROUND
    drawSolidRectangle(xStart, yStart, 52, 71, back_Color);

    //DRAW GRID
    drawHollowRectangle(grid_StartX, grid_StartY, 50, 50, green);

    //DRAW VERTICAL GRID LINES
    for (int x = grid_StartX + 5; x < grid_StartX + 50; x += 5)
    {
        if (x == grid_StartX + (5 * 5)) color = yellow;//CHANGE CENTER LINE TO YELLOW
        else color = green;
        drawLine(x, grid_StartY, x, grid_StartY + 50, color);
    }

    //DRAW HORAZONTAL GRID LINES
    for (int y = grid_StartY + 5; y < grid_StartY + 50; y += 5)
    {
        if (y == grid_StartY + (5 * 5)) color = yellow;//CHANGE CENTER LINR TO YELLOW
        else color = green;
        drawLine(grid_StartX, y, grid_StartX + 50, y, color);
    }

    //DRAW FOV ICON
    draw_Vector(xStart + 26, yStart + 26, 23, (float)angle - 20, 0, &end1_x, &end1_y, random(1, 63));
    draw_Vector(xStart + 26, yStart + 26, 23, (float)angle + 20, 0, &end3_x, &end3_y, random(1, 63));

    //DISPLAY TEXT DATA
    print("X", xStart + 1, yStart + 53, red, back_Color);  printNumberInt(xPos, xStart + 10, yStart + 53, yellow, back_Color);
    print("Y", xStart + 31, yStart + 53, red, back_Color); printNumberInt(yPos, xStart + 40, yStart + 53, yellow, back_Color);
    print("AN", xStart + 1, yStart + 63, red, back_Color); printNumberInt(angle, xStart + 15, yStart + 63, yellow, back_Color);
}









//#############################################################################################################
//#//////////////////////////////////////////OS FUNCTIONS/////////////////////////////////////////////////////#
//#############################################################################################################

void close_All_Programs()
{
    start_Menu_Enable = false;
  //  PROGRAM_1_Enable = false;
    PROGRAM_control_Panel_Enable = false;
    PROGRAM_midi_Monitor_Emulator_Enable = false;
    PROGRAM_hardware_Enable = false;
    PROGRAM_appearance_Enable = false;
    PROGRAM_3D_Model_Enable = false;
    PROGRAM_NEW_PROGRAM_NAME = false;
    PROGRAM_3D_Model_Editor_Enable = false;
}

void task_Bar(unsigned long Time)
{
    byte color;

    if (check_Button_Mouse(1, 232, 26,10, mouseX, mouseY))
    {
        if (mouse_Left_Make)
        {
            start_Menu_Enable = true;
            button_Click_Enter_Tone();
            mouse_Left_Make = false;
        }        
        color = green;
    }
    else
    {
        color = window_Bar_Color;
    }
  
   
    print("START", 0, 232, black, color);
    drawSolidRectangle(30, 232, 254, 8, window_Bar_Color);
    FPS_Counter(Time, black);
}

void screen_Saver_Handle(bool enable)
{



    static byte screen_Saver_Type = 2;


    if (mouse_Right_Make)
    {
        screen_Saver_Enable = true;
        mouse_Right_Make = false;

    }




    if (screen_Saver_Enable)
    {
        switch (screen_Saver_Type)
        {
        case 1: screen_Saver_Random_Lines(); break;
        case 2: screen_Saver_Random_Triangles(); break;
        case 3: screen_Saver_Spinning_World(); break;
        }
    

    //screen_Saver_Type++;
   // if (screen_Saver_Type == 4)  screen_Saver_Type = 1;

    }


}

void draw_Program_Window(window& win)
{

    //SET INDEX FOR DRAWING INTO THE WINDOW AREA
    win.index_X = win.xStart + 1;
    win.index_Y = win.yStart + 17;

    //DRAW WINDOW BOX
    drawHollowRectangle(win.xStart, win.yStart , win.xSize, win.ySize,win.w_border_Color);
    drawSolidRectangle(win.xStart+1 , win.yStart+1, win.xSize - 2, 8, win.title_Bar_Color);
    drawSolidRectangle(win.xStart + 1, win.yStart + 9, win.xSize - 2, 8, win.w_bar_Color );
    drawSolidRectangle(win.index_X, win.index_Y, win.xSize - 2, win.ySize - 18, win.background_Color);
    print(win.window_Name, win.xStart +1, win.yStart + 1, window_Bar_Text_Color, win.title_Bar_Color);
}

void start_Menu_Sort(bool enable, int * active_Button_Id)
{
    int start_Y = 221;
    static int active_ID;
    vector<button> PROGRAM_BUTTONS;//STATIC BUFFER
    create_Program_Buttons(PROGRAM_BUTTONS);
    int box_Start = (start_Y+8) - (10 * (PROGRAM_BUTTONS.size() + 1));
    int menu_Width = 130;


    if (enable)
    {
        drawSolidRectangle(0, box_Start, menu_Width, start_Y - box_Start,bar_Color);

        for (int i = 0; i != PROGRAM_BUTTONS.size(); i++)
        {
            PROGRAM_BUTTONS[i].xStart = 0;
            PROGRAM_BUTTONS[i].yStart = start_Y;
            PROGRAM_BUTTONS[i].xSize = menu_Width;
            PROGRAM_BUTTONS[i].ySize = 10;
            start_Y -= 10;
            if (i == PROGRAM_BUTTONS.size() - 1)
            {
                print("GPGUI 3 OS", 1, start_Y, window_Bar_Text_Color, bar_Color);
                drawSolidRectangle(72, start_Y, 6, 8, red);
                drawSolidRectangle(78, start_Y, 6, 8, green);
                drawSolidRectangle(84, start_Y, 6, 8, blue);
            }
        }
            mouse_Icon_List_Handle(PROGRAM_BUTTONS, &active_ID, mouseX, mouseY);

            *active_Button_Id = active_ID;
    }
    PROGRAM_BUTTONS.clear();
}

void timer_Ticker(unsigned long* start_Time, bool* tick_Flag, unsigned long interval)
{


    unsigned long current_Time = millis();

    if (current_Time - *start_Time >= interval)
    {
        *tick_Flag = true;
        *start_Time = current_Time;//RESTART TIMER
    }

}

bool check_Button_Mouse(int xStart, int yStart, int xSize, int ySize, int cursorX, int cursorY)
{
    if (cursorX > xStart && cursorX < (xStart + xSize) && cursorY > yStart && cursorY < (yStart + ySize))
        return true;
    else
        return false;

}

void mouse_Icon_List_Handle(vector<button>& buttons_Buff, int* active_Button_Id, float mouseX, float mouseY)
{
    static  int active_Button;



    //check mouse pos against aray of button structs and draw
    for (int i = 0; i != buttons_Buff.size(); i++)
    {
        button tempButton = buttons_Buff[i];
 
        tempButton.state =
            check_Button_Mouse
            (
                tempButton.xStart,
                tempButton.yStart,
                tempButton.xSize,
                tempButton.ySize,
                (int)mouseX,
                (int)mouseY
            );


        if (tempButton.state)
        {    
           
                drawSolidRectangle
                (
                    tempButton.xStart,
                    tempButton.yStart,
                    tempButton.xSize,
                    tempButton.ySize,
                    tempButton.onColor
                );
            


            if (tempButton.fill_Type == 1)
            {
                for (int y = 0; y < tempButton.texSize; y++)
                {
                    for (int x = 0; x < tempButton.texSize; x++)
                    {
                        if (tempButton.xStart + x < 319 && tempButton.yStart + y < 239)
                            setPixel((tempButton.xStart + tempButton.icon_Index_X) + x, (tempButton.yStart + tempButton.icon_Index_Y) + y, tempButton.icon[y][x]); 
                    }
                }
            }


            if (tempButton.text_Enable)
                print(tempButton.text, tempButton.xStart, tempButton.yStart, button_Text_Color, tempButton.onColor);

            *active_Button_Id = tempButton.id;         
            active_Button = i;
        }



        if(!tempButton.state)
        {       
            
                drawSolidRectangle
                (
                    tempButton.xStart,
                    tempButton.yStart,
                    tempButton.xSize,
                    tempButton.ySize,
                    tempButton.offColor
                );
          
              /*  if (tempButton.offColor == white)
                drawHollowRectangle
                (
                    tempButton.xStart,
                    tempButton.yStart,
                    tempButton.xSize,
                    tempButton.ySize,
                    black
                );*/

      
                if (tempButton.text_Enable) 
                    print(tempButton.text, tempButton.xStart, tempButton.yStart, button_Text_Color, tempButton.offColor);
            

            if (tempButton.fill_Type == 1)
            {
                for (int y = 0; y < tempButton.texSize; y++)
                {
                    for (int x = 0; x < tempButton.texSize; x++)
                    {
                     if(tempButton.xStart + x < 319 && tempButton.yStart + y < 239)  
                         setPixel((tempButton.xStart + tempButton.icon_Index_X) + x, tempButton.yStart + (tempButton.icon_Index_Y) + y, tempButton.icon[y][x]);
                    }
                }
               if(tempButton.text_Enable) print(tempButton.text, tempButton.xStart, tempButton.yStart, button_Text_Color, tempButton.offColor);
            }
        }
        
    }////////////////////////////////////////////////////////////////////////////////////////


    if (!check_Button_Mouse
    (
        buttons_Buff[active_Button].xStart,
        buttons_Buff[active_Button].yStart,
        buttons_Buff[active_Button].xSize,
        buttons_Buff[active_Button].ySize,
        (int)mouseX,
        (int)mouseY)
    )
    {
        *active_Button_Id = 0;
        active_Button = 0;
    }
}

void mouse_Cursor_Controller_Handle(float* cursorX, float* cursorY, bool active, bool* left_Click_Flag, bool* right_Click_Flag, int controller, bool cursor_Enable)
{
    static float step_Size = 1.0f;
    if (active) step_Size = 0.5f;
    else        step_Size = 1.0f;


        switch (controller)
        {
        case 0://ps2 keyboard
            switch (read_Keyboard_Port())
            {
            case LEFT_KEY_MAKE_CODE:
                if (*cursorX > 1) *cursorX -= step_Size;
                break;

            case RIGHT_KEY_MAKE_CODE:
                if (*cursorX < 318) *cursorX += step_Size;
                break;

            case DOWN_KEY_MAKE_CODE:
                if (*cursorY < 238) *cursorY += step_Size;
                break;

            case UP_KEY_MAKE_CODE:
                if (*cursorY > 1) *cursorY -= step_Size;
                break;

            case ENTER_KEY_MAKE_CODE:
                *left_Click_Flag = true;
                break;

            case ENTER_KEY_BREAK_CODE:
                *left_Click_Flag = false;
                break;

            case CURSOR_MENU_MAKE_CODE:
                *right_Click_Flag = true;
                break;

            case CURSOR_MENU_BREAK_CODE:
                *right_Click_Flag = false;
                break;
            };
            break;

        case 1:
            read_Mouse_Cpu();
            break;




        };

}

void draw_Mouse_Cursor(int mouseX, int mouseY, bool active)
{
    byte color;
    bool mouse_Sprite_Inactive[10][9] =
    {
        1,1,1,1,1,1,0,0,0,
        1,1,1,1,1,0,0,0,0,
        1,1,1,1,1,0,0,0,0,
        1,1,1,1,1,1,0,0,0,
        1,1,0,1,1,1,1,0,0,
        1,0,0,0,1,1,1,1,0,
        0,0,0,0,0,1,1,1,1,
        0,0,0,0,0,0,1,1,0,
        0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0
    };


    bool mouse_Sprite_active[10][9] =
    {
        0,0,0,0,1,0,0,0,0,
        0,0,0,1,1,1,0,0,0,
        0,0,1,1,1,1,1,0,0,
        0,1,1,1,1,1,1,1,0,
        0,1,1,1,1,1,1,1,0,
        0,0,0,1,1,1,0,0,0,
        0,0,0,1,1,1,0,0,0,
        0,0,0,1,1,1,0,0,0,
        0,0,0,1,1,1,0,0,0,
        0,0,0,1,1,1,0,0,0
    };


    for (int y = 0; y < 10; y++)
    {
        for (int x = 0; x < 9; x++)
        {
            if (mouseX + x > 320) break;
            
            if (!active)
            {
                if (mouse_Sprite_Inactive[y][x])
                {
                    color = mouse_Cursor_Color;
                    if (mouseY + y < 240) setPixel(mouseX + x, mouseY + y, color);
                }
            }

            else
            {
                if (mouse_Sprite_active[y][x])
                {
                    color = mouse_Cursor_Color;
                    if (mouseY + y < 240)  setPixel((mouseX + x) - 5, mouseY + y, color);
                }
            }           
        }      
    }
} 

void write_Icon_To_Button(button& b, byte Icon[16][16])
{

    for (int y = 0; y < 16; y++)
    {
        for (int x = 0; x < 16; x++)
        {
            b.icon [y][x] = Icon[y][x];

        }

    }
    

}

void create_Program_Buttons(vector<button>& buttons_Buff)
{
    #define PROGRAM_1_ID 1
    #define PROGRAM_2_ID 2
    #define PROGRAM_3_ID 3
    #define PROGRAM_4_ID 4
    #define PROGRAM_5_ID 5
    #define PROGRAM_6_ID 6


    //PROGRAM BUTTONS
    //button program_1;
    //program_1.onColor = green;
    //program_1.offColor = bar_Color;
    //program_1.id = PROGRAM_1_ID;
    //program_1.text = "Program template";
    //program_1.text_Enable = true;
    //program_1.fill_Type = 0;
    //buttons_Buff.push_back(program_1);
    

    button program_2;
    program_2.onColor = green;
    program_2.offColor = bar_Color;
    program_2.id = PROGRAM_2_ID;
    program_2.text = "CONTROL PANEL";
    program_2.text_Enable = true;
    program_2.fill_Type = 0;
    buttons_Buff.push_back(program_2);


    button program_3;
    program_3.onColor = green;
    program_3.offColor = bar_Color;
    program_3.id = PROGRAM_3_ID;
    program_3.text = "MIDI MONITOR EMULATOR";
    program_3.text_Enable = true;
    program_3.fill_Type = 0;
    buttons_Buff.push_back(program_3);

    button program_4;
    program_4.onColor = green;
    program_4.offColor = bar_Color;
    program_4.id = PROGRAM_4_ID;
    program_4.text = "3D MODEL";
    program_4.text_Enable = true;
    program_4.fill_Type = 0;
    buttons_Buff.push_back(program_4);


    button program_5;
    program_5.onColor = green;
    program_5.offColor = bar_Color;
    program_5.id = PROGRAM_5_ID;
    program_5.text = "NEW PROGRAM NAME";
    program_5.text_Enable = true;
    program_5.fill_Type = 0;
    buttons_Buff.push_back(program_5);

    button program_6;
    program_6.onColor = green;
    program_6.offColor = bar_Color;
    program_6.id = PROGRAM_6_ID;
    program_6.text = "3D Model Editor";
    program_6.text_Enable = true;
    program_6.fill_Type = 0;
    buttons_Buff.push_back(program_6);
    
}




//SCREEN SAVERS

void screen_Saver_Random_Lines()
{
    int n_Lines = 1;
    while (screen_Saver_Enable)
    {
        drawBackgroundColor(blue);


        for (int i = 0; i < n_Lines; i++)
        {
            drawLine(random(0, 320), random(0, 240), random(0, 320), random(0, 240), random(0, 48));
        }
        delay(100);
        n_Lines++;


        print("LINES", 0, 0, red, black);
        printNumberInt(n_Lines, 30, 0, green, black);
        refreshScreen();
        read_Mouse_Cpu();
        if (mouse_Left_Break)
        {
            mouse_Left_Break = false;
            screen_Saver_Enable = false;
            beep(100, 200);
            break;
        }
    }}

void screen_Saver_Random_Triangles()
{
    static int n_Triangles = 1;
    static bool back_Color_tick = 0;
    static bool tri_Color_tick = 0;

    static int back_Color = 0;
    static int tri_Color = 0;
    unsigned long tiker_1;
    unsigned long tiker_2;
    while (screen_Saver_Enable)
    {

        drawBackgroundColor(back_Color);
        timer_Ticker(&tiker_1, &tri_Color_tick, 100);
        timer_Ticker(&tiker_2, &back_Color_tick, 800);


        if (back_Color_tick)
        {
            back_Color_tick = false;
            back_Color++;
            if (back_Color >= 48) back_Color = 0;
            // 
        }

        if (tri_Color_tick)
        {
            tri_Color_tick = false;
            tri_Color++;
            if (tri_Color >= 48) tri_Color = 0;
            //  refreshScreen();

            for (int i = 0; i < n_Triangles; i++)
            {
                drawTriangle
                (
                    random(0, 310), random(0, 230),
                    random(0, 310), random(0, 230),
                    random(0, 310), random(0, 230),
                    1,
                    random(0, 40)

                );
            }
            refreshScreen();
        }


        // refreshScreen();

        read_Mouse_Cpu();
        if (mouse_Left_Break)
        {
            mouse_Left_Break = false;
            screen_Saver_Enable = false;
            beep(100, 200);
            break;
        }
    }
}

void screen_Saver_Raycast_Mouse()
{
    byte map_Width = 20;
    byte map_Height = 20;

    int   view_Port_Start_X = 0;
    int   view_Port_Start_Y = 0;
    int   view_Port_Width = 320;
    int   view_Port_Height = 240;
    int   camera_Clip_Dist = 10;
    bool  turn_Left = false;
    bool  turn_Right = false;
    bool  move_Fwd = false;
    bool  move_Back = false;
    bool  clipping_Enable = false;
    bool  camera_Up = false;
    bool  camera_Down = false;
    int   current_Camera_PosX = 0;
    int   current_Camera_PosY = 0;
    float move_Speed = 2;
    float turn_Speed = 1.0;
    bool  crosshair_Enable = false;
    bool  debug_Enable = false;

    int map_Pos = 0;
    byte map_Mem_Start_Address = 10;
    byte roof_Color = blue;
    byte floor_Color = 42;
    int bin;

    //////TEXTURE RAM ADDRESS VALUES 1-16
    //byte MAP[20][20] =
    //{
    //  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    //  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    //  {0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
    //  {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
    //  {0,0,0,0,0,0,0,1,0,0,0,0,1,0,1,1,1,1,1,0},
    //  {0,0,0,0,0,0,0,1,0,0,0,0,1,0,1,0,0,0,0,0},
    //  {0,0,0,0,0,0,0,1,0,0,0,0,1,0,1,0,0,0,0,0},
    //  {0,0,0,0,0,0,0,1,1,1,1,1,1,0,1,0,0,0,0,0},
    //  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
    //  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
    //  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
    //  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
    //  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
    //  {0,1,1,1,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0},
    //  {0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0},
    //  {0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0},
    //  {0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    //  {0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    //  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    //  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    //};



    //    ////MAP SETTING DATA IN RAM
    //    map_Ram[0] = map_Width;
    //    map_Ram[1] = map_Height;
    //    map_Ram[2] = roof_Color;
    //    map_Ram[3] = floor_Color;
    //    map_Ram[4] = 8;
    //    map_Ram[5] = 8;
    //    //



      //  map_Ram[map_Mem_Start_Address] = 0; //ADDRESS 0 OF MAP GRID
      ////  write map into raycast ram//////
      //  for (int y = 0; y < 20; y++)
      //  {
      //      for (int x = 0; x < 20; x++)
      //      {
      //          map_Ram[map_Pos + map_Mem_Start_Address] = MAP[y][x];
      //          map_Pos++;
      //      }
      //  }

    write_Default_Map_To_Ram(60, 10, map_Ram, 1);
    //FILL ALL 16 TEXTURE SLOTS IN RAM WITH DEFAULT TEXTURES/
    for (int i = 0; i < 17; i++) write_Default_Sprite_Texture_To_Ram(i, texture_Ram);





    while (screen_Saver_Enable)
    {

        if (mouse_Dir_X)
        {
            turn_Left = true;
            turn_Right = false;
            m.normalize(x_Delta, 0, 127, &turn_Speed);

        }
        if (!mouse_Dir_X)
        {
            turn_Right = true;
            turn_Left = false;
            m.normalize(x_Delta, 0, 127, &turn_Speed);
        }



        if (mouse_Scroll_Up)
        {
            camera_Up = true;
            camera_Down = false;
            mouse_Scroll_Up = false;
        }

        if (mouse_Scroll_Down)
        {
            camera_Up = false;
            camera_Down = true;
            mouse_Scroll_Down = false;
        }


        if (mouse_Middle_Make) { move_Fwd = true;  mouse_Middle_Make = false; }
        if (mouse_Middle_Break) { move_Fwd = false; mouse_Middle_Break = false; }


        if (mouse_Still)
        {
            turn_Right = false;
            turn_Left = false;
            camera_Up = false;
            camera_Down = false;
        }


        //DRAW FRAME
       // drawBackgroundColor(0);

        raycast_Map_From_Ram
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
            turn_Speed * 3,
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

        refreshScreen();

        read_Mouse_Cpu();
        if (mouse_Left_Break)
        {
            mouse_Left_Break = false;
            screen_Saver_Enable = false;
            beep(100, 200);
            break;
        }
    }








}

void screen_Saver_Spinning_World()
{
    //CREATE BUFFER TO HOLD ALL WORLD VERTICIES
    vector<vertex3D> world_Space_Vertex_Buffer;
    vector<triangle> world_Triangle_Buffer;
    vector<triangle>          OBJ_1_Buffer;
    vector<triangle>          OBJ_2_Buffer;

    int vertex_Count;
    int bin;




    //WORLD VIEW CAMERA SETTINGS
    int projection_Type = 1;
    float x_angle = 15;
    float y_angle = 0;
    float z_angle = 0;
    float z = 0; //camera zoom !!!!!!!!!!!!! NOW NOT USED
    float FOV = 80;//ZOOM smaller zoom in


    //GRID SETTINGS

   // 1000/1000 = 1 meter
    float ground_Plane_Size = 100000;//100 meters
    float plane_Size = (ground_Plane_Size / 1000) / 2;
    float y_Offset = 0;
    float x_Offset = 0;
    float z_Offset = 0;


    int back_Color = black;//36 sky blue
    int n_Lines = 17;
    int grid_Color = red;
    bool grid_Color_Highlight = true;



    //object position and angle
    float x_angle_OBJ1 = 0.0;
    float y_angle_OBJ1 = 0.0;
    float z_angle_OBJ1 = 0.0;
    float xPos1 = -plane_Size / 2;
    float yPos1 = 0;
    float zPos1 = 0;



    //object position and angle
    float x_angle_OBJ2 = 0.0;
    float y_angle_OBJ2 = 0.0;
    float z_angle_OBJ2 = 0.0;
    float xPos2 = plane_Size / 2;
    float yPos2 = 0;
    float zPos2 = 0;



    while (screen_Saver_Enable)
    {
        int line_Id = 0;
        unsigned long current_Time = millis();

        read_Keyboard_Port();
        read_Mouse_Cpu();




        //FRAME LOOP
        drawBackgroundColor(back_Color);//CLEAR VGA BACK BUFFER

        write_Origin_Cross_Mesh_To_Vector(world_Space_Vertex_Buffer, 30, true, &bin);

        //GROUND PLANE
        write_Grid_Plane_To_Vector_XZ
        (
            -plane_Size,//x pa
            -plane_Size,//z
            plane_Size,//x pb
            -plane_Size,//z
            -plane_Size,//x pc
            plane_Size,//z
            plane_Size,//x pd
            plane_Size,//z
            y_Offset,//y offset
            n_Lines,
            grid_Color,
            grid_Color_Highlight,
            line_Id,
            &line_Id,
            world_Space_Vertex_Buffer
        );



        ////wall
        //write_Grid_Plane_To_Vector_XY
        //(
        //    -plane_Size,//x pa
        //    -plane_Size,//z
        //    plane_Size,//x pb
        //    -plane_Size,//z
        //    -plane_Size,//x pc
        //    plane_Size,//z
        //    plane_Size,//x pd
        //    plane_Size,//z
        //    z_Offset,//z offset
        //    n_Lines,
        //    grid_Color,
        //    grid_Color_Highlight,
        //    line_Id,
        //    &line_Id,
        //    world_Space_Vertex_Buffer
        //);

        ////WALL
        //write_Grid_Plane_To_Vector_ZY
        //(
        //    -plane_Size,//x pa
        //    -plane_Size,//z
        //    plane_Size,//x pb
        //    -plane_Size,//z
        //    -plane_Size,//x pc
        //    plane_Size,//z
        //    plane_Size,//x pd
        //    plane_Size,//z
        //    x_Offset,//y offset
        //    n_Lines,
        //    grid_Color,
        //    grid_Color_Highlight,
        //    line_Id,
        //    &line_Id,
        //    world_Space_Vertex_Buffer
        //);

        // LOAD OBJECT DATA INTO MODEL SPACE     
        translate_Vertex_Array_To_Triangle_Buff_UNIT_CUBE(UNIT_CUBE_vertex_Array, UNIT_CUBE_faces, OBJ_1_Buffer);
        translate_Vertex_Array_To_Triangle_Buff_UNIT_ICOSAHEDRON(UNIT_ICOSAHEDRON_MESH_Vertex_Array, UNIT_ICOSAHEDRON_MESH_face_Array, OBJ_2_Buffer);


        // ROTATE OBJECTS IN MODEL SPACE
        rotate_Obj_Triangle(x_angle_OBJ1, y_angle_OBJ1, z_angle_OBJ1, OBJ_1_Buffer);
        rotate_Obj_Triangle(x_angle_OBJ2, y_angle_OBJ2, z_angle_OBJ2, OBJ_2_Buffer);

        //MOVE OBJECTS IN MODEL SPACE
        translate_Move_Triangle_Buffer(xPos1, -sin(m.deg_To_Rad(y_angle_OBJ1)) * (plane_Size / 2), zPos2, OBJ_1_Buffer);
        translate_Move_Triangle_Buffer(xPos2, -sin(m.deg_To_Rad(y_angle_OBJ2)) * (plane_Size / 2), zPos2, OBJ_2_Buffer);


        // MOVE OBJECTS FROM MODEL SPACE TO WORLD SPACE
        move_Obj_Triangle_Buffer_To_World_Triangle_Buffer(OBJ_1_Buffer, world_Triangle_Buffer);
        move_Obj_Triangle_Buffer_To_World_Triangle_Buffer(OBJ_2_Buffer, world_Triangle_Buffer);

        //GLOBAL ROTATE WORLD SPACE AND MOVE VERTEX BUFFER IN WORLD SPACE
        rotate_World_Vertex(x_angle, y_angle, z_angle, world_Space_Vertex_Buffer);
        rotate_Obj_Triangle(x_angle, y_angle, z_angle, world_Triangle_Buffer);

        ////PASS VERTEX AND TRIANGLE BUFFER TO RENDER PIPELINE
        vertex_Rastor_Pipeline
        (
            0,
            0,
            320,
            240,
            z,
            FOV,
            projection_Type,
            back_Color,
            &vertex_Count,
            &bin,
            world_Space_Vertex_Buffer
        );


        triangle_Rastor_Pipeline
        (
            0,                           // WINDOW START X
            0,                           // WINDOW START Y
            320,                         // WINDOW SIZE X
            240,                         // WINDOW SIZE Y
            z,                           // CAMERA Z
            FOV,                         // CAMMERA FOV
            projection_Type,             // 0 = ORTHO 1 = PERSPECTIVE
            0,                           // LIGHT X
            0,                           // LIGHT Y
            0,                           // LIGHT Z
            true,                        // ENABLE BACKFACE CULLING
            false,                       // LIGHT ENABLE
            true,                       // TRIANGLE FILL ENABLE
            &bin,                        // TRIANGLES DRAWN
            world_Triangle_Buffer        // WORLD SPACE TRIANGLE BUFFER
        );


        //AUTO ROTATE OBJECTS IN MODEL SPACE
        x_angle_OBJ1 += 1.5;
        y_angle_OBJ1 += 1.2;
        if (x_angle_OBJ1 > 179)
        {
            x_angle_OBJ1 = 0.0;
           // send_Midi_Message(153, 43, 127);
        }


        if (y_angle_OBJ1 > 179)
        {
            y_angle_OBJ1 = 0.0;
            send_Midi_Message(153, 43, 127);
        }
            
        x_angle_OBJ2 += 3.4;
        y_angle_OBJ2 += 3.5;
        if (x_angle_OBJ2 > 179)
        {
            x_angle_OBJ2 = 0.0;
        }

        if (y_angle_OBJ2 > 179)
        {
            y_angle_OBJ2 = 0.0;
            send_Midi_Message(153, 44, 127);
        }


        //ROTATE WORLD
        y_angle += .2;
        if (y_angle > 359) y_angle = 0.0;



        //CLEAR BUFFERS FOR NEXT FRAME
        world_Space_Vertex_Buffer.clear();
        world_Triangle_Buffer.clear();
        OBJ_1_Buffer.clear();
        OBJ_2_Buffer.clear();
        FPS_Counter(current_Time, black);
        refreshScreen();

    }//END OF RENDER LOOP######################################################################################


}


//SYSTEM SOUNDS
void button_Click_Enter_Tone()
{
    if (system_Sounds_Midi_En)
    {
        set_Soundcard_Max_Velocity(button_Vel);
        set_Soundcard_Max_Volume(button_Vol);
        set_Patch(1, button_Inst);
        note_On(1, menu_Enter_Note, button_Vel);
       // delay(button_Click_Bounce_Time);
       // note_Off(1, menu_Enter_Note);
    }
}

void button_Click_Close_Window_Tone()
{
    if (system_Sounds_Midi_En)
    {
        set_Soundcard_Max_Velocity(button_Vel);
        set_Soundcard_Max_Volume(button_Vol);
        set_Patch(1, button_Inst);
        note_On(1, window_Close_Note, button_Vel);
        delay(button_Click_Bounce_Time);
        note_Off(1, window_Close_Note);
    }
}

void button_Hover_Tone()
{
    if (system_Sounds_Midi_En)
    {
        set_Soundcard_Max_Velocity(button_Vel);
        set_Soundcard_Max_Volume(button_Vol);
        set_Patch(1, button_Inst);
        note_On(1, button_Hover_Note, button_Vel);    
    }
}




//#############################################################################################################
//#//////////////////////////////////////////OS PROGRAMS//////////////////////////////////////////////////////#
//#############################################################################################################

void GPGUI_OS()
{
    //PROGRAM AND BUTTON ID TAGS
     #define START_MENU_ID 900
     #define PROGRAM_1_ID 1
     #define PROGRAM_2_ID 2
     #define PROGRAM_3_ID 3
     #define PROGRAM_4_ID 4
     #define PROGRAM_5_ID 5
     #define PROGRAM_6_ID 6


    int active_Button_Id = 0;


   
   

   // bool start = 0;
  //  byte speed;

    GPGUI_INIT();

    byte s;
    byte n;
    byte v;
   if(set_Menu_Enter_Note_From_Midi_Keyboard)    enable_Midi_Port();
   if(set_window_Close_Note_From_Midi_Keyboard)  enable_Midi_Port();

    // start_Up_Data_Screen();
    while (1)
    {
        unsigned long current_Time = millis();

        if (set_Menu_Enter_Note_From_Midi_Keyboard)
        {
            read_Midi_Port(&s, &n, &v);
            if (s == 144) menu_Enter_Note = (char)n;
        }

        if (set_window_Close_Note_From_Midi_Keyboard)
        {
            read_Midi_Port(&s, &n, &v);
            if (s == 144) window_Close_Note = (char)n;
        }

       

        //READ ALL CONTROL INPUT

        switch (read_Keyboard_Port())
        {
        case F1_KEY_MAKE_CODE:
            drawBackgroundColor(blue);
            controller = 1;
            beep(1000, 100);
            print("CURSOR CONTROL SET TO (MOUSE)", 80, 110, red, blue);
            refreshScreen();
            delay(2000);

            break;
        case F2_KEY_MAKE_CODE:
            drawBackgroundColor(blue);
            controller = 0;
            beep(1200, 100);
            print("CURSOR CONTROL SET TO (KEYBOARD)", 80, 110, red, blue);
            refreshScreen();
            delay(2000);
            break;
        }


        //READS MOUSE HARDWARE AND SETS THE GLOBAL MOUSE VARIABLES.
        mouse_Cursor_Controller_Handle(&mouseX, &mouseY, button_Active, &mouse_Left_Make, &mouse_Right_Make, controller, Cursor_Enable);


        //HANDLE MOUSE CLICK FLAGS
   
        if (mouse_Right_Make && start_Menu_Enable) //DISABLE START MENU WITH RIGHT CLICK
        {
            start_Menu_Enable = false;
            mouse_Right_Make = false;
            beep(exit_Beep_Pitch, 100);
        }

        if (mouse_Left_Make && start_Menu_Enable)
        {
            switch (active_Button_Id)
            {
                /*      case PROGRAM_1_ID:
                          active_Button_Id = 0;
                          start_Menu_Enable = false;
                          PROGRAM_1_Enable = true;
                          beep(enter_Beep_Pitch, 200);
                      break;*/

            case PROGRAM_2_ID:
                active_Button_Id = 0;
                close_All_Programs();
                PROGRAM_control_Panel_Enable = true;
                button_Click_Enter_Tone();
                beep(enter_Beep_Pitch, 200);
                break;

            case PROGRAM_3_ID:
                active_Button_Id = 0;
                close_All_Programs();
                PROGRAM_midi_Monitor_Emulator_Enable = true;
                button_Click_Enter_Tone();
                beep(enter_Beep_Pitch, 200);
                break;

            case PROGRAM_4_ID:
                active_Button_Id = 0;
                close_All_Programs();
                PROGRAM_3D_Model_Enable = true;
                button_Click_Enter_Tone();
                beep(enter_Beep_Pitch, 200);
                break;

            case PROGRAM_5_ID:
                active_Button_Id = 0;
                close_All_Programs();
                PROGRAM_NEW_PROGRAM_NAME = true;
                button_Click_Enter_Tone();
                beep(enter_Beep_Pitch, 200);
                break;

            case PROGRAM_6_ID:
                active_Button_Id = 0;
                close_All_Programs();
                PROGRAM_3D_Model_Editor_Enable = true;
                button_Click_Enter_Tone();
                beep(enter_Beep_Pitch, 200);
                break;
            }
       
            mouse_Left_Make = false;
        }









        //START OF GRAPHICS LOOP

        //CLEAR SCREEN
        drawBackgroundColor(desktop_Color);
        //draw_Color_Map(true);

        //RUN PROGRAMS
        //***********************************************************************
        // program_Template(&PROGRAM_1_Enable, PROGRAM_1_Enable);
        control_Panel(&PROGRAM_control_Panel_Enable, PROGRAM_control_Panel_Enable);
        midi_Montitor_Emulator(&PROGRAM_midi_Monitor_Emulator_Enable, PROGRAM_midi_Monitor_Emulator_Enable);
        PROGRAM_3D_MODEL(&PROGRAM_3D_Model_Enable, PROGRAM_3D_Model_Enable);
        new_Program_Name(&PROGRAM_NEW_PROGRAM_NAME, PROGRAM_NEW_PROGRAM_NAME);
        PROGRAM_3D_MODEL_EDITOR(&PROGRAM_3D_Model_Editor_Enable, PROGRAM_3D_Model_Editor_Enable);
        hardware_Settings(&PROGRAM_hardware_Enable, PROGRAM_hardware_Enable);
        appearance(&PROGRAM_appearance_Enable, PROGRAM_appearance_Enable);
        //***********************************************************************




        start_Menu_Sort(start_Menu_Enable, &active_Button_Id);//SORTS AND DRAWS PROGRAM BUTTONS FROM BOTTOM LEFT CORNER UP
        task_Bar(current_Time);//CREATES START BUTTON AND FPS COUNTER
        draw_Mouse_Cursor(mouseX, mouseY, button_Active);
        refreshScreen();
        screen_Saver_Handle(screen_Saver_Enable);
    }
}

void program_Template(bool* enable_Out, bool enable_In)
{    
     if (enable_In)
     {
         static int background_Color = white;
         vector<button> buttons_Buff;
         static int   active_Button_Id = 0;
     
         //create window
         window w1;
         w1.xStart = 10;
         w1.yStart = 10;
         w1.xSize = 100;
         w1.ySize = 100;
         w1.title_Bar_Color = window_Bar_Color;
         w1.w_bar_Color = bar_Color;
         w1.w_border_Color = border_Line_Color;
         w1.background_Color = background_Color;
         w1.window_Name = "CONTROL PANEL";
         draw_Program_Window(w1);
     
         //MOUSE BUTTON  CLICK FLAGS
         if (mouse_Left_Make)
         {
             //PROGRAM BUTTONS
             switch (active_Button_Id)
             {
             case 1://EXIT
               // *enable_Out = false;
                 close_All_Programs();
                 beep(exit_Beep_Pitch, 200);
                 active_Button_Id = 0;
                 break;//ESCAPE BUTTON
     
     
             }
           //  mouse_Left_Make = false;
         }///////////////////////////////////////////////////////////////////////////////////
     
     
     
        //CREATE ALL BUTTONS 
         button exit;
         exit.xStart = (w1.xStart + w1.xSize) - 9;
         exit.yStart = w1.yStart + 1;
         exit.xSize = 8;
         exit.ySize = 8;
         exit.id = 1;
         exit.offColor = red;
         exit.onColor = green;
         exit.fill_Type = 1;
         exit.texSize = 8;
         exit.text_Enable
             = false;
         exit.icon_Index_X = 0;
         exit.icon_Index_Y = 0;
         write_Icon_To_Button(exit, exit_Sprite);
         buttons_Buff.push_back(exit);
     
     
     
         //#######################################################################################################################
        //GRAPHICS LOOP
     
     
         mouse_Icon_List_Handle(buttons_Buff, &active_Button_Id, mouseX, mouseY);                //CHECK MOUSE POSITION AGAINST ALL BUTTONS IN BUFFER
     
     
     
     
         buttons_Buff.clear();
         //#######################################################################################################################
     }
}    
////////////////////////////////////////////////////////////////    



void control_Panel(bool* enable_Out, bool enable_In)
{    
     if (enable_In)
     {
         static int background_Color = white;
         vector<button> buttons_Buff;
         static int   active_Button_Id = 0;
     
         //create window
         window w1;
         w1.xStart = 110;
         w1.yStart = 100;
         w1.xSize = 100;
         w1.ySize = 40;
         w1.title_Bar_Color = window_Bar_Color;
         w1.w_bar_Color = bar_Color;
         w1.w_border_Color = border_Line_Color;
         w1.background_Color = background_Color;
         w1.window_Name = "CONTROL PANEL";
         draw_Program_Window(w1);
     
         //MOUSE BUTTON  CLICK FLAGS
         if (mouse_Left_Make)
         {
             //PROGRAM BUTTONS
             switch (active_Button_Id)
             {
             case 1://EXIT
               // *enable_Out = false;
                 close_All_Programs();
                 button_Click_Close_Window_Tone();
                 beep(exit_Beep_Pitch, 200);
                 active_Button_Id = 0;
                 break;//ESCAPE BUTTON
     
             case 2:
                 *enable_Out = false;               
                 close_All_Programs();
                 PROGRAM_hardware_Enable = true;
                 button_Click_Enter_Tone();
                 beep(enter_Beep_Pitch, 200);
                 active_Button_Id = 0;
                 break;//ESCAPE BUTTON
     
             case 3:
                 *enable_Out = false;
                 close_All_Programs();
                 PROGRAM_appearance_Enable = true;
                 button_Click_Enter_Tone();
                 beep(enter_Beep_Pitch, 200);
                 active_Button_Id = 0;
                 break;//ESCAPE BUTTON
             }
             mouse_Left_Make = false;
         }///////////////////////////////////////////////////////////////////////////////////
     
       
     
        //CREATE ALL BUTTONS 
         button exit;
         exit.xStart = (w1.xStart + w1.xSize) - 9;
         exit.yStart = w1.yStart + 1;
         exit.xSize = 8;
         exit.ySize = 8;
         exit.id = 1;
         exit.offColor = red;
         exit.onColor = green;
         exit.fill_Type = 1;
         exit.texSize = 8;
         exit.text_Enable
             = false;
         exit.icon_Index_X = 0;
         exit.icon_Index_Y = 0;
         write_Icon_To_Button(exit, exit_Sprite);
         buttons_Buff.push_back(exit);
     
     
     
         button HARDWARE;
         HARDWARE.xStart = w1.index_X;
         HARDWARE.yStart = w1.index_Y;
         HARDWARE.xSize = 48;
         HARDWARE.ySize = 8;
         HARDWARE.id = 2;
         HARDWARE.offColor = white;
         HARDWARE.onColor = green;
         HARDWARE.fill_Type = 0;     
         HARDWARE.text_Enable = true;
         HARDWARE.text = "HARDWARE";
         buttons_Buff.push_back(HARDWARE);
     
     
         button APPEARANCE;
         APPEARANCE.xStart = w1.index_X;
         APPEARANCE.yStart = w1.index_Y +10;
         APPEARANCE.xSize = 48;
         APPEARANCE.ySize = 8;
         APPEARANCE.id = 3;
         APPEARANCE.offColor = white;
         APPEARANCE.onColor = green;
         APPEARANCE.fill_Type = 0;
         APPEARANCE.text_Enable = true;
         APPEARANCE.text = "APPEARANCE";
         buttons_Buff.push_back(APPEARANCE);
     
     
     
         
     
     
         //#######################################################################################################################
        //GRAPHICS LOOP
                                                              
         
         mouse_Icon_List_Handle(buttons_Buff, &active_Button_Id, mouseX, mouseY);                //CHECK MOUSE POSITION AGAINST ALL BUTTONS IN BUFFER
         
     
     
     
          buttons_Buff.clear();
         //#######################################################################################################################
     }
}    
    
void scene_3D_World_Template_Program()
{    
     //CREATE BUFFER TO HOLD ALL WORLD VERTICIES
     vector<vertex3D> world_Space_Vertex_Buffer;
     vector<triangle> world_Triangle_Buffer;
     vector<triangle>          OBJ_1_Buffer;
     vector<triangle>          OBJ_2_Buffer;
     
     int vertex_Count;
     int bin;
     
     
     
     
     //WORLD VIEW CAMERA SETTINGS
     int projection_Type = 1;
     float x_angle = 20;
     float y_angle = 0;
     float z_angle = 0;
     float z = 0; //camera zoom !!!!!!!!!!!!! NOW NOT USED
     float FOV = 90;//ZOOM smaller zoom in
     
     
     //GRID SETTINGS
     
     // 1000/1000 = 1 meter
     float ground_Plane_Size = 100000;//100 meters
     float plane_Size = (ground_Plane_Size / 1000) / 2;
     float y_Offset = plane_Size;
     float x_Offset = 0;
     float z_Offset = 0;
     
     
     int back_Color = white;//36 sky blue
     int n_Lines = 17;
     int grid_Color = green;
     bool grid_Color_Highlight = true;
     
     
     
     //object position and angle
     float x_angle_OBJ1 = 0.0;
     float y_angle_OBJ1 = 0.0;
     float z_angle_OBJ1 = 0.0;
     float xPos1 = -plane_Size / 2;
     float yPos1 = 0;
     float zPos1 = 0;
     
     
     
     //object position and angle
     float x_angle_OBJ2 = 0.0;
     float y_angle_OBJ2 = 0.0;
     float z_angle_OBJ2 = 0.0;
     float xPos2 = plane_Size / 2;
     float yPos2 = 0;
     float zPos2 = 0;
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     while (1)
     {
         
         drawBackgroundColor(blue);
         int line_Id = 0;
         unsigned long current_Time = millis();
         read_Mouse_Cpu();
         if (mouse_Scroll_Up)
         {
             FOV += .3;
             mouse_Scroll_Up = false;
         }
         if (mouse_Scroll_Down)
         {
             FOV -= .3;
             mouse_Scroll_Down = false;
         }
     
     
     
     
     
     
         //FRAME LOOP
       // drawBackgroundColor(16);//CLEAR VGA BACK BUFFER
     
         write_Origin_Cross_Mesh_To_Vector(world_Space_Vertex_Buffer, 30, true, &bin);
     
         //GROUND PLANE
        //write_Grid_Plane_To_Vector_XZ
        //(
        //    -plane_Size,//x pa
        //    -plane_Size,//z
        //    plane_Size,//x pb
        //    -plane_Size,//z
        //    -plane_Size,//x pc
        //    plane_Size,//z
        //    plane_Size,//x pd
        //    plane_Size,//z
        //    y_Offset,//y offset
        //    n_Lines,
        //    grid_Color,
        //    grid_Color_Highlight,
        //    line_Id,
        //    &line_Id,
        //    world_Space_Vertex_Buffer
        //);
     
     
     
         ////wall
        //write_Grid_Plane_To_Vector_XY
        //(
        //    -plane_Size,//x pa
        //    -plane_Size,//z
        //    plane_Size,//x pb
        //    -plane_Size,//z
        //    -plane_Size,//x pc
        //    plane_Size,//z
        //    plane_Size,//x pd
        //    plane_Size,//z
        //    z_Offset,//z offset
        //    n_Lines,
        //    grid_Color,
        //    grid_Color_Highlight,
        //    line_Id,
        //    &line_Id,
        //    world_Space_Vertex_Buffer
        //);
     
         ////WALL
        //write_Grid_Plane_To_Vector_ZY
        //(
        //    -plane_Size,//x pa
        //    -plane_Size,//z
        //    plane_Size,//x pb
        //    -plane_Size,//z
        //    -plane_Size,//x pc
        //    plane_Size,//z
        //    plane_Size,//x pd
        //    plane_Size,//z
        //    x_Offset,//y offset
        //    n_Lines,
        //    grid_Color,
        //    grid_Color_Highlight,
        //    line_Id,
        //    &line_Id,
        //    world_Space_Vertex_Buffer
        //);
     
         // LOAD OBJECT DATA INTO MODEL SPACE     
         translate_Vertex_Array_To_Triangle_Buff_UNIT_CUBE(UNIT_CUBE_vertex_Array, UNIT_CUBE_faces, OBJ_1_Buffer);
        // translate_Vertex_Array_To_Triangle_Buff_UNIT_ICOSAHEDRON(UNIT_ICOSAHEDRON_MESH_Vertex_Array, UNIT_ICOSAHEDRON_MESH_face_Array, OBJ_2_Buffer);
     
         // SCALE OBJECTS IN MODEL SPACE
         static float yScale;
         while (CONTROL_PORT.available())
         {
             if (CONTROL_PORT.read() == 144)
             {
                 byte data1 = CONTROL_PORT.read();
                 byte data2 = CONTROL_PORT.read();
     
                 yScale = data2/2;
             }
              
         }
         static float SF = 1;
         if (SF < 1) SF = 1;
         scale_Triangle_Buffer(yScale, yScale, yScale, OBJ_1_Buffer);
        // scale_Triangle_Buffer(SF, SF, SF, OBJ_2_Buffer);
       // SF = sin(m.deg_To_Rad(y_angle_OBJ1)) * 2;
     
         // ROTATE OBJECTS IN MODEL SPACE
      //  rotate_Obj_Triangle(x_angle_OBJ1, y_angle_OBJ1, z_angle_OBJ1, OBJ_1_Buffer);
       // rotate_Obj_Triangle(x_angle_OBJ2, y_angle_OBJ2, z_angle_OBJ2, OBJ_2_Buffer);
     
         //MOVE OBJECTS IN MODEL SPACE
         translate_Move_Triangle_Buffer(xPos1, 0, zPos2, OBJ_1_Buffer);
       //  translate_Move_Triangle_Buffer(xPos2, sin(m.deg_To_Rad(y_angle_OBJ2)) * (plane_Size / 4), zPos2, OBJ_2_Buffer);
     
     
     
         // MOVE OBJECTS FROM MODEL SPACE TO WORLD SPACE
         move_Obj_Triangle_Buffer_To_World_Triangle_Buffer(OBJ_1_Buffer, world_Triangle_Buffer);
       //  move_Obj_Triangle_Buffer_To_World_Triangle_Buffer(OBJ_2_Buffer, world_Triangle_Buffer);
     
         //GLOBAL ROTATE WORLD SPACE AND MOVE VERTEX BUFFER IN WORLD SPACE
       // rotate_World_Vertex(x_angle, y_angle, z_angle, world_Space_Vertex_Buffer);
        //rotate_Obj_Triangle(x_angle, y_angle, z_angle, world_Triangle_Buffer);
     
         ////PASS VERTEX AND TRIANGLE BUFFER TO RENDER PIPELINE
         vertex_Rastor_Pipeline
         (
             0,
             0,
             320,
             240,
             z,
             FOV,
             projection_Type,
             back_Color,
             &vertex_Count,
             &bin,
             world_Space_Vertex_Buffer
         );
     
     
         triangle_Rastor_Pipeline
         (
             0,                           // WINDOW START X
             0,                           // WINDOW START Y
             320,                         // WINDOW SIZE X
             240,                         // WINDOW SIZE Y
             z,                           // CAMERA Z
             FOV,                         // CAMMERA FOV
             projection_Type,             // 0 = ORTHO 1 = PERSPECTIVE
             0,                           // LIGHT X
             0,                           // LIGHT Y
             0,                           // LIGHT Z
             true,                        // ENABLE BACKFACE CULLING
             false,                       // LIGHT ENABLE
             true,                       // TRIANGLE FILL ENABLE
             &bin,                        // TRIANGLES DRAWN
             world_Triangle_Buffer        // WORLD SPACE TRIANGLE BUFFER
         );
     
     
         //AUTO ROTATE OBJECTS IN MODEL SPACE
         x_angle_OBJ1 += .5;
         y_angle_OBJ1 += .2;
         if (x_angle_OBJ1 > 359) x_angle_OBJ1 = 0.0;
         if (y_angle_OBJ1 > 359) y_angle_OBJ1 = 0.0;
         //    
         x_angle_OBJ2 += .4;
         y_angle_OBJ2 += .5;
         if (x_angle_OBJ2 > 359) x_angle_OBJ2 = 0.0;
         if (y_angle_OBJ2 > 359) y_angle_OBJ2 = 0.0;
     
        // x_angle += .4;
         y_angle += 2.5;
        // if (x_angle > 359) x_angle = 0.0;
         if (y_angle > 359) y_angle = 0.0;
     
     
     
         //CLEAR BUFFERS FOR NEXT FRAME
         world_Space_Vertex_Buffer.clear();
         world_Triangle_Buffer.clear();
         OBJ_1_Buffer.clear();
         OBJ_2_Buffer.clear();
         FPS_Counter(current_Time, black);
         refreshScreen();
     
     }//END OF RENDER LOOP######################################################################################
     
     
}    
    
void scene_3D_World()
{    
     //CREATE BUFFER TO HOLD ALL WORLD VERTICIES
     vector<vertex3D> world_Space_Vertex_Buffer;
     
     int vertex_Count;
     int bin;
     
     //WORLD VIEW CAMERA SETTINGS
     int projection_Type = 1;
     float x_angle = 0;
     float y_angle = 0;
     float z_angle = 0;
     float z = 0; //camera zoom !!!!!!!!!!!!! NOW NOT USED
     float FOV = 60;//ZOOM smaller zoom in
     int back_Color = white;//36 sky blue
     
     vec3D current_Pos;
     float speed = 0.5f;
     
     
     while (1)
     {
         unsigned long current_Time = millis();
         switch (read_Keyboard_Port())
         {
         case LEFT_KEY_MAKE_CODE:
             y_angle += speed;
             if (y_angle >= 360) y_angle = 0;
             break;
     
         case RIGHT_KEY_MAKE_CODE:
             y_angle -= speed;
             if (y_angle < 0) y_angle = 360;
             break;
     
         case DOWN_KEY_MAKE_CODE:
             x_angle += speed;
             if (x_angle > 360) x_angle = 0;
             break;
     
         case UP_KEY_MAKE_CODE:
             x_angle -= speed;
             if (x_angle < 0) x_angle = 360;
             break;
     
     
         case Page_Down_Key_Make_Code :
             if (FOV < 90)
                 FOV += 0.5;
             break;
     
     
         case Page_Up_Key_Make_Code:
             if (FOV > 20)
                 FOV -= 0.5;
             break;
         }
     
         drawBackgroundColor(back_Color); 
         float X;
         float camera_Fov = 60;
         float screen_X_Size = 200;
         float screen_Y_Size = 100;
         bool ray_Hit = false;
         int color = blue;
         float current_Length = 0.0f;
     
          bool cast_En = true;
          // for (float y = 0; y < screen_Y_Size; y++)
          //{
     
             for (float x = 0; x < camera_Fov; x++)
             {
                 RAY dir_Ray;
                 dir_Ray.origin.x = 0;
                 dir_Ray.origin.y = 0;
                 dir_Ray.origin.z = 0;
                 dir_Ray.direction.x = 0;
                 dir_Ray.direction.y = 0;
                 dir_Ray.direction.z = 1;
     
                 m.cast_Ray_Into_Scene//per pixel
                 (
                     dir_Ray,
                     current_Pos,
                     &current_Length,
                     20.0f,
                     0.05f,
                     0,
                     x + y_angle,//fov , screen width
                     0,
                     &ray_Hit,
                     cast_En
                 );
                 //delay(1);
     
                 if (ray_Hit)
                 {
                     color = red;
                   //  ray_Hit = false;
                    X = current_Pos.x;
                 }
                 else
                 {
                     color = blue;
                 }
                 setPixel(x,  50, color);
     
                 printNumberDouble(X, 0, 0, red, black);
                 printNumberDouble(current_Pos.y, 0, 10, red, black);
                 printNumberDouble(current_Pos.z, 0, 20, red, black);
     
                   // delayMicroseconds(500);
               line_Gen_3D(dir_Ray.origin.x, dir_Ray.origin.y, dir_Ray.origin.z, current_Pos.x, current_Pos.y, current_Pos.z, random(1, 64), 1, world_Space_Vertex_Buffer);
             }
     
     
         ////END ONE RAY////////////////////////////////////////////////////////////////////////////
     
     
     
     
     
         //FRAME LOOP
     
        // write_Origin_Cross_Mesh_To_Vector(world_Space_Vertex_Buffer, 8, false, &bin);
     
           //GLOBAL ROTATE WORLD SPACE AND MOVE VERTEX BUFFER IN WORLD SPACE
           rotate_World_Vertex(x_angle, y_angle, z_angle, world_Space_Vertex_Buffer);
       
           ////PASS VERTEX AND TRIANGLE BUFFER TO RENDER PIPELINE
         vertex_Rastor_Pipeline
         (
             0,
             0,
             320,
             240, 
             z,
             FOV,
             projection_Type,
             back_Color,
             &vertex_Count,
             &bin,
             world_Space_Vertex_Buffer
         );
     
     
         //CLEAR BUFFERS FOR NEXT FRAME
         world_Space_Vertex_Buffer.clear();
     
         FPS_Counter(current_Time, black);
         refreshScreen();
     
     }//END OF RENDER LOOP######################################################################################
     
     
}    
    
void scene_3D_World_Template_Program_Basic()
{    
     //CREATE BUFFER TO HOLD ALL WORLD VERTICIES
     
     vector<triangle> world_Triangle_Buffer;
     vector<triangle>          OBJ_1_Buffer;
     
     int bin;
     
     
     
     //WORLD VIEW CAMERA SETTINGS
     int projection_Type = 1;
     float x_angle = 20;
     float y_angle = 0;
     float z_angle = 0;
     float z = 0; //camera zoom !!!!!!!!!!!!! NOW NOT USED
     float FOV = 60;//ZOOM smaller zoom in
     
     
     int back_Color = white;//36 sky blue
     bool rotate_En = false;
     
     
     //object position and angle
     float x_angle_OBJ1 = 0.0;
     float y_angle_OBJ1 = 0.0;
     float z_angle_OBJ1 = 0.0;
     float xPos1 = 0;
     float yPos1 = 0;
     float zPos1 = 0;
     
     
     
     while (1)
     {
         unsigned long current_Time = millis();//FPS COUNTER
     
         drawBackgroundColor(back_Color);
         read_Mouse_Cpu();
     
         if (mouse_Scroll_Up)
         {
             FOV -= .3;
             mouse_Scroll_Up = false;
         }
     
         if (mouse_Scroll_Down)
         {
             FOV += .3;
             mouse_Scroll_Down = false;
         }
     
         if (mouse_Left_Make)
         {
             rotate_En = true;
             mouse_Left_Make = false;
         }
     
     
         if (mouse_Left_Break)
         {
             rotate_En = false;
             mouse_Left_Break = false;
         }
     
         if (rotate_En && !mouse_Still)
         {
             if (mouse_Dir_X)   y_angle += (x_Delta*.1);
             if (!mouse_Dir_X)  y_angle -= (x_Delta*.1);
             if (y_angle < 0)   y_angle = 360;
             if (y_angle > 360) y_angle = 0;
     
     
             if (!mouse_Dir_Y)   x_angle += (y_Delta * .1);
             if (mouse_Dir_Y)  x_angle -= (y_Delta * .1);
             if (x_angle < 0)   x_angle = 360;
             if (x_angle > 360) x_angle = 0;
           //  rotate_En = false;
         }
     
     
     
     
     
     
         //FRAME RENDER LOOP
     
         // LOAD OBJECT DATA INTO MODEL SPACE     
        //translate_Vertex_Array_To_Triangle_Buff_UNIT_CUBE(UNIT_CUBE_vertex_Array, UNIT_CUBE_faces, OBJ_1_Buffer);
         triangle t;
         t.p1.x = 10;
         t.p1.y = 10;
         t.p1.z = 0;
     
         t.p2.x = 20;
         t.p2.y = 10;
         t.p2.z = 0;
     
         t.p3.x = 10;
         t.p3.y = 20;
         t.p3.z = 0;
         t.face_Color = green;
         t.id = 1;
         OBJ_1_Buffer.push_back (t);
     
          // ROTATE OBJECTS IN MODEL SPACE
          rotate_Obj_Triangle(x_angle, y_angle, z_angle_OBJ1, OBJ_1_Buffer);
     
          //MOVE OBJECTS IN MODEL SPACE
         translate_Move_Triangle_Buffer(xPos1, yPos1, zPos1, OBJ_1_Buffer);
      
     
     
         // MOVE OBJECTS FROM MODEL SPACE TO WORLD SPACE
         move_Obj_Triangle_Buffer_To_World_Triangle_Buffer(OBJ_1_Buffer, world_Triangle_Buffer);
      
     
           ////PASS TRIANGLE BUFFER TO RENDER PIPELINE
     
         triangle_Rastor_Pipeline
         (
             70,                          // WINDOW START X
             30,                          // WINDOW START Y
             180,                         // WINDOW SIZE X
             180,                         // WINDOW SIZE Y
             z,                           // CAMERA Z    !!!!!!!!!!!!!!!!!REMOVE!!!!!!!!!!!!!!!!!!
             FOV,                         // CAMMERA FOV
             projection_Type,             // 0 = ORTHO 1 = PERSPECTIVE
             0,                           // LIGHT X
             0,                           // LIGHT Y
             0,                           // LIGHT Z
             false,                       // ENABLE BACKFACE CULLING
             false,                       // LIGHT ENABLE
             true,                       // TRIANGLE FILL ENABLE
             &bin,                        // TRIANGLES DRAWN
             world_Triangle_Buffer        // WORLD SPACE TRIANGLE BUFFER
         );
     
     
     
         //CLEAR BUFFERS FOR NEXT FRAME  
         world_Triangle_Buffer.clear();
         OBJ_1_Buffer.clear();   
         FPS_Counter(current_Time, black);
         refreshScreen();
     }//END OF RENDER LOOP######################################################################################
     
     
}    
    
void midi_Montitor_Emulator(bool* enable_Out, bool enable_In)
{    
     if (enable_In)
     {
         static int background_Color = grey;
         vector<button> buttons_Buff;
         static int   active_Button_Id = 0;
         static float x_Start = 10;
         static float y_Start = 5;
         static int channel_Vels[16] = { 0 };
         static int channel_Vols[16] = { 0 };
         //READ MIDI PORT
         static byte status;
         static byte note;
         static byte velocity;
         static bool midi_Pass_Through = true;
         static int voices_On = 0;
         static int x_Button_Margin = 156;
      
     
         //create window
         window w1;
         w1.xStart = (int)x_Start;
         w1.yStart = (int)y_Start;
         w1.xSize = 300;
         w1.ySize = 225;
         w1.title_Bar_Color = window_Bar_Color;
         w1.w_bar_Color = bar_Color;
         w1.w_border_Color = border_Line_Color;
         w1.background_Color = background_Color;
         w1.window_Name = "MIDI MONITOR EMULATOR";
     
     
         //create buttons
         button exit;
         exit.xStart = (w1.xStart + w1.xSize) - 9;
         exit.yStart = w1.yStart + 1;
         exit.xSize = 8;
         exit.ySize = 8;
         exit.window_Ref_Id = w1.id;
         exit.id = 1;
         exit.offColor = red;
         exit.onColor = green;
         exit.fill_Type = 1;
         exit.texSize = 8;
         exit.text_Enable = false;
         exit.icon_Index_X = 0;
         exit.icon_Index_Y = 0;
         write_Icon_To_Button(exit, exit_Sprite);
         buttons_Buff.push_back(exit);
     
     
         button MIDI_Out_En;
         MIDI_Out_En.xSize = 66;
         MIDI_Out_En.ySize = 8;
         MIDI_Out_En.window_Ref_Id = w1.id;
         MIDI_Out_En.id = 2;
         MIDI_Out_En.offColor = bar_Color;
         MIDI_Out_En.onColor = green;
         MIDI_Out_En.text = "MIDI OUT EN";
         MIDI_Out_En.text_Enable = true;
         buttons_Buff.push_back(MIDI_Out_En);
     
     
         //MOUSE BUTTON  CLICK FLAGS
         if (mouse_Left_Make)
         {
             //PROGRAM BUTTONS
             switch (active_Button_Id)
             {
             case 1:
                 *enable_Out = false;
                 button_Click_Close_Window_Tone();
                 beep(exit_Beep_Pitch, 200);
                 send_Midi_Message(CH1_CC_STATUS, ALL_SOUND_OFF, 0);
                 for (int i = 0; i < 16; i++)
                 {
                     channel_Vels[i] = 0;
                     channel_Vols[i] = 0;
                     status = 0;
                     note = 0;
                     velocity = 0;
                     voices_On = 0;
                     midi_Pass_Through = true;
                 }
                 close_All_Programs();
                 break;//ESCAPE BUTTON
     
             case 2://data to midi out port enable button
                 if (midi_Pass_Through)  midi_Pass_Through = false;
                 else                    midi_Pass_Through = true;
                 send_Midi_Message(CH1_CC_STATUS, ALL_SOUND_OFF, 0);
                 voices_On = 0;
                 break;
             }
     
     
             mouse_Left_Make = false;
         }///////////////////////////////////////////////////////////////////////////////////
     
     
     
     
     
     
     
     
     
     
     
     
     
     
       
         //ROW DRIVER RAM
         static bool note_Reg_Row_1[90] = { 0 };
         static bool note_Reg_Row_2[90] = { 0 };
         static bool note_Reg_Row_3[90] = { 0 };
         static bool note_Reg_Row_4[90] = { 0 };
     
     
         //EMULATED HARDWARE FUNCTIONS
         auto set_Led = [=](int row, int led, bool state)//lambda function
         {
             switch (row)
             {
             case 0:   note_Reg_Row_1[led] = state; break;
             case 1:   note_Reg_Row_2[led] = state; break;
             case 2:   note_Reg_Row_3[led] = state; break;
             case 3:   note_Reg_Row_4[led] = state; break;
             }
         };
     
         auto clear_Screen = [=]()
         {
             for (int i = 0; i < 90; i++)
             {
                 note_Reg_Row_1[i] = 0;
                 note_Reg_Row_2[i] = 0;
                 note_Reg_Row_3[i] = 0;
                 note_Reg_Row_4[i] = 0;
             }
         };
     
         auto set_Bar_On = [=](byte note)
         {
             note_Reg_Row_1[note] = 1;
             note_Reg_Row_2[note] = 1;
             note_Reg_Row_3[note] = 1;
             note_Reg_Row_4[note] = 1;
         };
     
         auto set_Bar_Off = [=](byte note)
         {
             note_Reg_Row_1[note] = 0;
             note_Reg_Row_2[note] = 0;
             note_Reg_Row_3[note] = 0;
             note_Reg_Row_4[note] = 0;
         };
     
         auto midi_Note_On = [=](byte ch, byte pitch, byte vel)
         {
             switch (ch)
             {
             case 1: if (vel > 0) { set_Led(0, pitch - 23, true); voices_On ++;} if (vel == 0) {set_Led(0, pitch - 23, false); if(voices_On > 0) voices_On --; } channel_Vels[0] = vel; break;
             case 2: if (vel > 0) { set_Led(1, pitch - 23, true); voices_On ++;} if (vel == 0) {set_Led(1, pitch - 23, false); if(voices_On > 0) voices_On --; } channel_Vels[1] = vel; break;
             case 3: if (vel > 0) { set_Led(2, pitch - 23, true); voices_On ++;} if (vel == 0) {set_Led(2, pitch - 23, false); if(voices_On > 0) voices_On --; } channel_Vels[2] = vel; break;
             case 4: if (vel > 0) { set_Led(3, pitch - 23, true); voices_On ++;} if (vel == 0) {set_Led(3, pitch - 23, false); if(voices_On > 0) voices_On --; } channel_Vels[3] = vel; break;
             case 5: if (vel > 0) { set_Led(3, pitch - 23, true); voices_On ++;} if (vel == 0) {set_Led(3, pitch - 23, false); if(voices_On > 0) voices_On --; } channel_Vels[4] = vel; break;
             case 6: if (vel > 0) { set_Led(3, pitch - 23, true); voices_On ++;} if (vel == 0) {set_Led(3, pitch - 23, false); if(voices_On > 0) voices_On --; } channel_Vels[5] = vel; break;
             case 7: if (vel > 0) { set_Led(3, pitch - 23, true); voices_On ++;} if (vel == 0) {set_Led(3, pitch - 23, false); if(voices_On > 0) voices_On --; } channel_Vels[6] = vel; break;
             case 8: if (vel > 0) { set_Led(3, pitch - 23, true); voices_On ++;} if (vel == 0) {set_Led(3, pitch - 23, false); if(voices_On > 0) voices_On --; } channel_Vels[7] = vel; break;
             case 9: if (vel > 0) { set_Led(3, pitch - 23, true); voices_On ++;} if (vel == 0) {set_Led(3, pitch - 23, false); if(voices_On > 0) voices_On --; } channel_Vels[8] = vel; break;
             case 10:if (vel > 0) { set_Led(3, pitch - 23, true); voices_On ++;} if (vel == 0) {set_Led(3, pitch - 23, false); if(voices_On > 0) voices_On --; } channel_Vels[9] = vel; break;
             case 11:if (vel > 0) { set_Led(3, pitch - 23, true); voices_On ++;} if (vel == 0) {set_Led(3, pitch - 23, false); if(voices_On > 0) voices_On --; } channel_Vels[10] = vel; break;
             case 12:if (vel > 0) { set_Led(3, pitch - 23, true); voices_On ++;} if (vel == 0) {set_Led(3, pitch - 23, false); if(voices_On > 0) voices_On --; } channel_Vels[11] = vel; break;
             case 13:if (vel > 0) { set_Led(3, pitch - 23, true); voices_On ++;} if (vel == 0) {set_Led(3, pitch - 23, false); if(voices_On > 0) voices_On --; } channel_Vels[12] = vel; break;
             case 14:if (vel > 0) { set_Led(3, pitch - 23, true); voices_On ++;} if (vel == 0) {set_Led(3, pitch - 23, false); if(voices_On > 0) voices_On --; } channel_Vels[13] = vel; break;
             case 15:if (vel > 0) { set_Led(3, pitch - 23, true); voices_On ++;} if (vel == 0) {set_Led(3, pitch - 23, false); if(voices_On > 0) voices_On --; } channel_Vels[14] = vel; break;
             case 16:if (vel > 0) { set_Led(3, pitch - 23, true); voices_On ++;} if (vel == 0) {set_Led(3, pitch - 23, false); if(voices_On > 0) voices_On --; } channel_Vels[15] = vel; break;
             }
         };
     
         auto midi_Note_Off = [=](byte ch, byte pitch, byte vel)
         {
             switch (ch)
             {
             case 1:  set_Led(0, pitch - 23, false); channel_Vels[0] = 0;  if(voices_On > 0) voices_On--; break;
             case 2:  set_Led(1, pitch - 23, false); channel_Vels[1] = 0;  if(voices_On > 0) voices_On--; break;
             case 3:  set_Led(2, pitch - 23, false); channel_Vels[2] = 0;  if(voices_On > 0) voices_On--; break;
             case 4:  set_Led(3, pitch - 23, false); channel_Vels[3] = 0;  if(voices_On > 0) voices_On--; break;
             case 5:  set_Led(3, pitch - 23, false); channel_Vels[4] = 0;  if(voices_On > 0) voices_On--; break;
             case 6:  set_Led(3, pitch - 23, false); channel_Vels[5] = 0;  if(voices_On > 0) voices_On--; break;
             case 7:  set_Led(3, pitch - 23, false); channel_Vels[6] = 0;  if(voices_On > 0) voices_On--; break;
             case 8:  set_Led(3, pitch - 23, false); channel_Vels[7] = 0;  if(voices_On > 0) voices_On--; break;
             case 9:  set_Led(3, pitch - 23, false); channel_Vels[8] = 0;  if(voices_On > 0) voices_On--; break;
             case 10: set_Led(3, pitch - 23, false); channel_Vels[9] = 0;  if(voices_On > 0) voices_On--; break;
             case 11: set_Led(3, pitch - 23, false); channel_Vels[10] = 0; if(voices_On > 0) voices_On--; break;
             case 12: set_Led(3, pitch - 23, false); channel_Vels[11] = 0; if(voices_On > 0) voices_On--; break;
             case 13: set_Led(3, pitch - 23, false); channel_Vels[12] = 0; if(voices_On > 0) voices_On--; break;
             case 14: set_Led(3, pitch - 23, false); channel_Vels[13] = 0; if(voices_On > 0) voices_On--; break;
             case 15: set_Led(3, pitch - 23, false); channel_Vels[14] = 0; if(voices_On > 0) voices_On--; break;
             case 16: set_Led(3, pitch - 23, false); channel_Vels[15] = 0; if(voices_On > 0) voices_On--; break;
             }
         };
     
     
        //READ MIDI IN PORT
         while (CONTROL_PORT.available())
         {
             set_HDD_Led(1);
             status = CONTROL_PORT.read();
     
             if (status >= 128 && status <= 191)
             {
                 note = CONTROL_PORT.read();
                 velocity = CONTROL_PORT.read();
     
                 if (midi_Pass_Through) send_Midi_Message(status, note, velocity);
     
                 switch (status)
                 {
                 case CH1_NOTE_ON:    midi_Note_On(1,   note, velocity); break;
                 case CH2_NOTE_ON:    midi_Note_On(2,   note, velocity); break;
                 case CH3_NOTE_ON:    midi_Note_On(3,   note, velocity); break;
                 case CH4_NOTE_ON:    midi_Note_On(4,   note, velocity); break;
                 case CH5_NOTE_ON:    midi_Note_On(5,   note, velocity); break;
                 case CH6_NOTE_ON:    midi_Note_On(6,   note, velocity); break;
                 case CH7_NOTE_ON:    midi_Note_On(7,   note, velocity); break;
                 case CH8_NOTE_ON:    midi_Note_On(8,   note, velocity); break;
                 case CH9_NOTE_ON:    midi_Note_On(9,   note, velocity); break;
                 case CH10_NOTE_ON:   midi_Note_On(10,  note, velocity); break;
                 case CH11_NOTE_ON:   midi_Note_On(11,  note, velocity); break;
                 case CH12_NOTE_ON:   midi_Note_On(12,  note, velocity); break;
                 case CH13_NOTE_ON:   midi_Note_On(13,  note, velocity); break;
                 case CH14_NOTE_ON:   midi_Note_On(14,  note, velocity); break;
                 case CH15_NOTE_ON:   midi_Note_On(15,  note, velocity); break;
                 case CH16_NOTE_ON:   midi_Note_On(16,  note, velocity); break;
                 case CH1_NOTE_OFF:   midi_Note_Off(1,  note, velocity); break;
                 case CH2_NOTE_OFF:   midi_Note_Off(2,  note, velocity); break;
                 case CH3_NOTE_OFF:   midi_Note_Off(3,  note, velocity); break;
                 case CH4_NOTE_OFF:   midi_Note_Off(4,  note, velocity); break;
                 case CH5_NOTE_OFF:   midi_Note_Off(5,  note, velocity); break;
                 case CH6_NOTE_OFF:   midi_Note_Off(6,  note, velocity); break;
                 case CH7_NOTE_OFF:   midi_Note_Off(7,  note, velocity); break;
                 case CH8_NOTE_OFF:   midi_Note_Off(8,  note, velocity); break;
                 case CH9_NOTE_OFF:   midi_Note_Off(9,  note, velocity); break;
                 case CH10_NOTE_OFF:  midi_Note_Off(10, note, velocity); break;
                 case CH11_NOTE_OFF:  midi_Note_Off(11, note, velocity); break;
                 case CH12_NOTE_OFF:  midi_Note_Off(12, note, velocity); break;
                 case CH13_NOTE_OFF:  midi_Note_Off(13, note, velocity); break;
                 case CH14_NOTE_OFF:  midi_Note_Off(14, note, velocity); break;
                 case CH15_NOTE_OFF:  midi_Note_Off(15, note, velocity); break;
                 case CH16_NOTE_OFF:  midi_Note_Off(16, note, velocity); break;
     
     
     
                 case CH1_CC_STATUS:
                     switch (note)
                     {
                     case VOLUME:channel_Vols[0] = velocity; break;
                     case ALL_SOUND_OFF: clear_Screen(); channel_Vols[0] = 0; channel_Vels[0] = 0;  send_Midi_Message(status, ALL_SOUND_OFF, 0); break;
                     }
                     break;
     
                 case CH2_CC_STATUS:
                     switch (note)
                     {
                     case VOLUME:channel_Vols[1] = velocity; break;
                     case ALL_SOUND_OFF: clear_Screen(); channel_Vols[1] = 0; channel_Vels[1] = 0; send_Midi_Message(status, ALL_SOUND_OFF, 0);  break;
                     }
                     break;
     
                 case CH3_CC_STATUS:
                     switch (note)
                     {
                     case VOLUME:channel_Vols[2] = velocity; break;
                     case ALL_SOUND_OFF: clear_Screen(); channel_Vols[2] = 0; channel_Vels[2] = 0;  send_Midi_Message(status, ALL_SOUND_OFF, 0);  break;
                     }
                     break;
     
                 case CH4_CC_STATUS:
                     switch (note)
                     {
                     case VOLUME:channel_Vols[3] = velocity; break;
                     case ALL_SOUND_OFF: clear_Screen(); channel_Vols[3] = 0; channel_Vels[3] = 0; send_Midi_Message(status, ALL_SOUND_OFF, 0);  break;
                     }
                     break;
     
                 case CH5_CC_STATUS:
                     switch (note)
                     {
                     case VOLUME:channel_Vols[4] = velocity; break;
                     case ALL_SOUND_OFF: clear_Screen(); channel_Vols[4] = 0; channel_Vels[4] = 0; send_Midi_Message(status, ALL_SOUND_OFF, 0);  break;
                     }
                     break;
     
                 case CH6_CC_STATUS:
                     switch (note)
                     {
                     case VOLUME:channel_Vols[5] = velocity; break;
                     case ALL_SOUND_OFF: clear_Screen(); channel_Vols[5] = 0; channel_Vels[5] = 0; send_Midi_Message(status, ALL_SOUND_OFF, 0);  break;
                     }
                     break;
     
                 case CH7_CC_STATUS:
                     switch (note)
                     {
                     case VOLUME:channel_Vols[6] = velocity; break;
                     case ALL_SOUND_OFF: clear_Screen(); channel_Vols[6] = 0; channel_Vels[6] = 0; send_Midi_Message(status, ALL_SOUND_OFF, 0);  break;
                     }
                     break;
     
                 case CH8_CC_STATUS:
                     switch (note)
                     {
                     case VOLUME:channel_Vols[7] = velocity; break;
                     case ALL_SOUND_OFF: clear_Screen(); channel_Vols[7] = 0; channel_Vels[7] = 0; send_Midi_Message(status, ALL_SOUND_OFF, 0);  break;
                     }
                     break;
     
                 case CH9_CC_STATUS:
                     switch (note)
                     {
                     case VOLUME:channel_Vols[8] = velocity; break;
                     case ALL_SOUND_OFF: clear_Screen(); channel_Vols[8] = 0; channel_Vels[8] = 0; send_Midi_Message(status, ALL_SOUND_OFF, 0);  break;
                     }
                     break;
     
                 case CH10_CC_STATUS:
                     switch (note)
                     {
                     case VOLUME:channel_Vols[9] = velocity; break;
                     case ALL_SOUND_OFF: clear_Screen(); channel_Vols[9] = 0; channel_Vels[9] = 0; send_Midi_Message(status, ALL_SOUND_OFF, 0);  break;
                     }
                     break;
     
                 case CH11_CC_STATUS:
                     switch (note)
                     {
                     case VOLUME:channel_Vols[10] = velocity; break;
                     case ALL_SOUND_OFF: clear_Screen(); channel_Vols[10] = 0; channel_Vels[10] = 0; send_Midi_Message(status, ALL_SOUND_OFF, 0);  break;
                     }
                     break;
     
                 case CH12_CC_STATUS:
                     switch (note)
                     {
                     case VOLUME:channel_Vols[11] = velocity; break;
                     case ALL_SOUND_OFF: clear_Screen(); channel_Vols[11] = 0; channel_Vels[11] = 0; send_Midi_Message(status, ALL_SOUND_OFF, 0);  break;
                     }
                     break;
     
                 case CH13_CC_STATUS:
                     switch (note)
                     {
                     case VOLUME:channel_Vols[12] = velocity; break;
                     case ALL_SOUND_OFF: clear_Screen(); channel_Vols[12] = 0; channel_Vels[12] = 0; send_Midi_Message(status, ALL_SOUND_OFF, 0);  break;
                     }
                     break;
     
                 case CH14_CC_STATUS:
                     switch (note)
                     {
                     case VOLUME:channel_Vols[13] = velocity; break;
                     case ALL_SOUND_OFF: 
                         clear_Screen();
                         channel_Vols[13] = 0;
                         channel_Vels[13] = 0; 
                         send_Midi_Message(status, ALL_SOUND_OFF, 0); 
                         break;
                     }
                     break;
     
                 case CH15_CC_STATUS:
                     switch (note)
                     {
                     case VOLUME:channel_Vols[14] = velocity; break;
                     case ALL_SOUND_OFF: clear_Screen(); channel_Vols[14] = 0; channel_Vels[14] = 0;  send_Midi_Message(status, ALL_SOUND_OFF, 0);  break;
                     }
                     break;
     
                 case CH16_CC_STATUS:
                     switch (note)
                     {
                     case VOLUME:channel_Vols[15] = velocity; break;
                     case ALL_SOUND_OFF: clear_Screen(); channel_Vols[15] = 0; channel_Vels[15] = 0;  send_Midi_Message(status, ALL_SOUND_OFF, 0);  break;
                     }
                     break;
     
                 }//end of status switch read
             }
         }
     
     
     
     
         //#######################################################################################################################
        //GRAPHICS LOOP
     
         //DRAW WINDOW
         draw_Program_Window(w1);          
         
         
     
         //draw button margin line
         static int y_Button_Margin = w1.index_Y + 60;
         MIDI_Out_En.xStart = x_Button_Margin+2;//add button margin line
         MIDI_Out_En.yStart = y_Button_Margin+1;
     
         drawLine(x_Button_Margin, w1.index_Y + 60, x_Button_Margin, 229, bar_Color);
     
         //CHECK MOUSE POSITION AGAINST ALL BUTTONS IN BUFFER
         mouse_Icon_List_Handle(buttons_Buff, &active_Button_Id, mouseX, mouseY);                
     
     
     
         //midi output enable light
         if (midi_Pass_Through) drawSolidCircle(w1.index_X + 290, y_Button_Margin+5, 4, green);
         else                   drawSolidCircle(w1.index_X + 290, y_Button_Margin+5, 4, red);
     
         //DRAW VELOCITY BARS
         drawSolidRectangle(w1.index_X, w1.index_Y + 147, 145, 8, window_Bar_Color);
         print("CHANNEL VELOCITY", w1.index_X + 25, w1.index_Y + 147, window_Bar_Text_Color, window_Bar_Color);
         vertical_16_Value_Bar_Grapgh(w1.index_X, w1.index_Y + 155, 127, 8, 50, channel_Vels);
         //
     
         //DRAW VOLUME BARS
         drawSolidRectangle(w1.index_X, w1.index_Y + 87, 145, 8, window_Bar_Color);
         print("CHANNEL VOLUME", w1.index_X + 30, w1.index_Y + 87, window_Bar_Text_Color, window_Bar_Color);
         vertical_16_Value_Bar_Grapgh(w1.index_X, w1.index_Y + 95, 127, 8, 50, channel_Vols);
         //
     
         //DRAW VOICE ON BARS
         drawSolidRectangle(w1.index_X, w1.index_Y + 60, 145, 8, window_Bar_Color);
         drawSolidRectangle(w1.index_X, w1.index_Y + 68, 145, 19, black);
         printNumberInt(voices_On, w1.index_X + 130, w1.index_Y + 60, window_Bar_Text_Color, window_Bar_Color);
         print("SOUND CARD VOICES", w1.index_X + 10, w1.index_Y + 60, window_Bar_Text_Color, window_Bar_Color);
     
         int voice_Bar_X_Size = 6;
         int step_X = w1.index_X + 9;
         if (voices_On > 0 && voices_On <= 18)
         {
             for (int i = 0; i < voices_On; i++)
             {
                 drawSolidRectangle(step_X, w1.index_Y + 70, voice_Bar_X_Size, 15, red);
                 step_X += voice_Bar_X_Size + 1;
             }
         }
         //
     
     
     
         //DRAW DATA BOX
         drawSolidRectangle(w1.index_X, w1.index_Y + 40, 298, 20, black);
         //
         print("STATUS", w1.index_X + 3,   w1.index_Y + 40, green,  black);
         print("DATA 1", w1.index_X + 40,  w1.index_Y + 40, red,    black);
         print("DATA 2", w1.index_X + 80,  w1.index_Y + 40, red,    black);
         print("CHAN",   w1.index_X + 120, w1.index_Y + 40, yellow, black);
         print("NOTE",   w1.index_X + 150, w1.index_Y + 40, cyan,   black);
         print("EVENT",  w1.index_X + 180, w1.index_Y + 40, green, black);
         //
     
         static int text_Color;
         if (status >= CH1_NOTE_OFF && status <= CH16_NOTE_OFF)  text_Color = white;
         if (status >= CH1_NOTE_ON  && status <= CH16_NOTE_ON)   text_Color = green;
         printNumberInt(status,   w1.index_X + 7,  w1.index_Y + 50, text_Color, black);
         printNumberInt(note,     w1.index_X + 45, w1.index_Y + 50, text_Color, black);
         printNumberInt(velocity, w1.index_X + 85, w1.index_Y + 50, text_Color, black);
     
         //DET CHANEL NUMBER 1 - 16
           if (status == CH1_NOTE_ON  || status == CH1_NOTE_OFF  || status == CH1_CC_STATUS)  print("1",         w1.index_X + 130, w1.index_Y + 50, text_Color, black);
           if (status == CH2_NOTE_ON  || status == CH2_NOTE_OFF  || status == CH2_CC_STATUS)  print("2",         w1.index_X + 130, w1.index_Y + 50, text_Color, black);
           if (status == CH3_NOTE_ON  || status == CH3_NOTE_OFF  || status == CH3_CC_STATUS)  print("3",         w1.index_X + 130, w1.index_Y + 50, text_Color, black);
           if (status == CH4_NOTE_ON  || status == CH4_NOTE_OFF  || status == CH4_CC_STATUS)  print("4",         w1.index_X + 130, w1.index_Y + 50, text_Color, black);
           if (status == CH5_NOTE_ON  || status == CH5_NOTE_OFF  || status == CH5_CC_STATUS)  print("5",         w1.index_X + 130, w1.index_Y + 50, text_Color, black);
           if (status == CH6_NOTE_ON  || status == CH6_NOTE_OFF  || status == CH6_CC_STATUS)  print("6",         w1.index_X + 130, w1.index_Y + 50, text_Color, black);
           if (status == CH7_NOTE_ON  || status == CH7_NOTE_OFF  || status == CH7_CC_STATUS)  print("7",         w1.index_X + 130, w1.index_Y + 50, text_Color, black);
           if (status == CH8_NOTE_ON  || status == CH8_NOTE_OFF  || status == CH8_CC_STATUS)  print("8",         w1.index_X + 130, w1.index_Y + 50, text_Color, black);
           if (status == CH9_NOTE_ON  || status == CH9_NOTE_OFF  || status == CH9_CC_STATUS)  print("9",         w1.index_X + 130, w1.index_Y + 50, text_Color, black);
           if (status == CH10_NOTE_ON || status == CH10_NOTE_OFF || status == CH10_CC_STATUS) print("10",        w1.index_X + 130, w1.index_Y + 50, text_Color, black);
           if (status == CH11_NOTE_ON || status == CH11_NOTE_OFF || status == CH11_CC_STATUS) print("11",        w1.index_X + 130, w1.index_Y + 50, text_Color, black);
           if (status == CH12_NOTE_ON || status == CH12_NOTE_OFF || status == CH12_CC_STATUS) print("12",        w1.index_X + 130, w1.index_Y + 50, text_Color, black);
           if (status == CH13_NOTE_ON || status == CH13_NOTE_OFF || status == CH13_CC_STATUS) print("13",        w1.index_X + 130, w1.index_Y + 50, text_Color, black);
           if (status == CH14_NOTE_ON || status == CH14_NOTE_OFF || status == CH14_CC_STATUS) print("14",        w1.index_X + 130, w1.index_Y + 50, text_Color, black);
           if (status == CH15_NOTE_ON || status == CH15_NOTE_OFF || status == CH15_CC_STATUS) print("15",        w1.index_X + 130, w1.index_Y + 50, text_Color, black);
           if (status == CH16_NOTE_ON || status == CH16_NOTE_OFF || status == CH16_CC_STATUS) print("16",        w1.index_X + 130, w1.index_Y + 50, text_Color, black); 
           if (status >= CH1_NOTE_OFF && status <= CH16_NOTE_OFF)                             print("NOTE OFF",  w1.index_X + 180, w1.index_Y + 50, red, black);
           if (status >= CH1_NOTE_ON  && status <= CH16_NOTE_ON)                              print("NOTE ON",   w1.index_X + 180, w1.index_Y + 50, green, black);
     
     
         if (status >= CH1_CC_STATUS && status <= CH16_CC_STATUS)
         {
             switch (note)
             {
             case VOLUME:        print("VOLUME",        w1.index_X + 185, w1.index_Y + 50, white, black); break;
             case ALL_SOUND_OFF: print("ALL SOUND OFF", w1.index_X + 185, w1.index_Y + 50, white, black); break;
             }
         }
     
         //GET NOTE NAME
         switch (note)
         {
         case 0:   print("C-1",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 1:   print("C#-1", w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 2:   print("D-1",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 3:   print("D#-1", w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 4:   print("E-1",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 5:   print("F-1",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 6:   print("F#-1", w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 7:   print("G-1",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 8:   print("G#-1", w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 9:   print("A-1",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 10:  print("A#-1", w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 11:  print("B-1",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 12:  print("C0",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 13:  print("C#0",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 14:  print("D0",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 15:  print("D#0",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 16:  print("E0",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 17:  print("F0",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 18:  print("F#0",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 19:  print("G0",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 20:  print("G#0",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 21:  print("A0",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 22:  print("A#0",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 23:  print("B0",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 24:  print("C1",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 25:  print("C#1",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 26:  print("D1",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 27:  print("D#1",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 28:  print("E1",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 29:  print("F1",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 30:  print("F#1",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 31:  print("G1",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 32:  print("G#1",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 33:  print("A1",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 34:  print("A#1",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 35:  print("B1",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 36:  print("C2",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 37:  print("C#2",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 38:  print("D2",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 39:  print("D#2",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 40:  print("E2",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 41:  print("F2",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 42:  print("F#2",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 43:  print("G2",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 44:  print("G#2",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 45:  print("A2",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 46:  print("A#2",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 47:  print("B2",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 48:  print("C3",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 49:  print("C#3",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 50:  print("D3",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 51:  print("D#3",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 52:  print("E3",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 53:  print("F3",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 54:  print("F#3",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 55:  print("G3",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 56:  print("G#3",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 57:  print("A3",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 58:  print("A#3",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 59:  print("B3",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 60:  print("C4",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 61:  print("C#4",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 62:  print("D4",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 63:  print("D#4",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 64:  print("E4",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 65:  print("F4",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 66:  print("F#4",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 67:  print("G4",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 68:  print("G#4",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 69:  print("A4",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 70:  print("A#4",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 71:  print("B4",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 72:  print("C5",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 73:  print("C#5",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 74:  print("D5",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 75:  print("D#5",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 76:  print("E5",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 77:  print("F5",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 78:  print("F#5",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 79:  print("G5",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 80:  print("G#5",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 81:  print("A5",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 82:  print("A#5",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 83:  print("B5",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 84:  print("C6",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 85:  print("C#6",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 86:  print("D6",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 87:  print("D#6",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 88:  print("E6",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 89:  print("F6",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 90:  print("F#6",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 91:  print("G6",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 92:  print("G#6",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 93:  print("A6",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 94:  print("A#6",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 95:  print("B6",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 96:  print("C7",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 97:  print("C#7",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 98:  print("D7",   w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 99:  print("D#7",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 100:  print("E7",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 101:  print("F7",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 102:  print("F#7", w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 103:  print("G7",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 104:  print("G#7", w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 105:  print("A7",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 106:  print("A#7", w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 107:  print("B7",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 108:  print("C8",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 109:  print("C#8", w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 110:  print("D8",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 111:  print("D#8", w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 112:  print("E8",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 113:  print("F8",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 114:  print("F#8", w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 115:  print("G8",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 116:  print("G#8", w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 117:  print("A8",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 118:  print("A#8", w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 119:  print("B8",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 120:  print("C9",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 121:  print("C#9", w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 122:  print("D9",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 123:  print("D#9", w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 124:  print("E9",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 125:  print("F9",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 126:  print("F#9", w1.index_X + 155, w1.index_Y + 50, white, black); break;
         case 127:  print("G9",  w1.index_X + 155, w1.index_Y + 50, white, black); break;
         default:
             break;
         }
         //END DATA BOX
     
     
         //DRAW LED SCREEN
         int xStart = w1.index_X + 15;
         int yStart = w1.index_Y + 10;
         int space = 3;
         static byte led_Color = green;
         byte color;
         static bool over_Led_Screen;
         drawSolidRectangle(xStart - 5, yStart - 5, 279, 30, grey);
         drawSolidRectangle(xStart - 1, yStart - 1, 271, 22, black);
     
         //hover and scroll to change led color
         if (check_Button_Mouse(xStart - 5, yStart - 5, 279, 30, mouseX, mouseY))
         {
             if (mouse_Scroll_Up && led_Color < 63)  { led_Color++; mouse_Scroll_Up = false;   beep(1000, 10);}
             if (mouse_Scroll_Down && led_Color > 1) { led_Color--; mouse_Scroll_Down = false; beep(800,  10);}
             over_Led_Screen = true;
         }
         else over_Led_Screen = false;
     
         //draw leds
         for (int i = 0; i < 90; i++)
         {
             if (note_Reg_Row_1[i]) color = led_Color;
             else color = black;
             drawSolidRectangle(xStart, yStart, 2, 2, color);
     
             if (note_Reg_Row_2[i]) color = led_Color;
             else color = black;
             drawSolidRectangle(xStart, yStart + 6, 2, 2, color);
     
             if (note_Reg_Row_3[i]) color = led_Color;
             else color = black;
             drawSolidRectangle(xStart, yStart + 12, 2, 2, color);
     
             if (note_Reg_Row_4[i]) color = led_Color;
             else color = black;
             drawSolidRectangle(xStart, yStart + 18, 2, 2, color);
             xStart += space;
         }
     
         //hint text pop up
         if (over_Led_Screen)
         {
             if (mouseX < 180)   print("SCROLL TO SET COLOR", mouseX + 10,  mouseY, white, blue);
             if (mouseX >= 180)  print("SCROLL TO SET COLOR", mouseX - 120, mouseY, white, blue);
         }
     
         if(check_Button_Mouse(w1.index_X,            120, 8, 105, mouseX, mouseY))  print("1",  mouseX + 10, mouseY, white, blue);
         if(check_Button_Mouse(w1.index_X + (9 * 1),  120, 8, 105, mouseX, mouseY))  print("2",  mouseX + 10, mouseY, white, blue);
         if(check_Button_Mouse(w1.index_X + (9 * 2),  120, 8, 105, mouseX, mouseY))  print("3",  mouseX + 10, mouseY, white, blue);
         if(check_Button_Mouse(w1.index_X + (9 * 3),  120, 8, 105, mouseX, mouseY))  print("4",  mouseX + 10, mouseY, white, blue);
         if(check_Button_Mouse(w1.index_X + (9 * 4),  120, 8, 105, mouseX, mouseY))  print("5",  mouseX + 10, mouseY, white, blue);
         if(check_Button_Mouse(w1.index_X + (9 * 5),  120, 8, 105, mouseX, mouseY))  print("6",  mouseX + 10, mouseY, white, blue);
         if(check_Button_Mouse(w1.index_X + (9 * 6),  120, 8, 105, mouseX, mouseY))  print("7",  mouseX + 10, mouseY, white, blue);
         if(check_Button_Mouse(w1.index_X + (9 * 7),  120, 8, 105, mouseX, mouseY))  print("8",  mouseX + 10, mouseY, white, blue);
         if(check_Button_Mouse(w1.index_X + (9 * 8),  120, 8, 105, mouseX, mouseY))  print("9",  mouseX + 10, mouseY, white, blue);
         if(check_Button_Mouse(w1.index_X + (9 * 9),  120, 8, 105, mouseX, mouseY))  print("10", mouseX + 10, mouseY, white, blue);
         if(check_Button_Mouse(w1.index_X + (9 * 10), 120, 8, 105, mouseX, mouseY))  print("11", mouseX + 10, mouseY, white, blue);
         if(check_Button_Mouse(w1.index_X + (9 * 11), 120, 8, 105, mouseX, mouseY))  print("12", mouseX + 10, mouseY, white, blue);
         if(check_Button_Mouse(w1.index_X + (9 * 12), 120, 8, 105, mouseX, mouseY))  print("13", mouseX + 10, mouseY, white, blue);
         if(check_Button_Mouse(w1.index_X + (9 * 13), 120, 8, 105, mouseX, mouseY))  print("14", mouseX + 10, mouseY, white, blue);
         if(check_Button_Mouse(w1.index_X + (9 * 14), 120, 8, 105, mouseX, mouseY))  print("15", mouseX + 10, mouseY, white, blue);
         if(check_Button_Mouse(w1.index_X + (9 * 15), 120, 8, 105, mouseX, mouseY))  print("16", mouseX + 10, mouseY, white, blue);
        //
     
         set_HDD_Led(0);
         //#######################################################################################################################
     }
}    
    
void midi_Calculator()
{    
     while (1)
     {
     
         drawBackgroundColor(white);
     
         static int key = 23;
         static int scale_Type = 0;
         static int count = 0;
         int notes[6][7];
         print("KEY", 0, 0, yellow, blue); printNumberInt(key, 20, 0, red, blue);
         mid.get_Scale(key, scale_Type, notes);
         int xPos = 150;
         int yPos = 100;
         bool note_Pos[6][7] = { 0 };
         byte color;
         byte x = random(0, 7);
         byte y = random(0, 6);
     
         byte note = notes[y][x];
         note_Pos[y][x] = true;
     
         for (int y = 0; y < 6; y++)
         {
             for (int x = 0; x < 7; x++)
             {
                 if (note_Pos[y][x]) color = red;
                 else color = white;
                 printNumberInt(notes[y][x], xPos, yPos, black, color);
                 xPos += 20;
     
             }
             yPos += 10;
             xPos = 150;
         }
     
     
         send_Midi_Message(144, note, 127);
         send_Midi_Message(144, note + 5, 127);
         delay(100);
         send_Midi_Message(128, note, 0);
         send_Midi_Message(128, note + 5, 0);
         delay(100);
     
         count++;
         if (count > 50) { count = 0; scale_Type++; key = random(23, 35); }
         if (scale_Type > 1) scale_Type = 0;
         refreshScreen();
     }
}    
    
void hardware_Settings(bool* enable_Out, bool enable_In)
{    
     if (enable_In)
     {
         static int background_Color = white;
         vector<button> buttons_Buff;
         static int   active_Button_Id = 0;
     
         //create window
         window w1;
         w1.xStart = 100;
         w1.yStart = 70;
         w1.xSize = 120;
         w1.ySize = 100;
         w1.title_Bar_Color = window_Bar_Color;
         w1.w_bar_Color = bar_Color;
         w1.w_border_Color = border_Line_Color;
         w1.background_Color = background_Color;
         w1.window_Name = "HARDWARE SETTINGS";
         draw_Program_Window(w1);
     
         //MOUSE BUTTON  CLICK FLAGS
         if (mouse_Left_Make)
         {
             //PROGRAM BUTTONS
             switch (active_Button_Id)
             {
             case 1:
                // *enable_Out = false;
                 close_All_Programs();
                 button_Click_Close_Window_Tone();
                 beep(exit_Beep_Pitch, 200);
                 active_Button_Id = 0;
                 break;//ESCAPE BUTTON
     
             case 2:
                 *enable_Out = false;
                 close_All_Programs();
                 beep(enter_Beep_Pitch, 200);
                 active_Button_Id = 0;
                 break;//ESCAPE BUTTON
     
             case 3:
                 *enable_Out = false;
                 close_All_Programs();
                 beep(enter_Beep_Pitch, 200);
                 active_Button_Id = 0;
                 break;//ESCAPE BUTTON
             }
             mouse_Left_Make = false;
         }///////////////////////////////////////////////////////////////////////////////////
     
     
     
        //CREATE ALL BUTTONS 
         button exit;
         exit.xStart = (w1.xStart + w1.xSize) - 9;
         exit.yStart = w1.yStart + 1;
         exit.xSize = 8;
         exit.ySize = 8;
         exit.id = 1;
         exit.offColor = red;
         exit.onColor = green;
         exit.fill_Type = 1;
         exit.texSize = 8;
         exit.text_Enable = false;
         exit.icon_Index_X = 0;
         exit.icon_Index_Y = 0;
         write_Icon_To_Button(exit, exit_Sprite);
         buttons_Buff.push_back(exit);
     
     
     
         button MOUSE;
         MOUSE.xStart = w1.index_X;
         MOUSE.yStart = w1.index_Y;
         MOUSE.xSize = 48;
         MOUSE.ySize = 8;
         MOUSE.id = 2;
         MOUSE.offColor = white;
         MOUSE.onColor = green;
         MOUSE.fill_Type = 0;
         MOUSE.text_Enable = true;
         MOUSE.text = "MOUSE";
         buttons_Buff.push_back(MOUSE);
     
     
         button KEYBOARD;
         KEYBOARD.xStart = w1.index_X;
         KEYBOARD.yStart = w1.index_Y + 10;
         KEYBOARD.xSize = 48;
         KEYBOARD.ySize = 8;
         KEYBOARD.id = 3;
         KEYBOARD.offColor = white;
         KEYBOARD.onColor = green;
         KEYBOARD.fill_Type = 0;
         KEYBOARD.text_Enable = true;
         KEYBOARD.text = "KEYBOARD";
         buttons_Buff.push_back(KEYBOARD);
     
     
     
     
     
     
         //#######################################################################################################################
        //GRAPHICS LOOP
     
     
         mouse_Icon_List_Handle(buttons_Buff, &active_Button_Id, mouseX, mouseY);                //CHECK MOUSE POSITION AGAINST ALL BUTTONS IN BUFFER
     
     
     
     
         buttons_Buff.clear();
         //#######################################################################################################################
     }
}    
    
void appearance(bool* enable_Out, bool enable_In)
{    
     if (enable_In)
     {
         static int background_Color = white;
         vector<button> buttons_Buff;
          int   active_Button_Id = 0;
     
         //create window
         window w1;
         w1.xStart = 0;
         w1.yStart = 0;
         w1.xSize = 320;
         w1.ySize = 232;
         w1.title_Bar_Color = window_Bar_Color;
         w1.w_bar_Color = bar_Color;
         w1.w_border_Color = border_Line_Color;
         w1.background_Color = background_Color;
         w1.window_Name = "APPEARANCE";
         draw_Program_Window(w1);
     
     
     
     
         //CREATE ALL BUTTONS 
         button exit;
         exit.xStart = (w1.xStart + w1.xSize) - 9;
         exit.yStart = w1.yStart + 1;
         exit.xSize = 8;
         exit.ySize = 8;
         exit.id = 666;
         exit.offColor = red;
         exit.onColor = green;
         exit.fill_Type = 1;
         exit.texSize = 8;
         exit.text_Enable = false;
         exit.icon_Index_X = 0;
         exit.icon_Index_Y = 0;
         write_Icon_To_Button(exit, exit_Sprite);
         buttons_Buff.push_back(exit);
         mouse_Icon_List_Handle(buttons_Buff, &active_Button_Id, mouseX, mouseY);                //CHECK MOUSE POSITION AGAINST ALL BUTTONS IN BUFFER
         buttons_Buff.clear();
     
     
     
     
     
         //CREATE COLOR BUTTONS ROW 1 DESKTOP COLOR
         vector<button> color_Buttons_Buff1;
         int color_Button_xStart1 = w1.index_X + 165;
         int color_Button_yStart1 = w1.index_Y + 10;
         int step_Size = 10;
         int color_Button_xPos1 = color_Button_xStart1;
         int color_Button_yPos1 = color_Button_yStart1;
         int color_Button_Id1 = 1;
         //
         for (int x = 0; x < 15; x++)
         {
     
             button n1;
             n1.xStart = color_Button_xPos1;
             n1.yStart = color_Button_yPos1;
             n1.xSize = 9;
             n1.ySize = 9;
             n1.id = color_Button_Id1;
             n1.offColor = COLORS[x];
             n1.onColor = random(1, 48);
             n1.text_Enable = false;
             color_Buttons_Buff1.push_back(n1);
             color_Button_xPos1 += step_Size;
             color_Button_Id1++;
         }
         //
         mouse_Icon_List_Handle(color_Buttons_Buff1, &color_Button_Id1, mouseX, mouseY);      
         drawSolidRectangle(w1.index_X+1, w1.index_Y + 10, 10, 10, desktop_Color);
         print("DESKTOP", w1.index_X + 13, w1.index_Y + 10, black, white);
         color_Buttons_Buff1.clear();
         //end row 1
     
     
         //CREATE COLOR BUTTONS ROW 2 WINDOW BAR COLOR
         vector<button> color_Buttons_Buff2;
         int color_Button_xStart2 = w1.index_X + 165;
         int color_Button_yStart2 = w1.index_Y + 20;
         int color_Button_xPos2 = color_Button_xStart2;
         int color_Button_yPos2 = color_Button_yStart2;
         int color_Button_Id2 = 1;
         //
         for (int x = 0; x < 15; x++)
         {
     
             button n2;
             n2.xStart = color_Button_xPos2;
             n2.yStart = color_Button_yPos2;
             n2.xSize = 9;
             n2.ySize = 9;
             n2.id = color_Button_Id2;
             n2.offColor = COLORS[x];
             n2.onColor = random(1, 48);
             n2.text_Enable = false;
             color_Buttons_Buff2.push_back(n2);
             color_Button_xPos2 += step_Size;
             color_Button_Id2++;
         }
         //
         mouse_Icon_List_Handle(color_Buttons_Buff2, &color_Button_Id2, mouseX, mouseY);
         drawSolidRectangle(w1.index_X + 1, w1.index_Y + 20, 10, 10, window_Bar_Color);
         print("WINDOW BAR", w1.index_X + 13, w1.index_Y + 20, black, white);
         color_Buttons_Buff2.clear();
         //end row 2
     
     
     
         //CREATE COLOR BUTTONS ROW 3 BOARDER COLOR
         vector<button> color_Buttons_Buff3;
         int color_Button_xStart3 = w1.index_X + 165;
         int color_Button_yStart3 = w1.index_Y + 30;
         int color_Button_xPos3 = color_Button_xStart3;
         int color_Button_yPos3 = color_Button_yStart3;
         int color_Button_Id3 = 1;
         //
         for (int x = 0; x < 15; x++)
         {
     
             button n3;
             n3.xStart = color_Button_xPos3;
             n3.yStart = color_Button_yPos3;
             n3.xSize = 9;
             n3.ySize = 9;
             n3.id = color_Button_Id3;
             n3.offColor = COLORS[x];
             n3.onColor = random(1, 48);
             n3.text_Enable = false;
             color_Buttons_Buff3.push_back(n3);
             color_Button_xPos3 += step_Size;
             color_Button_Id3++;
         }
         //
         mouse_Icon_List_Handle(color_Buttons_Buff3, &color_Button_Id3, mouseX, mouseY);
         drawSolidRectangle(w1.index_X + 1, w1.index_Y + 30, 10, 10, bar_Color);
         print("BAR", w1.index_X + 13, w1.index_Y + 30, black, white);
         color_Buttons_Buff3.clear();
         //end row 3
     
     
     
     
     
         //MOUSE BUTTON  CLICK FLAGS
         if (mouse_Left_Make)
         {
            
             
             //PROGRAM BUTTONS
             switch (active_Button_Id)
             {
             case 666://EXIT BUTTON
                 close_All_Programs();
                 button_Click_Close_Window_Tone();
                 beep(exit_Beep_Pitch, 200);
                 break;
             }
     
             switch (color_Button_Id1)
             {
             case 1:   desktop_Color = COLORS[0]; beep(enter_Beep_Pitch, 50);break;
             case 2:   desktop_Color = COLORS[1]; beep(enter_Beep_Pitch, 50);break;
             case 3:   desktop_Color = COLORS[2]; beep(enter_Beep_Pitch, 50);break;
             case 4:   desktop_Color = COLORS[3]; beep(enter_Beep_Pitch, 50);break;
             case 5:   desktop_Color = COLORS[4]; beep(enter_Beep_Pitch, 50);break;
             case 6:   desktop_Color = COLORS[5]; beep(enter_Beep_Pitch, 50);break;
             case 7:   desktop_Color = COLORS[6]; beep(enter_Beep_Pitch, 50);break;
             case 8:   desktop_Color = COLORS[7]; beep(enter_Beep_Pitch, 50);break;
             case 9:   desktop_Color = COLORS[8]; beep(enter_Beep_Pitch, 50);break;
             case 10:  desktop_Color = COLORS[9]; beep(enter_Beep_Pitch, 50);break;
             case 11:  desktop_Color = COLORS[10];beep(enter_Beep_Pitch, 50); break;
             case 12:  desktop_Color = COLORS[11];beep(enter_Beep_Pitch, 50); break;
             case 13:  desktop_Color = COLORS[12];beep(enter_Beep_Pitch, 50); break;
             case 14:  desktop_Color = COLORS[13];beep(enter_Beep_Pitch, 50); break;
             case 15:  desktop_Color = COLORS[14];beep(enter_Beep_Pitch, 50); break;
             }
     
             switch (color_Button_Id2)
             {
             case 1:   window_Bar_Color = COLORS[0]; beep(enter_Beep_Pitch, 50);break;
             case 2:   window_Bar_Color = COLORS[1]; beep(enter_Beep_Pitch, 50);break;
             case 3:   window_Bar_Color = COLORS[2]; beep(enter_Beep_Pitch, 50);break;
             case 4:   window_Bar_Color = COLORS[3]; beep(enter_Beep_Pitch, 50);break;
             case 5:   window_Bar_Color = COLORS[4]; beep(enter_Beep_Pitch, 50);break;
             case 6:   window_Bar_Color = COLORS[5]; beep(enter_Beep_Pitch, 50);break;
             case 7:   window_Bar_Color = COLORS[6]; beep(enter_Beep_Pitch, 50);break;
             case 8:   window_Bar_Color = COLORS[7]; beep(enter_Beep_Pitch, 50);break;
             case 9:   window_Bar_Color = COLORS[8]; beep(enter_Beep_Pitch, 50);break;
             case 10:  window_Bar_Color = COLORS[9]; beep(enter_Beep_Pitch, 50);break;
             case 11:  window_Bar_Color = COLORS[10];beep(enter_Beep_Pitch, 50); break;
             case 12:  window_Bar_Color = COLORS[11];beep(enter_Beep_Pitch, 50); break;
             case 13:  window_Bar_Color = COLORS[12];beep(enter_Beep_Pitch, 50); break;
             case 14:  window_Bar_Color = COLORS[13];beep(enter_Beep_Pitch, 50); break;
             case 15:  window_Bar_Color = COLORS[14];beep(enter_Beep_Pitch, 50); break;
             }                                   
     
             switch (color_Button_Id3)
             {
             case 1:   bar_Color = COLORS[0]; beep(enter_Beep_Pitch, 50);break;
             case 2:   bar_Color = COLORS[1]; beep(enter_Beep_Pitch, 50);break;
             case 3:   bar_Color = COLORS[2]; beep(enter_Beep_Pitch, 50);break;
             case 4:   bar_Color = COLORS[3]; beep(enter_Beep_Pitch, 50);break;
             case 5:   bar_Color = COLORS[4]; beep(enter_Beep_Pitch, 50);break;
             case 6:   bar_Color = COLORS[5]; beep(enter_Beep_Pitch, 50);break;
             case 7:   bar_Color = COLORS[6]; beep(enter_Beep_Pitch, 50);break;
             case 8:   bar_Color = COLORS[7]; beep(enter_Beep_Pitch, 50);break;
             case 9:   bar_Color = COLORS[8]; beep(enter_Beep_Pitch, 50);break;
             case 10:  bar_Color = COLORS[9]; beep(enter_Beep_Pitch, 50);break;
             case 11:  bar_Color = COLORS[10];beep(enter_Beep_Pitch, 50); break;
             case 12:  bar_Color = COLORS[11];beep(enter_Beep_Pitch, 50); break;
             case 13:  bar_Color = COLORS[12];beep(enter_Beep_Pitch, 50); break;
             case 14:  bar_Color = COLORS[13];beep(enter_Beep_Pitch, 50); break;
             case 15:  bar_Color = COLORS[14];beep(enter_Beep_Pitch, 50); break;
             }
     
   /*          switch (color_Button_Id4)
            {
            case 1:   border_Line_Color = COLORS[0]; beep(enter_Beep_Pitch, 50); break;
            case 2:   border_Line_Color = COLORS[1]; beep(enter_Beep_Pitch, 50); break;
            case 3:   border_Line_Color = COLORS[2]; beep(enter_Beep_Pitch, 50); break;
            case 4:   border_Line_Color = COLORS[3]; beep(enter_Beep_Pitch, 50); break;
            case 5:   border_Line_Color = COLORS[4]; beep(enter_Beep_Pitch, 50); break;
            case 6:   border_Line_Color = COLORS[5]; beep(enter_Beep_Pitch, 50); break;
            case 7:   border_Line_Color = COLORS[6]; beep(enter_Beep_Pitch, 50); break;
            case 8:   border_Line_Color = COLORS[7]; beep(enter_Beep_Pitch, 50); break;
            case 9:   border_Line_Color = COLORS[8]; beep(enter_Beep_Pitch, 50); break;
            case 10:  border_Line_Color = COLORS[9]; beep(enter_Beep_Pitch, 50); break;
            case 11:  border_Line_Color = COLORS[10]; beep(enter_Beep_Pitch, 50); break;
            case 12:  border_Line_Color = COLORS[11]; beep(enter_Beep_Pitch, 50); break;
            case 13:  border_Line_Color = COLORS[12]; beep(enter_Beep_Pitch, 50); break;
            case 14:  border_Line_Color = COLORS[13]; beep(enter_Beep_Pitch, 50); break;
            case 15:  border_Line_Color = COLORS[14]; beep(enter_Beep_Pitch, 50); break;
            }*/
             mouse_Left_Make = false;
         }
     }
}    
    
void PROGRAM_3D_MODEL(bool* enable_Out, bool enable_In)
{
    if (enable_In)
    {
        
        vector<button>       buttons_Buff;
        vector<vertex3D>     world_Space_Vertex_Buffer;
        vector<triangle>     world_Triangle_Buffer;
        vector<triangle>     OBJ_1_Buffer;

 
        static int active_Button_Id = 0;
        static int bin;



        //WORLD VIEW CAMERA SETTINGS
        static int projection_Type = 1;
        static float x_angle = 0;
        static float y_angle = 0;
        static float z_angle = 0;
        static float z = 0; //camera zoom !!!!!!!!!!!!! NOW NOT USED
        static float FOV = 60;//ZOOM smaller zoom in
        static float rotate_Speed = 0.3;
        static float zoom_Speed = 0.5;


        static int back_Color = black;//36 sky blue
        static bool rotate_En = false;


        //object position and angle
        static float x_angle_OBJ1 = 0.0;
        static float y_angle_OBJ1 = 0.0;
        static float z_angle_OBJ1 = 0.0;
        static float xPos1 = 0;
        static float yPos1 = 0;
        static float zPos1 = 10;


        //create window
        window w1;
        w1.xStart = 50;
        w1.yStart = 1;
        w1.xSize = 220;
        w1.ySize = 220;
        w1.title_Bar_Color = window_Bar_Color;
        w1.w_bar_Color = bar_Color;
        w1.w_border_Color = border_Line_Color;
        w1.background_Color = back_Color;
        w1.window_Name = "3D MODEL";
        draw_Program_Window(w1);
       

        //HANDLE MOUSE DATA
        if (mouse_Left_Make)
        {
            //PROGRAM BUTTONS
            switch (active_Button_Id)
            {
            case 1://EXIT
                close_All_Programs();
                button_Click_Close_Window_Tone();
                beep(exit_Beep_Pitch, 200);
                active_Button_Id = 0;
                mouse_Left_Make = false;
                break;//ESCAPE BUTTON
            }

            rotate_En = true;
            mouse_Left_Make = false;
        }


        //ZOOM IN/OUT
        if (mouse_Scroll_Up)
        {
            FOV -= zoom_Speed;
            mouse_Scroll_Up = false;
        }

        if (mouse_Scroll_Down)
        {
            FOV += zoom_Speed;
            mouse_Scroll_Down = false;
        }

        //CLICK AND DRAG ROTATE
        //if (mouse_Left_Make)
        //{
           // rotate_En = true;
         //   mouse_Left_Make = false;
       // }

        if (mouse_Left_Break)
        {
            rotate_En = false;
            mouse_Left_Break = false;
        }

        if (rotate_En && !mouse_Still)
        {
            if (mouse_Dir_X)   y_angle += (x_Delta * rotate_Speed);
            if (!mouse_Dir_X)  y_angle -= (x_Delta * rotate_Speed);
            if (y_angle < 0)   y_angle = 360;
            if (y_angle > 360) y_angle = 0;
            if (y_angle > 90 && y_angle < 270)
            {
                if (!mouse_Dir_Y)   x_angle += (y_Delta * rotate_Speed);
                if (mouse_Dir_Y)    x_angle -= (y_Delta * rotate_Speed);
                if (x_angle < 0)    x_angle = 360;
                if (x_angle > 360)  x_angle = 0;
            }

            if (y_angle < 90 && y_angle > 0 || y_angle < 360 && y_angle > 270)
            {
                if (mouse_Dir_Y)     x_angle += (y_Delta * rotate_Speed);
                if (!mouse_Dir_Y)    x_angle -= (y_Delta * rotate_Speed);
                if (x_angle < 0)     x_angle = 360;
                if (x_angle > 360)   x_angle = 0;
            }
        }
        
        
        
        
        
        ///////////////////////////////////////////////////////////////////////////////////
       //CREATE ALL BUTTONS 
        button exit;
        exit.xStart = (w1.xStart + w1.xSize) - 9;
        exit.yStart = w1.yStart + 1;
        exit.xSize = 8;
        exit.ySize = 8;
        exit.id = 1;
        exit.offColor = red;
        exit.onColor = green;
        exit.fill_Type = 1;
        exit.texSize = 8;
        exit.text_Enable = false;
        exit.icon_Index_X = 0;
        exit.icon_Index_Y = 0;
        write_Icon_To_Button(exit, exit_Sprite);
        buttons_Buff.push_back(exit);



        //#######################################################################################################################
        //GRAPHICS LOOP

            write_Origin_Cross_Mesh_To_Vector(world_Space_Vertex_Buffer, 15, false, &bin);

             translate_Vertex_Array_To_Triangle_Buff_UNIT_ICOSAHEDRON(UNIT_ICOSAHEDRON_MESH_Vertex_Array, UNIT_ICOSAHEDRON_MESH_face_Array, OBJ_1_Buffer);


            // LOAD OBJECT DATA INTO MODEL SPACE     
            translate_Vertex_Array_To_Triangle_Buff_UNIT_CUBE(UNIT_CUBE_vertex_Array, UNIT_CUBE_faces, OBJ_1_Buffer);
   

            // ROTATE OBJECTS IN MODEL SPACE
            rotate_Obj_Triangle(x_angle, y_angle, z_angle_OBJ1, OBJ_1_Buffer);


            //MOVE OBJECTS IN MODEL SPACE
            translate_Move_Triangle_Buffer(xPos1, yPos1, zPos1, OBJ_1_Buffer);


            // MOVE OBJECTS FROM MODEL SPACE TO WORLD SPACE
            move_Obj_Triangle_Buffer_To_World_Triangle_Buffer(OBJ_1_Buffer, world_Triangle_Buffer);


            rotate_World_Vertex(x_angle, y_angle, z_angle, world_Space_Vertex_Buffer);
            ////PASS TRIANGLE BUFFER TO RENDER PIPELINE

        

            vertex_Rastor_Pipeline
            (
                w1.index_X,                 
                w1.index_Y,                 
                w1.xSize - 2,               
                w1.ySize - 18,
                z,
                FOV,
                projection_Type,
                back_Color,//!!!!!!!!!!REMOVE!!!!!!!!!!!!!!!!!!
                &bin,
                &bin,
                world_Space_Vertex_Buffer
            );

            triangle_Rastor_Pipeline
            (
                w1.index_X,                  // WINDOW START X
                w1.index_Y,                  // WINDOW START Y
                w1.xSize - 2,                // WINDOW SIZE X
                w1.ySize - 18,               // WINDOW SIZE Y
                z,                           // CAMERA Z    !!!!!!!!!!!!!!!!!REMOVE!!!!!!!!!!!!!!!!!!
                FOV,                         // CAMMERA FOV
                projection_Type,             // 0 = ORTHO 1 = PERSPECTIVE
                0,                           // LIGHT X
                0,                           // LIGHT Y
                0,                           // LIGHT Z
                true,                       // ENABLE BACKFACE CULLING
                false,                       // LIGHT ENABLE
                false,                       // TRIANGLE FILL ENABLE
                &bin,                        // TRIANGLES DRAWN
                world_Triangle_Buffer        // WORLD SPACE TRIANGLE BUFFER
            );

            print("XA", w1.index_X + 1, w1.index_Y, red, black); printNumberDouble(x_angle, w1.index_X + 15, w1.index_Y, green, black);
            print("YA", w1.index_X + 1, w1.index_Y+10, red, black); printNumberDouble(y_angle, w1.index_X + 15, w1.index_Y+10, green, black);


            //CLEAR BUFFERS FOR NEXT FRAME  
            world_Triangle_Buffer.clear();
            OBJ_1_Buffer.clear();
            world_Space_Vertex_Buffer.clear();

            //DRAW BUTTONS
            mouse_Icon_List_Handle(buttons_Buff, &active_Button_Id, mouseX, mouseY);
            buttons_Buff.clear();
    }
}

void start_Up_Data_Screen()
    {
    bool escape_Flag = true;

    while (escape_Flag)
    {
        drawBackgroundColor(black);
        print("HARDWARE DIAGNOSTICS", 100, 0, white, blue);



        refreshScreen();
    }
    }

void new_Program_Name(bool* enable_Out, bool enable_In)
{
    if (enable_In)
    {
        
        vector<button> buttons_Buff;
        static int   active_Button_Id = 0;
        static int background_Color = black;
       

        //create window
        window w1;
        w1.xStart = 10;
        w1.yStart = 10;
        w1.xSize = 200;
        w1.ySize = 200;
        w1.title_Bar_Color = window_Bar_Color;
        w1.w_bar_Color = bar_Color;
        w1.w_border_Color = border_Line_Color;
        w1.background_Color = background_Color;
        w1.window_Name = " new_Program_Name";
        draw_Program_Window(w1);

        slide_Bar_Int bar;
        bar.xStart = w1.index_X + 10;
        bar.yStart = w1.index_Y + 70;
        bar.back_Color = grey;
        bar.bar_Color = green;
        bar.slide_Color = red;
        bar.boarder_Color = white;
        int start_Value = 70;
        float output_Range = 127;
        float output;
        bool en;
        vertical_Slide_Bar(bar, output_Range,&output, &en, start_Value);

        printNumberDouble(output, w1.index_X + 35,w1.index_Y + 70, green, black);
        printNumberInt((int)en,   w1.index_X + 35,w1.index_Y + 80, green, black);


        //CREATE ALL BUTTONS 
        button exit;
        exit.xStart = (w1.xStart + w1.xSize) - 9;
        exit.yStart = w1.yStart + 1;
        exit.xSize = 8;
        exit.ySize = 8;
        exit.id = 1;
        exit.fill_Type = 1;
        exit.texSize = 8;
        exit.text_Enable = false;
        exit.icon_Index_X = 0;
        exit.icon_Index_Y = 0;
        write_Icon_To_Button(exit, exit_Sprite);
        buttons_Buff.push_back(exit);



        button b1;
        b1.onColor = green;
        b1.offColor = grey;
        b1.xStart = w1.index_X + 10;
        b1.yStart = w1.index_Y + 10;
        b1.xSize = 30;
        b1.ySize = 30;
        b1.id = 2;
        b1.text_Enable = false;
        b1.fill_Type = 0;
        buttons_Buff.push_back(b1);


        button b2;
        b2.xStart = w1.index_X + 10;
        b2.yStart = w1.index_Y + 50;
        b2.xSize = 66;
        b2.ySize = 8;
        b2.id = 3;
        b2.text = "TEXT BUTTON";
        b2.text_Enable = true;
        b2.fill_Type = 0;
        buttons_Buff.push_back(b2);


 
        //#######################################################################################################################
       //GRAPHICS LOOP

        mouse_Icon_List_Handle(buttons_Buff, &active_Button_Id, mouseX, mouseY);


        print("ID", w1.index_X, w1.index_Y - 8, red, bar_Color);
        printNumberInt(active_Button_Id, w1.index_X+15, w1.index_Y - 8, red, bar_Color);


        //MOUSE BUTTON  CLICK FLAGS
        if (mouse_Left_Make)
        {
            //PROGRAM BUTTONS
            switch (active_Button_Id)
            {
            case 1://ICON
                close_All_Programs();
                button_Click_Close_Window_Tone();
                beep(exit_Beep_Pitch, 200);
                mouse_Left_Make = false;
                break;

            case 2://FLAT SHADED      
                button_Click_Enter_Tone();
                mouse_Left_Make = false;
                break;

            case 3://TEXT BOX
                button_Click_Close_Window_Tone();
                beep(exit_Beep_Pitch, 200);
                mouse_Left_Make = false;
                break;
            }
            
        }



        buttons_Buff.clear();
        //#######################################################################################################################
    }
}
  
void PROGRAM_3D_MODEL_EDITOR(bool* enable_Out, bool enable_In)
{
    if (enable_In)
    {

        vector<button>       buttons_Buff;
        vector<vertex3D>     world_Space_Vertex_Buffer;
        vector<triangle>     world_Triangle_Buffer;
        vector<triangle>     OBJ_1_Buffer;


        static int active_Button_Id = 0;
        static int bin;



        //WORLD VIEW CAMERA SETTINGS
        static int projection_Type = 1;
        static float x_angle = 0;
        static float y_angle = 0;
        static float z_angle = 0;
        static float z = 0; //camera zoom !!!!!!!!!!!!! NOW NOT USED
        static float FOV = 60;//ZOOM smaller zoom in
        static float rotate_Speed = 0.3;
        static float zoom_Speed = 0.5;


        static int back_Color = black;//36 sky blue
        static bool rotate_En = false;


        //object position and angle
        static float x_angle_OBJ1 = 0.0;
        static float y_angle_OBJ1 = 0.0;
        static float z_angle_OBJ1 = 0.0;
        static float xPos1 = 0;
        static float yPos1 = 0;
        static float zPos1 = 10;


        //create window
        window w1;
        w1.xStart = 0;
        w1.yStart = 0;
        w1.xSize = 320;
        w1.ySize = 230;
        w1.title_Bar_Color = window_Bar_Color;
        w1.w_bar_Color = bar_Color;
        w1.w_border_Color = border_Line_Color;
        w1.background_Color = white;
        w1.window_Name = " 3D MODEL EDITOR";
        draw_Program_Window(w1);


        //HANDLE MOUSE DATA
        if (mouse_Left_Make)
        {
            //PROGRAM BUTTONS
            switch (active_Button_Id)
            {
            case 1://EXIT
                close_All_Programs();
                button_Click_Close_Window_Tone();
                beep(exit_Beep_Pitch, 200);
                active_Button_Id = 0;
               // mouse_Left_Make = false;
                break;//ESCAPE BUTTON

            case 2://VIEW PORT ++
                
                beep(exit_Beep_Pitch, 200);
                
              //  mouse_Left_Make = false;
                break;//ESCAPE BUTTON
            }

            rotate_En = true;
          //  mouse_Left_Make = false;
        }

       
        //ZOOM IN/OUT
        if (mouse_Scroll_Up)
        {
            FOV -= zoom_Speed;
            mouse_Scroll_Up = false;
        }

        if (mouse_Scroll_Down)
        {
            FOV += zoom_Speed;
            mouse_Scroll_Down = false;
        }

        //CLICK AND DRAG ROTATE
        //if (mouse_Left_Make)
        //{
           // rotate_En = true;
         //   mouse_Left_Make = false;
       // }

        if (mouse_Left_Break)
        {
            rotate_En = false;
            mouse_Left_Break = false;
        }

        if (rotate_En && !mouse_Still)
        {
            if (mouse_Dir_X)   y_angle += (x_Delta * rotate_Speed);
            if (!mouse_Dir_X)  y_angle -= (x_Delta * rotate_Speed);
            if (y_angle < 0)   y_angle = 360;
            if (y_angle > 360) y_angle = 0;
            if (y_angle > 90 && y_angle < 270)
            {
                if (!mouse_Dir_Y)   x_angle += (y_Delta * rotate_Speed);
                if (mouse_Dir_Y)    x_angle -= (y_Delta * rotate_Speed);
                if (x_angle < 0)    x_angle = 360;
                if (x_angle > 360)  x_angle = 0;
            }

            if (y_angle < 90 && y_angle > 0 || y_angle < 360 && y_angle > 270)
            {
                if (mouse_Dir_Y)     x_angle += (y_Delta * rotate_Speed);
                if (!mouse_Dir_Y)    x_angle -= (y_Delta * rotate_Speed);
                if (x_angle < 0)     x_angle = 360;
                if (x_angle > 360)   x_angle = 0;
            }
        }





        ///////////////////////////////////////////////////////////////////////////////////
       //CREATE ALL BUTTONS 
        button exit;
        exit.xStart = (w1.xStart + w1.xSize) - 9;
        exit.yStart = w1.yStart + 1;
        exit.xSize = 8;
        exit.ySize = 8;
        exit.id = 1;
        exit.offColor = red;
        exit.onColor = green;
        exit.fill_Type = 1;
        exit.texSize = 8;
        exit.text_Enable = false;
        exit.icon_Index_X = 0;
        exit.icon_Index_Y = 0;
        write_Icon_To_Button(exit, exit_Sprite);
        buttons_Buff.push_back(exit);

        button view_Port_Size_Plus;
        view_Port_Size_Plus.xStart = (w1.xStart + w1.xSize) - 20;
        view_Port_Size_Plus.yStart = w1.yStart + 20;
        view_Port_Size_Plus.xSize = 11;
        view_Port_Size_Plus.ySize = 11;
        view_Port_Size_Plus.id = 2;
        view_Port_Size_Plus.offColor = red;
        view_Port_Size_Plus.onColor = green;
        view_Port_Size_Plus.fill_Type = 1;
        view_Port_Size_Plus.texSize = 11;
        view_Port_Size_Plus.text_Enable = false;
        view_Port_Size_Plus.icon_Index_X = 0;
        view_Port_Size_Plus.icon_Index_Y = 0;
        write_Icon_To_Button(view_Port_Size_Plus, Plus_Button);
        buttons_Buff.push_back(view_Port_Size_Plus);

















        //#######################################################################################################################
        //GRAPHICS LOOP

        write_Origin_Cross_Mesh_To_Vector(world_Space_Vertex_Buffer, 15, false, &bin);

        translate_Vertex_Array_To_Triangle_Buff_UNIT_ICOSAHEDRON(UNIT_ICOSAHEDRON_MESH_Vertex_Array, UNIT_ICOSAHEDRON_MESH_face_Array, OBJ_1_Buffer);


        // LOAD OBJECT DATA INTO MODEL SPACE     
        translate_Vertex_Array_To_Triangle_Buff_UNIT_CUBE(UNIT_CUBE_vertex_Array, UNIT_CUBE_faces, OBJ_1_Buffer);


        // ROTATE OBJECTS IN MODEL SPACE
        rotate_Obj_Triangle(x_angle, y_angle, z_angle_OBJ1, OBJ_1_Buffer);


        //MOVE OBJECTS IN MODEL SPACE
        translate_Move_Triangle_Buffer(xPos1, yPos1, zPos1, OBJ_1_Buffer);


        // MOVE OBJECTS FROM MODEL SPACE TO WORLD SPACE
        move_Obj_Triangle_Buffer_To_World_Triangle_Buffer(OBJ_1_Buffer, world_Triangle_Buffer);


        rotate_World_Vertex(x_angle, y_angle, z_angle, world_Space_Vertex_Buffer);
        ////PASS TRIANGLE BUFFER TO RENDER PIPELINE

        //DRAW VIEW PORT BACKGROUND HERE

        vertex_Rastor_Pipeline
        (
            w1.index_X+110,
            w1.index_Y,
            200,
            200,
            z,
            FOV,
            projection_Type,
            back_Color,//!!!!!!!!!!REMOVE!!!!!!!!!!!!!!!!!!
            &bin,
            &bin,
            world_Space_Vertex_Buffer
        );

        triangle_Rastor_Pipeline
        (
            w1.index_X+110,                  // WINDOW START X
            w1.index_Y,                  // WINDOW START Y
            200,                // WINDOW SIZE X
            200,               // WINDOW SIZE Y
            z,                           // CAMERA Z    !!!!!!!!!!!!!!!!!REMOVE!!!!!!!!!!!!!!!!!!
            FOV,                         // CAMMERA FOV
            projection_Type,             // 0 = ORTHO 1 = PERSPECTIVE
            0,                           // LIGHT X
            0,                           // LIGHT Y
            0,                           // LIGHT Z
            true,                       // ENABLE BACKFACE CULLING
            false,                       // LIGHT ENABLE
            false,                       // TRIANGLE FILL ENABLE
            &bin,                        // TRIANGLES DRAWN
            world_Triangle_Buffer        // WORLD SPACE TRIANGLE BUFFER
        );

       // print("XA", w1.index_X + 1, w1.index_Y, red, black); printNumberDouble(x_angle, w1.index_X + 15, w1.index_Y, green, black);
       // print("YA", w1.index_X + 1, w1.index_Y + 10, red, black); printNumberDouble(y_angle, w1.index_X + 15, w1.index_Y + 10, green, black);


        //CLEAR BUFFERS FOR NEXT FRAME  
        world_Triangle_Buffer.clear();
        OBJ_1_Buffer.clear();
        world_Space_Vertex_Buffer.clear();

        //DRAW BUTTONS
        mouse_Icon_List_Handle(buttons_Buff, &active_Button_Id, mouseX, mouseY);
        buttons_Buff.clear();
    }
    }


};