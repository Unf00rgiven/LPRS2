
///////////////////////////////////////////////////////////////////////////////

// Product of Serbia.

///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdint.h>

///////////////////////////////////////////////////////////////////////////////
// Macros.

#define IN 0
#define OUT 1
#define IN_WIDTH 32
#define OUT_WIDTH 32


#define SW_WIDTH 4
#define SW_LSB 0
#define SW0 0
#define SW1 1
#define SW_MSB 1

#define PB_WIDTH 4
#define PB_LSB 2
#define PB0 2
#define PB1 3
#define PB_MSB 3

#define LED_WIDTH 4
#define LED_LSB 0
#define LED0 0
#define LED1 1
#define LED2 2
#define LED3 3
#define LED_MSB 3

//byte position of SEGM
#define SEGM_0 6
#define SEGM_1 7

#define SEGM_WIDTH 8
#define SEGM0_LSB 16
#define SEGM0_MSB 23
#define SEGM1_LSB 24
#define SEGM1_MSB 31

///////////////////////////////////////////////////////////////////////////////
// Structs.

typedef struct {
	// reg 0
	uint32_t in;
	// reg 1
	uint32_t out;
} pio_bf_regs;

typedef struct {
	// reg 0
	struct {
		unsigned sw0 :  1;
		unsigned sw1 :  1;
		unsigned pb0 :  1;
		unsigned pb1 :  1;
		unsigned     : 28;
	} in;
	// reg 1
	struct {
		unsigned led0  :  1;
		unsigned led1  :  1;
		unsigned led2  :  1;
		unsigned led3  :  1;
		unsigned       : 12;
		uint8_t segm[2];
	} out;
} pio_bf_arrays;

typedef struct {
	// reg 0
	union {
		uint32_t r;
		struct {
			unsigned sw    :  2;
			unsigned pb    :  2;
			unsigned       : 28;
		} s;
		struct {
			unsigned sw0 :  1;
			unsigned sw1 :  1;
			unsigned pb0 :  1;
			unsigned pb1 :  1;
			unsigned     : 28;
		} b;
	} in;
	// reg 1
	union {
		uint32_t r;
		struct {
			unsigned led   :  4;
			unsigned       : 12;
			uint8_t segm[2];
		} s;
		struct {
			unsigned led0    :  1;
			unsigned led1    :  1;
			unsigned led2    :  1;
			unsigned led3    :  1;
			unsigned         : 12;
			unsigned segm0_0 :  1;
			unsigned segm0_1 :  1;
			unsigned segm0_2 :  1;
			unsigned segm0_3 :  1;
			unsigned segm0_4 :  1;
			unsigned segm0_5 :  1;
			unsigned segm0_6 :  1;
			unsigned segm0_7 :  1;
			unsigned segm1_0 :  1;
			unsigned segm1_1 :  1;
			unsigned segm1_2 :  1;
			unsigned segm1_3 :  1;
			unsigned segm1_4 :  1;
			unsigned segm1_5 :  1;
			unsigned segm1_6 :  1;
			unsigned segm1_7 :  1;
		} b;
	} out;
} pio_bf_unions;

///////////////////////////////////////////////////////////////////////////////

uint32_t pio_mem[2];

///////////////////////////////////////////////////////////////////////////////

int main(void) {
	printf("-------------------\n\n");
	
	volatile uint32_t* p32 = (volatile uint32_t*)pio_mem;
	volatile uint8_t*  p8  = (volatile uint8_t*) pio_mem;
	
	// SW0 and PB0 are set. SW1 and PB1 are cleared.
	p32[0] = 0x00000005;
	// LED0 and LED3 are set, while LED1 and LED2 are cleared.
	p32[1] = 0b1001;
	
	//printf("SW0 = %d\n", p32[0] & 1); // Query SW0.
	printf("SW0 = %d\n", p32[0] & 0b0001); // Query SW0.
	//printf("PB0 = %d\n", p32[0] & 0x00000004); // Query PB0. Wrong!
	//printf("PB0 = %d\n", (p32[0] & 0x00000004) >> 2); // Correct.

	//check query PB0:
	if((p32[0] & (1 << 2)))
	{
		printf("PB0 = 1\n");
	}
	else
	{
		printf("PB0 = 0\n");
	}

	//printf("PB0 = %d\n", p32[0]>>2 & 1); // Nicer.
	printf("\n");
	
	//printf("OUT = 0x%x\n", p32[1]);
	
	printf("OUT = 0x%08x\n", p32[1]); // Nicer.

	// Extract LED.
	printf("LED 0 = 0x%x\n", p32[1] & 0xf); 
	p32[1] |= 0b0010; // Set LED1.
	printf("LED 1 = 0x%x\n", p32[1] & 0xf);
	p32[1] &= ~0x1; // Clear LED0.
	printf("LED 2 = 0x%x\n", p32[1] & 0xf);
	p32[1] &= ~0b1000; // Clear LED3. Nicer with inverted mask.
	printf("LED 3 = 0x%x\n", p32[1] & 0xf);
	printf("\n");

	printf("LED1 = %d\n", p32[1]>>1 & 1); // Query LED1.
	p32[1] ^= 0b0010; // Toggle LED1.
	printf("LED1 = %d\n", p32[1]>>1 & 1);
	p32[1] ^= 0b0010; // Toggle LED1.
	printf("LED1 = %d\n", p32[1]>>1 & 1);
	printf("\n");
	
	//Set and clear LED using macros defined
	printf("LED = 0x%x\n", p32[OUT] & 0xf);
	p32[OUT] |= 1<<3; // Set LED3.
	printf("LED = 0x%x\n", p32[OUT] & 0xf);
	p32[OUT] |= 1<<LED0; // Set LED0. Using macro.
	printf("LED = 0x%x\n", p32[OUT] & 0xf); // Using macro.
	p32[OUT] |= 1<<LED2 | 1<<LED1; // Set LED1 and LED2.
	printf("LED = 0x%x\n", p32[OUT] & 0xf);
	p32[OUT] &= ~(1<<LED3 | 1<<LED2); // Clear LED2 and LED3.
	printf("LED = 0x%x\n", p32[OUT] & 0xf);
	printf("\n");
	
	printf("OUT = 0x%08x\n", p32[OUT]);
	p8[SEGM_0] = 0x10; // Assign SEGM0 over bytes.
	p8[SEGM_1] = 0x32; // Assign SEGM1 over bytes.
	printf("OUT = 0x%08x\n", p32[OUT]); // LED[3:0] was not changed.
	printf(
		"SEGM0 = 0x%02x\n",
		p32[OUT]>>16 & ((1 << 8)-1)
	); // Extract SEGM0.
	printf(
		"SEGM1 = 0x%02x\n",
		p32[OUT]>>SEGM1_LSB & ((1 << SEGM_WIDTH)-1)
	); // Extract SEGM1.
	printf(
		"SEGM[1:0] = 0x%04x\n", 
		p32[OUT]>>SEGM0_LSB & ((1 << SEGM_WIDTH+SEGM_WIDTH)-1)
	); // Extract Both.
	printf("\n");
	
	// Masking.
	printf("OUT = 0x%08x\n", p32[OUT]); // Whole OUT reg.
	printf("SEGM0 = 0x%02x\n", p32[OUT]>>SEGM0_LSB & 0xff); // Extract SEGM0.
	p32[OUT] |= 0x20<<SEGM0_LSB; // Assign only to SEGM0, do not touch others.
	printf("OUT = 0x%08x\n", p32[OUT]); // Wrong!
	printf("SEGM0 = 0x%02x\n", p32[OUT]>>SEGM0_LSB & 0xff);
	p32[OUT] &= ~(0xff<<SEGM0_LSB); // Clear space with mask.
	printf("OUT = 0x%08x\n", p32[OUT]);
	p32[OUT] |= 0x20<<SEGM0_LSB; // Now can be assigned.
	printf("OUT = 0x%08x\n", p32[OUT]);
	printf("SEGM0 = 0x%02x\n", p32[OUT]>>SEGM0_LSB & 0xff);
	printf("\n");
	printf("-------------------\n\n");
	
	volatile pio_bf_regs*   pr = (volatile pio_bf_regs*)  pio_mem;
	volatile pio_bf_arrays* pa = (volatile pio_bf_arrays*)pio_mem;
	volatile pio_bf_unions* pu = (volatile pio_bf_unions*)pio_mem;
	
	p32[IN] = 0x00000005; // Old.
	pr->out = 0b1001; // New.
	
	// Could mix multiple methods. All pointers point to the same memory.
	printf("SW0 = %d\n", pa->in.sw0); // Query SW0.
	printf("SW0 = %d\n", pu->in.b.sw0); // Query SW0.
	printf("PB0 = %d\n", p32[0] & 0x00000004); // Old.
	printf("PB0 = %d\n", (pr->in & 0x00000004) >> 2); // New.
	printf("PB0 = %d\n", pu->in.r>>2 & 1); // Union.
	printf("\n");
	
	printf("OUT = 0x%x\n", p32[OUT]); // Old.
	printf("OUT = 0x%08x\n", pr->out); // New.
	printf("LED = 0x%x\n", p32[OUT] & 0xf); // Old.
	pa->out.led1 = 1; // Set LED1.
	printf("LED = 0x%x\n", pr->out & 0xf); // New.
	pu->out.b.led0 = 0; // Clear LED0.
	printf("LED = 0x%x\n", pu->out.r & 0xf);
	pu->out.b.led3 = 0; // Clear LED3.
	printf(
		"LED = 0x%x\n", 
		pa->out.led3<<3 | pa->out.led2<<2 | 
			pa->out.led1<<1 | pa->out.led0
	); // Nightmare. Need concatenation.
	printf("\n");
	
	printf("LED1 = %d\n", p32[1]>>1 & 1); // Old.
	pu->out.b.led1 ^= 1; // Toggle LED1.
	printf("LED1 = %d\n", pu->out.b.led1); // New.
	pu->out.b.led1 = !pu->out.b.led1; // Nicer.
	printf("LED1 = %d\n", pu->out.b.led1);
	printf("\n");
	
	printf("LED = 0x%x\n", pu->out.s.led);
	pa->out.led3 = 1; // New way.
	printf("LED = 0x%x\n", pu->out.s.led);
	pa->out.led0 = 1;
	printf("LED = 0x%x\n", pu->out.s.led);
	pa->out.led2 = 1; pa->out.led1 = 1; // Could be wrong. Two access now.
	printf("LED = 0x%x\n", pu->out.s.led);
	pa->out.led3 = 0; pa->out.led2 = 0; // Also could be wrong.
	printf("LED = 0x%x\n", pu->out.s.led);
	printf("\n");
	
	printf("OUT = 0x%08x\n", pr->out);
	p8[6] = 0x10; // Old.
	pa->out.segm[1] = 0x32; // New.
	printf("OUT = 0x%08x\n", pu->out.r); // LED[3:0] was not changed.
	printf("SEGM0 = 0x%02x\n", pa->out.segm[0]);
	printf("SEGM1 = 0x%02x\n", pu->out.s.segm[1]);
	printf(
		"SEGM[1:0] = 0x%04x\n", 
		pa->out.segm[1]<<8 | pa->out.segm[0]
	); // Concatenation.
	printf("\n");
	
	// Masking.
	printf("OUT = 0x%08x\n", pr->out); // Whole OUT reg.
	printf("SEGM0 = 0x%02x\n", pa->out.segm[0]); // Extract SEGM0.
	pu->out.s.segm[0] = 0x20; // Much easier than masking.
	printf("OUT = 0x%08x\n", pr->out);
	printf("SEGM0 = 0x%02x\n", pa->out.segm[0]);
	printf("\n");
	printf("-------------------\n\n");
	
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
