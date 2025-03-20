#define F_CPU 1000000UL // Pas aan naar je kloksnelheid
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t state = 0; // Om te schakelen tussen hoog en laag

void timer2_init() {
	TCCR2 = (1 << WGM12) | (1 << CS22) | (1 << CS20); // CTC mode, prescaler 128
	OCR2 = 117; // 15ms bij 1MHz klok met prescaler 128
	ETIMSK |= (1 << OCIE2); // Interrupt aanzetten voor Timer 2
}

ISR(TIMER2_COMP_vect) {
	if (state == 0) {
		PORTD |= (1 << PD7);  // Zet PD7 hoog
		OCR2 = 195;           // 25ms wachten
		state = 1;
		} else {
		PORTD &= ~(1 << PD7); // Zet PD7 laag
		OCR2 = 117;           // 15ms wachten
		state = 0;
	}
}

int main(void) {
	DDRD |= (1 << PD7); // PD7 als output
	timer2_init();      // Timer instellen
	sei();              // Globale interrupts inschakelen

	while (1) {
		// Hoofdlus, hier gebeurt niks, alles wordt gedaan in de interrupt
	}
}
