

byte w = 63;
byte r = 3;
byte g = 8;
byte b = 48;

byte midi_Icon[16][16] =
{
	{w,w,0,0,0,w,0,0,0,w,w,0,w,w,0,0},
	{w,w,0,0,0,w,0,0,0,w,w,0,w,w,0,0},
	{w,w,0,0,0,w,0,0,0,w,w,0,w,w,0,0},
	{w,w,0,0,0,w,0,0,0,w,w,0,w,w,0,0},
	{w,w,0,0,0,w,0,0,0,w,w,0,w,w,0,0},
	{w,w,w,0,w,w,w,0,w,w,w,0,w,w,w,0},
	{w,w,w,0,w,w,w,0,w,w,w,0,w,w,w,0},
	{w,w,w,0,w,w,w,0,w,w,w,0,w,w,w,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,w,0,0,0,w,0,0,0,0,0,0,0,0},
	{0,0,w,0,0,0,w,0,r,r,r,r,r,r,r,r},
	{0,0,w,0,0,0,w,0,r,r,r,r,r,r,r,r},
	{0,0,w,0,0,w,w,0,g,g,g,g,g,g,g,g},
	{0,w,w,0,w,w,w,0,g,g,g,g,g,g,g,g},
	{w,w,w,0,0,w,0,0,b,b,b,b,b,b,b,b},
	{0,w,0,0,0,0,0,0,b,b,b,b,b,b,b,b}
};



byte exit_Sprite[16][16] =
{
{1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
{1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
{1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
{1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
{1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
{1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
{1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
{1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};




byte blank_Icon_Sprite[16][16] =
{
{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
{1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
{1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
{1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
{1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
{1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
{1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}

};

byte G = 42;

byte Plus_Button[16][16] =
{
{G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G},
{G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G},
{G, G, G, G, G, r, G, G, G, G, G, G, G, G, G, G},
{G, G, G, G, G, r, G, G, G, G, G, G, G, G, G, G},
{G, G, G, G, G, r, G, G, G, G, G, G, G, G, G, G},
{G, G, r, r, r, r, r, r, r, G, G, G, G, G, G, G},
{G, G, G, G, G, r, G, G, G, G, G, G, G, G, G, G},
{G, G, G, G, G, r, G, G, G, G, G, G, G, G, G, G},
{G, G, G, G, G, r, G, G, G, G, G, G, G, G, G, G},
{G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G},
{G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G},
{G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G},
{G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G},
{G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G},
{G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G},
{G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G}
};