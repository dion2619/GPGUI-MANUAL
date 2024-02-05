#include <Arduino.h>
#include "SAM2695_HARDWARE_SETTINGS.h"
#include "PATCH_NAMES.h"
#include "MIDI_CALC/MIDI_CALC.h"

byte global_Bank = 0;
byte velocity_Max = 100;
byte ch_Volume_Max = 100;
int notes_On_Ch1 = 0;
bool ch1_Sus_Pedle_State = false;

MIDI_CALC mCalc;


class GPGUI_MIDI_CORE
{
public:

	//HOST COMUNICATION
	int join_Two_7_Bit_Words_To_14_Bit_Val(byte v1, byte v2)
	{
		int vi = 0;
		bitWrite(vi, 0,  bitRead(v1, 0));
		bitWrite(vi, 1,  bitRead(v1, 1));
		bitWrite(vi, 2,  bitRead(v1, 2));
		bitWrite(vi, 3,  bitRead(v1, 3));
		bitWrite(vi, 4,  bitRead(v1, 4));
		bitWrite(vi, 5,  bitRead(v1, 5));
		bitWrite(vi, 6,  bitRead(v1, 6));
		bitWrite(vi, 7,  bitRead(v2, 0));
		bitWrite(vi, 8,  bitRead(v2, 1));
		bitWrite(vi, 9,  bitRead(v2, 2));
		bitWrite(vi, 10, bitRead(v2, 3));
		bitWrite(vi, 11, bitRead(v2, 4));
		bitWrite(vi, 12, bitRead(v2, 5));
		bitWrite(vi, 13, bitRead(v2, 6));
		return vi;
	}

	void i2c_Handle(byte STATUS, byte V1, byte V2)
	{
		switch (STATUS)
		{
		//NOTE OFF
	    case 0x80:  note_Off(1,  V1); break;
		case 0x81:  note_Off(2,  V1); break;
		case 0x82:  note_Off(3,  V1); break;
		case 0x83:  note_Off(4,  V1); break;
		case 0x84:  note_Off(5,  V1); break;
		case 0x85:  note_Off(6,  V1); break;
		case 0x86:  note_Off(7,  V1); break;
		case 0x87:  note_Off(8,  V1); break;
		case 0x88:  note_Off(9,  V1); break;
		case 0x89:  note_Off(10, V1); break;
		case 0x8A:  note_Off(11, V1); break;
		case 0x8B:  note_Off(12, V1); break;
		case 0x8C:  note_Off(13, V1); break;
		case 0x8D:  note_Off(14, V1); break;
		case 0x8E:  note_Off(15, V1); break;
		case 0x8F:  note_Off(16, V1); break;

		//NOTE ON OR NOTE OFF VELOCTIY 0
		case 0x90:  if(V2 > 0) note_On(1,  V1, V2); if(V2 == 0) note_Off(1,  V1); break;
		case 0x91:  if(V2 > 0) note_On(2,  V1, V2); if(V2 == 0) note_Off(2,  V1); break;
		case 0x92:  if(V2 > 0) note_On(3,  V1, V2); if(V2 == 0) note_Off(3,  V1); break;
		case 0x93:  if(V2 > 0) note_On(4,  V1, V2); if(V2 == 0) note_Off(4,  V1); break;
		case 0x94:  if(V2 > 0) note_On(5,  V1, V2); if(V2 == 0) note_Off(5,  V1); break;
		case 0x95:  if(V2 > 0) note_On(6,  V1, V2); if(V2 == 0) note_Off(6,  V1); break;
		case 0x96:  if(V2 > 0) note_On(7,  V1, V2); if(V2 == 0) note_Off(7,  V1); break;
		case 0x97:  if(V2 > 0) note_On(8,  V1, V2); if(V2 == 0) note_Off(8,  V1); break;
		case 0x98:  if(V2 > 0) note_On(9,  V1, V2); if(V2 == 0) note_Off(9,  V1); break;
		case 0x99:  if(V2 > 0) note_On(10, V1, V2); if(V2 == 0) note_Off(10, V1); break;
		case 0x9A:  if(V2 > 0) note_On(11, V1, V2); if(V2 == 0) note_Off(11, V1); break;
		case 0x9B:  if(V2 > 0) note_On(12, V1, V2); if(V2 == 0) note_Off(12, V1); break;
		case 0x9C:  if(V2 > 0) note_On(13, V1, V2); if(V2 == 0) note_Off(13, V1); break;
		case 0x9D:  if(V2 > 0) note_On(14, V1, V2); if(V2 == 0) note_Off(14, V1); break;
		case 0x9E:  if(V2 > 0) note_On(15, V1, V2); if(V2 == 0) note_Off(15, V1); break;
		case 0x9F:  if(V2 > 0) note_On(16, V1, V2); if(V2 == 0) note_Off(16, V1); break;

		//CC CONTROL MESSAGES
		case 0xB0: midi_CC_Handle(1,  V1, V2); break;
		case 0xB1: midi_CC_Handle(2,  V1, V2); break;
		case 0xB2: midi_CC_Handle(3,  V1, V2); break;
		case 0xB3: midi_CC_Handle(4,  V1, V2); break;
		case 0xB4: midi_CC_Handle(5,  V1, V2); break;
		case 0xB5: midi_CC_Handle(6,  V1, V2); break;
		case 0xB6: midi_CC_Handle(7,  V1, V2); break;
		case 0xB7: midi_CC_Handle(8,  V1, V2); break;
		case 0xB8: midi_CC_Handle(9,  V1, V2); break;
		case 0xB9: midi_CC_Handle(10, V1, V2); break;
		case 0xBA: midi_CC_Handle(11, V1, V2); break;
		case 0xBB: midi_CC_Handle(12, V1, V2); break;
		case 0xBC: midi_CC_Handle(13, V1, V2); break;
		case 0xBD: midi_CC_Handle(14, V1, V2); break;
		case 0xBE: midi_CC_Handle(15, V1, V2); break;

		//PATCH CHANGE
		case 0xC0: set_Patch(1,  V1); break;
		case 0xC1: set_Patch(2,  V1); break;
		case 0xC2: set_Patch(3,  V1); break;
		case 0xC3: set_Patch(4,  V1); break;
		case 0xC4: set_Patch(5,  V1); break;
		case 0xC5: set_Patch(6,  V1); break;
		case 0xC6: set_Patch(7,  V1); break;
		case 0xC7: set_Patch(8,  V1); break;
		case 0xC8: set_Patch(9,  V1); break;
		case 0xC9: set_Patch(10, V1); break;
		case 0xCA: set_Patch(11, V1); break;
		case 0xCB: set_Patch(12, V1); break;
		case 0xCC: set_Patch(13, V1); break;
		case 0xCD: set_Patch(14, V1); break;
		case 0xCE: set_Patch(15, V1); break;
		case 0xCF: set_Patch(16, V1); break;

		//PITCH BEND
		case 0xE0:  pitch_Bend(1,  join_Two_7_Bit_Words_To_14_Bit_Val(V1, V2)); break;
		case 0xE1:  pitch_Bend(2,  join_Two_7_Bit_Words_To_14_Bit_Val(V1, V2)); break;
		case 0xE2:  pitch_Bend(3,  join_Two_7_Bit_Words_To_14_Bit_Val(V1, V2)); break;
		case 0xE3:  pitch_Bend(4,  join_Two_7_Bit_Words_To_14_Bit_Val(V1, V2)); break;
		case 0xE4:  pitch_Bend(5,  join_Two_7_Bit_Words_To_14_Bit_Val(V1, V2)); break;
		case 0xE5:  pitch_Bend(6,  join_Two_7_Bit_Words_To_14_Bit_Val(V1, V2)); break;
		case 0xE6:  pitch_Bend(7,  join_Two_7_Bit_Words_To_14_Bit_Val(V1, V2)); break;
		case 0xE7:  pitch_Bend(8,  join_Two_7_Bit_Words_To_14_Bit_Val(V1, V2)); break;
		case 0xE8:  pitch_Bend(9,  join_Two_7_Bit_Words_To_14_Bit_Val(V1, V2)); break;
		case 0xE9:  pitch_Bend(10, join_Two_7_Bit_Words_To_14_Bit_Val(V1, V2)); break;
		case 0xEA:  pitch_Bend(11, join_Two_7_Bit_Words_To_14_Bit_Val(V1, V2)); break;
		case 0xEB:  pitch_Bend(12, join_Two_7_Bit_Words_To_14_Bit_Val(V1, V2)); break;
		case 0xEC:  pitch_Bend(13, join_Two_7_Bit_Words_To_14_Bit_Val(V1, V2)); break;
		case 0xED:  pitch_Bend(14, join_Two_7_Bit_Words_To_14_Bit_Val(V1, V2)); break;
		case 0xEE:  pitch_Bend(15, join_Two_7_Bit_Words_To_14_Bit_Val(V1, V2)); break;
		case 0xEF:  pitch_Bend(16, join_Two_7_Bit_Words_To_14_Bit_Val(V1, V2)); break;
		case 250: velocity_Max = V1; break;
		case 251: ch_Volume_Max = V1; break;
		}
	}
	 
	void midi_CC_Handle(byte ch, byte v1, byte v2)
	{
		switch (v1)
		{
		case 0x00: set_Bank(ch, v2); break;
		case 0x1: en_Modulation_Coarse(ch, v2); break;
		case 0x7: set_Ch_Volume(ch, v2); break;
		case 0xA: set_Pan(ch, v2); break;
		//master volume
		case 0x40: if (v2 < 64) en_Sustain(ch, false);  if (v2 >= 64)  en_Sustain(ch, true); break;
		case 0x5B: en_Reverb_Coarse(ch, v2); break;
		case 0x5D: en_Chorus_Coarse(ch, v2); break;

		
		

		}
	}

	int get_Ch1_Notes_On()
	{
		return notes_On_Ch1;
	}


	//SAM2695 CHIP CONTROLS
	void SAM_INIT()
	{
		Serial.begin(115200);
		Serial2.begin(31250);
		pinMode(SAM_Reset_Pin, OUTPUT);
		SAM_Power_Up_Reset();
		set_Dac_Output_Gain(DAC_GAIN_0_DB);
		reset_Midi_Driver();
	}

	void send_Byte_To_SAM(byte vv)
	{
		Serial2.write(vv);
	}

	void SAM_Power_Up_Reset()
	{
		digitalWrite(SAM_Reset_Pin, 1);
		delay(20);
		digitalWrite(SAM_Reset_Pin, 0);
		delay(100);
	}

	void reset_Midi_Driver()
	{
		send_Byte_To_SAM(255);
	}

	void set_Dac_Output_Gain(byte level)
	{
		bool codec_Control_0[16] =
		{
			1,//0
			0,//1
			0,//2
			1,//3
			1,//4
			1,//5
			1,//6
			0,//7
			1,//8
			1,//9
			0,//10
			1,//11
			1,//12
			0,//13
			0,//14
			0 //15
		};

		//bits 5 - 0 in codec control 0 reg
		bool output_Bits[21][6] =
		{
			{1,1,1,1,1,1},//0   +6db
			{1,1,1,1,1,0},//1   +5db
			{1,1,1,1,0,1},//2   +4db
			{1,1,1,1,0,0},//3   +3db
			{1,1,1,0,1,1},//4   +2db
			{1,1,1,0,1,0},//5   +1db//any thing below here makes bad noise
			{1,1,1,0,0,1},//6   0db
			{1,1,1,0,0,0},//7   -1db
			{1,1,0,1,1,1},//8   -2db
			{1,1,0,1,1,0},//9   -3db
			{1,1,0,1,0,1},//10  -4db
			{1,1,0,1,0,0},//11  -5db
			{1,1,0,0,1,1},//12  -6db
			{1,1,0,0,1,0},//13  -7db
			{1,1,0,0,0,1},//14  -8db
			{1,1,0,0,0,0},//15  -9db
			{0,1,0,0,1,1},//16  -38db
			{0,1,0,0,1,0},//17  -39db
			{0,1,0,0,0,1},//18  -40db
			{0,1,0,0,0,0},//19  -43.5db
			{0,0,0,0,0,0} //20  -58.5db
		};

		//WRITE DAC CONTROL BITS TO CODEC CONTROL 0 REG
		for (int i = 0; i < 6; i++) codec_Control_0[i] = output_Bits[level][5 - i];

		//break into x4 4bit nibbles
		byte n0 = 0;
		byte n1 = 0;
		byte n2 = 0;
		byte n3 = 0;

		bitWrite(n0, 0, codec_Control_0[0]);
		bitWrite(n0, 1, codec_Control_0[1]);
		bitWrite(n0, 2, codec_Control_0[2]);
		bitWrite(n0, 3, codec_Control_0[3]);
		bitWrite(n1, 0, codec_Control_0[4]);
		bitWrite(n1, 1, codec_Control_0[5]);
		bitWrite(n1, 2, codec_Control_0[6]);
		bitWrite(n1, 3, codec_Control_0[7]);
		bitWrite(n2, 0, codec_Control_0[8]);
		bitWrite(n2, 1, codec_Control_0[9]);
		bitWrite(n2, 2, codec_Control_0[10]);
		bitWrite(n2, 3, codec_Control_0[11]);
		bitWrite(n3, 0, codec_Control_0[12]);
		bitWrite(n3, 1, codec_Control_0[13]);
		bitWrite(n3, 2, codec_Control_0[14]);
		bitWrite(n3, 3, codec_Control_0[15]);

		send_Byte_To_SAM(0xf0);
		send_Byte_To_SAM(0x00);
		send_Byte_To_SAM(0x20);
		send_Byte_To_SAM(0x00);
		send_Byte_To_SAM(0x00);
		send_Byte_To_SAM(0x00);
		send_Byte_To_SAM(0x12);
		send_Byte_To_SAM(0x33);
		send_Byte_To_SAM(0x77);
		send_Byte_To_SAM(0x12);
		//
		send_Byte_To_SAM(n3);
		send_Byte_To_SAM(n2);
		send_Byte_To_SAM(n1);
		send_Byte_To_SAM(n0);
		//
		send_Byte_To_SAM(0xf7);
	}




	//MIDI VOICE CONTROLS

	void set_Bank(byte ch, byte bank)
	{
		//bank 1 = 0
        //bank 2 = 127
		if (bank > 0 && bank < 0x7f) bank = 0x7f;
		if (bank > 0x7F) bank = 0x7f;

		switch (ch)
		{
		case 1:  send_Byte_To_SAM(0xB0); send_Byte_To_SAM(0x00); send_Byte_To_SAM(bank); break;
		case 2:  send_Byte_To_SAM(0xB1); send_Byte_To_SAM(0x00); send_Byte_To_SAM(bank); break;
		case 3:  send_Byte_To_SAM(0xB2); send_Byte_To_SAM(0x00); send_Byte_To_SAM(bank); break;
		case 4:  send_Byte_To_SAM(0xB3); send_Byte_To_SAM(0x00); send_Byte_To_SAM(bank); break;
		case 5:  send_Byte_To_SAM(0xB4); send_Byte_To_SAM(0x00); send_Byte_To_SAM(bank); break;
		case 6:  send_Byte_To_SAM(0xB5); send_Byte_To_SAM(0x00); send_Byte_To_SAM(bank); break;
		case 7:  send_Byte_To_SAM(0xB6); send_Byte_To_SAM(0x00); send_Byte_To_SAM(bank); break;
		case 8:  send_Byte_To_SAM(0xB7); send_Byte_To_SAM(0x00); send_Byte_To_SAM(bank); break;
		case 9:  send_Byte_To_SAM(0xB8); send_Byte_To_SAM(0x00); send_Byte_To_SAM(bank); break;
		case 10: send_Byte_To_SAM(0xB9); send_Byte_To_SAM(0x00); send_Byte_To_SAM(bank); break;
		case 11: send_Byte_To_SAM(0xBA); send_Byte_To_SAM(0x00); send_Byte_To_SAM(bank); break;
		case 12: send_Byte_To_SAM(0xBB); send_Byte_To_SAM(0x00); send_Byte_To_SAM(bank); break;
		case 13: send_Byte_To_SAM(0xBC); send_Byte_To_SAM(0x00); send_Byte_To_SAM(bank); break;
		case 14: send_Byte_To_SAM(0xBD); send_Byte_To_SAM(0x00); send_Byte_To_SAM(bank); break;
		case 15: send_Byte_To_SAM(0xBE); send_Byte_To_SAM(0x00); send_Byte_To_SAM(bank); break;
		case 16: send_Byte_To_SAM(0xBF); send_Byte_To_SAM(0x00); send_Byte_To_SAM(bank); break;
		}
	}

	void set_Patch(byte ch, byte patch)
	{
		//patch = patch - 1;
		if (patch > 0x7F) patch = 0x7f;

		switch (ch)
		{
		case 1:  send_Byte_To_SAM(0xC0); send_Byte_To_SAM(patch); break;
		case 2:  send_Byte_To_SAM(0xC1); send_Byte_To_SAM(patch); break;
		case 3:  send_Byte_To_SAM(0xC2); send_Byte_To_SAM(patch); break;
		case 4:  send_Byte_To_SAM(0xC3); send_Byte_To_SAM(patch); break;
		case 5:  send_Byte_To_SAM(0xC4); send_Byte_To_SAM(patch); break;
		case 6:  send_Byte_To_SAM(0xC5); send_Byte_To_SAM(patch); break;
		case 7:  send_Byte_To_SAM(0xC6); send_Byte_To_SAM(patch); break;
		case 8:  send_Byte_To_SAM(0xC7); send_Byte_To_SAM(patch); break;
		case 9:  send_Byte_To_SAM(0xC8); send_Byte_To_SAM(patch); break;
		case 10: send_Byte_To_SAM(0xC9); send_Byte_To_SAM(patch); break;
		case 11: send_Byte_To_SAM(0xCA); send_Byte_To_SAM(patch); break;
		case 12: send_Byte_To_SAM(0xCB); send_Byte_To_SAM(patch); break;
		case 13: send_Byte_To_SAM(0xCC); send_Byte_To_SAM(patch); break;
		case 14: send_Byte_To_SAM(0xCD); send_Byte_To_SAM(patch); break;
		case 15: send_Byte_To_SAM(0xCE); send_Byte_To_SAM(patch); break;
		case 16: send_Byte_To_SAM(0xCF); send_Byte_To_SAM(patch); break;
		}
	}

	void set_Voice(byte ch, byte v, byte bank)
	{
		if (bank > 0 && bank < 0x7f) bank = 0x7f;
		if (bank > 0x7F) bank = 0x7f;
		set_Bank(ch, bank);
		set_Patch(ch, v);
	}

	void note_On(byte ch, byte note, byte vel)
	{
		if (vel > velocity_Max) vel = velocity_Max;
		if (note > 0x7F) note = 0x7F;

		switch (ch)
		{
		case 1:   send_Byte_To_SAM(0x90); send_Byte_To_SAM(note); send_Byte_To_SAM(vel); if (vel > 0) notes_On_Ch1++; else if (notes_On_Ch1 > 0 && !ch1_Sus_Pedle_State) notes_On_Ch1--; break;
		case 2:   send_Byte_To_SAM(0x91); send_Byte_To_SAM(note); send_Byte_To_SAM(vel); break;
		case 3:   send_Byte_To_SAM(0x92); send_Byte_To_SAM(note); send_Byte_To_SAM(vel); break;
		case 4:   send_Byte_To_SAM(0x93); send_Byte_To_SAM(note); send_Byte_To_SAM(vel); break;
		case 5:   send_Byte_To_SAM(0x94); send_Byte_To_SAM(note); send_Byte_To_SAM(vel); break;
		case 6:   send_Byte_To_SAM(0x95); send_Byte_To_SAM(note); send_Byte_To_SAM(vel); break;
		case 7:   send_Byte_To_SAM(0x96); send_Byte_To_SAM(note); send_Byte_To_SAM(vel); break;
		case 8:   send_Byte_To_SAM(0x97); send_Byte_To_SAM(note); send_Byte_To_SAM(vel); break;
		case 9:   send_Byte_To_SAM(0x98); send_Byte_To_SAM(note); send_Byte_To_SAM(vel); break;
		case 10:  send_Byte_To_SAM(0x99); send_Byte_To_SAM(note); send_Byte_To_SAM(vel); break;
		case 11:  send_Byte_To_SAM(0x9A); send_Byte_To_SAM(note); send_Byte_To_SAM(vel); break;
		case 12:  send_Byte_To_SAM(0x9B); send_Byte_To_SAM(note); send_Byte_To_SAM(vel); break;
		case 13:  send_Byte_To_SAM(0x9C); send_Byte_To_SAM(note); send_Byte_To_SAM(vel); break;
		case 14:  send_Byte_To_SAM(0x9D); send_Byte_To_SAM(note); send_Byte_To_SAM(vel); break;
		case 15:  send_Byte_To_SAM(0x9E); send_Byte_To_SAM(note); send_Byte_To_SAM(vel); break;
		case 16:  send_Byte_To_SAM(0x9F); send_Byte_To_SAM(note); send_Byte_To_SAM(vel); break;
		}
	}

	void note_Off(byte ch, byte note)
	{
		if (note > 0x7F) note = 0x7F;

		switch (ch)
		{
		case 1:   send_Byte_To_SAM(0x80); send_Byte_To_SAM(note); send_Byte_To_SAM(0x00); if (notes_On_Ch1 > 0 && !ch1_Sus_Pedle_State) notes_On_Ch1--; break;
		case 2:   send_Byte_To_SAM(0x81); send_Byte_To_SAM(note); send_Byte_To_SAM(0x00); break;
		case 3:   send_Byte_To_SAM(0x82); send_Byte_To_SAM(note); send_Byte_To_SAM(0x00); break;
		case 4:   send_Byte_To_SAM(0x83); send_Byte_To_SAM(note); send_Byte_To_SAM(0x00); break;
		case 5:   send_Byte_To_SAM(0x84); send_Byte_To_SAM(note); send_Byte_To_SAM(0x00); break;
		case 6:   send_Byte_To_SAM(0x85); send_Byte_To_SAM(note); send_Byte_To_SAM(0x00); break;
		case 7:   send_Byte_To_SAM(0x86); send_Byte_To_SAM(note); send_Byte_To_SAM(0x00); break;
		case 8:   send_Byte_To_SAM(0x87); send_Byte_To_SAM(note); send_Byte_To_SAM(0x00); break;
		case 9:   send_Byte_To_SAM(0x88); send_Byte_To_SAM(note); send_Byte_To_SAM(0x00); break;
		case 10:  send_Byte_To_SAM(0x89); send_Byte_To_SAM(note); send_Byte_To_SAM(0x00); break;
		case 11:  send_Byte_To_SAM(0x8A); send_Byte_To_SAM(note); send_Byte_To_SAM(0x00); break;
		case 12:  send_Byte_To_SAM(0x8B); send_Byte_To_SAM(note); send_Byte_To_SAM(0x00); break;
		case 13:  send_Byte_To_SAM(0x8C); send_Byte_To_SAM(note); send_Byte_To_SAM(0x00); break;
		case 14:  send_Byte_To_SAM(0x8D); send_Byte_To_SAM(note); send_Byte_To_SAM(0x00); break;
		case 15:  send_Byte_To_SAM(0x8E); send_Byte_To_SAM(note); send_Byte_To_SAM(0x00); break;
		case 16:  send_Byte_To_SAM(0x8F); send_Byte_To_SAM(note); send_Byte_To_SAM(0x00); break;
		}
	}

	void en_Sustain(byte ch, bool state)
	{
		byte pedel_State;
		if (state)  pedel_State = 0x40;
		if (!state) pedel_State = 0x3f;
		switch (ch)
		{
		case 1:  send_Byte_To_SAM(0xB0); send_Byte_To_SAM(0x40); send_Byte_To_SAM(pedel_State); ch1_Sus_Pedle_State = state; if (!ch1_Sus_Pedle_State) notes_On_Ch1 = 0; break;
		case 2:  send_Byte_To_SAM(0xB1); send_Byte_To_SAM(0x40); send_Byte_To_SAM(pedel_State);  break;
		case 3:  send_Byte_To_SAM(0xB2); send_Byte_To_SAM(0x40); send_Byte_To_SAM(pedel_State);  break;
		case 4:  send_Byte_To_SAM(0xB3); send_Byte_To_SAM(0x40); send_Byte_To_SAM(pedel_State);  break;
		case 5:  send_Byte_To_SAM(0xB4); send_Byte_To_SAM(0x40); send_Byte_To_SAM(pedel_State);  break;
		case 6:  send_Byte_To_SAM(0xB5); send_Byte_To_SAM(0x40); send_Byte_To_SAM(pedel_State);  break;
		case 7:  send_Byte_To_SAM(0xB6); send_Byte_To_SAM(0x40); send_Byte_To_SAM(pedel_State);  break;
		case 8:  send_Byte_To_SAM(0xB7); send_Byte_To_SAM(0x40); send_Byte_To_SAM(pedel_State);  break;
		case 9:  send_Byte_To_SAM(0xB8); send_Byte_To_SAM(0x40); send_Byte_To_SAM(pedel_State);  break;
		case 10: send_Byte_To_SAM(0xB9); send_Byte_To_SAM(0x40); send_Byte_To_SAM(pedel_State);  break;
		case 11: send_Byte_To_SAM(0xBA); send_Byte_To_SAM(0x40); send_Byte_To_SAM(pedel_State);  break;
		case 12: send_Byte_To_SAM(0xBB); send_Byte_To_SAM(0x40); send_Byte_To_SAM(pedel_State);  break;
		case 13: send_Byte_To_SAM(0xBC); send_Byte_To_SAM(0x40); send_Byte_To_SAM(pedel_State);  break;
		case 14: send_Byte_To_SAM(0xBD); send_Byte_To_SAM(0x40); send_Byte_To_SAM(pedel_State);  break;
		case 15: send_Byte_To_SAM(0xBE); send_Byte_To_SAM(0x40); send_Byte_To_SAM(pedel_State);  break;
		case 16: send_Byte_To_SAM(0xBF); send_Byte_To_SAM(0x40); send_Byte_To_SAM(pedel_State);  break;
		}
	}

	//VOLUME

	void set_Ch_Volume(byte ch, byte level)
	{
		if (level > ch_Volume_Max) level = ch_Volume_Max;
		switch (ch)
		{
		case 1:  send_Byte_To_SAM(0xB0); send_Byte_To_SAM(0x7); send_Byte_To_SAM(level);  break;
		case 2:  send_Byte_To_SAM(0xB1); send_Byte_To_SAM(0x7); send_Byte_To_SAM(level);  break;
		case 3:  send_Byte_To_SAM(0xB2); send_Byte_To_SAM(0x7); send_Byte_To_SAM(level);  break;
		case 4:  send_Byte_To_SAM(0xB3); send_Byte_To_SAM(0x7); send_Byte_To_SAM(level);  break;
		case 5:  send_Byte_To_SAM(0xB4); send_Byte_To_SAM(0x7); send_Byte_To_SAM(level);  break;
		case 6:  send_Byte_To_SAM(0xB5); send_Byte_To_SAM(0x7); send_Byte_To_SAM(level);  break;
		case 7:  send_Byte_To_SAM(0xB6); send_Byte_To_SAM(0x7); send_Byte_To_SAM(level);  break;
		case 8:  send_Byte_To_SAM(0xB7); send_Byte_To_SAM(0x7); send_Byte_To_SAM(level);  break;
		case 9:  send_Byte_To_SAM(0xB8); send_Byte_To_SAM(0x7); send_Byte_To_SAM(level);  break;
		case 10: send_Byte_To_SAM(0xB9); send_Byte_To_SAM(0x7); send_Byte_To_SAM(level);  break;
		case 11: send_Byte_To_SAM(0xBA); send_Byte_To_SAM(0x7); send_Byte_To_SAM(level);  break;
		case 12: send_Byte_To_SAM(0xBB); send_Byte_To_SAM(0x7); send_Byte_To_SAM(level);  break;
		case 13: send_Byte_To_SAM(0xBC); send_Byte_To_SAM(0x7); send_Byte_To_SAM(level);  break;
		case 14: send_Byte_To_SAM(0xBD); send_Byte_To_SAM(0x7); send_Byte_To_SAM(level);  break;
		case 15: send_Byte_To_SAM(0xBE); send_Byte_To_SAM(0x7); send_Byte_To_SAM(level);  break;
		case 16: send_Byte_To_SAM(0xBF); send_Byte_To_SAM(0x7); send_Byte_To_SAM(level);  break;
		}
	}

	void set_Pan(byte ch, byte pos)
	{
		if (pos > 0x7F) pos = 0x7F;
		switch (ch)
		{
		case 1:  send_Byte_To_SAM(0xB0); send_Byte_To_SAM(0xA); send_Byte_To_SAM(pos);  break;
		case 2:  send_Byte_To_SAM(0xB1); send_Byte_To_SAM(0xA); send_Byte_To_SAM(pos);  break;
		case 3:  send_Byte_To_SAM(0xB2); send_Byte_To_SAM(0xA); send_Byte_To_SAM(pos);  break;
		case 4:  send_Byte_To_SAM(0xB3); send_Byte_To_SAM(0xA); send_Byte_To_SAM(pos);  break;
		case 5:  send_Byte_To_SAM(0xB4); send_Byte_To_SAM(0xA); send_Byte_To_SAM(pos);  break;
		case 6:  send_Byte_To_SAM(0xB5); send_Byte_To_SAM(0xA); send_Byte_To_SAM(pos);  break;
		case 7:  send_Byte_To_SAM(0xB6); send_Byte_To_SAM(0xA); send_Byte_To_SAM(pos);  break;
		case 8:  send_Byte_To_SAM(0xB7); send_Byte_To_SAM(0xA); send_Byte_To_SAM(pos);  break;
		case 9:  send_Byte_To_SAM(0xB8); send_Byte_To_SAM(0xA); send_Byte_To_SAM(pos);  break;
		case 10: send_Byte_To_SAM(0xB9); send_Byte_To_SAM(0xA); send_Byte_To_SAM(pos);  break;
		case 11: send_Byte_To_SAM(0xBA); send_Byte_To_SAM(0xA); send_Byte_To_SAM(pos);  break;
		case 12: send_Byte_To_SAM(0xBB); send_Byte_To_SAM(0xA); send_Byte_To_SAM(pos);  break;
		case 13: send_Byte_To_SAM(0xBC); send_Byte_To_SAM(0xA); send_Byte_To_SAM(pos);  break;
		case 14: send_Byte_To_SAM(0xBD); send_Byte_To_SAM(0xA); send_Byte_To_SAM(pos);  break;
		case 15: send_Byte_To_SAM(0xBE); send_Byte_To_SAM(0xA); send_Byte_To_SAM(pos);  break;
		case 16: send_Byte_To_SAM(0xBF); send_Byte_To_SAM(0xA); send_Byte_To_SAM(pos);  break;
		}
	}

	//REVERB
	void en_Reverb_Coarse(byte ch, byte level)
	{
		if (level > 0x7F) level = 0x7F;

		switch (ch)
		{
		case 1:   send_Byte_To_SAM(0xB0); send_Byte_To_SAM(0x5B); send_Byte_To_SAM(level); break;
		case 2:   send_Byte_To_SAM(0xB1); send_Byte_To_SAM(0x5B); send_Byte_To_SAM(level); break;
		case 3:   send_Byte_To_SAM(0xB2); send_Byte_To_SAM(0x5B); send_Byte_To_SAM(level); break;
		case 4:   send_Byte_To_SAM(0xB3); send_Byte_To_SAM(0x5B); send_Byte_To_SAM(level); break;
		case 5:   send_Byte_To_SAM(0xB4); send_Byte_To_SAM(0x5B); send_Byte_To_SAM(level); break;
		case 6:   send_Byte_To_SAM(0xB5); send_Byte_To_SAM(0x5B); send_Byte_To_SAM(level); break;
		case 7:   send_Byte_To_SAM(0xB6); send_Byte_To_SAM(0x5B); send_Byte_To_SAM(level); break;
		case 8:   send_Byte_To_SAM(0xB7); send_Byte_To_SAM(0x5B); send_Byte_To_SAM(level); break;
		case 9:   send_Byte_To_SAM(0xB8); send_Byte_To_SAM(0x5B); send_Byte_To_SAM(level); break;
		case 10:  send_Byte_To_SAM(0xB9); send_Byte_To_SAM(0x5B); send_Byte_To_SAM(level); break;
		case 11:  send_Byte_To_SAM(0xBA); send_Byte_To_SAM(0x5B); send_Byte_To_SAM(level); break;
		case 12:  send_Byte_To_SAM(0xBB); send_Byte_To_SAM(0x5B); send_Byte_To_SAM(level); break;
		case 13:  send_Byte_To_SAM(0xBC); send_Byte_To_SAM(0x5B); send_Byte_To_SAM(level); break;
		case 14:  send_Byte_To_SAM(0xBD); send_Byte_To_SAM(0x5B); send_Byte_To_SAM(level); break;
		case 15:  send_Byte_To_SAM(0xBE); send_Byte_To_SAM(0x5B); send_Byte_To_SAM(level); break;
		case 16:  send_Byte_To_SAM(0xBF); send_Byte_To_SAM(0x5B); send_Byte_To_SAM(level); break;

		}
	
	}




	//CHORUS
	void en_Chorus_Coarse(byte ch, byte level)
	{
		if (level > 0x7F) level = 0x7F;

		switch (ch)
		{
		case 1:   send_Byte_To_SAM(0xB0); send_Byte_To_SAM(0x5D); send_Byte_To_SAM(level); break;
		case 2:   send_Byte_To_SAM(0xB1); send_Byte_To_SAM(0x5D); send_Byte_To_SAM(level); break;
		case 3:   send_Byte_To_SAM(0xB2); send_Byte_To_SAM(0x5D); send_Byte_To_SAM(level); break;
		case 4:   send_Byte_To_SAM(0xB3); send_Byte_To_SAM(0x5D); send_Byte_To_SAM(level); break;
		case 5:   send_Byte_To_SAM(0xB4); send_Byte_To_SAM(0x5D); send_Byte_To_SAM(level); break;
		case 6:   send_Byte_To_SAM(0xB5); send_Byte_To_SAM(0x5D); send_Byte_To_SAM(level); break;
		case 7:   send_Byte_To_SAM(0xB6); send_Byte_To_SAM(0x5D); send_Byte_To_SAM(level); break;
		case 8:   send_Byte_To_SAM(0xB7); send_Byte_To_SAM(0x5D); send_Byte_To_SAM(level); break;
		case 9:   send_Byte_To_SAM(0xB8); send_Byte_To_SAM(0x5D); send_Byte_To_SAM(level); break;
		case 10:  send_Byte_To_SAM(0xB9); send_Byte_To_SAM(0x5D); send_Byte_To_SAM(level); break;
		case 11:  send_Byte_To_SAM(0xBA); send_Byte_To_SAM(0x5D); send_Byte_To_SAM(level); break;
		case 12:  send_Byte_To_SAM(0xBB); send_Byte_To_SAM(0x5D); send_Byte_To_SAM(level); break;
		case 13:  send_Byte_To_SAM(0xBC); send_Byte_To_SAM(0x5D); send_Byte_To_SAM(level); break;
		case 14:  send_Byte_To_SAM(0xBD); send_Byte_To_SAM(0x5D); send_Byte_To_SAM(level); break;
		case 15:  send_Byte_To_SAM(0xBE); send_Byte_To_SAM(0x5D); send_Byte_To_SAM(level); break;
		case 16:  send_Byte_To_SAM(0xBF); send_Byte_To_SAM(0x5D); send_Byte_To_SAM(level); break;
		}
	}



	//MODULATION
	void en_Modulation_Coarse(byte ch, byte level)
	{
		if (level > 0x7F) level = 0x7F;
		switch (ch)
		{
		case 1:  send_Byte_To_SAM(0xB0); send_Byte_To_SAM(0x01); send_Byte_To_SAM(level);  break;
		case 2:  send_Byte_To_SAM(0xB1); send_Byte_To_SAM(0x01); send_Byte_To_SAM(level);  break;
		case 3:  send_Byte_To_SAM(0xB2); send_Byte_To_SAM(0x01); send_Byte_To_SAM(level);  break;
		case 4:  send_Byte_To_SAM(0xB3); send_Byte_To_SAM(0x01); send_Byte_To_SAM(level);  break;
		case 5:  send_Byte_To_SAM(0xB4); send_Byte_To_SAM(0x01); send_Byte_To_SAM(level);  break;
		case 6:  send_Byte_To_SAM(0xB5); send_Byte_To_SAM(0x01); send_Byte_To_SAM(level);  break;
		case 7:  send_Byte_To_SAM(0xB6); send_Byte_To_SAM(0x01); send_Byte_To_SAM(level);  break;
		case 8:  send_Byte_To_SAM(0xB7); send_Byte_To_SAM(0x01); send_Byte_To_SAM(level);  break;
		case 9:  send_Byte_To_SAM(0xB8); send_Byte_To_SAM(0x01); send_Byte_To_SAM(level);  break;
		case 10: send_Byte_To_SAM(0xB9); send_Byte_To_SAM(0x01); send_Byte_To_SAM(level);  break;
		case 11: send_Byte_To_SAM(0xBA); send_Byte_To_SAM(0x01); send_Byte_To_SAM(level);  break;
		case 12: send_Byte_To_SAM(0xBB); send_Byte_To_SAM(0x01); send_Byte_To_SAM(level);  break;
		case 13: send_Byte_To_SAM(0xBC); send_Byte_To_SAM(0x01); send_Byte_To_SAM(level);  break;
		case 14: send_Byte_To_SAM(0xBD); send_Byte_To_SAM(0x01); send_Byte_To_SAM(level);  break;
		case 15: send_Byte_To_SAM(0xBE); send_Byte_To_SAM(0x01); send_Byte_To_SAM(level);  break;
		case 16: send_Byte_To_SAM(0xBF); send_Byte_To_SAM(0x01); send_Byte_To_SAM(level);  break;
		}	
	}



	//PITCH BEND
	void pitch_Bend(byte ch, int pos)
	{
		if (pos > 4095) pos = 4095;
		if (pos < 0)    pos = 0;
		int POS = map(pos, 0, 4095, 0x00, 0x4000);

		    byte LSB = 0;
			byte MSB = 0;

			bitWrite(LSB, 0, bitRead(POS, 0));
			bitWrite(LSB, 1, bitRead(POS, 1));
			bitWrite(LSB, 2, bitRead(POS, 2));
			bitWrite(LSB, 3, bitRead(POS, 3));
			bitWrite(LSB, 4, bitRead(POS, 4));
			bitWrite(LSB, 5, bitRead(POS, 5));
			bitWrite(LSB, 6, bitRead(POS, 6));

			bitWrite(MSB, 0, bitRead(POS, 7));
			bitWrite(MSB, 1, bitRead(POS, 8));
			bitWrite(MSB, 2, bitRead(POS, 9));
			bitWrite(MSB, 3, bitRead(POS, 10));
			bitWrite(MSB, 4, bitRead(POS, 11));
			bitWrite(MSB, 5, bitRead(POS, 12));
			bitWrite(MSB, 6, bitRead(POS, 13));


		switch (ch)
		{
		case 1:  send_Byte_To_SAM(0xE0); send_Byte_To_SAM(LSB); send_Byte_To_SAM(MSB);  break;
		case 2:  send_Byte_To_SAM(0xE1); send_Byte_To_SAM(LSB); send_Byte_To_SAM(MSB);  break;
		case 3:  send_Byte_To_SAM(0xE2); send_Byte_To_SAM(LSB); send_Byte_To_SAM(MSB);  break;
		case 4:  send_Byte_To_SAM(0xE3); send_Byte_To_SAM(LSB); send_Byte_To_SAM(MSB);  break;
		case 5:  send_Byte_To_SAM(0xE4); send_Byte_To_SAM(LSB); send_Byte_To_SAM(MSB);  break;
		case 6:  send_Byte_To_SAM(0xE5); send_Byte_To_SAM(LSB); send_Byte_To_SAM(MSB);  break;
		case 7:  send_Byte_To_SAM(0xE6); send_Byte_To_SAM(LSB); send_Byte_To_SAM(MSB);  break;
		case 8:  send_Byte_To_SAM(0xE7); send_Byte_To_SAM(LSB); send_Byte_To_SAM(MSB);  break;
		case 9:  send_Byte_To_SAM(0xE8); send_Byte_To_SAM(LSB); send_Byte_To_SAM(MSB);  break;
		case 10: send_Byte_To_SAM(0xE9); send_Byte_To_SAM(LSB); send_Byte_To_SAM(MSB);  break;
		case 11: send_Byte_To_SAM(0xEA); send_Byte_To_SAM(LSB); send_Byte_To_SAM(MSB);  break;
		case 12: send_Byte_To_SAM(0xEB); send_Byte_To_SAM(LSB); send_Byte_To_SAM(MSB);  break;
		case 13: send_Byte_To_SAM(0xEC); send_Byte_To_SAM(LSB); send_Byte_To_SAM(MSB);  break;
		case 14: send_Byte_To_SAM(0xED); send_Byte_To_SAM(LSB); send_Byte_To_SAM(MSB);  break;
		case 15: send_Byte_To_SAM(0xEE); send_Byte_To_SAM(LSB); send_Byte_To_SAM(MSB);  break;
		case 16: send_Byte_To_SAM(0xEF); send_Byte_To_SAM(LSB); send_Byte_To_SAM(MSB);  break;
		}
	}

	void set_Pitch_Bend_To_Center(byte ch)
	{
		pitch_Bend(ch, 0x2000);
	}


	
	//SOUND FX

	void machine_Gun_FX(byte ch, byte note, byte velocity, byte rate, byte volume, bool en)
	{
		static bool setup_Sound = false;
		static unsigned long start_Time = 0;
		unsigned long current_Time = millis();

		if (rate < 30)  rate = 30;
		if (rate > 127) rate = 127;

		if (current_Time - start_Time > rate)
		{
			start_Time = current_Time;
			if (en)
			{
				set_Patch(ch, GUN_SHOT);
				set_Ch_Volume(ch, volume);
				note_Off(ch, note);
			    note_On(ch, note, velocity);
			}
		}
	}

	void random_Music_Gen_1(bool en)
	{
		static byte drum_Sets[5] = {STANDARD_SET,POWER_SET,BRUSH_SET,ORCHESTRA_SET,CM_SET};
		static byte current_Set = 0;
		static byte drum_Hit_Count = 0;
		static byte ch = 10;        
		static byte velocity = 100;
		static byte note;
		static byte note2;
		static bool drums_En = 1;
		static bool drums_Wait = false;
		static bool gun_Enable = false;
		static unsigned long start_Time_Note = 0;
		static unsigned long start_Time_Drum = 0;
		static unsigned long start_Time_Drum_Wait = 0;
		static unsigned long current_Time_Drum_Wait = 0;
		static unsigned long interval = 1000;
		static unsigned long interval_Wait = 1000;

		static int notes[6][7];
		static bool coin_Flip = 0;
		static byte chance = random(0,100);

		if (chance <= 50) coin_Flip = 0;//HEAD
		if (chance >  50) coin_Flip = 1;//TAIL
		mCalc.get_Scale(12, (int)coin_Flip, notes);

		

		if (en)
		{
			unsigned long current_Time_Note = millis();
			if (current_Time_Note - start_Time_Note > interval)
			{
				start_Time_Note = current_Time_Note;
				note_Off(1, note2);
				note_Off(1, note2 + 12);
				note2 =   (byte)notes[random(0, 3)][random(0, 7)];
				set_Patch(1, 53);
				note_On(1, note2, velocity);
				note_On(1, note2 + 12, velocity);
				interval = random(500, 8000);
			}

			unsigned long current_Time_Drum = millis();
			if (current_Time_Drum - start_Time_Drum > 500)
			{
				start_Time_Drum = current_Time_Drum;
				if (!drums_Wait)
				{
					if (drum_Hit_Count == 0)
					{
						set_Patch(ch, drum_Sets[current_Set]);
						current_Set++;
						if (current_Set > 5) current_Set = 0;
					}
					drum_Hit_Count++;
					note_Off(ch, note);
					note = random(48, 60);
					note_On(ch, note, velocity);
					if (drum_Hit_Count > 20)
					{
						drum_Hit_Count = 0;
						drums_Wait = true;
						start_Time_Drum_Wait = millis();
						chance = random(0, 100);
					}
				}
			}


			current_Time_Drum_Wait = millis();
			if (current_Time_Drum_Wait - start_Time_Drum_Wait > interval_Wait && drums_Wait)
			{
				start_Time_Drum_Wait = current_Time_Drum_Wait;
				drums_Wait = false;
				interval_Wait = random(500, 3000);
			}
		}

	}




};
