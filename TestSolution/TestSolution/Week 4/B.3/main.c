
#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "lcd.h"


#define BIT(x)	(1 << (x))

// wait(): busy waiting for 'ms' millisecond
// Used library: util/delay.h
void wait( int ms )
{
	for (int tms=0; tms<ms; tms++)
	{
		_delay_ms( 1 );			// library function (max 30 ms at 8MHz)
	}
}


// Initialize ADC: 
void adcInit( void )
{
	ADMUX = 0b11100001;			// AREF=2,56 V, result left adjusted, channel1 at pin PF1
	ADCSRA = 0b10000110;		// ADC-enable, no interrupt, no free running, division by 64
}


uint16_t readADC(void)
{
	ADCSRA |= BIT(6);			        // Start ADC
	while (ADCSRA & BIT(6));		    // Wacht tot klaar
	return ADC;                         // Volledige 10-bit waarde
}


// Main program: Counting on T1
int main( void )
{
	DDRF = 0x00;					// set PORTF for input (ADC)
	DDRA = 0xFF;					// set PORTA for output 
	adcInit();						// initialize ADC

	char buffer[16];        // Voor sprintf()
	
	while (1)
	{
		uint16_t adcValue = readADC();
		PORTA = ADCH;					// Show MSB (bit 9:2) of ADC
		
		// Toon op LCD
		lcd_set_line1();
		lcd_write_string("ADC Waarde:     ");

		lcd_set_line2();
		sprintf(buffer, "%4u", adcValue);
		lcd_write_string(buffer);
		
		wait(500);						// every 50 ms (busy waiting)
	}
}