// 1ms 태스크
#include <avr/io.h>
#include <avr/interrupt.h>

#include "define.h"
#include "ADC.h"
#include "uart.h"
#include "timer1.h"
#include "External_Interrupt.h"
#include "task.h"
#include "FND.h"

unsigned volatile int task_1ms = 0; // 1ms
unsigned volatile int task_2ms = 0; // 2ms
unsigned volatile int task_300ms = 0; // 300ms
unsigned volatile int task_1s = 0; // 10ms

unsigned volatile int task_digit = 0;

volatile unsigned int flag_1ms =0;

volatile unsigned int fnd_i = 0;

volatile unsigned int current_sec = 0;
volatile unsigned int current_min = 0;
volatile unsigned int current_hour = 0;

volatile unsigned int alarm_sec = 0;
volatile unsigned int alarm_min = 0;
volatile unsigned int alarm_hour = 0;

volatile uint8_t alarm_mode = FALSE;

volatile uint16_t t_duration = 0;


unsigned int ultrasound_flag= 0;





void task_init()
{
	TCCR0 |= (1 << WGM01);	// CTC 모드
	
	TCCR0 |= (1 << CS02);  // 64 분주
	OCR0 = 249; // F_CPU / 64 / 1000 - 1 -> 1ms
	
	TIMSK |= (1 << OCIE0);
	
	TCCR2 |= (1<<WGM01);	// CTC 모드
	
	TCCR2 |= (1<<CS01);		// 8 분주
	OCR2 = 19; // 10us
	
	TIMSK |= (1<< OCIE2);
	
}

void timer2_task_init(){
	TCCR2 |= (1<<WGM01);	// CTC 모드
	
	TCCR2 |= (1<<CS01);		// 8 분주
	OCR2 = 19; // 10us
	
	TIMSK |= (1<< OCIE2); 
}

void play_note()
{
	if (play_mode == 1)
	{
		if ((song_idx >= 0) && (song_idx < song_len))
		{
			pitch = song_pointer[song_idx];
			set_pitch();
			set_pwm_duty(volume);
		}
		else
		{
			play_mode = 0;
			pre_volume = volume;

			set_pwm_duty(0);

			song_pointer = 0; // 끝나서 리셋
			song_len = 0;
			song_idx = 0;
		}
	}
}


void do_every_1ms()	
{
	// 모드 확인용 led 점등---------------------------------------------
	if (play_mode)
		PORTA |= 0x20;
	else
		PORTA &= ~0x20;

	if (volume_onoff_mode)
		PORTA |= 0x40;
	else
		PORTA &= ~0X40;
	//----------------------------------------------------------------
	// 1ms 마다 uart receive하기
	
	char recv;
	if (uart_receive(&recv))
	{
		change_pitch(recv);
	}
	//----------------------------------------------------------------
	
	if (adc_volume_mode == 1)
	{
		unsigned short value = ADC_read();

		uint16_t duty = (uint32_t)value * 100 / 1023;

		set_pwm_duty(duty);
	} // -------------------------------
	
	else
	{
		set_pwm_duty(volume);
	}
}

void do_every_2ms(){
	
	
	if (adc_volume_mode == 1)
	{
		uart_send(adc_high);
		uart_send(adc_low);
	}
	
	if(fnd_i>=0 && fnd_i<4)
		display_fnd(fnd_i++);
	else fnd_i=0;
	
}

void do_every_1s()
{	
	
	current_sec ++;
	if(current_sec >= 60){
		current_sec = 0;
		current_min++;	
	}
	if(current_min >= 60){
		current_min=0;
		current_hour++;
		
	}
	if(current_hour >= 24){
		current_hour = 0;
	}
	
	if(alarm_mode && current_hour == alarm_hour && current_min == alarm_min)
	{
		uart_send('P');
		ultrasound_flag = WAIT;
		alarm_mode = FALSE;	
	}
	set_fnd();
}

void US_sensor_trigger()	// 초음파 센서 트리거
{
	
	if(ultrasound_flag == WAIT) {
		ultrasound_flag = TRUE;
		PORTA |= (1<<4);
	}
	else if (ultrasound_flag){
		ultrasound_flag = FALSE;
		PORTA &= ~(1<<4);
	}
}

void task_ms(){	// ms 단위 태스크
	
	do_every_1ms();		// 1ms task		
	
	if (++task_2ms >= 2) { // 2ms task
		task_2ms = 0;
		do_every_2ms();
	}
	
	/*if (++task_300ms >= 300) // 300ms task
	{
		task_300ms = 0;
		
		if (play_mode == 1)
		{
			song_idx++;
			play_note();
		}
	}*/
	
	if (++task_1s >= 1000) // 1s task
	{ 
		task_1s = 0;
		do_every_1s();
		
		PORTA ^= 0X01;
	}

	/*if (task_10us >= 100000)
	{
		task_10us = 0;
		task_1ms = 0;
		task_1s = 0;
	}*/
}

ISR(TIMER0_COMP_vect)
{	
	task_1ms++;
	flag_1ms = 1;
}

ISR(TIMER2_COMP_vect){
	US_sensor_trigger();
	if (measuring) {
		t_duration++;
	}
}