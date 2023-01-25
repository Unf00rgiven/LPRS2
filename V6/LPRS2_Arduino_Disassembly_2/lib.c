
#include "lib.h"

#include <avr/io.h>

typedef struct {
	unsigned b0 : 1;
	unsigned b1 : 1;
	unsigned b2 : 1;
	unsigned b3 : 1;
	unsigned b4 : 1;
	unsigned b5 : 1;
	unsigned b6 : 1;
	unsigned b7 : 1;
} bf_8b;

#define portb (*((volatile bf_8b*)(&PORTB)))

// Does not work.
//#define likely(x)       __builtin_expect((x),1)
//#define unlikely(x)     __builtin_expect((x),0)

void pipeline_test_1(uint16_t c) {
	
	
	
	
	
	
	uint16_t i = 1000;
	do{
		
		
		
		
		if(i == c){
			
			
			
			
			portb.b1 = 0;
			
			
			
			
			portb.b1 = 1;
			
			
			
			
		}else{
			
			
			
			
			portb.b2 = 0;
			
			
			
			
			portb.b2 = 1;
			
			
			
			
		}
		
		
		
		
	}while(i--);
	
	
	
	return;
}
