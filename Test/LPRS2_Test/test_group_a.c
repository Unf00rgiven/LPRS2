

#include <stdint.h>
#include "system.h"
#include "sys/alt_irq.h"
#include <stdio.h>

#define pio_p32 ((volatile uint32_t*)PIO_BASE)

int main() {

	while(1){
		///////////////////
		// Read inputs.

		///////////////////
		// Calculate state.

		///////////////////
		// Write outputs.
		
		pio_p32[16]++;
		
		///////////////////
		// Other things to do.
		
	}

	return 0;
}
