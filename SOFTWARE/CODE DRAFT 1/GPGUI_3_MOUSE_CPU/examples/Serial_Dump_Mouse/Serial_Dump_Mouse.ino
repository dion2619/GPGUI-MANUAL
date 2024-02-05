#include <MOUSE_CPU.h>
MOUSE_CPU cpu;

void setup() 
{
cpu.Init();
}



void loop()

{
cpu.Serial_Dump_Mouse();
}
