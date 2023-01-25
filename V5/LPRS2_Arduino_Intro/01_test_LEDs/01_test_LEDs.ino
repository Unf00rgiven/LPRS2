
#define LED0  9   // PB1
#define LED1 10   // PB2

void setup() {
	pinMode(LED0, OUTPUT);
	pinMode(LED1, OUTPUT);

}

void loop() {
	static uint8_t x = 0;
	digitalWrite(LED0, x>>0 & 1);
	digitalWrite(LED1, x>>1 & 1);
	x++;
	delay(1000); // [ms]

}
