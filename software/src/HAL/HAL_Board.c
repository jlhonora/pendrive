/*******************************************************************************
 *
 *  HAL_Board.c - General experimenter's board port initialization and control
 *
 *  Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************/

/***************************************************************************//**
 * @file       HAL_Board.c
 * @addtogroup HAL_Board
 * @{
 ******************************************************************************/
#include <msp430.h>
#include "HAL_Board.h"

#define XT1_XT2_PORT_DIR            P5DIR
#define XT1_XT2_PORT_OUT            P5OUT
#define XT1_XT2_PORT_SEL            P5SEL
#define XT1_ENABLE                  (BIT4 | BIT5)
#define XT2_ENABLE                  (BIT2 | BIT3)
#define LED145678_PORT_OUT          P1OUT

/***************************************************************************//**
 * @brief  Initialize the board - configure ports
 * @param  None
 * @return none
 ******************************************************************************/

void Board_init(void)
{
    // Setup XT1 and XT2
    XT1_XT2_PORT_SEL |= XT1_ENABLE | XT2_ENABLE;
    P5SEL |= 0x0C;

    // Configure button ports
    PADIR &= ~0x0480;               // Buttons on P1.7/P2.2 are inputs

    // Configure SDCard ports
    P3OUT |= BIT7;                  // SD_CS to high
    P3DIR |= BIT7;

    // Configure unused ports for low power
    P2OUT &= ~(BIT0 + BIT1 + BIT3 + BIT4 + BIT6);
    P2DIR |= BIT0 + BIT1 + BIT3 + BIT4 + BIT6;

    P3OUT &= ~(BIT0 + BIT1 + BIT2);
    P3DIR |= BIT0 + BIT1 + BIT2;

    P4OUT &= ~(BIT0 + BIT6 + BIT7);
    P4DIR |= BIT0 + BIT6 + BIT7;

    P5OUT &= ~BIT1;
    P5DIR |= BIT1;

    P6OUT &= ~(BIT6 + BIT7);
    P6DIR |= BIT6 + BIT7;

    P7OUT &= ~(BIT0 + BIT1 + BIT2 + BIT3 + BIT5 + BIT7);
    P7DIR |= BIT0 + BIT1 + BIT2 + BIT3 + BIT5 + BIT7;

    // LEDs
    P1SEL &= ~(BIT1 | BIT2 | BIT3 | BIT4 | BIT5);
    P1DIR |= (BIT1 | BIT2 | BIT3 | BIT4 | BIT5);
    P1OUT &= ~(BIT1 | BIT2 | BIT3 | BIT4 | BIT5);

    P4SEL |= BIT4 | BIT5; // UART
    P4MAP4 = PM_UCA1TXD;
    P4MAP5 = PM_UCA1RXD;
}

/***************************************************************************//**
 * @brief  Turn on LEDs
 * @param  ledMask   Use values defined in HAL_board.h for the LEDs to turn on
 * @return none
 ******************************************************************************/

void Board_ledOn(uint8_t ledMask)
{
    if (ledMask & LED1) LED145678_PORT_OUT |= BIT0;
    if (ledMask & LED4) LED145678_PORT_OUT |= BIT1;
    if (ledMask & LED5) LED145678_PORT_OUT |= BIT2;
    if (ledMask & LED6) LED145678_PORT_OUT |= BIT3;
    if (ledMask & LED7) LED145678_PORT_OUT |= BIT4;
    if (ledMask & LED8) LED145678_PORT_OUT |= BIT5;
}

/***************************************************************************//**
 * @brief  Turn off LEDs
 * @param  ledMask   Use values defined in HAL_board.h for the LEDs to turn off
 * @return none
 ******************************************************************************/

void Board_ledOff(uint8_t ledMask)
{
    if (ledMask & LED1) LED145678_PORT_OUT &= ~BIT0;
    if (ledMask & LED4) LED145678_PORT_OUT &= ~BIT1;
    if (ledMask & LED5) LED145678_PORT_OUT &= ~BIT2;
    if (ledMask & LED6) LED145678_PORT_OUT &= ~BIT3;
    if (ledMask & LED7) LED145678_PORT_OUT &= ~BIT4;
    if (ledMask & LED8) LED145678_PORT_OUT &= ~BIT5;
}

/***************************************************************************//**
 * @brief  Toggle LEDs
 * @param  ledMask   Use values defined in HAL_board.h for the LEDs to toggle
 * @return none
 ******************************************************************************/

void Board_ledToggle(uint8_t ledMask)
{
    if (ledMask & LED1) LED145678_PORT_OUT ^= BIT0;
    if (ledMask & LED4) LED145678_PORT_OUT ^= BIT1;
    if (ledMask & LED5) LED145678_PORT_OUT ^= BIT2;
    if (ledMask & LED6) LED145678_PORT_OUT ^= BIT3;
    if (ledMask & LED7) LED145678_PORT_OUT ^= BIT4;
    if (ledMask & LED8) LED145678_PORT_OUT ^= BIT5;
}

/***************************************************************************//**
 * @}
 ******************************************************************************/
