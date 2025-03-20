#include <avr/io.h>
#include <util/delay.h>

int main() {
	DDRD = 0xFF;
	
	while (1) {
		PORTD = 0b01000000;
		//geeft een delay (op een of andere manier moet je de delay op 5000 zetten, ipv 500. Anders gaat die te snel)
		_delay_ms(5000);
		PORTD = 0b10000000;
		_delay_ms(5000);
	}
}