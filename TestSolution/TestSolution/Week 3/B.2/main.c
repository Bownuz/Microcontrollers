#define F_CPU 10000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t state = 0;      // 0 = laag, 1 = hoog
volatile uint16_t counter = 0;   // teller voor duur

void timer2_init() {
	TCCR2 = (1 << WGM21) | (1 << CS22) | (1 << CS20); // CTC mode, prescaler 1024
	OCR2 = 155; // 1ms interrupt bij 10MHz en prescaler 1024
	TIMSK |= (1 << OCIE2); // Interrupt aanzetten
}

ISR(TIMER2_COMP_vect) {
	counter++;
	if (state == 0 && counter >= 25) {      // laag -> hoog na 25ms
		PORTD |= (1 << PD7);
		state = 1;
		counter = 0;
	} else if (state == 1 && counter >= 15) { // hoog -> laag na 15ms
		PORTD &= ~(1 << PD7);
		state = 0;
		counter = 0;
	}
}

int main(void) {
	DDRD |= (1 << PD7); // PORTD.7 als output
	timer2_init();      // Timer initialiseren
	sei();              // Globale interrupts aanzetten

	while (1) {
		// niets nodig hier
	}
}
