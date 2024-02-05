#include <GPGUI_3_SYSTEM_A_CPU.h>
GPGUI_3_SYSTEM_A_CPU cpu;

byte command = 10;

void setup() 
{
cpu.power_Up();
}

void loop()
{
cpu.send_Command_To_Sys_B(command, 1, 2, 3, 4);
delay(1000);
}
