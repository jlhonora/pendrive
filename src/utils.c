/** \file utils.c
 *  \brief Different utils used throughout the program
 */
#include <stdlib.h>
#include <stdint.h>
#include <msp430.h>
#include <string.h>
#include <stdio.h>
#include "utils.h"

extern void USB_disconnect(void);
extern void USB_disable(void);

/** Jumps into BSL routine */
__attribute__((critical)) void msp_reset_to_bsl(void) {
    // Disable USB connections
    USB_disconnect();
    USB_disable();
    // Unprotect BSl section
    SYSBSLC &= ~(SYSBSLPE);
    // Jump to BSL area
    ((void (*)())0x1000)(); 
}

int putchar(int c) {
    uart_a1_send_char((uint8_t) c);
    return 1;
}
