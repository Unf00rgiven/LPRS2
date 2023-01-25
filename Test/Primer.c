

#include <stdint.h>
#include "system.h"
#include <stdio.h>
#include <stdbool.h>
#include "sys/alt_irq.h"


#define WAIT_UNITL_TRUE(x) while(!(x)){} 

#define pio_p32 ((volatile uint32_t*)PIO_BASE)
#define digits_p32 ((volatile uint32_t*)DIGITS_BASE)
#define timer_p32 ((volatile uint32_t*)TIMER_BASE )


#define LED_PACKED		16
#define SW_PACKED		17

#define LED0			0
#define SW0			8

#define SEGM_0			0
#define SEGM_1			1
#define SEGM_2			2
#define SEGM_3			3
#define SEGM_PACK		4

# define TIMER_CNT		0
# define TIMER_MODULO		1
# define TIMER_CTRL_STATUS	2
# define TIMER_MAGIC		3
# define TIMER_RESET_FLAG	0
# define TIMER_PAUSE_FLAG	1
# define TIMER_WRAP_FLAG	2
# define TIMER_WRAPPED_FLAG	3
# define TIMER_RESET		(TIMER_RESET_FLAG + 4)
# define TIMER_PAUSE		(TIMER_PAUSE_FLAG + 4)
# define TIMER_WRAP		(TIMER_WRAP_FLAG + 4)
# define TIMER_WRAPPED		(TIMER_WRAPPED_FLAG + 4)


enum DIRECTION{LEFT, RIGHT};
static volatile bool direction = LEFT;
static volatile int red_jed = 0;
static volatile int red_des = 0;
static volatile int green_jed = 0;
static volatile int green_des = 0;
static void timer_isr ( void * context ) {
	if(direction == LEFT){
		if(pio_p32[LED0+7]){
			direction = RIGHT;
			red_jed++;
			if(red_jed > 9){
				red_jed = 0;
				red_des++;
			}
		} else {
			pio_p32[LED_PACKED] = pio_p32[LED_PACKED] << 1;
		}
	} else if(direction == RIGHT){
		if(pio_p32[LED0]){
			direction = LEFT;
			green_jed++;
			if(green_jed > 9){
				green_jed = 0;
				green_des++;
			}
		} else {
			pio_p32[LED_PACKED] = pio_p32[LED_PACKED] >> 1;
		}
	}
	
}


int main() {

	digits_p32[SEGM_PACK] = 0;
	
	timer_p32[TIMER_MODULO] = 12000000;
	timer_p32[TIMER_CTRL_STATUS] = 0;
	
	//bool direction = LEFT;
	//int red_jed = 0, green_jed = 0, red_des = 0, green_des = 0;

	pio_p32[LED_PACKED] = 0x01;
	
	alt_ic_isr_register (
		TIMER_IRQ_INTERRUPT_CONTROLLER_ID , // alt_u32 ic_id
		TIMER_IRQ , // alt_u32 irq
		timer_isr , // alt_isr_func isr
		NULL , // void * isr_context
		NULL // void * flags
	);

	while(1){
	
		int interval = pio_p32[SW_PACKED] & 0x03;
		int rst = pio_p32[SW0+7];
		//int kod = pio_p32[SW_PACKED] & 0x03;
		
		//WAIT_UNITL_TRUE(timer_p32[TIMER_WRAPPED]) ;
		//timer_p32[TIMER_WRAPPED] = 0;
		
		switch(pio_p32[SW_PACKED] & 0x03){
			
			case 0:
				timer_p32[TIMER_MODULO] = 2*12000000;
				break;
			case 1:
				timer_p32[TIMER_MODULO] = 12000000;
				break;
			case 2:
				timer_p32[TIMER_MODULO] = 12000000/2;
				break;
			case 3:
				timer_p32[TIMER_MODULO] = 12000000/4;
				break;
		}
		
		if(rst){
			pio_p32[LED_PACKED] = 0x80;
			digits_p32[SEGM_PACK] = 0;
			red_des = 0;
			red_jed = 0;
			green_des = 0;
			green_jed = 0;
			direction = RIGHT;
		} else {
			/*if(direction == LEFT){
				if(pio_p32[LED0+7]){
					direction = RIGHT;
					red_jed++;
					if(red_jed > 9){
						red_jed = 0;
						red_des++;
					}
				} else {
					pio_p32[LED_PACKED] = pio_p32[LED_PACKED] << 1;
				}
			} else if(direction == RIGHT){
				if(pio_p32[LED0]){
					direction = LEFT;
					green_jed++;
					if(green_jed > 9){
						green_jed = 0;
						green_des++;
					}
				} else {
					pio_p32[LED_PACKED] = pio_p32[LED_PACKED] >> 1;
				}
			}*/
		}
		
		digits_p32[SEGM_3] = red_des;
		digits_p32[SEGM_2] = red_jed;
		digits_p32[SEGM_1] = green_des;
		digits_p32[SEGM_0] = green_jed;
		
		
	
	}

	return 0;
}
