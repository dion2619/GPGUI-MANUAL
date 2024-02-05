#include <GPGUI_3_MAIN_CPU.h>
GPGUI_3_MAIN_CPU cpu;
byte status;
byte d1;
byte d2;



void setup()
{
cpu.GPGUI_INIT();
cpu.enable_Midi_Port();
}

void loop()
{
  
while(cpu.read_Midi_Port(&status, &d1, &d2))
{
  switch(status)
  {
    case 144: 
    Serial.println("NOTE ON CH 1");
    Serial.print("NOTE = "); Serial.println(d1);
    Serial.print("VELOCITY = "); Serial.println(d2);
    Serial.println();
    break;

    case 128: 
    Serial.println("NOTE OFF CH 1");
    Serial.print("NOTE = "); Serial.println(d1);
    Serial.print("VELOCITY = "); Serial.println(d2);
    Serial.println();
    break;
  }
}

}
