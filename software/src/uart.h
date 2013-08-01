#include <msp430.h>
#include <stdint.h>

#ifndef _MSP430_UART
#define _MSP430_UART

#define BAUD_RATE_4800 2500
#define BAUD_RATE_9600 1250
#define BAUD_RATE_19200 650

#if defined (__MSP430_HAS_USCI_A0__) || defined (__MSP430_HAS_USCI_AB0__)

//UART
#define UART_A0_SEL   P3SEL
#define UART_A0_DIR   P3DIR
#define UART_A0_TXD   BIT3
#define UART_A0_RXD   BIT4

void uart_a0_with_xt2();
void uart_a0_enable();
void uart_a0_disable();
void uart_a0_send_char(uint8_t character);
void uart_a0_transmit_data(uint16_t length, uint8_t *tx_data);
void uart_a0_rx_interrupt();
void uart_a0_process_rx(uint8_t rx_byte);

#endif //USCI_AB0

#if defined (__MSP430_HAS_USCI_A1__) || defined (__MSP430_HAS_USCI_AB1__)

#define UART_A1_TXD   BIT4
#define UART_A1_RXD   BIT5
#define UART_A1_SEL   P4SEL
#define UART_A1_DIR   P4DIR

void uart_a1_with_xt2();
void uart_a1_enable();
void uart_a1_disable();
void uart_a1_send_char(uint8_t character);
void uart_a1_transmit_data(uint16_t length, uint8_t *tx_data);
void uart_a1_rx_interrupt();
void uart_a1_process_rx(uint8_t rx_byte);
void uart_a1_change_baud(uint16_t br_const);

#endif //USCI_AB1

#endif //_MSP430_UART
