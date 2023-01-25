
#include <stdint.h>
#include "system.h"
#include <stdio.h>


#define pio_p32 ((volatile uint32_t*)PIO_BASE)

#define PIO_LED0 0
#define PIO_SW0 8
#define PIO_LED_PACKED 16
#define PIO_SW_PACKED 17

int main() {
	uint8_t x = 0;
	
	pio_p32[PIO_LED_PACKED] = x;

	while(1){
		///////////////////
		// Read inputs.

		x = pio_p32[PIO_SW_PACKED];

		///////////////////
		// Calculate state.
		
		//TODO Uncomment meeeeee, aaaaaaaaaa!
		//x++;
		
		//TODO Sum of all sw bits to led. 

		///////////////////
		// Write outputs.
		
		pio_p32[PIO_LED_PACKED] = x;
		
	}

	return 0;
}
