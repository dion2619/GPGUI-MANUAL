#include <GPGUI_3_MAIN_CPU.h>
GPGUI_3_MAIN_CPU cpu;

void setup()
{
cpu.GPGUI_INIT();
}

void loop()
{ 
cpu.SendCommandI2c(1,255,1,2,3);
delay(500);
}
