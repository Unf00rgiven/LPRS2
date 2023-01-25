#include <stdint.h>
#include "system.h"
#include <stdio.h>
#include <unistd.h>

#define pio_p32 ((volatile uint32_t*)PIO_BASE)

#define PIO_LED0 0
#define PIO_SW0 8
#define PIO_LED_PACKED 16
#define PIO_SW_PACKED 17

#define digits_p32 ((volatile uint32_t*)DIGITS_BASE)

#define SEGM_0             0
#define SEGM_1             1
#define SEGM_2             2
#define SEGM_3             3
#define SEGM_PACK          4


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

void busywait(int ms){
	for(int i = 0; i < ms ; i++) {}
}

int main(){
	int brojac = 0;
	int ubrojac = 0;

	digits_p32[SEGM_0] = 0;
	digits_p32[SEGM_1] = 0;
	digits_p32[SEGM_2] = 0;
	digits_p32[SEGM_3] = 0;
	for(int i=0; i < 8; i++)
	{
		pio.led_unpacked[i] = 0;
	}
	
	while(1)
	{
		// reset
		if((pio_p32[PIO_SW_PACKED] & 1) == 1)
		{
			for(int i=0; i < 8; i++)
			{
				pio.led_unpacked[i] = 0;
			}
			
			brojac =0;
			ubrojac=0;	
		} 
		
		// pauza
		busywait(5000000);
		while((pio_p32[PIO_SW_PACKED] & 2) == 2) 
		{
			if((pio_p32[PIO_SW_PACKED] & 1) == 1) break;
		}

		if((pio_p32[PIO_SW_PACKED] & 1) != 1) ubrojac++;

		if(ubrojac == 100)
		{
			brojac++;
			ubrojac = 0;
		}

		// binary counter 
		for(int i=0; i < 8; i++)
		{
			pio.led_unpacked[i] = ((ubrojac/10 % 10) >> i) & 1;
		}
		for(int i=4; i < 8; i++)
		{
			pio.led_unpacked[i] = ((brojac % 10) >> (i-4)) & 1;
		}
		
		// 7seg counter
		digits_p32[SEGM_0] = ubrojac % 10;
		digits_p32[SEGM_1] = ubrojac/10 % 10;
		digits_p32[SEGM_2] = brojac % 10;
		digits_p32[SEGM_3] = brojac/10 % 10;	
		
		
	}

	return 0;
}