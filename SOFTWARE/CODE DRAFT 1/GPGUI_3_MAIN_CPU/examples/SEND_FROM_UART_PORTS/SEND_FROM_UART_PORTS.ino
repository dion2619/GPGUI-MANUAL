#include <GPGUI_3_MAIN_CPU.h>
GPGUI_3_MAIN_CPU cpu;

void setup()
{
cpu.GPGUI_INIT();
}

void loop()
{
  cpu.send_Byte_To_Control_Port(100);
  cpu.send_Byte_To_Device_Port(100);
  delay(500);
}