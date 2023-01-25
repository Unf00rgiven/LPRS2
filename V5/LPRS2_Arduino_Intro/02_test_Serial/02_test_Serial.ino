// Ctrl+Shift+M to open console.

void setup() {
	Serial.begin(115200);
}

void loop() {
	static uint8_t x = 0;
	Serial.println(x);
	x++;
	delay(200); // [ms]
}
