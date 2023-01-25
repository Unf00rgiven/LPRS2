
#include <avr/io.h>


#include "lib.h"






int main(void) {
	
	// Pragmatic error.
	PORTC = calc1(1, 2);
	
	return 0;
}
