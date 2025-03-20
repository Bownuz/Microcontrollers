#include <avr/io.h>
#include <util/delay.h>

int main() {
	DDRC = 0x00; //zet de pins op input
	DDRD = 0xFF; //zet de pins op output
	
	while (1) {
		if (PINC & 0b00000001) { //controlleert of de knop wordt ingedrukt
			PORTD = 0b10000000;
			_delay_ms(5000);
			PORTD = 0b00000000;
			_delay_ms(5000);
		}
		PORTD = 0b00000000;
	}
}