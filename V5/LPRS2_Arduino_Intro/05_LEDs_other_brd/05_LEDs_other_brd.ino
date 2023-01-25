typedef struct {
	unsigned b0   : 1;
	unsigned b1   : 1;
	unsigned b2   : 1;
	unsigned b3   : 1;
	unsigned SDA  : 1;
	unsigned SCL  : 1;
	unsigned b6   : 1;
	unsigned b7   : 1;
} c_bf; // c registar BF

typedef struct {
  unsigned b0   : 1;
  unsigned led0 : 1;
  unsigned led1 : 1;
  unsigned MOSI : 1;
  unsigned MISO : 1;
  unsigned b5   : 1;
  unsigned b6   : 1;
  unsigned b7   : 1;
} b_bf; // b registar BF


#define portc (*((volatile c_bf*)(&PORTC)))
#define ddrc (*((volatile c_bf*)(&DDRC)))
#define pinc (*((volatile c_bf*)(&PINC)))

#define portb (*((volatile b_bf*)(&PORTB)))
#define ddrb (*((volatile b_bf*)(&DDRB)))
#define pinb (*((volatile b_bf*)(&PINB)))



void setup() {
  Serial.begin(115200);
  // c5 output c4 input
	ddrc.SCL = 1;
  ddrc.SDA = 0;

  // led 0 & led 1 otputs
  ddrb.led0 = 1;
  ddrb.led1 = 1;
}

void loop() {
	static uint8_t x = 0;
	
	//TODO x[0] -> SCL -> jumper cable -> SDA -> LED0
	portc.SCL = x & 1;        // u scl upisujemo nulti bit x
  portb.led0 = pinc.SDA;    // u led 0 upisujemo ucitanu vrednost sa SDA

	//TODO x[1] -> MOSI -> jumper cable -> MISO -> LED1
	portb.MOSI = x>>1 & 1;    // u mosi upisujemo prvi bit x
  portb.led1 = pinb.MISO;   // u led 1 upisujemo ucitanu vrednost sa MISO
	
	x++;
	delay(1000); // [ms]
}
