#include <arduino.h>



class MOUSE_CPU
{
public:

	void Serial_Dump_Mouse();
	void Init();
	void read_Mouse();
	void set_Cpu_Led(bool state);
	void encode_Axis_Byte(int delta, int *data_Byte);
	void send_Mouse_Pos_Change(int x, int y);
	void flash_Cpu_Led(int times, int dur);
	void send_Left_Break();
	void send_Left_Make();
	void send_Middle_Break();
	void send_Middle_Make();
	void send_Right_Break();
	void send_Right_Make();
	void send_Scroll_Up();
	void send_Scroll_Down();


};