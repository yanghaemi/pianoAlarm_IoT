#include <Arduino.h>

#include "freertos/FreeRTOS.h" // Queue, Task 등 RTOS 사용 시
#include "esp_err.h"           // ESP_ERROR_CHECK 등 매크로
#include "freertos/queue.h"    // QueueHandle_t 정의
#include "freertos/task.h"

#include "uart.h"
#include "define.h"

// const uart_port_t uart_num = UART_NUM_1;

// char data[128] = {0}; // received data

// void install_uart_driver() // Allocating ESP32's resources for the UART driver
// {
//     const int uart_buffer_size = (1024 * 2);
//     QueueHandle_t uart_queue;
//     ESP_ERROR_CHECK(uart_driver_install(uart_num, uart_buffer_size, uart_buffer_size, 10, &uart_queue, 0));
// }

// void set_communication_parameters() // Setting baud rate, data bits, stop bits, etc.
// {
//     // 한 번에 매개변수 set
//     uart_config_t uart_config = {
//         .baud_rate = 9600,
//         .data_bits = UART_DATA_8_BITS,
//         .parity = UART_PARITY_ODD,
//         .stop_bits = UART_STOP_BITS_1,
//         .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
//         .rx_flow_ctrl_thresh = 0,
//     };

//     ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config));
// }

// void set_communication_pins() // Assigning pins for connection to a device
// {
//     // TXD = IO16, RXD = IO17, RTS = IO18, CTS = IO19
//     // RTS : 수신이 가능한 상태면 0, 불가능하면 1
//     // CTS : TX를 하기 전 CTS 입력을 확인해서 CTS가 0이면 데이터를 보낸다.
//     ESP_ERROR_CHECK(uart_set_pin(uart_num, IO16, IO17, IO18, IO19));
// }

// void uart_send(char c)
// {
//     // Write data to UART.
//     // char *test_str = "This is a test string.\r\n";
//     uart_write_bytes(uart_num, &c, 1);
// }

// void uart_receive()
// {
//     // uint8_t data[128];

//     int length = 0;
//     ESP_ERROR_CHECK(uart_get_buffered_data_len(uart_num, (size_t *)&length));
//     if (length > 0)
//     {
//         length = uart_read_bytes(uart_num, data, length, 100);
//         data[length] = '\0'; // 널 종료 (for 디버깅)
//     }
//     else
//     {
//         data[0] = '\0'; // 없을 땐 초기화
//     }
// }

void uart_init()
{
    Serial2.begin(96000, SERIAL_8N1, IO17, IO16);
}

void uart_send(char c)
{
    Serial2.write(c);
}