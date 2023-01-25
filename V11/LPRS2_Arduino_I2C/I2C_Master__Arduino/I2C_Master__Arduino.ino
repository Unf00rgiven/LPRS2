
#define LED0 9
#define LED1 10

// I2C library.
#include <Wire.h>

void setup() {
	pinMode(LED0, OUTPUT);
	pinMode(LED1, OUTPUT);
	digitalWrite(LED0, 0);
	digitalWrite(LED1, 0);
	
	// UART to console.
	Serial.begin(115200);

	// Join I2C bus (address optional for the Master).
	Wire.begin();
}

void loop() {	
	// LED0 with slower blinking indicate the Master.
	digitalWrite(LED0, 0);
	delay(500);
	digitalWrite(LED0, 1);
	delay(500);
	
	// Request 1 byte from slave device #8 over I2C.
	Wire.requestFrom(8, 1);
	// WARN: Slave may send less than requested.
	if(Wire.available()){ // Slave send something
		uint8_t i2c_rxd = Wire.read(); // Receive a byte over I2C.
	
		Serial.println(i2c_rxd);
		digitalWrite(LED1, i2c_rxd & 1);
	}
}
