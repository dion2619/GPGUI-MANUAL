#include <GPGUI_3_SYSTEM_A_CPU.h>
GPGUI_3_SYSTEM_A_CPU cpu;
byte CONTROL_ADDRESS = 0;
byte DEVICE_ADDRESS = 0;
byte SOUND_ADDRESS = 0;
void setup() 
{
cpu.power_Up();
}

void loop()
{
 cpu.set_Control_Mux_Address(CONTROL_ADDRESS);
 cpu.set_Device_Mux_Address(DEVICE_ADDRESS);
 cpu.set_Sound_Mux_Address(SOUND_ADDRESS);
 delay(1000);
 CONTROL_ADDRESS++;
 DEVICE_ADDRESS++;
 SOUND_ADDRESS++;

 
 if(CONTROL_ADDRESS>7) CONTROL_ADDRESS = 0;
 if(DEVICE_ADDRESS>7)  DEVICE_ADDRESS = 0;
 if(SOUND_ADDRESS>3)   SOUND_ADDRESS = 0;
}