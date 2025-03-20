

#define F_CPU 8e6

#include <avr/io.h>
#include <util/delay.h>

typedef struct {
	unsigned char data;
	unsigned int delay ;
} PATTERN_STRUCT;

PATTERN_STRUCT pattern[] = {

	
	{0b00000001, 200}, {0b00000010, 200}, {0b00000100, 200}, {0b00001000, 200}, {0b00010000, 200}, {0b00100000, 200},
	{0b00000001, 50}, {0b00000010, 50}, {0b00000100, 50}, {0b00001000, 50}, {0b00010000, 50}, {0b00100000, 50},
	
	{0b00000001, 200}, {0b00000010, 200}, {0b01000000, 200}, {0b00010000, 200}, {0b00001000, 200}, {0b00000100, 200}, {0b01000000, 200}, {0b00100000, 200},
	{0b00000001, 50}, {0b00000010, 50}, {0b01000000, 50}, {0b00010000, 50}, {0b00001000, 50}, {0b00000100, 50}, {0b01000000, 50}, {0b00100000, 50},
	{0x00, 0x00}
};

/*Busy wait number of millisecs*/
void wait( int ms ) {
	for (int i=0; i<ms; i++) {
		_delay_ms( 1 );		// library function (max 30 ms at 8MHz)
	}
}


/*
main() loop, entry point of executable
*/
int main( void )
{
	DDRD = 0b11111111;					// PORTD all output
	
	while (1==1) {
		// Set index to begin of pattern array
		int index = 0;
		// as long as delay has meaningful content
		while( pattern[index].delay != 0 ) {
			// Write data to PORTD
			PORTD = pattern[index].data;
			// wait
			wait(pattern[index].delay);
			// increment for next round
			index++;
		}
	}

	return 1;
}