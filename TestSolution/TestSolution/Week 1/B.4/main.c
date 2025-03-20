#define F_CPU 10000000UL  // 10MHz CPU-klok

#include <avr/io.h>
#include <util/delay.h>

int main(void) {
	DDRD = 0xFF; // Alle pins van PORTD als output instellen
	
	while (1) {
		uint8_t currentBit = 1; // Begin met eerste LED

		for (uint8_t currentLamp = 0; currentLamp < 8; currentLamp++) {
			PORTD = currentBit;  // Zet de juiste LED aan
			currentBit <<= 1;    // Schuif naar de volgende LED
			_delay_ms(200);         // Wacht 200ms
		}
	}
	return 0;
}
