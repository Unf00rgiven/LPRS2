
#include "uart_stdio.h"

#include <stdio.h>
#include <avr/io.h>

void UART_Init() {
//#define BAUD 115200 //FIXME Cannot have it?
#define BAUD 9600
#include <util/setbaud.h>
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
#if USE_2X
	UCSR0A |= (1 << U2X0);
	#else
	UCSR0A &= ~(1 << U2X0);
#endif

	// Enable receiver and transmitter
	//TODO Clean up.
	//UCSR0B = (1<<RXEN0)|(1<<TXEN0);//|(0<<RXCIE)|(0<<UDRIE);TODO
	UCSR0B = (1<<TXEN0);

	// Set the USART to asynchronous at 8 bits, no parity and 1 stop bit.
	UCSR0C = (0<<UMSEL00)|(0<<UPM00)|(0<<USBS0)|(3<<UCSZ00)|(0<<UCPOL0);
}

void UART_Tx(char buffer) {
	// Wait for empty transmit buffer.
	while (!(UCSR0A & (1 << UDRE0))){}

	// Put data into buffer, sends data.
	UDR0 = buffer;
}

char UART_Rx(void) {
	while (!(UCSR0A & (1<<RXC0))){}
	return UDR0;
}


static int uart_putchar(char c, FILE *stream) {
	UART_Tx(c);
	return 0;
}

static FILE uart_str = FDEV_SETUP_STREAM(
	uart_putchar,
	NULL,
	_FDEV_SETUP_WRITE
);

void uart_stdio_init(void) {
	
	UART_Init();

	stdout = stdin = &uart_str;
}