  void i2c_Slave_Command(int howMany)
{
  byte control; 
  cpu.set_Cpu_Led(1);

  
  while (Wire.available()) 
  {
    control = Wire.read();  
    Serial.println(control);    
  }



   cpu.set_Cpu_Led(0);
}
