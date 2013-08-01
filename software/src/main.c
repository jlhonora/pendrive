/** 
 *  Build your own pendrive
 *  An open-source pendrive based on msp430
 *  microcontrollers.
 *
 *  To build the source code, just run:
 * 
 *  rake
 *
 *  To install it in the msp-exp430f5529 board:
 *  sudo mspdebug rf2500 "prog bin.elf"
 *
 *  This code was modified from the USB
 *  Developers Package available from the TI
 *  website. This code is provided as-is
 *  and comes with no warranty or support
 *  whatsoever. You are free to use it as long
 *  as you cite this work.
 *
 *  Enjoy!
 *
 *  Jose L. Honorato
 *  July, 2013
 */

#include <msp430.h>
#include <stdint.h>
#include "utils.h"

#define _MSC_

#ifdef _MSC_
#include "HAL_Board.h"
#include "F5xx_F6xx_Core_Lib/HAL_UCS.h"
#include "F5xx_F6xx_Core_Lib/HAL_PMM.h"
#include "MassStorage.h"
#endif

// Function declarations
void msp_init(void);
uint8_t events_available(void);
void process_events(void);

// To activate testing the rake command
// should be invoked like this:
// rake test=1
#ifdef TESTING
#include "test.h"
#endif

int main(void) {
    // If TESTING is defined at compile time then
    // we don't run the main application
    // This can be done by executing
    // rake test=1
#ifdef TESTING
    return test();
#endif
    // Initialize the different parts
    msp_init();

    DEBUG("Running\r\n");

    // Main loop
    while(1) {  
        // Check if there are events available. If there are
        // then we process them
        if(events_available()) {
            process_events();
        }
    }
}

/** Initializes modules and pins */
void msp_init(void) {
    // Stop the watchdog
    wdt_stop();
#ifdef _MSC_
    // Stop WDT
    WDTCTL = WDTPW + WDTHOLD;
    // Basic GPIO initialization
    Board_init();

    // Set Vcore to accomodate for max. allowed system speed
    SetVCore(3);

    // Use 32.768kHz XTAL as reference
    LFXT_Start(XT1DRIVE_0);

    // Set system clock to 12MHz
    Init_FLL_Settle(12000, 12000 / 32768);

    uart_a1_enable();
    uart_a1_change_baud(BAUD_RATE_19200);
    DEBUG("Testing Debug\r\n");

    SFRIFG1 = 0;

    // Globally enable interrupts
    __enable_interrupt();

    __delay_cycles(10000);
    DEBUG("Entering MassStorage\r\n")
    MassStorage();
#endif
    // Enable global interrupts
    __enable_interrupt();
    while(1);
}

/** Checks if there are events available to be processed
 * The currently supported events are:
 *   - none
 *
 *   These events are checked quite frequently because the main
 *   loop is constantly checking them. Anyhow, whenever we need to process
 *   a lengthy task, the main loop will cease to check for an event. 
 *   This doesn't mean that the events will be dropped, but they will 
 *   not be managed instantly.
 */
uint8_t events_available(void) {
    return (
        0
        );
}

/** Process then events queried in the \ref events_available() function */
void process_events(void) {
}
