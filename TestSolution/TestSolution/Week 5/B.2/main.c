#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>

#define BIT(x)		( 1<<x )
#define DDR_SPI		DDRB					// spi Data direction register
#define PORT_SPI	PORTB					// spi Output register
#define SPI_SCK		1						// PB1: spi Pin System Clock
#define SPI_MOSI	2						// PB2: spi Pin MOSI
#define SPI_MISO	3						// PB3: spi Pin MISO
#define SPI_SS		0						// PB0: spi Pin Slave Select

// wait(): busy waiting for 'ms' millisecond
// used library: util/delay.h
void wait(int ms)
{
	for (int i=0; i<ms; i++)
		{
			_delay_ms(1);
		}
}

void spi_masterInit(void)
{
	DDR_SPI = 0xff;							// All pins output: MOSI, SCK, SS, SS_display as output
	DDR_SPI &= ~BIT(SPI_MISO);				// 	except: MISO input
	PORT_SPI |= BIT(SPI_SS);				// SS_ADC == 1: deselect slave
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1);	// or: SPCR = 0b11010010;
											// Enable spi, MasterMode, Clock rate fck/64, bitrate=125kHz
											// Mode = 0: CPOL=0, CPPH=0;
}


// Write a byte from master to slave
void spi_write( unsigned char data )				
{
	SPDR = data;							// Load byte to Data register --> starts transmission
	while( !(SPSR & BIT(SPIF)) ); 			// Wait for transmission complete 
}

// Write a byte from master to slave and read a byte from slave
// nice to have; not used here
char spi_writeRead( unsigned char data )
{
	SPDR = data;							// Load byte to Data register --> starts transmission
	while( !(SPSR & BIT(SPIF)) ); 			// Wait for transmission complete 
	data = SPDR;							// New received data (eventually, MISO) in SPDR
	return data;							// Return received byte
}

// Select device on pinnumer PORTB
void spi_slaveSelect(unsigned char chipNumber)
{
	PORTB &= ~BIT(chipNumber);
}

// Deselect device on pinnumer PORTB
void spi_slaveDeSelect(unsigned char chipNumber)
{

	PORTB |= BIT(chipNumber);
}


// Initialize the driver chip (type MAX 7219)
void displayDriverInit() 
{
	spi_slaveSelect(0);				// Select display chip (MAX7219)
  	spi_write(0x09);      			// Register 09: Decode Mode
  	spi_write(0xFF);				// 	-> 1's = BCD mode for all digits
  	spi_slaveDeSelect(0);			// Deselect display chip

  	spi_slaveSelect(0);				// Select dispaly chip
  	spi_write(0x0A);      			// Register 0A: Intensity
  	spi_write(0x0f);    			//  -> Level 4 (in range [1..F])
  	spi_slaveDeSelect(0);			// Deselect display chip

  	spi_slaveSelect(0);				// Select display chip
  	spi_write(0x0B);  				// Register 0B: Scan-limit
  	spi_write(0x03);   				// 	-> 1 = Display digits 0..3
  	spi_slaveDeSelect(0);			// Deselect display chip

  	spi_slaveSelect(0);				// Select display chip
  	spi_write(0x0C); 				// Register 0B: Shutdown register
  	spi_write(0x01); 				// 	-> 1 = Normal operation
  	spi_slaveDeSelect(0);			// Deselect display chip
}

// Set display on ('normal operation')
void displayOn() 
{
  	spi_slaveSelect(0);				// Select display chip
  	spi_write(0x0C); 				// Register 0B: Shutdown register
  	spi_write(0x01); 				// 	-> 1 = Normal operation
  	spi_slaveDeSelect(0);			// Deselect display chip
}

// Set display off ('shut down')
void displayOff() 
{
  	spi_slaveSelect(0);				// Select display chip
  	spi_write(0x0C); 				// Register 0B: Shutdown register
  	spi_write(0x00); 				// 	-> 1 = Normal operation
  	spi_slaveDeSelect(0);			// Deselect display chip
}

// Write a word = address byte + data byte from master to slave
void spi_writeWord(unsigned char address, unsigned char data)
{
	spi_slaveSelect(0); 
	spi_write(address);   
	spi_write(data);       
	spi_slaveDeSelect(0); 
}

// toont de waarde van value op het 4-digit display
void writeLedDisplay(int value) {
	int isNegative = (value < 0);  // Check of het getal negatief is

	if (isNegative) {
		value = -value;  // Maak de waarde positief voor verwerking
	}

	for (char i = 1; i <= 3; i++) {
		spi_writeWord(i, value % 10);  // Laatste cijfer versturen
		value /= 10;                   // Verwijder het laatste cijfer
	}
	
	if (isNegative) {
		spi_writeWord(4, 10);  // 10 = code voor minteken (-) in MAX7219
	} else {
		spi_writeWord(4, value);   // Anders een lege plek of 0
	}
}



int main()
{
	// inilialize
	DDRB=0x01;					  	// Set PB0 pin as output for display select
	spi_masterInit();              	// Initialize spi module
	displayDriverInit();            // Initialize display chip

 	// clear display (all zero's)
	for (char i =1; i<=4; i++)
	{
      	spi_slaveSelect(0); 		// Select display chip
      	spi_write(i);  				// 	digit adress: (digit place)
      	spi_write(0);				// 	digit value: 0 
  	  	spi_slaveDeSelect(0);		// Deselect display chip
	}    
	wait(1000);
	
	writeLedDisplay(1890);
	
  	return (1);
}