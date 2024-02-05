#include "GPGUI_MIDI_CORE.h"
GPGUI_MIDI_CORE mc;

byte ch = 1;         //1 - 16
byte voice = 1;      //0 - 127
byte bank = 0;       //0 = bank 1  127 = bank 2
byte velocity = 100; //1 - 127


void setup() 
{
mc.SAM_INIT();
mc.set_Ch_Volume(ch,0);
mc.set_Ch_Volume(ch+1,127);
}

void loop()
{
  byte note = random(30,90);
  voice = random(30,90);
  mc.set_Voice(ch,voice,bank);
  mc.set_Voice(ch+1,voice,bank);
  mc.note_On(ch,note,velocity);
  mc.note_On(ch+1,note+5,velocity);
  for(int i = 0; i< 127; i++)
  {
     mc.set_Ch_Volume(ch,i);
     delay(20);
  }
  delay(2000);
  mc.note_Off(ch,note);
  mc.note_Off(ch+1,note+5);
  delay(1000);
}
