

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

#define portb (*((volatile bf_8b*)(&PORTB)))


void setup() {
	// LED0 and LED1
	DDRB |= (1<<PB2) | (1<<PB1);
	
	
	TCCR1A = 0; // On Arduino default is 1.
	//TCCR1B = 1; // CLK/1
	//TCCR1B = 2; // CLK/8
	TCCR1B = 0b101; // CLK/1024

	Serial.begin(115200);
}


static void restart_timer(void) {
	// Write first to high reg.
	TCNT1H = 0;
	TCNT1L = 0;
}
static uint16_t read_timer() {
	return (uint16_t)TCNT1L | TCNT1H<<8; // First read low reg.
}

void loop() {
	Serial.println(read_timer());

	if(read_timer() >= 15625){
		restart_timer();
			
		portb.b1 = !portb.b1;
	}
	
}
