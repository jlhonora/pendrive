#ifndef __MSP430_UTILS
#define __MSP430_UTILS
/** \file utils.h
 *  \brief Different utils used throughout the program
 */
#include <msp430.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "wdt.h"
#ifdef _CDC_
#include "usb_printf.h"
#endif


/** For jumping into bsl section */
void            msp_reset_to_bsl(void);
#ifndef DEBUG
#include "uart.h"
#define DEBUG(...) do {printf(__VA_ARGS__);} while(0);
#endif

#ifndef interrupt
#define interrupt(x) void __attribute__((interrupt (x)))
#endif

#endif
