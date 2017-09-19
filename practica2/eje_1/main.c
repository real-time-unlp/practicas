#include <htc.h>

#define _XTAL_FREQ 8000000

void main()
{
	TRISA = 0x03;
	TRISB = 0x00;
	PORTB = 0x30;
	// Pollear pulsadores
	while (0x3 & PORTA == 0x3);

	while (1)
	{
		PORTB = 0x00;
		__delay_ms(250);
		PORTB = 0x30;
	}
}
