#include <htc.h>

#define _XTAL_FREQ 1000000
#define LE_LO 6
#define LE_HI 7

static volatile unsigned short value;

static void show_number(unsigned short n)
{
	PORTD = (1 << LE_LO);
	PORTB = 0xFF & n;
	__delay_us(100);
	PORTD = (1 << LE_HI);
	PORTB = n >> 8;
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

	
	while (1)
	{
                /* Inicio de conversión GO = 1 */
                GO = 1;
		while (GO);
		value = (ADRESH << 8) | ADRESL;
		show_number(value);
	}
}
