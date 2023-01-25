
#define LED0 9
#define LED1 10

// Success: not using Arduino's SPI lib at all; everything Low Level.
//#include <SPI.h>

void setup() {
	pinMode(LED0, OUTPUT);
	digitalWrite(LED0, 0);
	
	// UART to console.
	Serial.begin(115200);

	// Begins the SPI commnuication.
	//SPI.begin();
	// Setup pins before setting SPI.
	pinMode(SS, OUTPUT);
	pinMode(SCK, OUTPUT);
	pinMode(MOSI, OUTPUT);
#if 1
	// CPOL=0
	digitalWrite(SCK, 0);
#else
	// Datasheet, p176
	SPCR = (1<<SPE)|(1<<MSTR); // Enable SPI and set it Master.
	// Sets clock for SPI communication at 16MHz/16 = 1Mhz.
	//SPI.setClockDivider(SPI_CLOCK_DIV16);
	// Datadsheet, p177, Table 19-5
	SPCR |= 0b01;
#endif
	// Setting SS (Slave Select) to inactive.
	digitalWrite(SS, HIGH);
}

void loop() {
	static uint8_t master_cnt = 0;
	master_cnt++;
	
	uint8_t master_txd = master_cnt;

	// Starts SPI communication with Slave connected to Master.
	digitalWrite(SS, LOW);

#if 1
	uint8_t master_rxd = 0;
	for(int i = 0; i < 8; i++){
		// Falling edge.
		digitalWrite(SCK, 0);
		// CPHA=0, change data on falling edge.
		bool b = master_txd>>7 & 1;
		digitalWrite(MOSI, b); // DORD=0, MSB first.
		master_txd <<= 1;
		delay(1); // [ms]
		
		// Rising edge.
		digitalWrite(SCK, 1);
		// CPHA=0, sample data on rising edge.
		b = digitalRead(MISO);
		master_rxd <<= 1;
		master_rxd |= b;
		delay(1); // [ms]
	}
	digitalWrite(SCK, 0);
	master_txd = master_cnt;
#else
	// Send the master_txd value to Slave
	// and also receives value from Slave.
	//uint8_t master_rxd = SPI.transfer(master_txd);
	// Datasheet, p178
	SPDR = master_txd; // Start transfer by writting Tx data to SPDR.
	// Datasheet, p177
	while(!(SPSR & (1<<SPIF))){} // Busy wait.
	uint8_t master_rxd = SPDR; // Collect value from Slave.
#endif

	// Show LSB of Rx data on LED0.
	digitalWrite(LED0, master_rxd & 1);
	Serial.println(master_txd);
	Serial.println(master_rxd);
	Serial.println();
	
	delay(100);
	digitalWrite(SS, HIGH);
	delay(900);

}
