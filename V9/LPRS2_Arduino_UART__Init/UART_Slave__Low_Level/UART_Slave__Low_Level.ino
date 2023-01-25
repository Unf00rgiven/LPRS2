
#define LED0 9
#define LED1 10

void setup() {
	pinMode(LED0, OUTPUT);
	pinMode(LED1, OUTPUT);
	digitalWrite(LED0, 0);
	digitalWrite(LED1, 0);
	
	Serial.begin(9600);
}

void loop() {
	// Read one byte from Master.
	if(Serial.available()) {
		char rxd = Serial.read();
		
		// Blinking LED1 indicate Rx works.
		digitalWrite(LED1, rxd & 1);

		// Loop it back to Master.
		Serial.write(rxd);
	}
	
	delay(50); // [ms]
	
	static uint8_t slave_cnt = 0;
	slave_cnt++;
	// Blinking LED0 indicate Slave.
	digitalWrite(LED0, slave_cnt & 1);
}
