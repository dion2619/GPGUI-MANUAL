#include <GPGUI_3_SYSTEM_A_CPU.h>
GPGUI_3_SYSTEM_A_CPU cpu;

void setup() 
{
cpu.power_Up();
}

void loop()
{
cpu.send_Com_Test_To_Sys_B_Cpu();
delay(1000);
}
