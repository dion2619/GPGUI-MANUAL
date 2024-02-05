#include "GPGUI_MIDI_CORE.h"
GPGUI_MIDI_CORE mc;

byte ch = 1;         //1 - 16
byte voice = 1;      //0 - 127
byte bank = 0;       //0 = bank 1  127 = bank 2
byte note = 60;      //1 - 127
byte velocity = 100; //1 - 127


void setup() 
{
mc.SAM_INIT();
mc.set_Voice(ch,voice,bank);
}

void loop()
{
  note = random(40,90);
 
  mc.note_On(ch,note,velocity);
  delay(100);
  mc.note_Off(ch,note);
  delay(100);
}
