#include "teller.h"
#include <avr/interrupt.h>

volatile int count = 0;

void teller_init() {
	DDRD &= ~(1 << PD2);
	EICRA |= (1 << ISC01);
	EIMSK |= (1 << INT0);
	sei();
}

ISR(INT0_vect) {
	count++;
}

void teller_increment() {
	count++;
}

int teller_get() {
	return count;
}
