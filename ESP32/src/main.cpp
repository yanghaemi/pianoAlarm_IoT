// #include <WiFi.h>
// #include <SPI.h>
#include <stdio.h>
#include <string.h>
#include <Arduino.h>
#include "piano_wifi.h"
#include "task.h"
#include "define.h"

void setup()
{

  Serial.begin(115200);                        // serial 통신 시작
  Serial2.begin(9600, SERIAL_8N1, IO17, IO16); // JKIT 통신

  wifi_init();

  set_timer();

  *(volatile uint32_t *)(0x3FF44024) = (1 << 2); // GPIO 2 OUTPUT으로 설정
}

void loop()
{

  wifi_loop();

  if (flag_1ms)
  {
    flag_1ms = false;
    task();
  }
}
