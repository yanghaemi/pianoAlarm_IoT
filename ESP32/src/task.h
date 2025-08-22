#ifndef TASK_H_
#define TASK_H_

extern unsigned int task_1ms;

void IRAM_ATTR task(void *param); // 타이머 인터럽트
void set_timer();                 // timer init

#endif
