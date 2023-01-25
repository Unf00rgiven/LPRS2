
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
	static uint8_t master_cnt = 0;
	// mod 10 counting.
	master_cnt++;
	if(master_cnt == 10){
		master_cnt = 0;
	}
	char txd = 0x30+master_cnt;
	Serial.write(txd);
	

	// Blinking LED0 indicate Tx works.
	digitalWrite(LED0, txd & 1);

	// Read one byte from Slave.
	if(Serial.available()) {
		char rxd = Serial.read();

		// Blinking LED1 indicate Rx works.
		digitalWrite(LED1, rxd & 1);
	}
	delay(250); // [ms]
}
