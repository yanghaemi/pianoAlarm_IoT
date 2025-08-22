// #include <WiFi.h>
// #include <SPI.h>
#include <stdio.h>
#include <string.h>
#include <Arduino.h>
#include "piano_wifi.h"
#include "uart.h"
#include "task.h"
#include "define.h"

void setup()
{

  Serial.begin(115200); // serial 통신 시작
  Serial2.begin(9600, SERIAL_8N1, IO17, IO16);

  wifi_init();

  // install_uart_driver();
  // set_communication_parameters();
  // set_communication_pins();

  // pinMode(R_LED, OUTPUT);

  set_timer();
}

void loop()
{

  wifi_loop();

  // if (flag_1ms)
  // {
  //   flag_1ms = FALSE;
  //   task();
  // }

  if (task_1ms % 1000 == 0)
  {
    // uart_send('D');
    Serial2.write('D');
  }

  // uart_receive();

  // if (data[0] == 'c' || data[0] == 'C')
  // {
  //   // digitalWrite(R_LED, HIGH);
  // }
  // if (data[0] == 'd' || data[0] == 'D')
  // {
  //   // digitalWrite(R_LED, LOW);
  // }
}
