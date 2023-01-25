
#include <stdint.h>
#include "system.h"
#include <stdio.h>


#define pio_p32 ((volatile uint32_t*)PIO_BASE)

#define PIO_LED0 0
#define PIO_SW0 8
#define PIO_LED_PACKED 16
#define PIO_SW_PACKED 17


typedef struct {
	// Unpacked.
	// reg 0-7
	uint32_t led_unpacked[8];
	// reg 8-15
	uint32_t sw_unpacked[8];
	// Packed.
	// reg 16
	unsigned led_packed : 8;
	unsigned            : 24;
	// reg 17
	unsigned sw_packed  : 8;
	unsigned            : 24;
	uint32_t babadeda[14];
} bf_pio;

#define pio (*((volatile bf_pio*)PIO_BASE))


int main() {
	uint8_t x = 0;
	
	pio_p32[PIO_LED_PACKED] = x;

	while(1){
		///////////////////
		// Read inputs.
		
		
		///////////////////
		// Calculate state.
		
		//TODO Uncomment meeeeee, aaaaaaaaaa!
		//x++;
		
		//TODO Sum of all sw bits to led. 
		
		///////////////////
		// Write outputs.
		
		
	}

	return 0;
}
