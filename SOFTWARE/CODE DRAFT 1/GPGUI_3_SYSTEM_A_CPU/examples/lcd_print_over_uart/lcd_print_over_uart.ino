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
cpu.print_Lcd_Text("DATA 1 =",0,0);
cpu.print_Lcd_Text("DATA 2 =",0,1);

cpu.print_Lcd_Text("   ",9,0);//CLEAR OLD NUMBERS WITH WHITESPACE
cpu.print_Lcd_Text("   ",9,1);
cpu.print_Lcd_Byte(9,0,random(0,254));
cpu.print_Lcd_Byte(9,1,random(0,254));


delay(2000);
}
