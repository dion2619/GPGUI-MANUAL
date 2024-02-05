#include <GPGUI_3_SYSTEM_B_CPU.h>
GPGUI_3_SYSTEM_B_CPU cpu;

void setup() 
{
  cpu.power_Up();
}

void loop() 
{
 cpu.reset_Sys_A_Cpu();
 delay(2000);
}
