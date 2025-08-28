#include <Arduino.h>

#include "driver/timer.h"
#include "freertos/FreeRTOS.h" // Queue, Task 등 RTOS 사용 시
#include "esp_err.h"           // ESP_ERROR_CHECK 등 매크로
#include "freertos/queue.h"    // QueueHandle_t 정의
#include "freertos/task.h"

#include "task.h"
#include "define.h"

volatile unsigned int task_1ms = 0;
volatile unsigned int delay_1ms = 0;
volatile unsigned int task_300ms = 0;
volatile boolean flag_1ms = false;

volatile boolean playSongFlag = false;
volatile unsigned int song_idx = 0;
String currentSong;

void do_every_1ms()
{
    int c = Serial2.read();
    if (c == 'P')
    {
        playSongFlag = true;
        Serial.println("알람시작");

        song_idx = 0;
    }

    if (c == 'S')
    {
        playSongFlag = false;
        Serial.println("멈췃소");
        song_idx = 0;
    }

    while (Serial2.available() > 0)
    {
        Serial2.read();
    }

    return;
}

void delay_ms(unsigned int ms)
{
    delay_1ms = 0;
    while (delay_1ms < ms)
        ;

    return;
}

void task()
{
    do_every_1ms();

    if (++task_300ms >= 300) // 300ms task
    {
        if (playSongFlag == true)
        {

            if (currentSong.length() == 0)
            {
                playSongFlag = false;
                return;
            }

            Serial.println(currentSong[song_idx]);
            Serial2.write(currentSong[song_idx++]);

            if (song_idx >= currentSong.length())
            {
                Serial.print("끝");
                Serial.println(currentSong.length());

                song_idx = 0;
            }
        }

        task_300ms = 0;
    }
}

void IRAM_ATTR isr_task(void *param)
{

    timer_group_clr_intr_status_in_isr(TIMER_GROUP_0, TIMER_0); // Clear timer 인터럽트 상태 레지스터(ISR)
    timer_group_enable_alarm_in_isr(TIMER_GROUP_0, TIMER_0);    // 인터럽트 사용

    task_1ms++;
    delay_1ms++;
    flag_1ms = true;
}

void set_timer()
{
    timer_config_t timer_config = {};
    timer_config.divider = 80;
    timer_config.counter_dir = TIMER_COUNT_UP;      // 시간이 올라감
    timer_config.counter_en = TIMER_PAUSE;          // timer 초기화 후 멈춘 상태에서 시작 -> timer_start해서 수동 시작해야 함
    timer_config.alarm_en = TIMER_ALARM_EN;         // 인터럽트 활성화 ( 지정해 준 alarm 값에서 인터럽트 발생 )
    timer_config.auto_reload = TIMER_AUTORELOAD_EN; // 자동 재시작

    ESP_ERROR_CHECK(timer_init(TIMER_GROUP_0, TIMER_0, &timer_config));
    ESP_ERROR_CHECK(timer_set_counter_value(TIMER_GROUP_0, TIMER_0, 0x00000000ULL));
    ESP_ERROR_CHECK(timer_set_alarm_value(TIMER_GROUP_0, TIMER_0, 1000)); // 1ms (1MHz에서 1000)
    ESP_ERROR_CHECK(timer_enable_intr(TIMER_GROUP_0, TIMER_0));
    ESP_ERROR_CHECK(timer_isr_register(TIMER_GROUP_0, TIMER_0, isr_task, (void *)TIMER_0, ESP_INTR_FLAG_IRAM, NULL));
    ESP_ERROR_CHECK(timer_start(TIMER_GROUP_0, TIMER_0));
}
