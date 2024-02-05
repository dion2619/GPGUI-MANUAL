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
cpu.enable_Lcd_Back_Light(true);
delay(1000);
cpu.clear_Lcd();
cpu.enable_Lcd_Back_Light(false);
delay(1000);

}
