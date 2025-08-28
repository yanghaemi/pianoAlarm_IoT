#include <Arduino.h>
#include "task.h"

void setAlarmTime(int hour, int min)
{
    Serial2.write('Q');
    delay_ms(10);
    Serial2.write(hour);
    delay_ms(10);
    Serial2.write(min);

    return;
}

void setCurrentTime(int hour, int min)
{
    Serial2.write('T');
    delay_ms(10);
    Serial2.write(hour);
    delay_ms(10);
    Serial2.write(min);

    return;
}

void playSong(String song)
{
    Serial2.write("p");
    delay_ms(10);

    playSongFlag = true;
    song_idx = 0;

    return;
}