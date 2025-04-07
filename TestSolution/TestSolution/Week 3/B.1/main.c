#define F_CPU 10000000UL  // Definieer de CPU-kloksnelheid van 10MHz
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>  // Voor itoa()
#include "lcd.h"

#define BUTTON_PIN PB0 // Gebruik PB0 als knopinput (voor externe puls)

void lcd_write_number(uint8_t number) {
	char buffer[6];
	itoa(number, buffer, 10); // Converteer naar string
	lcd_clear();
	lcd_write_string("Teller: ");
	lcd_write_string(buffer);
}

void timer2_counter_init() {
	// Timer2 in counter mode: telt bij elke falling edge op T2 (pin PD6)
	TCCR2 = (1 << CS22) | (1 << CS21) | (1 << CS20);
	TCNT2 = 0; // Beginwaarde
}

int main(void) {
	lcd_init_4bits_mode(); // LCD initialiseren
	lcd_clear();
	lcd_write_number(0);
	timer2_counter_init(); // Start Timer2 in counter mode

	// PB0 als input voor knop
	DDRB &= ~(1 << BUTTON_PIN);  // PB0 als input
	PORTB |= (1 << BUTTON_PIN);  // Pull-up inschakelen

	// Zet PD6 als input (T2 pin), dit is de externe ingang voor counter mode
	DDRD &= ~(1 << PD6);
	PORTD |= (1 << PD6);  // Optioneel: pull-up aan op PD6

	uint8_t previous_value = TCNT2; // Houd de vorige waarde van de teller bij
	uint8_t counter_updated = 0; // Flag om bij te houden of de teller al is verhoogd

	while (1) {
		// Wacht tot de knop is ingedrukt
		while (PINB & (1 << BUTTON_PIN)); // Wacht totdat de knop is ingedrukt (low)

		// Verhoog de teller alleen als de knop wordt ingedrukt en losgelaten
		if (!counter_updated) {
			TCNT2++; // Verhoog de timer waarde (of gebruik een andere tellermechanisme)
			counter_updated = 1; // Zorg ervoor dat de teller niet opnieuw wordt verhoogd totdat de knop losgelaten is
		}

		// Wacht tot de knop weer is losgelaten
		while (!(PINB & (1 << BUTTON_PIN))); // Wacht totdat de knop weer is losgelaten (high)

		// Reset de flag nadat de knop is losgelaten
		counter_updated = 0;

		// Laat altijd de huidige tellerwaarde op het LCD zien
		lcd_write_number(TCNT2);
		_delay_ms(200);  // Refresh vertraging
	}
}
