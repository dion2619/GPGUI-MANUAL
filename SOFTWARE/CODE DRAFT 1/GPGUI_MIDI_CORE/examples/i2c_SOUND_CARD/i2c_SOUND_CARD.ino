#include "GPGUI_MIDI_CORE.h"
#include <Wire_slave.h>

GPGUI_MIDI_CORE mc;

bool d_Available = false;
byte STATUS;
byte V1;
byte V2;


void setup() 
{
  mc.SAM_INIT();
  Wire.begin(64);                
  Wire.onReceive(receiveEvent); 
}


void receiveEvent(int howMany)
{
  if(Wire.available())
  { 
   d_Available = true; 
   STATUS = Wire.read(); 
   V1 = Wire.read(); 
   V2 = Wire.read();       
  }
}



void loop()
{
  if(d_Available)
  {
    mc.i2c_Handle(STATUS,V1,V2); 
    d_Available = false;     
  }
}
