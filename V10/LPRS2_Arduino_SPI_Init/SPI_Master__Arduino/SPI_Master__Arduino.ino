
#define LED0 9
#define LED1 10

#include <SPI.h>

void setup() {
	pinMode(LED0, OUTPUT);
	digitalWrite(LED0, 0);
	
	// UART to console.
	Serial.begin(115200);

	// Begins the SPI commnuication.
	SPI.begin();
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
