#define F_CPU 10000000UL  // Definieer de CPU-kloksnelheid van 10MHz
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>  

#define F_CPU 10000000UL
#define BUTTON_UP_PIN   PB1  
#define BUTTON_DOWN_PIN PB2

uint8_t hex_to_7seg[] = {
	0b00111111,  // 0
	0b00000110,  // 1
	0b01011011,  // 2
	0b01001111,  // 3
	0b01100110,  // 4
	0b01101101,  // 5
	0b01111101,  // 6
	0b00000111,  // 7
	0b01111111,  // 8
	0b01101111,  // 9
	0b01110111,  // A
	0b01111100,  // B
	0b00111001,  // C
	0b01011110,  // D
	0b01111001,  // E
	0b01110001,  // F
	0b01111001   // E
};

uint8_t digit = 0; 

void init_button() {
	DDRD = 0xFF; 
	DDRB &= ~(1 << BUTTON_UP_PIN);   
	DDRB &= ~(1 << BUTTON_DOWN_PIN);
	PORTB |= (1 << BUTTON_UP_PIN) | (1 << BUTTON_DOWN_PIN); 
}

void update_display(uint8_t digit) {
	PORTD = hex_to_7seg[digit];  
}

void check_buttons() {
	// Controleer of beide knoppen ingedrukt zijn (reset naar 0)
	if ((PINB & (1 << BUTTON_UP_PIN)) && (PINB & (1 << BUTTON_DOWN_PIN))) {
		digit = 0; // Reset naar 0
		update_display(digit);
		while (!(PINB & (1 << BUTTON_UP_PIN)) && !(PINB & (1 << BUTTON_DOWN_PIN)));  // Wacht tot knoppen losgelaten zijn
	}
	// Controleer of de UP knop ingedrukt is
	else if ((PINB & (1 << BUTTON_UP_PIN))) {
		// Up knop ingedrukt: verhoog digit
		if (digit < 16) {
			digit++;
			update_display(digit);
		}
		while (!(PINB & (1 << BUTTON_UP_PIN)));  // Wacht tot de knop wordt losgelaten
	}
	// Controleer of de DOWN knop ingedrukt is
	else if ((PINB & (1 << BUTTON_DOWN_PIN))) {
		// Down knop ingedrukt: verlaag digit
		if (digit > 0) {
			digit--;
			update_display(digit);
		}
		while (!(PINB & (1 << BUTTON_DOWN_PIN)));  // Wacht tot de knop wordt losgelaten
	}
}

int main(void) {
	init_button();
	update_display(0);
	
	while (1) {
		check_buttons(); 
		_delay_ms(200);
	}
}
