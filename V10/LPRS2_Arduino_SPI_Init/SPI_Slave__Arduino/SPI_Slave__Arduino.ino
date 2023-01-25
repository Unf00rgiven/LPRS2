
#define LED0 9
#define LED1 10

#include <SPI.h>

volatile bool received;
volatile uint8_t slave_rxd;

// SPI ISR (Interrupt Service Routine).
ISR(SPI_STC_vect)
{
	// Store value received from Master.
	slave_rxd = SPDR;
	// Signal to polling in loop().
	received = true;
}

void setup() {
	pinMode(LED0, OUTPUT);
	digitalWrite(LED0, 0);
	
	// UART to console.
	Serial.begin(115200);

	
	//Sets MISO as OUTPUT (Have to Send data to Master IN).
	pinMode(MISO, OUTPUT);
	// Turn on SPI in Slave Mode.
	SPCR |= 1 << SPE;
	received = false;
	// Enabling SPI ISR (Interrupt Service Routine).
	SPI.attachInterrupt();
}

void loop() {
	if(received){ // Polling something received.
		received = false;
		
		// Could be done in IRQ.
		uint8_t slave_txd = slave_rxd+10;
		SPDR = slave_txd; // Sends value to master via SPDR.

		// Show LSB of Rx data on LED0.
		digitalWrite(LED0, slave_rxd & 1);
		Serial.println(slave_rxd);
		Serial.println(slave_txd);
		Serial.println();
	}
}
