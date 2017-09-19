#include <htc.h>

static volatile unsigned char pressed = 0;

void main()
{
    TRISA = 0x03;
	TRISB = 0x00;
    PORTB = 0x30;

    OPTION_REG = 0b00000111;
    INTCON = 0b10100000;

    while (0x3 & PORTA == 0x3);
    pressed = 1;
    while(1);
}

void interrupt timerHandler()
{
    if (pressed) {
        PORTB = PORTB ^ 0x30;
    }
    INTCON &= ~(1 << 2);
}
