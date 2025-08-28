#ifndef TASK_H_
#define TASK_H_

extern volatile unsigned int task_1ms;
extern volatile boolean flag_1ms;

extern String currentSong;
extern volatile unsigned int song_idx;
extern volatile boolean playSongFlag;

void delay_ms(unsigned int ms);

void IRAM_ATTR isr_task(void *param); // 타이머 인터럽트
void set_timer();                     // timer init
void task();

#endif
