typedef struct {
	unsigned b0 : 1;
	unsigned b1 : 1;
	unsigned b2 : 1;
	unsigned b3 : 1;
	unsigned b4 : 1;
	unsigned b5 : 1;
	unsigned b6 : 1;
	unsigned b7 : 1;
} bf_8b;

//TODO ddrb
#define portb (*((volatile bf_8b*)(&PORTB)))
#define ddrb (*((volatile bf_8b*)(&DDRB)))

void setup() {
	//TODO ddrb
	//DDRB= (1<<PB2) | (1<<PB1);
        ddrb.b1 = 1;
        ddrb.b2 = 1;
}

void loop() {
	static uint8_t x = 0;
	
	//digitalWrite(LED0, x>>0 & 1);
	//digitalWrite(LED1, x>>1 & 1);
	//PORTB = x << PB1;
	portb.b1 = x>>0 & 1;
	portb.b2 = x>>1 & 1;
	
	x++;
	delay(1000); // [ms]
}
