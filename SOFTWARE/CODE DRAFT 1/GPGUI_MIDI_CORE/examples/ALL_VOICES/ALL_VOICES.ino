#include "GPGUI_MIDI_CORE.h"
GPGUI_MIDI_CORE mc;

byte ch = 1;         //1 - 16
byte voice = 1;      //0 - 127
byte bank = 0;       //0 = bank 1  127 = bank 2
byte velocity = 100; //1 - 127


void setup() 
{
mc.SAM_INIT();
}

void loop()
{
 for(int i = 1; i<128;i++)
 {
  mc.set_Voice(ch,i,0);
 for(int j = 0; j<127;j++)
 {
   mc.note_On(ch,j,velocity);
  delay(50);
  mc.note_Off(ch,j);
  delay(50);
 }
 }

 
 for(int i = 1; i<128;i++)
 {
  mc.set_Voice(ch,i,127);
 for(int j = 0; j<127;j++)
 {
  mc.note_On(ch,j,velocity);
  delay(50);
  mc.note_Off(ch,j);
  delay(50);
 }
 }

}
