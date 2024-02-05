#include "GPGUI_MIDI_CORE.h"
GPGUI_MIDI_CORE mc;
#define STANDARD_SET   1
#define POWER_SET      17
#define BRUSH_SET      41
#define ORCHESTRA_SET  49
#define CM_SET         128

byte ch = 10; //CH 10 ONLY PLAYS DRUMS         
byte velocity = 100; 
byte note; 
byte note2; 

void setup() 
{
Serial.begin(9600);
mc.SAM_INIT();
mc.en_Reverb_Coarse(1,127);
mc.en_Reverb_Coarse(ch,127);
}

void loop()
{
    note2 = random(35,60);
    mc.set_Patch(1,random(35,81));
    mc.note_On(1,note2,velocity);
    mc.note_On(1,note2+12,velocity);
  
  mc.set_Patch(ch,STANDARD_SET);
  Serial.println("STANDARD_SET");
  for(int i=0; i<20; i++)
  {
    note = random(35,81);
    mc.note_On(ch,note,velocity);
    delay(300);
    mc.note_Off(ch,note);
  }
  delay(1000);
  mc.note_Off(1,note2);
  mc.note_Off(1,note2+12);





  
    note2 = random(35,60);
    mc.set_Patch(1,random(35,81));
    mc.note_On(1,note2,velocity);
    mc.note_On(1,note2+12,velocity);
    mc.set_Patch(ch,POWER_SET);
    Serial.println("POWER_SET");
  for(int i=0; i<20; i++)
  {
    note = random(35,81);
    mc.note_On(ch,note,velocity);
    delay(300);
    mc.note_Off(ch,note);
  }
  delay(1000);
  mc.note_Off(1,note2);
  mc.note_Off(1,note2+12);





  
    note2 = random(35,60);
    mc.set_Patch(1,random(35,81));
    mc.note_On(1,note2,velocity);
    mc.note_On(1,note2+12,velocity);
    mc.set_Patch(ch,BRUSH_SET);
    Serial.println("BRUSH_SET");
  for(int i=0; i<20; i++)
  {
    note = random(35,81);
    mc.note_On(ch,note,velocity);
    delay(300);
    mc.note_Off(ch,note);
  }
  mc.note_Off(1,note2);
  mc.note_Off(1,note2+12);


    mc.set_Patch(2,128);
    mc.set_Ch_Volume(2,127);
    byte gun = random(30,90);
    for(int i=0; i<10; i++)
    {
    mc.note_On(2,gun,127);
    delay(100);
    mc.note_Off(2,gun);
    }


  
    note2 = random(35,60);
    mc.set_Patch(1,random(35,81));
    mc.note_On(1,note2,velocity);
    mc.note_On(1,note2+12,velocity);
    mc.set_Patch(ch,ORCHESTRA_SET);
    Serial.println("ORCHESTRA_SET");
  for(int i=0; i<20; i++)
  {
    note = random(35,81);
    mc.note_On(ch,note,velocity);
    delay(300);
    mc.note_Off(ch,note);
  }
  delay(1000);
  mc.note_Off(1,note2);
  mc.note_Off(1,note2+12);
 




  
    note2 = random(35,60);
    mc.set_Patch(1,random(35,81));
    mc.note_On(1,note2,velocity);
    mc.note_On(1,note2+12,velocity);
    mc.set_Patch(ch,CM_SET);
    Serial.println("CM_SET");
  for(int i=0; i<20; i++)
  {
    note = random(35,81);
    mc.note_On(ch,note,velocity);
    delay(300);
    mc.note_Off(ch,note);
  }
  delay(1000);

  mc.note_Off(1,note2);
  mc.note_Off(1,note2+12);
}