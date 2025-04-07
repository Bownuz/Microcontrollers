#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "lcd.h"
#include <stdio.h>
#include "ultrasone.h"

#define BIT(x) (1 << (x))

#define TRIG_PIN 1  // Trigger op pin 1
#define ECHO_PIN 0  // Echo op pin 0

volatile uint16_t pulseDuration = 0;  // De duur van de echo-puls (timer waarde)

// Initialiseer de pinnen en timer
void ultrasoneInit() {
	DDRB |= BIT(TRIG_PIN);   // TRIG als output
	DDRB &= ~BIT(ECHO_PIN);  // ECHO als input

	// Configureer Timer1 (16-bit timer) voor het meten van de tijd
	TCCR1B |= (1 << CS11);  // Timer1 met prescaler 8 (CS11 = 1, CS10 = 0)
	TCNT1 = 0;               // Zet de timer op 0

	// Zet globale interrupt enable
	sei();
}

// Stuur een triggerpuls
void sendTrigger() {
	PORTB |= BIT(TRIG_PIN);   // Zet TRIG hoog
	_delay_us(10);            // Wacht 10 µs
	PORTB &= ~BIT(TRIG_PIN);  // Zet TRIG laag
}

// Meet de echo-pulsduur
uint16_t measurePulse() {
	uint16_t count = 0;

	// Wacht tot de echo-puls start (rising edge)
	while (!(PINB & BIT(ECHO_PIN)));

	// Start de timer bij ontvangst van de rising edge
	TCNT1 = 0;  // Reset Timer1

	// Meet de duur van de echo-puls (wacht tot falling edge)
	while (PINB & BIT(ECHO_PIN)) {
		// Timer telt automatisch door als de echo actief is
	}

	// Wanneer de echo stopt, lees de waarde van de timer
	pulseDuration = TCNT1;

	return pulseDuration;
}

// Bereken de afstand in cm
float calculateDistance(float pulseDuration) {
	return (pulseDuration * 0.0343) / 2.0;  // 343 m/s -> 0.0343 cm/µs en gedeeld door 2 (heen & terug)
}
