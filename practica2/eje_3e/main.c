#include <htc.h>

#define _XTAL_FREQ 1000000
#define LE_LO 6
#define LE_HI 7

static volatile unsigned short value;

static unsigned short convert(unsigned short n)
{
	unsigned long tmp = (unsigned long) n * 123;
	unsigned short res = (tmp) / 25;
	return (res > 5000) ? 5000 : res;
}

static void show_number(unsigned short n)
{
	unsigned char l, h;
	l = n % 10;
	n /= 10;	
	h = n % 10;
	PORTD = (1 << LE_LO);
	PORTB = h << 4 | l;
	__delay_us(100);

	n /= 10;	
	l = n % 10;
	n /= 10;	
	h = n % 10;
	PORTD = (1 << LE_HI);
	PORTB = h << 4 | l;
	__delay_us(100);
}

void main()
{
	/*
	 * bit 7 = 1: justificacion a derecha ADRESH:ADRESL = 2:8
	 * bits 0,1,2,3 = 0101 AN0 con VDD
	 */
	ADCON1 = 0b10000101;
	/*
	 * bits 7 y 6 = 10: frec OSC/32
	 * bits 5,4,3 = 000: canal 0, pin RA0/AN0
	 * bit 2 = 0: conversion no esta en progreso (bit GO)
	 * bit 0 = 1: A/D activado
	 */
	ADCON0 = 0b10000001;
	/* Si se configura en ADCON0 con el pin RA0 y RA3 este tiene que ser de entrada */
	TRISA = 0b001001;
	TRISD = 0x00;
	TRISB = 0x00;
	
	ADIF = 0;
	ADIE = 1;
	PEIE = GIE = 1;

	GO = 1;
	while (1);
}

void interrupt timerHandler()
{
	value = (ADRESH << 8) | ADRESL;
	show_number(convert(value));
	GO = 1;
}
