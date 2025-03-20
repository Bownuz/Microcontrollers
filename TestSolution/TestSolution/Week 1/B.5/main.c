#define F_CPU 10000000UL  // 10MHz CPU-klok

#include <avr/io.h>
#include <util/delay.h>

int main(void) {
	DDRD = 0xFF; // Zet alle pins van PORTD als output

	uint8_t array[8] = { // Correct datatype gebruiken (uint8_t i.p.v. int)
		0b10000000,
		0b01000000,
		0b00100000,
		0b00010000,
		0b00001000,
		0b00000100,
		0b00000010,
		0b00000001
	};

	while (1) {
		for (uint8_t j = 0; j < 8; j++) {
			PORTD = array[j]; // Zet de juiste LED aan
			_delay_ms(100);   // Wacht 100ms
		}
	}
	return 0;
}
