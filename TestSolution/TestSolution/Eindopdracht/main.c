#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "lcd.h"
#include <stdio.h>
#include "ultrasone.h"
#include "buzzer.h"

#define BIT(x) (1 << (x))

#define TRIG_PIN 1  // Trigger op pin 1
#define ECHO_PIN 0  // Echo op pin 0

// Wachtfunctie

int main(void) {
	lcd_init_4bits_mode();
	lcd_clear();
	buzzerInit(); 
	ultrasoneInit();

	char buffer[16];  // Buffer voor LCD-uitvoer
	
	// Toon op LCD
	lcd_set_line1();
	lcd_write_string("Afstand (cm):   ");

	while (1) {
		sendTrigger();  // Stuur ultrasoon signaal
		uint16_t duration = measurePulse();  // Meet echo-duur
		uint16_t distance = calculateDistance(duration);  // Bereken afstand

		lcd_set_line2();
		sprintf(buffer, "%4u cm", distance);
		lcd_write_string(buffer);
		if (distance <= 50) {
			buzzerBeep(distance); 
		}

		wait(500);
	}
}
