  void i2c_Slave_Command(int howMany)
{
  i2c_Available = true;
  while (Wire.available()) 
  {
    command = Wire.read();  
      d1 = Wire.read();
      d2 = Wire.read();
      d3 = Wire.read();
  }
}
