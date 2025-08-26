#include <avr/io.h>

#include "define.h"
#include "task.h"
#include "FND.h"
#include "External_Interrupt.h"


// 시계 관련 코드

unsigned int digit[10]= {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7c, 0x07, 0x7f, 0x67};
unsigned int fnd[4];


void FND_init(){
	DDRC = 0XFF;
	DDRG = 0X0F;
	
}

void display_fnd(unsigned int i){
	if(i < 4){
		PORTC = digit[fnd[i]];
		PORTG = 1<<i;
	}else return;
}

void set_fnd(){
	fnd[3] = (current_hour / 10) % 10;
	fnd[2] = (current_hour % 10 );
	fnd[1] = (current_min / 10) % 10;
	fnd[0] = (current_min % 10);
}