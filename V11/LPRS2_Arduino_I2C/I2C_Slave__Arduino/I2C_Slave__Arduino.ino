
#define LED0 9
#define LED1 10

// I2C library.
#include <Wire.h>


// I2C ISR (Interrupt Service Routine).
void I2C_request_event() {
	static uint8_t i2c_txd = 0;
	i2c_txd++;
	
	digitalWrite(LED1, i2c_txd & 1);
	
	// Respond with message of 1 byte, as expected by Master.
	Wire.write(i2c_txd);
}

void setup() {
	pinMode(LED0, OUTPUT);
	pinMode(LED1, OUTPUT);
	digitalWrite(LED0, 0);
	digitalWrite(LED1, 0);

	
	// Join i2c bus with address #8.
	Wire.begin(8);
	// Register I2C ISR.
	Wire.onRequest(I2C_request_event);
}


void loop() {
	// LED0 with faster blinking indicate the Saster.
	digitalWrite(LED0, 0);
	delay(100);
	digitalWrite(LED0, 1);
	delay(100);
}
