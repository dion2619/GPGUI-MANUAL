#include "GPGUI_MIDI_CORE.h"
GPGUI_MIDI_CORE mc;

byte ch = 1;         //1 - 16
byte voice = 1;      //0 - 127
byte bank = 0;       //0 = bank 1  127 = bank 2
byte velocity = 127; //1 - 127


void setup() 
{
mc.SAM_INIT();
mc.en_Reverb_Coarse(ch,127);
}

void loop()
{

  byte note = random(30,90);
  mc.set_Voice(ch,random(1,128),bank);
  mc.note_On(ch,note,velocity);
  delay(1000);
  mc.note_Off(ch,note);
  delay(1000);
 
 


}
