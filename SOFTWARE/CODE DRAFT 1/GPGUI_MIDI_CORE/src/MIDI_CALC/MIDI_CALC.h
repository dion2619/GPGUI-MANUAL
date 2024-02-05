#include <Arduino.h>

class MIDI_CALC
{
public:



	void get_Scale(int key, int scale_Type, int note_Out_Buff[6][7])
	{
		byte key_Tones_Minor[7] = { 0,2,3,5,7,8,10 };
		byte key_Tones_Major[7] = { 0,2,4,5,7,9,11 };
		byte octave_Count = 0;
		byte note = key;
		for (int j = 0; j < 6; j++)
		{
			for (int i = 0; i < 7; i++)
			{
				switch (scale_Type)
				{
				case 0: note_Out_Buff[j][i] = note + key_Tones_Minor[i] + octave_Count; break;
				case 1: note_Out_Buff[j][i] = note + key_Tones_Major[i] + octave_Count; break;
				}
			}

			octave_Count += 12;
		}

		//MINOR KEY
		//T   2
		//st  3
		//T   5
		//T   7
		//st  8
		//T   10
		//T   12


		//MAJOR KEY
		//T   2
		//T   4
		//ST   5
		//T   7
		//T  9
		//T   11
		//ST   12

	}




};