
#include <avr/io.h>
#include <avr/interrupt.h>
#include "define.h"
#include "timer1.h"
#include "External_Interrupt.h"
#include "task.h"
#include "uart.h"

volatile int duty_mode = 1;

volatile uint8_t  measuring = 0;

void ExternalInterrupt_init()
{
	EICRA |= (1<<ISC21) | (1<<ISC20);	// 상승 edge
	EICRB |= (1<<ISC41);
	EICRB |= (1 << ISC51);
	EIMSK |= (1 << INT5) | (1 << INT4) | (1<<INT2);
}


ISR(INT2_vect){
	
	
	uint8_t rising = (EICRA & (1<<ISC20)) != 0; // 11이면 rising, 10이면 falling

	if (rising) { // 상승엣지: 측정 시작
		t_duration = 0;
		measuring = 1;
		// 하강엣지로 변경
		
		EICRA = (EICRA & ~(1<<ISC20)) | (1<<ISC21); // 10: falling
	} 
	else{	// 하강엣지: 측정 종료
	
		measuring = 0;
		t_duration = ((uint32_t)(t_duration*10)/58);
		uart_send(t_duration>>8); uart_send(t_duration);
		// 다시 상승엣지로 변경
		EICRA |= (1<<ISC21)|(1<<ISC20); // 11: rising
		
		if(t_duration >= 6)
			ultrasound_flag = WAIT;	// 6cm보다 크면 다시 trigger
		else{
			play_mode = FALSE;
			alarm_mode = FALSE;
		
		}
	} 
}

ISR(INT4_vect){
	if(!alarm_mode) {
		alarm_mode = TRUE;
		PORTA |= 0X01;
	}
	else {
		alarm_mode = FALSE;
		PORTA &= ~0X01;
	}
}

ISR(INT5_vect)
{ // sw2
	switch (duty_mode)
	{ // timer1 pwm 듀티 조절
	case 1:
		//set_pwm_duty(80);
		volume = 80;
		pre_volume = volume;
		
		duty_mode = 2;
		PORTA = 0x02;
		break;
	case 2:
		//set_pwm_duty(100);
		volume = 100;
		pre_volume = volume;
		
		duty_mode = 1;
		PORTA = 0x01;
		break;
	}
}