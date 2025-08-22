// #include <WiFi.h>
// #include <SPI.h>
#include <stdio.h>
#include <string>
#include <Arduino.h>
#include "piano_wifi.h"
#include "uart.h"
#include "task.h"

void setup()
{

  Serial.begin(115200); // serial 통신 시작

  wifi_init();

  install_uart_driver();
  set_communication_parameters();
  set_communication_pins();

  // pinMode(R_LED, OUTPUT);

  set_timer();
}

void loop()
{

  wifi_loop();

  if (task_1ms % 1000 == 0)
  {
    uart_send('D');
  }

  uart_receive();

  if (data[0] == 'c' || data[0] == 'C')
  {
    // digitalWrite(R_LED, HIGH);
  }
  if (data[0] == 'd' || data[0] == 'D')
  {
    // digitalWrite(R_LED, LOW);
  }
}
