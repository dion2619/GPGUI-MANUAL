#include "Tone32.h"

void tone(uint8_t pin, unsigned int frequency)
{

    ledcAttachPin(pin, 0);
    ledcWriteTone(0, frequency);
}



void noTone(uint8_t pin)
{
    ledcDetachPin(pin);
    ledcWrite(0, 0);
}
