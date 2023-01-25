

#define DEBUG(x) \
	do{ \
		Serial.print(#x" = "); Serial.println(x); \
	}while(0)

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


typedef union {
	struct {
		uint8_t pcicr;
		uint8_t eicra;
		uint8_t _res;
		uint8_t pcmsk[3];
		uint8_t timsk[3];
	} r;
	struct {
		unsigned pcie0   : 1;
		unsigned pcie1   : 1;
		unsigned pcie2   : 1;
		unsigned         : 5;
		
		unsigned ics     : 4;
		unsigned         : 4;
		
		unsigned         : 8;
		
		unsigned pcint00 : 1;
		unsigned pcint01 : 1;
		unsigned pcint02 : 1;
		unsigned pcint03 : 1;
		unsigned pcint04 : 1;
		unsigned pcint05 : 1;
		unsigned pcint06 : 1;
		unsigned pcint07 : 1;

		unsigned pcint08 : 1;
		unsigned pcint09 : 1;
		unsigned pcint10 : 1;
		unsigned pcint11 : 1;
		unsigned pcint12 : 1;
		unsigned pcint13 : 1;
		unsigned pcint14 : 1;
		unsigned         : 1;

		unsigned pcint16 : 1;
		unsigned pcint17 : 1;
		unsigned pcint18 : 1;
		unsigned pcint19 : 1;
		unsigned pcint20 : 1;
		unsigned pcint21 : 1;
		unsigned pcint22 : 1;
		unsigned pcint23 : 1;
		
		//TODO Better.
		unsigned toie0   : 1;
		unsigned ocie0a  : 1;
		unsigned ocie0b  : 1;
		unsigned         : 5;
		
		unsigned toie1   : 1;
		unsigned ocie1a  : 1;
		unsigned ocie1b  : 1;
		unsigned         : 2;
		unsigned icie1   : 1;
		unsigned         : 2;
		
		unsigned toie2   : 1;
		unsigned ocie2a  : 1;
		unsigned ocie2b  : 1;
		unsigned         : 5;
	} f;
} bf_irq;

typedef union {
	struct {
		uint8_t tccra;
		uint8_t tccrb;
		uint8_t tccrc;
		uint8_t _res;
		uint16_t tcnt;
		uint16_t icr;
		uint16_t ocra;
		uint16_t ocrb;
	} r;
	struct {
		unsigned wgm0  : 1;
		unsigned wgm1  : 1;
		unsigned       : 2;
		unsigned comb  : 2;
		unsigned coma  : 2;

		unsigned cs    : 3; // Prescaling.
		unsigned wgm2  : 1;
		unsigned wgm3  : 1;
		unsigned       : 1;
		unsigned ices  : 1;
		unsigned icnc  : 1;
		
		unsigned       : 6;
		unsigned foca  : 1;
		unsigned focb  : 1;
		
		unsigned       : 8;
		
		//TODO Other.
	} f;
} bf_tc_16b;

typedef union {
	struct {
		uint8_t tccra;
		uint8_t tccrb;
		uint8_t tcnt;
		uint8_t ocra;
		uint8_t ocrb;
	} r;
	struct {
		unsigned wgm0  : 1;
		unsigned wgm1  : 1;
		unsigned       : 2;
		unsigned comb  : 2;
		unsigned coma  : 2;

		unsigned cs    : 3; // Prescaling.
		unsigned wgm2  : 1;
		unsigned       : 2;
		unsigned foca  : 1;
		unsigned focb  : 1;
	} f;
} bf_tc_8b;


#define ddrb (*((volatile bf_8b*)(&DDRB)))
#define portb (*((volatile bf_8b*)(&PORTB)))
#define irq (*((volatile bf_irq*)(&PCICR)))
#define tc0 (*((volatile bf_tc_8b*)(&TCCR0A)))
#define tc1 (*((volatile bf_tc_16b*)(&TCCR1A)))
#define tc2 (*((volatile bf_tc_8b*)(&TCCR2A)))



ISR(TIMER1_COMPA_vect){
	//portb.b2 = !portb.b2;
}

ISR(PCINT1_vect){
	portb.b1 = !portb.b1;
}

void setup() {
	Serial.begin(115200);
	
	// LED0 and LED1
	DDRB |= (1<<PB2) | (1<<PB1);
	
	TCCR1A = 0;
	TCCR1B = 1; // CLK/1
	//TCCR1B = 0b101; // CLK/1024
	TCCR1B |= 1<<WGM12; // CTC mode.
	OCR1A = 15625;
	// ISR on OCR1A.
	TIMSK1 |= 1<<OCIE1A; // IRQ enabled.
	// OC1B toggle.
	TCCR1A = 1<<COM1B0;
	OCR1B = 15625;
	
	// ISR on pin 16 toggle.
	PCMSK1 = 1<<PCINT13;
	PCICR |= 1<<PCIE1;
	
	// PWM.
	//-T 2MHz.
	//-T Fast PWM 14
	//-T ICR1 as top
	//-T Enable IRQ on TOV1
	//-T OCR1B = ICR1 - OCR1A
	//-T PWM OC1A/OC1B
	//-T Up down OCR1A/OCR1B by SW in ISR.
	//-T Timer0 Phase Correct PWM 5
	//-T Clock Timer0 by T0 toggled from ISR.
	//-T Update OCR1A/OCR1B from Timer0 in ISR.
	
	sei();

}



void loop() {
	//uint16_t x = (uint16_t)TCNT1L | TCNT1H<<8; // First read low reg.
	//uint16_t x = TCNT1;
	uint16_t x = tc1.r.tcnt;
	
	Serial.println(x);
}
