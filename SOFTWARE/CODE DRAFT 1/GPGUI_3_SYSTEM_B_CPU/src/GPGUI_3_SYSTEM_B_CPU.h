

#include <arduino.h>
#include <IRremote/src/IRremote.hpp>
#include <LiquidCrystal_I2C.h>
#include <IR_CODES.h>

#define IR_TX_PIN                 2
#define cpu_Led                   13
#define run_Program_Switch        11
#define reset_Button              10
#define mouse_Cpu_Reset_Trig      5
#define sys_A_Cpu_Reset_Trig      12
#define slot_Cards_Cpu_Reset_Trig 7
#define device_Plug_Cpu_Reset_Trig 8
#define main_Cpu_Boot_Trig        3
#define main_Cpu_Reset_Trig       4

LiquidCrystal_I2C lcd(0x27, 20, 4);

bool enable_IR_Led_Feedback = false;

class GPGUI_3_SYSTEM_B_CPU
{
public:

	//HARDWARE
	void power_Up()
	{
		Serial.begin(115200);
		lcd.init();
		lcd.backlight();
		IrReceiver.begin(IR_TX_PIN, enable_IR_Led_Feedback);
		pinMode(cpu_Led, 1);
		pinMode(mouse_Cpu_Reset_Trig, 1);
		pinMode(device_Plug_Cpu_Reset_Trig, 1);
		pinMode(sys_A_Cpu_Reset_Trig, 1);
		pinMode(slot_Cards_Cpu_Reset_Trig, 1);
		pinMode(main_Cpu_Boot_Trig, 1);
		pinMode(main_Cpu_Reset_Trig, 1);
		pinMode(run_Program_Switch, 0);
		pinMode(reset_Button, 0);
	}

	void set_Cpu_Led(bool state)
	{
		digitalWrite(cpu_Led, state);

	}

	void reset_Mouse_Cpu()
	{
		set_Cpu_Led(1);
		digitalWrite(mouse_Cpu_Reset_Trig, 1);
		delay(10);
		digitalWrite(mouse_Cpu_Reset_Trig, 0);
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("MOUSE CPU RESET");
		set_Cpu_Led(0);
	}

	void reset_Sys_A_Cpu()
	{
		set_Cpu_Led(1);
		digitalWrite(sys_A_Cpu_Reset_Trig, 1);
		delay(10);
		digitalWrite(sys_A_Cpu_Reset_Trig, 0);
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("SYS A CPU RESET");
		set_Cpu_Led(0);
	}

	void reset_SoundCard_Cpu()
	{
		set_Cpu_Led(1);
		digitalWrite(slot_Cards_Cpu_Reset_Trig, 1);
		delay(10);
		digitalWrite(slot_Cards_Cpu_Reset_Trig, 0);
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("SOUND CPU RESET");
		set_Cpu_Led(0);
	}

	void reset_Main_Cpu()
	{
		set_Cpu_Led(1);
		digitalWrite(main_Cpu_Reset_Trig, 1);
		delay(10);
		digitalWrite(main_Cpu_Reset_Trig, 0);
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("MAIN CPU RESET");
		set_Cpu_Led(0);
	}

	void reset_slot_Cards()
	{
		set_Cpu_Led(1);
		digitalWrite(slot_Cards_Cpu_Reset_Trig, 1);
		delay(10);
		digitalWrite(slot_Cards_Cpu_Reset_Trig, 0);
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("ALL SLOTS RESET");
		set_Cpu_Led(0);
	}

	void reset_Device_Plug()
	{
		set_Cpu_Led(1);
		digitalWrite(device_Plug_Cpu_Reset_Trig, 1);
		delay(10);
		digitalWrite(device_Plug_Cpu_Reset_Trig, 0);
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("DEVICE PLUG RESET");
		set_Cpu_Led(0);
	}

	void set_Main_Cpu_To_Program_Mode()
	{
		set_Cpu_Led(1);
		digitalWrite(main_Cpu_Boot_Trig, 1);
		digitalWrite(main_Cpu_Reset_Trig, 1);
		delay(10);
		digitalWrite(main_Cpu_Reset_Trig, 0);
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("CPU PROGRAM MODE");
		set_Cpu_Led(0);
	}

	void set_Main_Cpu_To_Run_Mode()
	{
		set_Cpu_Led(1);
		digitalWrite(main_Cpu_Boot_Trig, 0);
		digitalWrite(main_Cpu_Reset_Trig, 1);
		delay(10);
		digitalWrite(main_Cpu_Reset_Trig, 0);
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("CPU RUN MODE");
		set_Cpu_Led(0);
	}

	void enable_IR_Signal_Led()
	{
		enable_IR_Led_Feedback = true;
	}

	void disable_IR_Signal_Led()
	{
		enable_IR_Led_Feedback = false;
	}

	void read_IR_Remote()
	{
		byte button;
		if (IrReceiver.decode())
		{
			lcd.clear();
			button = IrReceiver.decodedIRData.command;
			lcd.setCursor(0, 0);
			lcd.print("IR");
			lcd.setCursor(4, 0);
			lcd.print(button);


			switch (button)
			{
			case CH1:  reset_Mouse_Cpu();               break;
			case CH2:                break;
			case CH3:  reset_Sys_A_Cpu();               break;
			case CH4:  reset_SoundCard_Cpu();           break;
			case CH5:  reset_Main_Cpu();                break;
			case RED:  set_Main_Cpu_To_Program_Mode();  break;
			case GREEN:  set_Main_Cpu_To_Run_Mode();    break;
			}






			IrReceiver.resume();
		}


	}





	//OS LOOP
	void OS()
	{
		read_IR_Remote();
		read_Com_Port();
	}







	//SYSTEM A CPU COMMANDS
	//RX
	void read_Com_Port()
	{
		byte sync;
		byte com_Address;
		byte command;
		byte data1;
		byte data2;
		byte data3;

		while (Serial.available())
		{
			set_Cpu_Led(1);
			delay(1);//NEEDED TO FIX TIMING ISSUSE
			sync = Serial.read();                  //find start of command

			if (sync == 255)
			{
				com_Address = Serial.read();       // check com address

				if (com_Address == 127)
				{
					command = Serial.read();        //read command

					switch (command)
					{
					case 1://FREE

						break;


					case 2://PRINT SINGLE CHARACTER TO LCD						
						lcd.setCursor(Serial.read(), Serial.read());
						lcd.print((char)Serial.read());
						break;


					case 3://CLEAR LCD
						lcd.clear();
						break;

					case 4: //FREE//////////////////

						break;


					case 5://TURN ON/OFF LCD BACKLIGHT
						data1 = Serial.read();
						if (data1 == 1) lcd.backlight();
						if (data1 == 0) lcd.noBacklight();
						break;


					case 6://PRINT NUMBER BYTE
						data1 = Serial.read();//x
						data2 = Serial.read();//y
						data3 = Serial.read();//data
						if (data1 > 12) data1 = 12;
						if (data2 > 1)  data2 = 1;
						lcd.setCursor(data1, data2);
						lcd.print(data3);
						break;



					case 7: reset_Mouse_Cpu(); break;
					case 8: reset_slot_Cards();   break;
					case 9: reset_Sys_A_Cpu(); break;
					case 10: reset_Device_Plug(); break;
					case 11: reset_Main_Cpu(); break;
					case 12: set_Main_Cpu_To_Program_Mode(); break;
					case 13: set_Main_Cpu_To_Run_Mode(); break;


					case 254://COM TEST
						data1 = Serial.read();  //read command
						data2 = Serial.read();  //read command					
						lcd.clear();
						lcd.setCursor(0, 0); lcd.print("SYS A - SYS B");
						lcd.setCursor(0, 1); lcd.print(command);
						lcd.setCursor(4, 1); lcd.print(data1);
						lcd.setCursor(8, 1); lcd.print(data2);
						delay(800);
						lcd.clear();
						break;
					}
				}
			}
			set_Cpu_Led(0);
		}

	}




};

