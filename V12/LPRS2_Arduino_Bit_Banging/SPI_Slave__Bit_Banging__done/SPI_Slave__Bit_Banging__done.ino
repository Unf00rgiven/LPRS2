
#define LED0 9
#define LED1 10

// Success: not using Arduino's SPI lib at all; everything Low Level.
//#include <SPI.h>

#if 1
#else
volatile bool received;
volatile uint8_t slave_rxd;

// SPI ISR (Interrupt Service Routine).
ISR(SPI_STC_vect)
{
	// Store value received from Master.
	slave_rxd = SPDR;
	// Done in IRQ.
	uint8_t slave_txd = slave_rxd+10;
	SPDR = slave_txd; // Sends value to master via SPDR.
	// Signal to polling in loop().
	//received = true;

	digitalWrite(LED0, slave_rxd & 1);
}
#endif

void setup() {
	pinMode(LED0, OUTPUT);
	digitalWrite(LED0, 0);
	
	// UART to console.
	Serial.begin(115200);

	//Sets MISO as OUTPUT (Have to Send data to Master IN).
	pinMode(MISO, OUTPUT);
#if 1
#else
	// Turn on SPI in Slave Mode.
	SPCR |= 1 << SPE;
	received = false;
	// Enabling SPI ISR (Interrupt Service Routine).
	//SPI.attachInterrupt();
	SPCR |= 1<<SPIE;
#endif
}

void loop() {
/*
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
*/
#if 1
	static uint8_t slave_rxd;
	static uint8_t slave_txd;
	static bool prev_ss = 1;
	static bool prev_sck = 0;
	
	bool ss = digitalRead(SS);
	bool sck = digitalRead(SCK);

	bool re_ss = prev_ss==0 && ss==1;
	bool fe_ss = prev_ss==1 && ss==0;
	bool re_sck = prev_sck==0 && sck==1;
	bool fe_sck = prev_sck==1 && sck==0;

	if(fe_ss){
		slave_rxd = 0;
		
		bool b = slave_txd>>7 & 1;
		digitalWrite(MISO, b);
		slave_txd <<= 1;
	}
	if(ss == 0){
		if(re_sck){
			bool b = digitalRead(MOSI);
			slave_rxd <<= 1;
			slave_rxd |= b;
		}
		if(fe_sck){
			bool b = slave_txd>>7 & 1;
			digitalWrite(MISO, b);
			slave_txd <<= 1;
		}
	}
	if(re_ss){
		// SS rising edge.
		slave_txd = slave_rxd+10;
		digitalWrite(LED0, slave_rxd & 1);
		Serial.println(slave_rxd);
		Serial.println(slave_txd);
		Serial.println();
	}

	prev_ss = ss;
	prev_sck = sck;
#else
	// Done in IRQ. Could do other stuff here.
#endif
}
