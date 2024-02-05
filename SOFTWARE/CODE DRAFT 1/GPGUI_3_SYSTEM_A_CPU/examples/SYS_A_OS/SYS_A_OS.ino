#include <Wire.h>
#include <GPGUI_3_SYSTEM_A_CPU.h>
GPGUI_3_SYSTEM_A_CPU cpu;

#define i2c_Address 1
bool i2c_Available = false;
byte command = 0;
byte d1 = 0;
byte d2 = 0;
byte d3 = 0;



void setup() 
{
cpu.power_Up();
Wire.begin(i2c_Address);                
Wire.onReceive(i2c_Slave_Command); 
}

void loop()
{
  cpu.OS(&i2c_Available,command,d1,d2,d3);
}
