
#include <avr/io.h>


#include "lib.h"






int main(void) {
	
	
	
	// Make all pins outputs.
	DDRB = 0xff;
	
	
	
	
	uint8_t s = 0;
	
	
	
	
	while(1){ //infinite loop
		
		
		
		
		
		s = ~s;
		
		
		
		PORTB = s;
		
		
		
		
		delay_ms(1000/2/5); // For 5Hz flashing.
		
		
		
		
	}
}
