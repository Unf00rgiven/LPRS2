#include <stdio.h>
#include <stdint.h>

// paljenje/gasenje dioda


int main()
{
	uint32_t pio_mem = 0b1001; // dioda 3 i dioda 0 upaljene na pocetku
	int n,i,p;
	
	printf("Unesite 0 diode info, 1 za paljenje gasenje diode:");
	scanf("%d", &n);

	switch(n)
	{
		case 0:
			for(i = 0 ; i < 4 ; i++)
			{
				if(pio_mem & (1<<i)) printf("%d. dioda ON\n", i);
				else printf("%d. dioda OFF\n", i);
			}
			break;
		case 1:
			for(i = 0 ; i < 4 ; i++)
			{
				printf("%d. dioda: 1 ON, 0 OFF\n", i);
				scanf("%d", &p);
				
				if(p == 1)
				{
					pio_mem |= 1 << i;	
				} 
				if(p == 0)
				{
					pio_mem &= ~1 << i;
				}
			}
			break;
		default:
			printf("Greska pri izboru");
			break;
	}
	

	return 0;
}
