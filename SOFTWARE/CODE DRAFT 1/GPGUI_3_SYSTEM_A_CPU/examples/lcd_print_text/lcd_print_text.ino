#include <GPGUI_3_SYSTEM_A_CPU.h>
GPGUI_3_SYSTEM_A_CPU cpu;

void setup() 
{
cpu.power_Up();
delay(1000);
cpu.clear_Lcd();
}

void loop()
{
cpu.print_Lcd_Text
(
  "123456789!@#$%^&",
  "ABCDEFGHIJKLMNOP"
);
delay(1000);
cpu.clear_Lcd();
delay(1000);
}