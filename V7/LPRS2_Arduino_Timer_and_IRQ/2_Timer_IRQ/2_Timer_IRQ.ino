

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

ISR(TIMER1_COMPA_vect){
	portb.b1 = !portb.b1;
}

void setup() {
	// LED0 and LED1
	DDRB |= (1<<PB2) | (1<<PB1);
	
	
	TCCR1A = 0;
	TCCR1B = 0b101; // CLK/1024
	
	TCCR1B |= 1<<WGM12; // CTC mode.
	
	OCR1A = 15625;
	
	TIMSK1 |= 1<<OCIE1A; // IRQ
	sei();

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
	/*
	if(read_timer() == ){
		restart_timer();
			
		portb.b1 != portb.b1;
	}
	*/
}
