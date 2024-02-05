#include <Wire.h>
#include <GPGUI_3_SYSTEM_A_CPU.h>
GPGUI_3_SYSTEM_A_CPU cpu;

#define i2c_Address 1

void setup() 
{

Wire.begin(i2c_Address);                
Wire.onReceive(i2c_Slave_Command); 
cpu.power_Up();

}

void loop()
{
  

}
