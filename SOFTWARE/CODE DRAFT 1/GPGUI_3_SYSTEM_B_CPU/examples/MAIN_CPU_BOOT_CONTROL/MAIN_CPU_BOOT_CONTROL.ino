#include <GPGUI_3_SYSTEM_B_CPU.h>
GPGUI_3_SYSTEM_B_CPU cpu;

void setup() 
{
  cpu.power_Up();
}

void loop() 
{
 cpu.set_Main_Cpu_To_Run_Mode();
 delay(2000);
 cpu.set_Main_Cpu_To_Program_Mode();
 delay(2000);
}
