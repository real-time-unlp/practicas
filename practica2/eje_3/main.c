#include <htc.h>

static volatile unsigned short conv;

void main()
{
	// bit 7 = 1: justificacion a derecha ADRESH:ADRESL = 2:8
	// bits 0,1,2,3 = 0101 AN0 con VDD
	ADCON1 = 0b10000101;
	// bits 7 y 6 = 10: frec OSC/32
	// bits 5,4,3 = 000: canal 0, pin RA0/AN0
	// bit 2 = 0: conversion no esta en progreso (bit GO)
	// bit 0 = 1: A/D activado
	ADCON0 = 0b10000001;
	// Si se configura en ADCON0 con el pin RA0 y RA3 este tiene que ser de entrada
	TRISA = 0b001001;
	// interrupcion fnalizada
	ADIF = 0;

	// Configuracion para los LATCHs
	// RD7 y RD6 en 0 para controlar el pin LE
	TRISD = 0x00;
	// PORTB todos como salida
	TRISB = 0x00;

	// Empizo a realizar las converciones bit GO = 1
	//ADCON0 = ADCON0 | 0x04;
	GO = 1;
	
	while (1)
	{
		while (GO == 0);
		PORTD = 0x11000000;
		PORTB = conv;
		conv = (ADRESH << 8) | ADRESL;
		PORTD = 0x00000000;
	}
}