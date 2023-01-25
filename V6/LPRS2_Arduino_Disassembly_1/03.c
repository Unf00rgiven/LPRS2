
#include <avr/io.h>


#include "lib.h"






int main(void) {
	uint16_t x = calc2(0x0201, 0x0403);
	PORTC = x & 0xff;
	PORTD = x>>8;
	
	return 0;
}
