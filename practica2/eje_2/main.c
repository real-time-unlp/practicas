#include <htc.h>

static volatile unsigned char pressed = 0;

void main()
{
    TRISA = 0x03;
	TRISB = 0x00;
    PORTB = 0x30;

    // bit 5 en cero para timer incremental
    // bit 0,1,2 en 111 = preescaler 1:256
    OPTION_REG = 0b00000111;
    // bit 7 en uno para habilitar las interrupciones
    // bit 5 en uno habilita interrupciones del timer0
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
    //Es lo mismo que hacer:
    //T0IF = 0;
}
