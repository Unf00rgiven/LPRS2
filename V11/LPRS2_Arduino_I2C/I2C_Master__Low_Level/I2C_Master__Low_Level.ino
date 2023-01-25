
#define LED0 9
#define LED1 10

//// I2C library.
//#include <Wire.h>
#include <util/twi.h>

#define DEBUG(x) \
	do{ \
		Serial.print(#x" = "); Serial.println(x); \
	}while(0)
#define DEBUG_HEX(x) \
	do{ \
		Serial.print(#x" = "); Serial.println(x, HEX); \
	}while(0)

void setup() {
	pinMode(LED0, OUTPUT);
	pinMode(LED1, OUTPUT);
	digitalWrite(LED0, 0);
	digitalWrite(LED1, 0);
	
	// UART to console.
	Serial.begin(115200);

	//// Join I2C bus (address optional for the Master).
	//Wire.begin();
    // Set SCL to 400kHz.
    TWSR = 0x00;
    TWBR = 0x0C;
    // Enable TWI.
    TWCR = 1<<TWEN;
}


#define I2C_WAIT_DONE() while((TWCR & (1<<TWINT)) == 0)

void I2C_start(void) {
    TWCR = 1<<TWEN | 1<<TWINT | 1<<TWSTA;
    I2C_WAIT_DONE();
}
void I2C_stop(void) {
    TWCR = 1<<TWEN | 1<<TWINT | 1<<TWSTO;
}
void I2C_write(uint8_t u8data) {
    TWDR = u8data;
    TWCR = 1<<TWEN | 1<<TWINT;
    I2C_WAIT_DONE();
}
uint8_t I2C_read_ACK(void) {
	TWCR = 1<<TWEN | 1<<TWINT | 1<<TWEA;
    I2C_WAIT_DONE();
    return TWDR;
}
// Read byte with NACK.
uint8_t I2C_read_NACK(void) {
	TWCR = 1<<TWEN | 1<<TWINT;
    I2C_WAIT_DONE();
    return TWDR;
}

void loop() {
	// LED0 with slower blinking indicate the Master.
	digitalWrite(LED0, 0);
	delay(500);
	digitalWrite(LED0, 1);
	delay(500);
	
	//// Request 1 byte from slave device #8 over I2C.
	//Wire.requestFrom(8, 1);
	//// WARN: Slave may send less than requested.
	//if(Wire.available()){ // Slave send something
	//	uint8_t i2c_rxd = Wire.read(); // Receive a byte over I2C.
	uint8_t i2c_rxd;
    // Send start.
    I2C_start();
    if(TW_STATUS != TW_START){
    	return;
    }
    // Select devise and send read bit.
    I2C_write(8<<1 | 1);
    if(TW_STATUS != TW_MR_SLA_ACK){
    	return;
    }
    i2c_rxd = I2C_read_NACK();
    if(TW_STATUS != TW_MR_DATA_NACK){
    	return;
    }
    I2C_stop();
	
	Serial.println(i2c_rxd);
	digitalWrite(LED1, i2c_rxd & 1);
}
