//********************************************************************************
//Universidad del Valle de Guatemala
//IE2023: Programacion de Microcontroladores
//Autor: Fernando Gabriel Caballeros Cu
//Proyecto: Laboratorio 05
//Archivo: main.c
//Hardware: ATMega328p
//Created: 15/04/2023
//********************************************************************************

#define F_CPU 16000000
#include <xc.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "PWM0/PMW0.h"
#include "PWM1/PWM1.h"
#include "ADC/ADC.h"
volatile uint8_t valorADC=0;
volatile uint8_t PuertoADC=4;
volatile uint16_t destino=0;

void setup_pins(void){
	DDRC &= ~(1 << DDC4) & ~(1 << DDC5) & ~(1 << DDC6);
	PORTC |= (1 << PORTC4) | (1 << PORTC5)| (1 << PORTC6);

}

uint16_t reescalar(uint8_t valor, uint8_t max_origen, uint16_t max_destino) {
	float valor_normalizado = (float)valor / max_origen;
	uint16_t valor_reescalado = valor_normalizado * max_destino;
	return valor_reescalado;
}

int main() {
	setup_pins();
	init_ADC(0,0,128);
	
	// pr=1024 entonces ICR1 = 312
	
	// pre=256 entonces ICR1 = 1249
	
	destino=312;
	int preescaler=1024;
	init_PWM1A(0,6,preescaler,destino);
	init_PWM1B(0);
	uint8_t valor = 0;
	while (1) {
		if (PuertoADC==4)
		{
			valor = readADC(4);
			uint16_t valor_reescalado = reescalar(valor, 255, destino);
			duty_cycle1A(valor_reescalado);
			PuertoADC++;
		} else if (PuertoADC==5)
		{
			valor =readADC(5);
			uint16_t valor_reescalado = reescalar(valor, 255, destino);
			duty_cycle1B(valor_reescalado);
			PuertoADC=4;
			} else{
			PuertoADC=4;
		}
	}

	return 0;
}
