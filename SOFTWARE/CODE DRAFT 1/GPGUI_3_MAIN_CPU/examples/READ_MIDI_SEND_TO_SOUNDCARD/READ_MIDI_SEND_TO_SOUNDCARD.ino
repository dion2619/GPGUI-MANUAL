#include <GPGUI_3_MAIN_CPU.h>
GPGUI_3_MAIN_CPU cpu;
byte status;
byte d1;
byte d2;



void setup()
{
cpu.GPGUI_INIT();
cpu.enable_Midi_To_Soundcard();
cpu.enable_Midi_Port();
cpu.set_Soundcard_Max_Velocity(80);
cpu.set_Soundcard_Max_Volume(100);
}

void loop()
{
cpu.read_Midi_Port(&status, &d1, &d2);
}
