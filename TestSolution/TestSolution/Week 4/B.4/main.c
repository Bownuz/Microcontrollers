#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "lcd.h"
#include <stdio.h>

#define BIT(x)    (1 << (x))

// wait(): busy waiting for 'ms' millisecond
void wait(int ms)
{
	for (int tms = 0; tms < ms; tms++)
	{
		_delay_ms(1);  // library function (max 30 ms at 8MHz)
	}
}

// Initialize ADC:
void adcInit(void)
{
	ADMUX = 0b11100001;  // AREF=2,56 V, result left adjusted, channel 1 (PF1)
	ADCSRA = 0b10000110;  // ADC-enable, no interrupt, no free running, division by 64
}

uint16_t readADCH(void)
{
	ADCSRA |= BIT(6);  // Start ADC
	while (ADCSRA & BIT(6));  // Wait for conversion to finish
	return ADCH;  // Return the lower 8-bit ADC result (the upper 2 bits are dropped)
}

// Main program:
int main(void)
{
	lcd_init_4bits_mode();
	lcd_clear();
	DDRF = 0x00;  // Set PORTF for input (ADC)
	DDRB = 0xFF;  // Set PORTA for output (LEDs)

	adcInit();  // Initialize the ADC

	char buffer[16];  // For sprintf()

	while (1)
	{
		uint16_t adchValue = readADCH();  // Read the ADC value (8-bit)
		
		// Show the MSB (bit 7:0) of the ADC result on PORTA (LEDs)
		PORTB = adchValue;

		// Convert the ADC value to temperature (1 bit = 1°C)
		uint16_t temperature = adchValue;  // Since 1 bit = 1°C

		// Display on LCD
		lcd_set_line1();
		lcd_write_string("Temperatuur:    ");
		
		lcd_set_line2();
		sprintf(buffer, "%4u", temperature);  // Convert temperature to string
		lcd_write_string(buffer);

		wait(500);  // Wait 500 ms (busy waiting)
	}
}
