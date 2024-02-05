#include "VGA.h"

//hfront hsync hback pixels vfront vsync vback lines divy pixelclock hpolaritynegative vpolaritynegative



const Mode VGA::MODE320x175(8, 48, 24, 320, 37, 2, 60, 350, 2, 12587500, 0, 1);
const Mode VGA::MODE320x480(8, 48, 24, 320, 11, 2, 31, 480, 1, 12587500, 1, 1);
const Mode VGA::MODE360x200(8, 54, 28, 360, 11, 2, 32, 400, 2, 14161000, 1, 0);//nw
const Mode VGA::MODE320x200(8, 48, 24, 320, 12, 2, 35, 400, 2, 12587500, 1, 0);//not working 
const Mode VGA::MODE400x300(12, 36, 64, 400, 1, 2, 22, 600, 2, 18000000, 0, 0);//works with 1 buffer
const Mode VGA::MODE320x240(8, 48, 24, 320, 11, 2, 31, 480, 2, 12587500, 1, 1);//not working
const Mode VGA::MODE400x150(12, 36, 64, 400, 1, 2, 22, 600, 4, 18000000, 0, 0);
const Mode VGA::MODE360x175(8, 54, 28, 360, 11, 2, 32, 350, 2, 14161000, 1, 1);
const Mode VGA::MODE640x480(16, 96, 48, 640, 11, 2, 31, 480, 1, 25175000, 1, 1);
const Mode VGA::MODE200x150(6, 18, 32, 200, 1, 2, 22, 600, 4, 9000000, 0, 0);//nw

VGA::VGA(const int i2sIndex)
	: I2S(i2sIndex)
{
	lineBufferCount = 8;
	dmaBufferDescriptors = 0;
}








bool VGA::init(const Mode &mode, const int *pinMap, const int bitCount, const int clockPin)
{
	this->mode = mode;
	int xres = mode.hRes;
	int yres = mode.vRes / mode.vDiv;
	initSyncBits();
	propagateResolution(xres, yres);
	this->vsyncPin = vsyncPin;
	this->hsyncPin = hsyncPin;
	totalLines = mode.linesPerField();
	allocateLineBuffers();
	currentLine = 0;
	vSyncPassed = false;
	initParallelOutputMode(pinMap, mode.pixelClock, bitCount, clockPin);
	startTX();
	return true;
}












void VGA::setLineBufferCount(int lineBufferCount)
{
	this->lineBufferCount = lineBufferCount;
}










void VGA::allocateLineBuffers()
{
	allocateLineBuffers(lineBufferCount);
}








/// simple ringbuffer of blocks of size bytes each
void VGA::allocateLineBuffers(const int lines)
{
	dmaBufferDescriptorCount = lines;
	dmaBufferDescriptors = DMABufferDescriptor::allocateDescriptors(dmaBufferDescriptorCount);
	int bytes = (mode.hFront + mode.hSync + mode.hBack + mode.hRes) * bytesPerSample();
	for (int i = 0; i < dmaBufferDescriptorCount; i++)
	{
		dmaBufferDescriptors[i].setBuffer(DMABufferDescriptor::allocateBuffer(bytes, true), bytes); //front porch + hsync + back porch + pixels
		if (i)
			dmaBufferDescriptors[i - 1].next(dmaBufferDescriptors[i]);
	}
	dmaBufferDescriptors[dmaBufferDescriptorCount - 1].next(dmaBufferDescriptors[0]);
}















///complete ringbuffer from frame
void VGA::allocateLineBuffers(void **frameBuffer)
{
	dmaBufferDescriptorCount = totalLines * 2;
	int inactiveSamples = (mode.hFront + mode.hSync + mode.hBack + 3) & 0xfffffffc;
	vSyncInactiveBuffer = DMABufferDescriptor::allocateBuffer(inactiveSamples * bytesPerSample(), true);
	vSyncActiveBuffer = DMABufferDescriptor::allocateBuffer(mode.hRes * bytesPerSample(), true);
	inactiveBuffer = DMABufferDescriptor::allocateBuffer(inactiveSamples * bytesPerSample(), true);
	blankActiveBuffer = DMABufferDescriptor::allocateBuffer(mode.hRes * bytesPerSample(), true);
	if(bytesPerSample() == 1)
	{
		for (int i = 0; i < inactiveSamples; i++)
		{
			if (i >= mode.hFront && i < mode.hFront + mode.hSync)
			{
				((unsigned char *)vSyncInactiveBuffer)[i ^ 2] = hsyncBit | vsyncBit;
				((unsigned char *)inactiveBuffer)[i ^ 2] = hsyncBit | vsyncBitI;
			}
			else
			{
				((unsigned char *)vSyncInactiveBuffer)[i ^ 2] = hsyncBitI | vsyncBit;
				((unsigned char *)inactiveBuffer)[i ^ 2] = hsyncBitI | vsyncBitI;
			}
		}
		for (int i = 0; i < mode.hRes; i++)
		{
			((unsigned char *)vSyncActiveBuffer)[i ^ 2] = hsyncBitI | vsyncBit;
			((unsigned char *)blankActiveBuffer)[i ^ 2] = hsyncBitI | vsyncBitI;
		}
	}
	else if(bytesPerSample() == 2)
	{
		for (int i = 0; i < inactiveSamples; i++)
		{
			if (i >= mode.hFront && i < mode.hFront + mode.hSync)
			{
				((unsigned short *)vSyncInactiveBuffer)[i ^ 1] = hsyncBit | vsyncBit;
				((unsigned short *)inactiveBuffer)[i ^ 1] = hsyncBit | vsyncBitI;
			}
			else
			{
				((unsigned short *)vSyncInactiveBuffer)[i ^ 1] = hsyncBitI | vsyncBit;
				((unsigned short *)inactiveBuffer)[i ^ 1] = hsyncBitI | vsyncBitI;
			}
		}
		for (int i = 0; i < mode.hRes; i++)
		{
			((unsigned short *)vSyncActiveBuffer)[i ^ 1] = hsyncBitI | vsyncBit;
			((unsigned short *)blankActiveBuffer)[i ^ 1] = hsyncBitI | vsyncBitI;
		}
	}

	dmaBufferDescriptors = DMABufferDescriptor::allocateDescriptors(dmaBufferDescriptorCount);
	for (int i = 0; i < dmaBufferDescriptorCount; i++)
		dmaBufferDescriptors[i].next(dmaBufferDescriptors[(i + 1) % dmaBufferDescriptorCount]);
	int d = 0;
	for (int i = 0; i < mode.vFront; i++)
	{
		dmaBufferDescriptors[d++].setBuffer(inactiveBuffer, inactiveSamples * bytesPerSample());
		dmaBufferDescriptors[d++].setBuffer(blankActiveBuffer, mode.hRes * bytesPerSample());
	}
	for (int i = 0; i < mode.vSync; i++)
	{
		dmaBufferDescriptors[d++].setBuffer(vSyncInactiveBuffer, inactiveSamples * bytesPerSample());
		dmaBufferDescriptors[d++].setBuffer(vSyncActiveBuffer, mode.hRes * bytesPerSample());
	}
	for (int i = 0; i < mode.vBack; i++)
	{
		dmaBufferDescriptors[d++].setBuffer(inactiveBuffer, inactiveSamples * bytesPerSample());
		dmaBufferDescriptors[d++].setBuffer(blankActiveBuffer, mode.hRes * bytesPerSample());
	}
	for (int i = 0; i < mode.vRes; i++)
	{
		dmaBufferDescriptors[d++].setBuffer(inactiveBuffer, inactiveSamples * bytesPerSample());
		dmaBufferDescriptors[d++].setBuffer(frameBuffer[i / mode.vDiv], mode.hRes * bytesPerSample());
	}
}












void VGA::vSync()
{
	vSyncPassed = true;
}











void VGA::interrupt()
{
	unsigned long *signal = (unsigned long *)dmaBufferDescriptors[dmaBufferDescriptorActive].buffer();
	unsigned long *pixels = &((unsigned long *)dmaBufferDescriptors[dmaBufferDescriptorActive].buffer())[(mode.hSync + mode.hBack) / 2];
	unsigned long base, baseh;
	if (currentLine >= mode.vFront && currentLine < mode.vFront + mode.vSync)
	{
		baseh = (vsyncBit | hsyncBit) | ((vsyncBit | hsyncBit) << 16);
		base = (vsyncBit | hsyncBitI) | ((vsyncBit | hsyncBitI) << 16);
	}
	else
	{
		baseh = (vsyncBitI | hsyncBit) | ((vsyncBitI | hsyncBit) << 16);
		base = (vsyncBitI | hsyncBitI) | ((vsyncBitI | hsyncBitI) << 16);
	}
	for (int i = 0; i < mode.hSync / 2; i++)
		signal[i] = baseh;
	for (int i = mode.hSync / 2; i < (mode.hSync + mode.hBack) / 2; i++)
		signal[i] = base;

	int y = (currentLine - mode.vFront - mode.vSync - mode.vBack) / mode.vDiv;
	if (y >= 0 && y < mode.vRes)
		interruptPixelLine(y, pixels, base);
	else
		for (int i = 0; i < mode.hRes / 2; i++)
		{
			pixels[i] = base | (base << 16);
		}
	for (int i = 0; i < mode.hFront / 2; i++)
		signal[i + (mode.hSync + mode.hBack + mode.hRes) / 2] = base;
	currentLine = (currentLine + 1) % totalLines;
	dmaBufferDescriptorActive = (dmaBufferDescriptorActive + 1) % dmaBufferDescriptorCount;
	if (currentLine == 0)
		vSync();
}

void VGA::interruptPixelLine(int y, unsigned long *pixels, unsigned long syncBits)
{
}
