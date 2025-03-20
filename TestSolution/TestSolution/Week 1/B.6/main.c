#define F_CPU 10000000UL  // 10MHz CPU-klok

#include <avr/io.h>
#include <util/delay.h>

#define LED_PIN PD7      // LED op PORTD.7
#define BUTTON_PIN PC0   // Knop op PORTC.0

void init_pins() {
	DDRC = 0x00; //zet de pins op input
	DDRD = 0xFF; //zet de pins op output
}

void wait(int ms) {
	for (int i = 0; i < ms; i++) {
		_delay_ms(1);
	}
}

int main(void) {
	init_pins(); // Initialiseer de pinnen

	int blink_speed = 500;

	while (1) {
		if ((PINC & (1 << BUTTON_PIN))) {
			_delay_ms(50); // debounce
			if (!(PINC & (1 << BUTTON_PIN))) {
				// Toggle de snelheid
				if (blink_speed == 500) {
					blink_speed = 125;
				}
				else {
					blink_speed = 500;
				}

				// Wacht tot knop wordt losgelaten
				while (!(PINC & (1 << BUTTON_PIN)));
			}
		}

		// LED toggelen
		PORTD = 0b10000000;
		wait(blink_speed);
		PORTD = 0b00000000;
		wait(blink_speed);
	}
}
