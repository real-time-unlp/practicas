#include <dos.h>
#include <graphics.h>
#include <conio.h>
#include <math.h>

#define HEIGHT 500
#define WIDTH  650
#define VCENTER HEIGHT/2

#define FAN_THOLD 554
#define SHUTOFF_THOLD 725

#define CTREG 0x300
#define STINR CTREG
#define OUTBR 0x304
#define ADLOW 0x308
#define ADHIGH 0x309
#define EOC_MASK 0x20


void interrupt (*old_handler)(void);
volatile short a;
volatile char new_values = 0;
char motor_on;
char fan_on;

unsigned short adc_read(void)
{
	/* Lectura "esporadica" */
	unsigned short res;
	inportb(ADLOW);
	while (inportb(STINR) & EOC_MASK == 0);
	res = inportb(ADHIGH) << 8;
	res |= inportb(ADLOW);
	return res;
}

void interrupt handler(void)
{
	if (!new_values) {
		a = adc_read()/17;
		if (a > FAN_THOLD) {
			if (!fan_on) {
				fan_on = 1;
				outportb(OUTBR, 0x9);
			}
		} else {
			if (fan_on) {
				fan_on = 0;
				outportb(OUTBR, 0x1);
			}

		}
	       //	outportb(OUTBR, a > SHUTOFF_THOLD ? 0x2 : 0xA);
	}
	new_values = 1;
	old_handler();
}

void plot_init(void)
{
	cleardevice();
	setcolor(WHITE);
	line(0, VCENTER, WIDTH, VCENTER);
}

void plot_draw(short a)
{
	static short cursor, oa;
	if (!cursor) {
		plot_init();
		oa = a;
	}
	setcolor(MAGENTA);
	line(cursor - 1, VCENTER - oa, cursor, VCENTER - a);
	oa = a;
	cursor = (cursor + 1) % WIDTH;
}

int main()
{
	int gd = DETECT, gm;

	outportb(0x43, 0x34);
	outportb(0x40, 0xA4);
	outportb(0x40, 0x4D);

	old_handler = getvect(0x8);
	setvect(0x8, handler);

	initgraph(&gd, &gm, "C:\\BORLANDC\\BGI");

	setbkcolor(BLACK);

	while (!kbhit()) {
		while(!new_values);
		plot_draw(a);
		new_values = 0;
	}

	closegraph();

	outportb(0x43, 0x34);
	outportb(0x40, 0xFF);
	outportb(0x40, 0xFF);
	setvect(0x8, old_handler);

	return 0;
}