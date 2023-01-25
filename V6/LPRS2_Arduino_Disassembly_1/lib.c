
#include <util/delay.h>


#include "lib.h"



void delay_ms(uint8_t ms) {
	while(ms--){
		_delay_ms(1);
	}
}


uint8_t calc1(uint8_t x, uint8_t y) {
	return x + y;
}

uint16_t calc2(uint16_t x, uint16_t y) {
	return x + y;
}

