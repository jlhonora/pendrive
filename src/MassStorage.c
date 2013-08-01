/*******************************************************************************
 *
 *  MassStorage.c - Uses the USB MSC stack
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
 * @file       MassStorage.c
 * @addtogroup MassStorage
 * @{
 ******************************************************************************/
#include <stdint.h>
#include <string.h>
#include "msp430.h"
#include "HAL_PMM.h"
#include "HAL_UCS.h"
#include "HAL_Board.h"
#include "HAL_Buttons.h"
//#include "HAL_Dogs102x6.h"
#include "mmc.h"
#include "diskio.h" // Low level disk interface module from the FatFs package
#include "device.h"
#include "types.h"  // Basic Type declarations
#include "descriptors.h"
#include "usb.h"    // USB-specific functions
#include "UsbMscScsi.h"
#include "USB/USB_API/USB_MSC_API/UsbMsc.h"
#include "USB/USB_API/USB_MSC_API/UsbMscStateMachine.h"
#include "UsbMscUser.h"
#include "utils.h"

#include "HAL_FLASH.h"
#include "MassStorageCommon.h"

void msc_Init(void);
void msc_Loop(void);

uint8_t bDetectCard = 0x00;

/***************************************************************************//**
 * @brief  Uses USB MSC stack to make MSP430 appear as a "drive" to the host PC
 * @param  none
 * @return none
 ******************************************************************************/

void MassStorage(void)
{
    buttonsPressed = 0;

    SFRIE1 &= ~OFIE;
    disk_initialize(0);                   // Initialize Disk Drive #0

    SFRIE1 |= OFIE;

    DEBUG("Init clock\r\n");
    ClockUSB();

    DEBUG("Init USB\r\n");
    USB_init();                           // Initialize the USB module
    P1OUT |= BIT1;

    // Enable all USB events
    USB_setEnabledEvents(kUSB_allUsbEvents);

    // Clal Initialization Function
    DEBUG("Init MSC\r\n");
    msc_Init();
    P1OUT |= BIT2;

    // If USB is already connected when the program starts up, then there won't be a
    // USB_handleVbusOnEvent().
    // So we need to check for it, and manually connect if the host is already present.
    if (USB_connectionInfo() & kUSB_vbusPresent)
    {
        if (USB_enable() == kUSB_succeed)
        {
            USB_reset();
            USB_connect();
            P1OUT |= BIT3;
        }
    }

    while (1)
    {
        switch (USB_connectionState())
        {
            DEBUG("Connection state: %u\r\n", USB_connectionState());
            case ST_USB_DISCONNECTED:
                //__bis_SR_register(LPM3_bits + GIE);    // Enter LPM3 until VBUS-on event
                P8OUT |= BIT1;
                _NOP();
                break;

            case ST_USB_CONNECTED_NO_ENUM:
                P1OUT |= BIT4;
                break;

            case ST_ENUM_ACTIVE:

                msc_Loop();
                P1OUT |= BIT5;
                break;

            case ST_ENUM_SUSPENDED:
                P1OUT |= BIT0;
                //__bis_SR_register(LPM3_bits + GIE);    // Enter LPM3, until a resume or VBUS-off
                                                       // event
                break;

            case ST_ENUM_IN_PROGRESS:
                break;

            case ST_ERROR:
                P8OUT |= BIT2;
                break;
            default:;
        }
    }

    DEBUG("Done with MassStorage\r\n");
    buttonsPressed = 0;
    Board_ledOff(LED_ALL);
    USB_disable();
    SFRIE1 &= ~OFIE;
    Init_FLL_Settle(25000, 762);          // Return to normal clock settings
    SFRIE1 |= OFIE;
    //Dogs102x6_clearScreen();
}

/***************************************************************************//**
 * @}
 ******************************************************************************/
