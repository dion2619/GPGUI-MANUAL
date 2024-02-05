#include <arduino.h>
#include <MOUSE_CPU.h>
#include <PS2MouseHandler.h>

#define MOUSE_DATA 5
#define MOUSE_CLOCK 6
#define cpu_Led 13
bool debug = false;

PS2MouseHandler mouse(MOUSE_CLOCK, MOUSE_DATA, PS2_MOUSE_REMOTE); 

void MOUSE_CPU::Init()
{
    Serial.begin(115200);
    pinMode(cpu_Led, OUTPUT);
    if (mouse.initialise() != 0) {
        // mouse error
        Serial.println("CONNECT MOUSE");
    };

  
}

void MOUSE_CPU::Serial_Dump_Mouse()
{
    mouse.get_data();
    Serial.print(mouse.device_id()); // device id
    Serial.print(":");
    Serial.print(mouse.status()); // Status Byte
    Serial.print(":");
    Serial.print(mouse.x_movement()); // X Movement Data
    Serial.print(",");
    Serial.print(mouse.y_movement()); // Y Movement Data
    Serial.print(",");
    Serial.print(mouse.z_movement()); // Z Movement Data - scroll wheel
    Serial.print(",");
    Serial.print(mouse.button(0));
    Serial.print(",");
    Serial.print(mouse.button(1));
    Serial.print(",");
    Serial.print(mouse.button(2));
    Serial.print(",");
    Serial.print(mouse.clicked(0));
    Serial.print(",");
    Serial.print(mouse.clicked(1));
    Serial.print(",");
    Serial.print(mouse.clicked(2));
    Serial.println();
}

void MOUSE_CPU::set_Cpu_Led(bool state)
{
    digitalWrite(cpu_Led, state);

}

void MOUSE_CPU::flash_Cpu_Led(int times, int dur)
{
    for (int i = 0; i < times; i++)
    {
        digitalWrite(cpu_Led, 1);
        delay(dur);
        digitalWrite(cpu_Led, 0);
        delay(dur);

    }
}

void MOUSE_CPU::encode_Axis_Byte(int delta, int *data_Byte)
{
    if (delta < 0)
    {
        for (int i = 0; i < 7; i++) { bitWrite(*data_Byte, i, bitRead(delta * -1, i)); }
        bitWrite(*data_Byte, 7, 1);
    }

    if (delta > 0)
    {
        for (int i = 0; i < 7; i++) { bitWrite(*data_Byte, i, bitRead(delta, i)); }
        bitWrite(*data_Byte, 7, 0);
    }
    if (*data_Byte == 255) *data_Byte = 254;
}

void MOUSE_CPU::send_Left_Make()
{
    if (!debug)
    {
        Serial.write(255);//sync
        Serial.write(0);//com add
        Serial.write(1);
        delay(5);
    }
}

void MOUSE_CPU::send_Left_Break()
{
    if (!debug)
    {
        Serial.write(255);//sync
        Serial.write(0);//com add
        Serial.write(2);
        delay(5);
    }
}

void MOUSE_CPU::send_Middle_Make()
{
    if (!debug)
    {
        Serial.write(255);//sync
        Serial.write(0);//com add
        Serial.write(3);
        delay(5);
    }
}

void MOUSE_CPU::send_Middle_Break()
{
    if (!debug)
    {
        Serial.write(255);//sync
        Serial.write(0);//com add
        Serial.write(4);
        delay(5);
    }
}

void MOUSE_CPU::send_Right_Make()
{
    if (!debug)
    {
        Serial.write(255);//sync
        Serial.write(0);//com add
        Serial.write(5);
        delay(5);
    }
}

void MOUSE_CPU::send_Right_Break()
{
    if (!debug)
    {
        Serial.write(255);//sync
        Serial.write(0);//com add
        Serial.write(6);
        delay(5);
    }
}

void MOUSE_CPU::send_Mouse_Pos_Change(int x, int y)
{
    if (!debug)
    {
        Serial.write(255);//sync
        Serial.write(0);//com add
        Serial.write(7);//cursor pos change
        Serial.write(x);//encoded pos byte
        Serial.write(y);
        delay(5);
    }

}

void MOUSE_CPU::send_Scroll_Up()
{
    if (!debug)
    {
        Serial.write(255);//sync
        Serial.write(0);//com add
        Serial.write(8);
        delay(5);
    }
}

void MOUSE_CPU::send_Scroll_Down()
{
    if (!debug)
    {
        Serial.write(255);//sync
        Serial.write(0);//com add
        Serial.write(9);
        delay(5);
    }

}

void MOUSE_CPU::read_Mouse()
{

    mouse.get_data();
    byte command = mouse.status();
    int dx = mouse.x_movement();// -127 - 127
    int dy = mouse.y_movement();// -127 - 127
    int dz = mouse.z_movement();//-1 0 1
    bool left_Click = mouse.clicked(0);
    int x_data_Byte = 0;
    int y_data_Byte = 0;
    int z_data_Byte = 0;
    static bool left_Make = 0;
    static bool middle_Make = 0;
    static bool right_make = 0;
    if (command != 8) set_Cpu_Led(1);
    if (command == 8) set_Cpu_Led(0);



    if (dx != 0) encode_Axis_Byte(dx, &x_data_Byte);
    if (dy != 0) encode_Axis_Byte(dy, &y_data_Byte);
    if (dz != 0) encode_Axis_Byte(dz, &z_data_Byte);

    if (x_data_Byte > 0 || y_data_Byte > 0 )
    {
        send_Mouse_Pos_Change(x_data_Byte, y_data_Byte);
        if (debug) { Serial.print("x  ");   Serial.print(x_data_Byte); }
        if (debug) { Serial.print("  y  "); Serial.println(y_data_Byte); }      
    }
  
    //LEFT MAKE
    if (mouse.button(0) && !left_Make)
    {
        left_Make = true;
        send_Left_Make();
        if (debug) Serial.println("LEFT MAKE");
    }

    //LEFT BREAK
    if (!mouse.button(0) && left_Make)
    {
        left_Make = false;
        send_Left_Break();
        if (debug) Serial.println("LEFT BREAK");
    }

    //MIDDLE MAKE
    if (mouse.button(1) && !middle_Make)
    {
        middle_Make = true;
        send_Middle_Make();
        if (debug) Serial.println("MIDDLE MAKE");
    }

    //MIDDLE BREAK
    if (!mouse.button(1) && middle_Make)
    {
        middle_Make = false;
        send_Middle_Break();
        if (debug) Serial.println("MIDDLE BREAK");
    }

    //RIGHT MAKE
    if (mouse.button(2) && !right_make)
    {
        right_make = true;
        send_Right_Make();
        if (debug) Serial.println("RIGHT MAKE");
    }

    //RIGHT BREAK
    if (!mouse.button(2) && right_make)
    {
        right_make = false;
        send_Right_Break();
        if (debug) Serial.println("RIGHT BREAK");
    }

    //SCROLL DOWN
    if (mouse.z_movement() > 0)
    {
        send_Scroll_Down();
        if (debug) Serial.println("SCROLL DOWN");
    }

    //SCROLL UP
    if (mouse.z_movement() < 0)
    {
        send_Scroll_Up();
        if (debug) Serial.println("SCROLL UP");
    }
}


