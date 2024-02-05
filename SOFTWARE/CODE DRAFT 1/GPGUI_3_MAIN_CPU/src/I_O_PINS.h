//VGA PORT PINS
const int hsyncPin = 32;
const int vsyncPin = 33;

const int redPins[] = { 25, 26 };    //R0,R1
const int greenPins[] = { 27, 14 };    //G0,G1
const int bluePins[] = { 12, 13 };    //B0,B1

#define DATA_PIN        34   //keyboard data
#define CLK_PIN         4    //keyboard clock
#define testLed         19
#define SDA_1           21
#define SCL_1           22
#define BUZZER_PIN      23