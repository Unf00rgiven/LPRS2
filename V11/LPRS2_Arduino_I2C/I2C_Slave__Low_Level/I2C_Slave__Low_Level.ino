
#define LED0 9
#define LED1 10

//// I2C library.
//#include <Wire.h>
#include <avr/interrupt.h>
#include <util/twi.h>

#define DEBUG(x) \
	do{ \
		Serial.print(#x" = "); Serial.println(x); \
	}while(0)
#define DEBUG_HEX(x) \
	do{ \
		Serial.print(#x" = "); Serial.println(x, HEX); \
	}while(0)

// I2C ISR (Interrupt Service Routine).
void I2C_req_event() {
	static uint8_t i2c_txd = 0;
	i2c_txd++;
	
	digitalWrite(LED1, i2c_txd & 1);
	
	// Respond with message of 1 byte, as expected by Master.
	//Wire.write(i2c_txd);
	TWDR = i2c_txd;
}

void I2C_recv_event(uint8_t b) {
}

#define SET_TWCR() \
	do{ \
		TWCR = 1<<TWEN | 1<<TWINT | 1<<TWIE | 1<<TWEA; \
	}while(0)

ISR(TWI_vect){
	//DEBUG_HEX(TW_STATUS);
	switch(TW_STATUS){
	case TW_ST_SLA_ACK:
	case TW_ST_DATA_ACK:
		// Master is requesting data, call the request callback.
		I2C_req_event();
		SET_TWCR();
		break;
    case TW_SR_DATA_ACK:
		I2C_recv_event(TWDR); 
		SET_TWCR();
		break;
	case TW_BUS_ERROR:
		// Some sort of erroneous state, prepare TWI to be readdressed.
		TWCR = 0;
		SET_TWCR();
		break;
	default:
		SET_TWCR();
		break;
	}
} 

void setup() {
	pinMode(LED0, OUTPUT);
	pinMode(LED1, OUTPUT);
	digitalWrite(LED0, 0);
	digitalWrite(LED1, 0);

	// UART to console.
	Serial.begin(115200);
	
	//// Join i2c bus with address #8.
	//Wire.begin(8);
	//// Register I2C ISR.
	//Wire.onRequest(I2C_req_event);
	TWAR = 8 << 1;
	SET_TWCR();
	sei();
}


void loop() {
	// LED0 with faster blinking indicate the Saster.
	digitalWrite(LED0, 0);
	delay(200);
	digitalWrite(LED0, 1);
	delay(200);
}
