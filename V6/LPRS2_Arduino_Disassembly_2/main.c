
#include <avr/io.h>
#include <stdio.h>

#include "lib.h"
#include "uart_stdio.h"

void setup() {
	// Make all pins outputs.
	DDRB = 0xff;
	uart_stdio_init();
	
	
	TCCR1B = 1; // CLK/1
	//TCCR1B = 2; // CLK/8
}


static void start_timer(void) {
	// Write first to high reg.
	TCNT1H = 0;
	TCNT1L = 0;
}
static uint16_t read_timer() {
	return (uint16_t)TCNT1L | TCNT1H<<8; // First read low reg.
}

void loop() {
	
	start_timer();
	pipeline_test_1(10);
	uint16_t clks_passed = read_timer();
	printf("%d\n", clks_passed);
	//UART_Tx('a');
	printf("hello\n");
}


#ifndef ARDUINO
int main(void) {
	setup();
	
	while(1){ // Infinite loop.
		loop();
	}
}
#endif
