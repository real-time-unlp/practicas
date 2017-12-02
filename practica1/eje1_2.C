#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <conio.h>

#define INTR 0x08
#define CLK 1193182

void interrupt (*oldhandler)(void);
unsigned long long count = 0;
unsigned char print = 0;

void interrupt handler(void)
{
	++count;
	print = 0;
	oldhandler();
}


int main(int argc, char **argv)
{
	unsigned long freq;
	unsigned int n;
	unsigned long long top;

	oldhandler = getvect(INTR);
	setvect(INTR, handler);

	if (argc != 2)
		return -1;

	freq = atol(argv[1]);

	printf("Frecuencia dada: %lu Hz\n", freq);
	if (freq < 19 || freq > CLK) {
		printf("Frecuencia fuera de rango.\n");
		return -1;
	}

	n = CLK/freq;

	printf("n: %u\n", n);
    // El valor 34 define:
    //      No BCD,
    //      Modo 2: X10 (generador de impulsos)
    //      Priero de lee (o escribe) el byte menos significativo del contador
    //      Se selecciona el contador 0
	outportb(0x43, 0x34);
	outportb(0x40, 0xFF & n);
	outportb(0x40, (0xFF00 & n) >> 8);

	top = freq * 20;
	while (count < top)
	{
		if (!print && count % freq == 0)
		{
			print = 1;
			printf("count is %llu\n", count);
		}
	}

	outportb(0x43, 0x34);
	outportb(0x40, 0xFF);
	outportb(0x40, 0xFF);
	setvect(INTR, oldhandler);
	return 0;
}
