#define F_CPU 10000000UL  // Definieer de CPU-kloksnelheid van 10MHz
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>  // Bevat de declaratie van itoa()
#include "lcd.h"

#define F_CPU 10000000UL
#define BUTTON_PIN PB0 // Gebruik PB0 als knop

volatile uint16_t teller = 0;

// Converteert integer naar string en toont op LCD
void lcd_write_number(uint16_t number) {
	char buffer[6];  // Buffer voor maximaal 5 cijfers + null-terminator
	itoa(number, buffer, 10); // Zet integer om naar string
	lcd_clear();
	lcd_write_string("Teller: ");
	lcd_write_string(buffer);
}

void init_button() {
	DDRB &= ~(1 << BUTTON_PIN); // Zet PB0 als input
	PORTB |= (1 << BUTTON_PIN); // Interne pull-up weerstand aan
}

int main(void) {
	lcd_init_4bits_mode(); // LCD initialiseren
	lcd_clear();
	init_button(); // Knop initialiseren
	lcd_write_number(0);
	
	while (1) {
		// Knop wordt ingedrukt (laag actief)
		if (!(PINB & (1 << BUTTON_PIN))) {
			_delay_ms(50); // Debounce vertraging
			if (!(PINB & (1 << BUTTON_PIN))) { // Dubbele check
				teller++; // Verhoog teller
				lcd_write_number(teller);
				while (!(PINB & (1 << BUTTON_PIN))); // Wacht tot knop wordt losgelaten
			}
		}
	}
}
