#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Houdt bij welke LED aan staat
int static currentBit = 1;

// Eenvoudige wachttijd in milliseconden
void wait(int ms) {
	for (int i = 0; i < ms; i++) {
		_delay_ms(1); // Let op: werkt alleen tot 30ms per keer bij 8MHz
	}
}

// Functie om de LED te veranderen
void changeLampState() {
	PORTC = currentBit;       // Zet de huidige LED aan
	currentBit <<= 1;         // Verschuif naar links (volgende LED)

	if (currentBit > 128) {   // Na de laatste LED (0b10000000)
		currentBit = 1;       // Begin opnieuw bij eerste LED
	}
}

// Interrupt Service Routine voor INT1
ISR(INT1_vect) {
	//PORTD |= (1 << 5); // Debug led aanzetten eventueel
	changeLampState();
}

// Interrupt Service Routine voor INT2
ISR(INT2_vect) {
	//PORTD &= ~(1 << 5); // Debug led uitzetten eventueel
	changeLampState();
}

int main(void) {
	// Init I/O
	DDRD = 0xF0;  // PORTD7:4 als output, PORTD3:0 als input
	DDRC = 0xFF;  // PORTC als output

	PORTC = currentBit; // Zet eerste LED aan

	// Init interrupt hardware
	EICRA = 0b00111000; // INT1 op vallende flank, INT2 op stijgende flank
	EIMSK = 0b00000110; // INT1 en INT2 inschakelen

	// Globale interrupt inschakelen
	sei();

	// Oneindige loop
	while (1) {
		// Geen actie in main loop, alles gebeurt via interrupts
	}

	return 0;
}
