
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
	// Datasheet, p176
	SPCR = (1<<SPE)|(1<<MSTR); // Enable SPI and set it Master.
	// Sets clock for SPI communication at 16MHz/16 = 1Mhz.
	//SPI.setClockDivider(SPI_CLOCK_DIV16);
	// Datadsheet, p177, Table 19-5
	SPCR |= 0b01;
	// Setting SS (Slave Select) to inactive.
	digitalWrite(SS, HIGH);
}

void loop() {
	static uint8_t master_cnt = 0;
	master_cnt++;
	
	uint8_t master_txd = master_cnt;

	// Starts SPI communication with Slave connected to Master.
	digitalWrite(SS, LOW);
	
	// Send the master_txd value to Slave
	// and also receives value from Slave.
	//uint8_t master_rxd = SPI.transfer(master_txd);
	// Datasheet, p178
	SPDR = master_txd; // Start transfer by writting Tx data to SPDR.
	// Datasheet, p177
	while(!(SPSR & (1<<SPIF))){} // Busy wait.
	uint8_t master_rxd = SPDR; // Collect value from Slave.

	// Show LSB of Rx data on LED0.
	digitalWrite(LED0, master_rxd & 1);
	Serial.println(master_txd);
	Serial.println(master_rxd);
	Serial.println();
	
	delay(100);
	digitalWrite(SS, HIGH);
	delay(900);

}
