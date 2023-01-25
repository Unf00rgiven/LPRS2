
#define LED0 9
#define LED1 10

// TODO Avoid using SPI library.
#include <SPI.h>

void setup() {
	pinMode(LED0, OUTPUT);
	digitalWrite(LED0, 0);
	
	// UART to console.
	Serial.begin(115200);

	// Begins the SPI commnuication.
	// TODO Avoid using SPI library:
	// - Set pin dir
	// - Setup SPI
	SPI.begin();
	// TODO Avoid using SPI library: setup clk speed.
	// Sets clock for SPI communication at 16MHz/16 = 1Mhz.
	SPI.setClockDivider(SPI_CLOCK_DIV16);
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
	// TODO Avoid using SPI library:
	// - Set Tx data
	// - Poll transfer end
	// - read Rx data
	uint8_t master_rxd = SPI.transfer(master_txd);

	// Show LSB of Rx data on LED0.
	digitalWrite(LED0, master_rxd & 1);
	Serial.println(master_txd);
	Serial.println(master_rxd);
	Serial.println();
	
	delay(500);
	digitalWrite(SS, HIGH);
	delay(500);

}
