#include "buzzer.h"
#include <avr/io.h>
#include <util/delay.h>

// Initialiseer de buzzer (pin als output)
void buzzerInit() {
	DDRC = 0xff;  // Zet BUZZER_PIN als output
}

void wait(int ms) {
	for (int tms = 0; tms < ms; tms++) {
		_delay_ms(1);
	}
}

// Laat de buzzer piepen voor een bepaalde tijd
void buzzerBeep(int distance) {
	// Hoe kleiner de afstand, hoe sneller de piepjes
	uint16_t delayTime = distance;  // Bij kortere afstand is de vertraging kleiner
	
	// Hoe kleiner de afstand, hoe meer piepjes (harder)
	uint16_t repeatCount = 500 - (distance / 2);  

	// Herhaal het piepen meerdere keren afhankelijk van de afstand
	for (uint16_t i = 0; i < repeatCount; i++) {
		PORTC = 0xff;  // Zet buzzer aan
		wait(delayTime);  // Dynamische vertraging gebaseerd op de afstand
		PORTC = 0x00;  // Zet buzzer uit
		wait(delayTime);  // Wacht voor de volgende piep
	}
}

