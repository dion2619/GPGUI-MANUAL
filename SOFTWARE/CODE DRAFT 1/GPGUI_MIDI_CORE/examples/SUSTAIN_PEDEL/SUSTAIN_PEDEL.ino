#include "GPGUI_MIDI_CORE.h"
GPGUI_MIDI_CORE mc;

byte ch = 1;         //1 - 16
bool down = 1;
bool up = 0;

void setup() 
{
mc.SAM_INIT();
}

void loop()
{
mc.en_Sustain(ch,down);//PEDEL DOWN
mc.note_On(ch,60,100);
delay(100);
mc.note_Off(ch,60);//IGNORES NOTE OFF
delay(100);
mc.note_On(ch,72,100);
delay(100);
mc.note_Off(ch,72);
delay(1000);




mc.en_Sustain(ch,up);//PEDEL UP
delay(1000);




mc.note_On(ch,60,100);
delay(100);
mc.note_Off(ch,60);
delay(100);
mc.note_On(ch,72,100);
delay(100);
mc.note_Off(ch,72);
delay(1000);
}