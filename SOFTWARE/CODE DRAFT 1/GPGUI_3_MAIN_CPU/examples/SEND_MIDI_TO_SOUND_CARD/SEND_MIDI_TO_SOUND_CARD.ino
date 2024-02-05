#include <GPGUI_3_MAIN_CPU.h>
GPGUI_3_MAIN_CPU cpu;

void setup()
{
cpu.GPGUI_INIT();
}

void loop()
{ 
cpu.send_Midi_Message_To_Soundcard(144, 60, 120);
delay(1000);
}
