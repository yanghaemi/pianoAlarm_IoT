#ifndef TASK_H_
#define TASK_H_

unsigned volatile int task_1ms;

unsigned int ultrasound_flag;

volatile unsigned int flag_1ms;

volatile unsigned int current_sec;
volatile unsigned int current_min;
volatile unsigned int current_hour;

volatile unsigned int alarm_sec;
volatile unsigned int alarm_min;
volatile unsigned int alarm_hour;

volatile uint8_t alarm_mode;

volatile uint16_t t_duration;


void task_init();

void task_ms();


#endif