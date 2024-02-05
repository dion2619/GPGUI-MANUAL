#pragma once
#include <stdlib.h>
#include <math.h>
#include "Font.h"


template<typename Color>
class Graphics 
{
  public:
	int cursorX, cursorY, cursorBaseX;
	long frontColor, backColor;
	Font *font;
	int frameBufferCount;
	int currentFrameBuffer;
	Color **frameBuffers[3];
	Color **frontBuffer;
	Color **backBuffer;
	int xres;
	int yres;
	virtual void dotFast(int x, int y, Color color) = 0;
	virtual void dot(int x, int y, Color color) = 0;
	virtual void dotAdd(int x, int y, Color color) = 0;
	virtual void dotMix(int x, int y, Color color) = 0;
	virtual Color get(int x, int y) = 0;
	virtual Color** allocateFrameBuffer() = 0;
	virtual Color** allocateFrameBuffer(int xres, int yres, Color value)
	{
		Color** frame = (Color **)malloc(yres * sizeof(Color *));
				
		for (int y = 0; y < yres; y++)
		{
			frame[y] = (Color *)malloc(xres * sizeof(Color));

			for (int x = 0; x < xres; x++)
				frame[y][x] = value;
		}
		return frame;
	}
	virtual Color RGBA(int r, int g, int b, int a = 255) const = 0;
	virtual int R(Color c) const = 0;
	virtual int G(Color c) const = 0;
	virtual int B(Color c) const = 0;
	virtual int A(Color c) const = 0;

	Color RGB(unsigned long rgb) const 
	{
		return RGBA(rgb & 255, (rgb >> 8) & 255, (rgb >> 16) & 255);
	}

	Color RGBA(unsigned long rgba) const 
	{
		return RGBA(rgba & 255, (rgba >> 8) & 255, (rgba >> 16) & 255, rgba >> 24);
	}

	Color RGB(int r, int g, int b) const 
	{
		return RGBA(r, g, b);
	}

	void setFrameBufferCount(unsigned char i)
	{
		frameBufferCount = i > 3 ? 3 : i;
	}

	virtual void show(bool vSync = false)
	{
		if(!frameBufferCount)
			return;
		currentFrameBuffer = (currentFrameBuffer + 1) % frameBufferCount;
		frontBuffer = frameBuffers[currentFrameBuffer];
		backBuffer = frameBuffers[(currentFrameBuffer + frameBufferCount - 1) % frameBufferCount];
	}

	Graphics(int xres = 0, int yres = 0)
	{
		this->xres = xres;
		this->yres = yres;
		font = 0;
		cursorX = cursorY = cursorBaseX = 0;
		frontColor = -1;
		backColor = 0;
		frameBufferCount = 1;
		for(int i = 0; i < 3; i++)
			frameBuffers[i] = 0;
		frontBuffer = 0;
		backBuffer = 0;
	}

	virtual bool allocateFrameBuffers()
	{
		if(yres <= 0 || xres <= 0)
			return false;
		for(int i = 0; i < frameBufferCount; i++)
			frameBuffers[i] = allocateFrameBuffer();
		currentFrameBuffer = 0;
		show();///////////////////////////////////////////////////////////?
		return true;
	}

	virtual void setResolution(int xres, int yres)
	{
		this->xres = xres;
		this->yres = yres;
		allocateFrameBuffers();
	}

	void setTextColor(long front, long back = 0)
	{
		frontColor = front;
		backColor = back;
	}

	void setFont(Font &font)
	{
		this->font = &font;
	}

	void setCursor(int x, int y)
	{
		cursorX = cursorBaseX = x;
		cursorY = y;
	}

	virtual void drawChar(int x, int y, int ch)
	{
		if (!font)
			return;
		if (!font->valid(ch))
			return;
		const unsigned char *pix = &font->pixels[font->charWidth * font->charHeight * (ch - font->firstChar)];
		for (int py = 0; py < font->charHeight; py++)
			for (int px = 0; px < font->charWidth; px++)
				if (*(pix++))
					dotMix(px + x, py + y, frontColor);
				else
					dotMix(px + x, py + y, backColor);
	}

	void print(const char ch)
	{
		if (!font)
			return;
		if (font->valid(ch))
			drawChar(cursorX, cursorY, ch);
		else
			drawChar(cursorX, cursorY, ' ');		
		cursorX += font->charWidth;
		if (cursorX + font->charWidth > xres)
		{
			cursorX = cursorBaseX;
			cursorY += font->charHeight;
		}
	}

	void println(const char ch)
	{
		print(ch);
		print("\n");
	}

	void print(const char *str)
	{
		if (!font)
			return;
		while (*str)
		{
			if(*str == '\n')
			{
				cursorX = cursorBaseX;
				cursorY += font->charHeight;
			}
			else
				print(*str);
			str++;
		}
	}

	void println(const char *str)
	{
		print(str); 
		print("\n");
	}

	void print(long number, int base = 10, int minCharacters = 1)
	{
		if(minCharacters < 1)
			minCharacters = 1;
		bool sign = number < 0;
		if (sign)
			number = -number;
		const char baseChars[] = "0123456789ABCDEF";
		char temp[33];
		temp[32] = 0;
		int i = 31;
		do
		{
			temp[i--] = baseChars[number % base];
			number /= base;
		} while (number > 0);
		if (sign)
			temp[i--] = '-';
		for (; i > 31 - minCharacters; i--)
			temp[i] = ' ';
		print(&temp[i + 1]);
	}

	void print(unsigned long number, int base = 10, int minCharacters = 1)
	{
		if(minCharacters < 1)
			minCharacters = 1;
		const char baseChars[] = "0123456789ABCDEF";
		char temp[33];
		temp[32] = 0;
		int i = 31;
		do
		{
			temp[i--] = baseChars[number % base];
			number /= base;
		} while (number > 0);
		for (; i > 31 - minCharacters; i--)
			temp[i] = ' ';
		print(&temp[i + 1]);
	}	

	void println(long number, int base = 10, int minCharacters = 1)
	{
		print(number, base, minCharacters); print("\n");
	}

	void println(unsigned long number, int base = 10, int minCharacters = 1)
	{
		print(number, base, minCharacters); print("\n");
	}

	void print(int number, int base = 10, int minCharacters = 1)
	{
		print(long(number), base, minCharacters);
	}

	void println(int number, int base = 10, int minCharacters = 1)
	{
		println(long(number), base, minCharacters);
	}

	void print(unsigned int number, int base = 10, int minCharacters = 1)
	{
		print((unsigned long)(number), base, minCharacters);
	}

	void println(unsigned int number, int base = 10, int minCharacters = 1)
	{
		println((unsigned long)(number), base, minCharacters);
	}

	void print(short number, int base = 10, int minCharacters = 1)
	{
		print(long(number), base, minCharacters);
	}

	void println(short number, int base = 10, int minCharacters = 1)
	{
		println(long(number), base, minCharacters);
	}

	void print(unsigned short number, int base = 10, int minCharacters = 1)
	{
		print(long(number), base, minCharacters);
	}

	void println(unsigned short number, int base = 10, int minCharacters = 1)
	{
		println(long(number), base, minCharacters);
	}

	void print(unsigned char number, int base = 10, int minCharacters = 1)
	{
		print(long(number), base, minCharacters);
	}

	void println(unsigned char number, int base = 10, int minCharacters = 1)
	{
		println(long(number), base, minCharacters);
	}

	void println()
	{
		print("\n");
	}

	void print(double number, int fractionalDigits = 2, int minCharacters = 1)
	{
		long p = long(pow(10, fractionalDigits));
		long long n = (double(number) * p + 0.5f);
		print(long(n / p), 10, minCharacters - 1 - fractionalDigits);
		if(fractionalDigits)
		{
			print(".");
			for(int i = 0; i < fractionalDigits; i++)
			{
				p /= 10;
				print(long(n / p) % 10);
			}
		}
	}

	void println(double number, int fractionalDigits = 2, int minCharacters = 1)
	{
		print(number, fractionalDigits, minCharacters); 
		print("\n");
	}

	virtual void clear(Color color = 0)
	{
		for (int y = 0; y < yres; y++)
			for (int x = 0; x < xres; x++)
				dotFast(x, y, color);
	}

	virtual void xLine(int x0, int x1, int y, Color color)
	{
		if (y < 0 || y >= yres)
			return;
		if (x0 > x1)
		{
			int xb = x0;
			x0 = x1;
			x1 = xb;
		}
		if (x0 < 0)
			x0 = 0;
		if (x1 > xres)
			x1 = xres;
		for (int x = x0; x < x1; x++)
			dotFast(x, y, color);
	}

	void circle(int x, int y, int r, Color color)
	{
		int oxr = r;
		for(int i = 0; i < r + 1; i++)
		{
			int xr = (int)sqrt(r * r - i * i);
			xLine(x - oxr, x - xr + 1, y + i, color);
			xLine(x + xr, x + oxr + 1, y + i, color);
			if(i) 
			{
				xLine(x - oxr, x - xr + 1, y - i, color);
				xLine(x + xr, x + oxr + 1, y - i, color);
			}
			oxr = xr;
		}
	}

	void fillCircle(int x, int y, int r, Color color)
	{
		for(int i = 0; i < r ; i++)
		{
			int xr = (int)sqrt(r * r - i * i);
			xLine(x - xr, x + xr , y + i, color);
			if(i) 
				xLine(x - xr , x + xr  , y -i, color);
		}
	}

	virtual Color R5G5B4A2ToColor(unsigned short c)
	{
		int r = (((c << 1) & 0x3e) * 255 + 1) / 0x3e;
		int g = (((c >> 4) & 0x3e) * 255 + 1) / 0x3e;
		int b = (((c >> 9) & 0x1e) * 255 + 1) / 0x1e;
		int a = (((c >> 13) & 6) * 255 + 1) / 6;
		return RGBA(r, g, b, a);
	}

};
