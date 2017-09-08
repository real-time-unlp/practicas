#include <dos.h>
#include <conio.h>
#include <stdio.h>

int main()
{
	unsigned short temp;
	unsigned char low, high;

	while(!kbhit())
	{
		outportb(0x300, 0x0);
		inportb(0x308);
		while (inportb(0x300) & 0x20 == 0x0);
		high = inportb(0x309);
		low  = inportb(0x308);
		temp = low + high * 256;
		printf("%hu\n", temp);
		if (temp >  2218)
		{
			printf("Se alcanzo los 65\n");
			if (temp > 2901)
			{
				printf("Se alcanzo los 85\n");
			}
		}
		sleep(1);
	}
	return 0;
}
