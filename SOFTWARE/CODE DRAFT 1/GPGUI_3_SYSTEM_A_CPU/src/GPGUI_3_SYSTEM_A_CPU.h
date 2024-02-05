#include <Arduino.h>
#include <EEPROM.h>

using namespace std;

//i/o
#define DATA_PIN  2
#define LATCH_PIN 3
#define CLOCK_PIN 4
#define cpu_Led 13
#define beeper 8
#define i2c_Address 1



//MUX ADDRESS'S
#define CONTROL_COM1_MOUSE 0
#define CONTROL_COM1_DEVICE_PLUG 1
#define CONTROL_COM1_SLOT_1 2
#define CONTROL_COM1_SLOT_2 3
#define CONTROL_COM1_SLOT_3 4
#define CONTROL_COM1_SLOT_4 5
//
#define DEVICE_COM2_DEVICE_PLUG 0
#define DEVICE_COM2_SLOT_1      1
#define DEVICE_COM2_SLOT_2      2
#define DEVICE_COM2_SLOT_3      3
#define DEVICE_COM2_SLOT_4      4
#define DEVICE_COM2_MIDI_PORT   5





byte default_Control_Address = EEPROM.read(0);

bool com_Address_Reg[8] =
{
0,//CONTROL MUX A
0,//CONTROL MUX B
0,//CONTROL MUX C
0,//DEVICE MUX A
0,//DEVICE MUX B
0,//DEVICE MUX C
0,//SOUND MUX A
0 //SOUND MUX B
};





class GPGUI_3_SYSTEM_A_CPU
{
public:

	void power_Up()
	{
		Serial.begin(115200);
		pinMode(DATA_PIN, 1);
		pinMode(LATCH_PIN, 1);
		pinMode(CLOCK_PIN, 1);
		pinMode(cpu_Led, 1);
		pinMode(beeper, 1);
		//system_Start();
	}

	void set_Cpu_Led(bool state)
	{
		digitalWrite(cpu_Led, state);
	}

	void beep(int pitch, int dur)
	{
		tone(beeper, pitch);
		delay(dur);
		noTone(beeper);
	}

	void load_Address_Reg()
	{
		set_Cpu_Led(1);
		for (int i = 0; i < 8; i++)
		{
			digitalWrite(DATA_PIN, com_Address_Reg[7-i]);
			digitalWrite(CLOCK_PIN, 1);
			delay(1);
			digitalWrite(CLOCK_PIN, 0);
		}
		digitalWrite(LATCH_PIN, 1);
		delay(1);
		digitalWrite(LATCH_PIN, 0);
		set_Cpu_Led(0);
	}

	void set_Control_Mux_Address(byte address)
	{
		if (address > 7) address = 7;
		com_Address_Reg[0] = bitRead(address, 0);
		com_Address_Reg[1] = bitRead(address, 1);
		com_Address_Reg[2] = bitRead(address, 2);
		load_Address_Reg();
	}

	void set_Device_Mux_Address(byte address)
	{
		if (address > 7) address = 7;
		com_Address_Reg[3] = bitRead(address, 0);
		com_Address_Reg[4] = bitRead(address, 1);
		com_Address_Reg[5] = bitRead(address, 2);
		load_Address_Reg();
	}

	void set_Sound_Mux_Address(byte address)
	{
		if (address > 3) address = 3;
		com_Address_Reg[6] = bitRead(address, 0);
		com_Address_Reg[7] = bitRead(address, 1);
		load_Address_Reg();
	}

	void i2c_Handle(byte command, byte d1, byte d2, byte d3)
	{
		switch (command)
		{
		case 1: reset_Mouse_Cpu(); break;
		case 2: reset_Slots(); break;
		case 3: reset_Sys_A_Cpu(); break;
		case 4: reset_Device_plug(); break;
		case 5: clear_Lcd(); break;
		case 6: enable_Lcd_Back_Light((bool)d1); break;
		case 7: write_Char_To_Lcd(d1, d2, (char)d3);  break;
		case 8: set_Device_Mux_Address(d1); break;
		case 9: set_Control_Mux_Address(d1); break;
		case 10: set_Sound_Mux_Address(d1); break;
		case 255:  beep(1000,100); break;
		}
	}

	void set_Default_Com_Port(byte port)
	{
		EEPROM.write(0, port);
		default_Control_Address = port;
		delay(10);

	}















	//UART SYS A - SYS B
	void print_Lcd_Text(char text_Row1[], byte X, byte y)
	{
		byte pos = 0;
		byte x = X;



		while (text_Row1[pos] > 0)
		{
			write_Char_To_Lcd(x, y, text_Row1[pos]);
			x++;
			pos++;
			if (x > 15)
			{
				x = X;
				break;
			}
		}
	}

	void send_Com_Test_To_Sys_B_Cpu()
	{
		Serial.write(255);
		Serial.write(127);
		Serial.write(254);
		Serial.write(0);
		Serial.write(1);
		set_Cpu_Led(1);
		delay(50);
		set_Cpu_Led(0);
	}

	void send_Command_To_Sys_B(byte command, byte data1, byte data2, byte data3, byte data4)
	{
		set_Cpu_Led(1);
		Serial.write(255);//sync
		Serial.write(127);//address
		Serial.write(command);//command
		Serial.write(data1);//data
		Serial.write(data2);//data
		Serial.write(data3);//data
		Serial.write(data4);//data
		set_Cpu_Led(0);
	}

	void read_Com_Port()
	{
		byte sync;
		byte com_Address;
		byte command;
		byte data1;
		byte data2;
		byte data3;
		byte data4;
		int pitch;
		if (Serial.available())
		{
			set_Cpu_Led(1);//
			while (Serial.available())
			{

				sync = Serial.read();
				if (sync == 255)                  //find start of command
				{
					com_Address = Serial.read();  // check com address

					if (com_Address == 127)
					{
						command = Serial.read();  //read command

					}


					switch (command)
					{

					case 1:
						data1 = Serial.read();  //read command
						data2 = Serial.read();  //read command
						data3 = Serial.read();  //read command
						data4 = Serial.read();  //read command
						pitch = data1 * data2;
						beep(pitch, data3);
						break;




					case 254:
						set_Cpu_Led(1);
						beep(200, 50);
						set_Cpu_Led(0);
						break;
					}

				}





			}
			set_Cpu_Led(0);
		}
	}

	void write_Char_To_Lcd(byte x, byte y, char ch)
	{
		if (x > 15) x = 15;
		if (y > 1) y = 1;




		set_Cpu_Led(1);
		Serial.write(255);
		Serial.write(127);
		Serial.write(2);
		Serial.write(y);
		Serial.write(x);
		Serial.write((byte)ch);
		delay(5);//wait for lcd to draw
		set_Cpu_Led(0);
	}

	void clear_Lcd()
	{
		set_Cpu_Led(1);
		Serial.write(255);
		Serial.write(127);
		Serial.write(3);
		delay(5);
		set_Cpu_Led(0);
	}

	void enable_Lcd_Back_Light(bool state)
	{
		set_Cpu_Led(1);
		Serial.write(255);
		Serial.write(127);
		Serial.write(5);
		Serial.write((byte)state);
		delay(5);
		set_Cpu_Led(0);
	}

	void print_Lcd_Byte(byte x, byte y, byte data)
	{
		set_Cpu_Led(1);
		Serial.write(255);
		Serial.write(127);
		Serial.write(6);
		Serial.write(x);
		Serial.write(y);
		Serial.write(data);
		delay(5);
		set_Cpu_Led(0);

	}

	void reset_Mouse_Cpu()
	{
		set_Cpu_Led(1);
		Serial.write(255);
		Serial.write(127);
		Serial.write(7);
		delay(5);
		set_Cpu_Led(0);
	}

	void reset_Slots()
	{
		set_Cpu_Led(1);
		Serial.write(255);
		Serial.write(127);
		Serial.write(8);
		delay(5);
		set_Cpu_Led(0);
	}

	void reset_Sys_A_Cpu()
	{
		set_Cpu_Led(1);
		Serial.write(255);
		Serial.write(127);
		Serial.write(9);
		delay(5);
		set_Cpu_Led(0);
	}

	void reset_Device_plug()
	{
		set_Cpu_Led(1);
		Serial.write(255);
		Serial.write(127);
		Serial.write(10);
		delay(5);
		set_Cpu_Led(0);
	}

	void reset_Main_Cpu()
	{
		set_Cpu_Led(1);
		Serial.write(255);
		Serial.write(127);
		Serial.write(11);
		delay(5);
		set_Cpu_Led(0);
	}

	void set_Main_Cpu_To_Program_Mode()
	{
		set_Cpu_Led(1);
		Serial.write(255);
		Serial.write(127);
		Serial.write(12);
		delay(5);
		set_Cpu_Led(0);
	}

	void set_Main_Cpu_To_Run_Mode()
	{
		set_Cpu_Led(1);
		Serial.write(255);
		Serial.write(127);
		Serial.write(13);
		delay(5);
		set_Cpu_Led(0);
	}

	void OS(bool* i2cAvailable, byte command, byte d1, byte d2, byte d3)
	{
		if (*i2cAvailable)
		{
			*i2cAvailable = false;
			i2c_Handle(command, d1, d2, d3);
		}
	}

	void system_Start()
	{
		delay(100);
		set_Main_Cpu_To_Program_Mode();

		clear_Lcd();
		delay(100);
		print_Lcd_Text("SYSTEM LOADING", 1, 0);
		delay(1000);


		beep(200, 50);
		reset_Slots();
		delay(1000);


		print_Lcd_Text("GETTING COM PORT", 0, 0);
		print_Lcd_Text("  FROM EEPROM   ", 0, 1);
		delay(1000);


		beep(300, 50);
		clear_Lcd();
		print_Lcd_Text("COM PORT =", 0, 0);
		print_Lcd_Byte(11, 0, default_Control_Address);
		delay(3000);

		//SET CONTROL MUX
		//set_Control_Mux_Address(default_Control_Address);
		set_Control_Mux_Address(CONTROL_COM1_MOUSE);

		if (default_Control_Address == CONTROL_COM1_MOUSE) { reset_Mouse_Cpu(); beep(800, 50); }
		delay(3000);

		//SET SOUND MUX
		set_Sound_Mux_Address(0);
		set_Device_Mux_Address(DEVICE_COM2_MIDI_PORT);
		



		beep(1000, 50);
		set_Main_Cpu_To_Run_Mode();
		delay(3000);
		clear_Lcd();
		beep(1000, 50);




	}
};