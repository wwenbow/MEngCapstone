/*******************************************************************************
  UART Library Interface Example

  Summary:
    This file contains the interface definition for the UART peripheral library.
    
  Description:
    This library provides a low-level abstraction of the UART (Universal 
    Asynchronous Receiver/Transmtter) module on Microchip PIC32MX family 
    microcontrollers with a convenient C language interface.  It can be used to
    simplify low-level access to the module without the necessity of interacting
    directly with the module's registers, thus hiding differences from one 
    microcontroller variant to another.
*******************************************************************************/
//DOM-IGNORE-BEGIN
/*******************************************************************************
FileName:       main.h
Dependencies:   See includes
Processor:      PIC32MX

Complier:       Microchip MPLAB C32 v1.06 or higher
Company:        Microchip Technology Inc.

Copyright © 2008-2009 released Microchip Technology Inc.  All rights
reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*******************************************************************************/
//DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Includes
// *****************************************************************************
// *****************************************************************************
#include <GenericTypeDefs.h>
#include <plib.h>

// *****************************************************************************
// *****************************************************************************
// Section: Configuration bits
// *****************************************************************************
// *****************************************************************************
#pragma config FPLLODIV = DIV_1, FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FWDTEN = OFF, FCKSM = CSECME, FPBDIV = DIV_1
#pragma config OSCIOFNC = ON, POSCMOD = XT, FSOSCEN = ON, FNOSC = PRIPLL
#pragma config CP = OFF, BWP = OFF, PWP = OFF

// *****************************************************************************
// *****************************************************************************
// Section: System Macros
// *****************************************************************************
// *****************************************************************************
#define	GetSystemClock() 			(80000000ul)
#define	GetPeripheralClock()		(GetSystemClock()/(1 << OSCCONbits.PBDIV))
#define	GetInstructionClock()		(GetSystemClock())

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************
void SendDataBuffer(const char *buffer, UINT32 size);
UINT32 GetMenuChoice(void);
UINT32 GetDataBuffer(char *buffer, UINT32 max_size);
// *****************************************************************************
// *****************************************************************************
// Section: Constant Data
// *****************************************************************************
// *****************************************************************************
const char mainMenu[] =
{
    "Welcome to PIC32 UART Peripheral Library Demo!\r\n"\
    "Here are the main menu choices\r\n"\
    "1. View Actual BAUD rate\r\n"\
    "2. Use AUTOBAUD\r\n"\
    "3. Set Line Control\r\n"\
    "4. ECHO\r\n"
    "\r\n\r\nPlease Choose a number\r\n"
};
const char lineMenu[] =
{
    "Line Control Menu\r\n"\
    "You may need to change your line control on your terminal to see data\r\n"\
    "1. 8-N-1\r\n"\
    "2. 8-E-1\r\n"\
    "3. 8-O-1\r\n"\
    "4. 8-N-2\r\n"\
    "5. 8-E-2\r\n"\
    "6. 8-O-2\r\n"\
    "\r\n\r\nPlease Choose a number\r\n"
};
const UINT32 lineControl[] =
{
    (UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1),
    (UART_DATA_SIZE_8_BITS | UART_PARITY_EVEN | UART_STOP_BITS_1),
    (UART_DATA_SIZE_8_BITS | UART_PARITY_ODD | UART_STOP_BITS_1),
    (UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_2),
    (UART_DATA_SIZE_8_BITS | UART_PARITY_EVEN | UART_STOP_BITS_2),
    (UART_DATA_SIZE_8_BITS | UART_PARITY_ODD | UART_STOP_BITS_2)
};
// *****************************************************************************
// Section: Code
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// int main(void)
// *****************************************************************************
int main(void)
{
    UINT32  menu_choice;
    UINT8   buf[1024];

    UARTConfigure(UART2, UART_ENABLE_PINS_TX_RX_ONLY);
    UARTSetFifoMode(UART2, UART_INTERRUPT_ON_TX_NOT_FULL | UART_INTERRUPT_ON_RX_NOT_EMPTY);
    UARTSetLineControl(UART2, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
    UARTSetDataRate(UART2, GetPeripheralClock(), 57600);
    UARTEnable(UART2, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));

    SendDataBuffer(mainMenu, sizeof(mainMenu));

    while(1)
    {
        menu_choice = GetMenuChoice();

        switch(menu_choice)
        {
        case 1:
            sprintf(buf, "Actual Baud Rate: %ld\r\n\r\n", UARTGetDataRate(UART2, GetPeripheralClock()));
            SendDataBuffer(buf, strlen(buf));
            break;
        
        case 2:
            SendDataBuffer("Press 'U' to allow AUTO BAUD to sync\r\n", strlen("Press 'U' to allow AUTO BAUD to sync\r\n"));
            
            UARTStartAutoDataRateDetect(UART2);

            while(!UARTDataRateDetected(UART2))
                ;

            sprintf(buf, "Actual Baud Rate: %ld\r\n\r\n", UARTGetDataRate(UART2, GetPeripheralClock()));
            SendDataBuffer(buf, strlen(buf));
            break;

        case 3:
            SendDataBuffer(lineMenu, sizeof(lineMenu));
            menu_choice = GetMenuChoice();
            menu_choice--;

            if(menu_choice >= 6)
            {
                SendDataBuffer("Invalid Choice", sizeof("Invalid Choice"));
                SendDataBuffer(mainMenu, sizeof(mainMenu));
                break;
            }

            SendDataBuffer("Press Any Character after re-configuring you terminal\r\n", strlen("Press Any Character after re-configuring you terminal\r\n"));
            UARTSetLineControl(UART2, lineControl[menu_choice]);

            menu_choice = GetMenuChoice();
            SendDataBuffer(mainMenu, sizeof(mainMenu));
           break;
            
        case 4:
            {
                UINT32 rx_size;

                SendDataBuffer("Type a message (less than 100 characters) and press return\r\n", strlen("Type a message (less than 100 characters) and press return\r\n"));
                
                rx_size = GetDataBuffer(buf, 1024);

                SendDataBuffer("You Typed:\r\n\r\n", strlen("You Typed:\r\n\r\n"));
                SendDataBuffer(buf, rx_size);
                SendDataBuffer("\r\n\r\nPress any key to continue", strlen("\r\n\r\nPress any key to continue"));
                GetMenuChoice();
            }
            break;

        default:
            SendDataBuffer(mainMenu, sizeof(mainMenu));

        }
    }

    return -1;
}
// *****************************************************************************
// void UARTTxBuffer(char *buffer, UINT32 size)
// *****************************************************************************
void SendDataBuffer(const char *buffer, UINT32 size)
{
    while(size)
    {
        while(!UARTTransmitterIsReady(UART2))
            ;

        UARTSendDataByte(UART2, *buffer);

        buffer++;
        size--;
    }

    while(!UARTTransmissionHasCompleted(UART2))
        ;
}
// *****************************************************************************
// UINT32 GetDataBuffer(char *buffer, UINT32 max_size)
// *****************************************************************************
UINT32 GetDataBuffer(char *buffer, UINT32 max_size)
{
    UINT32 num_char;

    num_char = 0;

    while(num_char < max_size)
    {
        UINT8 character;

        while(!UARTReceivedDataIsAvailable(UART2))
            ;

        character = UARTGetDataByte(UART2);

        if(character == '\r')
            break;

        *buffer = character;

        buffer++;
        num_char++;
    }

    return num_char;
}
// *****************************************************************************
// UINT32 GetMenuChoice(void)
// *****************************************************************************
UINT32 GetMenuChoice(void)
{
    UINT8  menu_item;

    while(!UARTReceivedDataIsAvailable(UART2))
        ;

    menu_item = UARTGetDataByte(UART2);

    menu_item -= '0';

    return (UINT32)menu_item;
}

