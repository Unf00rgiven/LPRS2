

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

#define N_TIMER_LOG 512
uint16_t timer_log[N_TIMER_LOG];
uint16_t timer_log_top;

void loop() {
	uint16_t t = read_timer();
	
	//Serial.println(read_timer());
	timer_log[timer_log_top] = t;
	timer_log_top++;
	if(timer_log_top == N_TIMER_LOG){
		timer_log_top = 0;
	}

	if(t == 15625){
		restart_timer();
			
		portb.b1 = !portb.b1;

		Serial.println("Timer Log:");
		Serial.print("timer_log_top = "); Serial.println(timer_log_top);
#if 0
		for(uint16_t i = 0; i < N_TIMER_LOG; i++){
			Serial.println(timer_log[timer_log_top]);
			
			timer_log_top++;
			if(timer_log_top == N_TIMER_LOG){
				timer_log_top = 0;
			}
		}
#elif 0
		for(uint16_t i = 0; i < N_TIMER_LOG; i++){
			uint16_t idx = timer_log_top + i;
			if(idx >= N_TIMER_LOG){
				idx -= N_TIMER_LOG;
			}
			Serial.println(timer_log[idx]);
		}
#else
		uint16_t idx = timer_log_top;
		do{
			Serial.println(timer_log[idx]);

			idx++;
			if(idx == N_TIMER_LOG){
				idx = 0;
			}
		}while(idx != timer_log_top);
#endif
	}
	
}
