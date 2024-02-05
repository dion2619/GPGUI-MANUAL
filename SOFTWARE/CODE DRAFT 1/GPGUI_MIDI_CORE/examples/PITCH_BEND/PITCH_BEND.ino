#include "GPGUI_MIDI_CORE.h"
GPGUI_MIDI_CORE mc;

byte ch = 1;          //1 - 16
byte voice = 20;      //0 - 127
byte bank = 0;        //0 = bank 1  127 = bank 2
byte velocity = 100;  //1 - 127


void setup() 
{

mc.SAM_INIT();
mc.en_Chorus_Coarse(ch,127);
mc.en_Reverb_Coarse(ch,127);
}

void loop()
{
  byte note = 60;
  mc.set_Voice(ch,voice,bank);
  mc.note_On(ch,note,velocity);
  delay(3000);

  for(int i = 2047; i< 4095; i++) mc.pitch_Bend(ch,i);//range 0 - 4095 (2047 = no bend) ( > 2047 = bend up from note) ( < 2047 = bend down from note)
  delay(1000);

  for(int i = 4095; i > 2047; i--) mc.pitch_Bend(ch,i); 
  delay(3000);

  for(int i = 2047; i> 0; i--) mc.pitch_Bend(ch,i);
  delay(1000);

  for(int i = 0; i < 2047; i++) mc.pitch_Bend(ch,i);
  delay(3000);

  mc.note_Off(ch,note);
  delay(1000);
}