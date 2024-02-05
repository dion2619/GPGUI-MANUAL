#include "GPGUI_MIDI_CORE.h"
GPGUI_MIDI_CORE mc;
bool gun_Enable = false;

void setup() 
{
Serial.begin(9600);
mc.SAM_INIT();
mc.en_Reverb_Coarse(1,127);
mc.en_Reverb_Coarse(10,127);
mc.en_Reverb_Coarse(2,127);
}

void loop()
{
  mc.random_Music_Gen_1(true);
  mc.machine_Gun_FX(2, 70, 100, 64, 120, gun_Enable);
  
  if(Serial.available())
  {
    byte data = Serial.read();
    switch (data)
    {
      case 'l':  gun_Enable = true;  break;
      case 'k':  gun_Enable = false; break;
      case 's':  mc.note_On(2,60,127); break;
    } 
  }  
}