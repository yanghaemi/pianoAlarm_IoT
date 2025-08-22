
#ifndef UART_H_
#define UART_H_

// extern const uart_port_t uart_num;
// extern char data[128];

// void install_uart_driver();          // Allocating ESP32's resources for the UART driver
// void set_communication_parameters(); // Setting baud rate, data bits, stop bits, etc.
// void set_communication_pins();       // Assigning pins for connection to a device

// void uart_send(char c);
// void uart_receive();

void uart_send(char c);

#endif
