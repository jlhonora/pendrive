/** \file uart.c 
 * \brief UART initialization and handling */
#include "uart.h"
#include "utils.h"

#if defined (__MSP430_HAS_USCI_A0__) || defined (__MSP430_HAS_USCI_AB0__)

void uart_a0_with_xt2() {
	UCA0CTL1 |= UCSWRST;
	UCA0CTL1 |= UCSSEL__SMCLK;
	UCA0BR0 = 625 & 0x00FF;
	UCA0BR1 = (625 & 0xFF00) >> 8;                            
	UCA0CTL1 &= ~UCSWRST;
	UCA0IE |= UCRXIE;
}

void uart_a0_enable() {
	UCA0CTL1 |= UCSWRST;
	UCA0CTL1 |= UCSSEL__SMCLK;
	UCA0BR0 = 47; // 104 for 9600, 51 (or 52) for 19200 (Iridium), 26 for 38400                             
	UCA0BR1 = 0;                            
	UCA0MCTL = UCBRS0;               
	UCA0CTL1 &= ~UCSWRST;
	UCA0IE |= UCRXIE;
}

void uart_a0_disable() {
	UCA0IE &= ~UCRXIE;
}

__attribute__((critical)) void uart_a0_send_char(uint8_t character) {
	while((UCA0IFG & UCTXIFG) == 0);
	UCA0TXBUF = character;
}

void uart_a0_transmit_data(uint16_t length, uint8_t *tx_data) {
	while (length--)
		uart_a0_send_char(*tx_data++);
}

interrupt(USCI_A0_VECTOR) USCIA0_ISR(void) {
	if(UCA0IFG & UCRXIFG) {
		uart_a0_rx_interrupt();
	}
}

#endif

#if defined (__MSP430_HAS_USCI_A1__) || defined (__MSP430_HAS_USCI_AB1__)
void uart_a1_with_xt2() {
    P4MAP4 = PM_UCA1TXD;
    P4MAP5 = PM_UCA1RXD;
	UCA1CTL0 = 0x00;
	UCA1CTL1 = UCSSEL__SMCLK | UCSWRST;          // Use SMCLK, keep RESET
	UCA1BR0 = BAUD_RATE_9600 & 0x00FF; // At 12 MHz, 1250 for 9600
	UCA1BR1 = (BAUD_RATE_9600 & 0xFF00) >> 8;                            
	UCA1CTL1 &= ~UCSWRST;
	UCA1IE |= UCRXIE;
}

void uart_a1_change_baud(uint16_t br_const) {
	UCA1CTL1 |= UCSWRST;
	UCA1CTL1 |= UCSSEL__SMCLK; // SMCLK as source
	UCA1BR0 = br_const & 0x00FF; // At 12 MHz, 1250 for 9600
	UCA1BR1 = (br_const & 0xFF00) >> 8;                            
	UCA1CTL1 &= ~UCSWRST;
	UCA1IE |= UCRXIE;
}

void uart_a1_enable() {
	UCA1CTL1 |= UCSWRST;
	UCA1CTL1 |= UCSSEL__SMCLK;
	UCA1BR0 = 94; // 104 for 9600, 52 for 19200 (Iridium), 26 for 38400 @12MHz
	UCA1BR1 = 0;                            
	UCA1MCTL = UCBRS0;               
	UCA1CTL1 &= ~UCSWRST;
	UCA1IE |= UCRXIE;
}

void uart_a1_disable() {
	UCA1IE &= ~UCRXIE;
}

__attribute__((critical)) void uart_a1_send_char(uint8_t character) {
	while((UCA1IFG & UCTXIFG) == 0);
	UCA1TXBUF = character;
}

void uart_a1_transmit_data(uint16_t length, uint8_t *tx_data) {
	while (length--)
		uart_a1_send_char(*tx_data++);
}

interrupt(USCI_A1_VECTOR) USCIA1_ISR(void) {
	if(UCA1IFG & UCRXIFG) {
		uart_a1_rx_interrupt();
	}
}
#endif

void uart_a0_process_rx(uint8_t byte) {
    return;
}

void uart_a1_process_rx(uint8_t byte) {
    return;
}

void uart_a0_rx_interrupt() {
	uart_a0_process_rx(UCA0RXBUF);
}

void uart_a1_rx_interrupt() {
	uart_a1_process_rx(UCA1RXBUF);
}
