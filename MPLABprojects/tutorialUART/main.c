/* ===========================================================================
* PIC32 Tutorial 5 – Transmitting into the Air: UART and the XBee Wireless Module
* ===========================================================================
* Rodrigo S. Bismonte II and Sean Klaiber
* last updated August 2011
*
* with info from the Northwestern Mechatronics Wiki page on the NU32v2:
*
* and Lucio Di Jasio’s book “Programming 32-bit Microcontrollers in C: Exploring the PIC32?
* and the informative engineer at ladyada.net about configuring XBees:
*
*
* Objectives:
* -Read the analog value of a potentiometer wired to pin AN15.
* -Remap this value to the range of printed ASCII values (33 to 126)
* -Send this value through UART into an XBee wireless module
* -Have a second XBee wireless module receive the value, and print out the corresponding
* ASCII value on a computer through HyperTerminal/PuTTy (Windows) or Terminal (Mac).
* -Display the same value as an 8-bit binary number on an LED bar connected to
* pins RD0 – RD7.
* *** Don’t forget a current-limiting resistor network for the LED bar!!! ***
*
* *** The schematic for wiring up an XBee to work properly with the UBW32 is included ***
* *** in XBee-CUI32.pdf ***
*
* *** The schematic for wiring up an XBee to accept an FTDI cable is included in ***
* *** XBee-FTDI.pdf. (The triangle-shaped components in the schematic is a single buffer***
* *** in a 5v compliant buffer chip 74AHC125N). The chip has 4 buffers in it and serves ***
* *** as a level converter–USB to Serial signals are 5V, and the XBee operates in 3.3V***
*
* Tutorial covers:
* -Setup the UART module on the CUI32/UBW32.
* -Configuring the XBee modules
* -#pragma
*
* ============== NOTES on the Universal Asynchronous Receiver/Transmitter (UART) ===============
*
* The CUI32/UBW32 have two Universal Asynchronous Receiver/Transmitter (UART) peripheral modules. They
* are connected to pins RD2/RD3 (for CUI32 UART1), RF2/RF8 (for UBW32 UART1), and RF4/RF5 (for both UART2).
* UART is one of the serial interface modules available on the CUI32. It is ASYNCHRONOUS, which means
* that the two devices communicating need not be using the same clock.
*
* The PIC32's UART module is capable of 8-bit or 9-bit data transmission with even, odd, or no
* parity bits and one or two stop bits. It also has a fully integrated Baud Rate Generator (BRG)
* and can range from 76 bps to 20 Mbps at 80MHz.
*
* The UART Peripheral Module has several peripheral library functions specified in the
* Peripheral Library Guide (.pdf). For the scope of this tutorial, only OpenUARTx(), putcUARTx(),
* BusyUARTx(), ReadUARTx(), and putsUARTx(), where x is either 1 or 2, depending on which UART
* module is used.
*
* OpenUARTx(config1, config2, ubrg);
* -Opens and configures the UART module. The parameters [config1] and [config2] are the
* bit-wise OR’d bit-masks given in page 233 to page 235. The parameter [ubrg] is the value
* that will set the baud rate.
* BusyUARTx();
* -Checks if the UART module is busy. Returns a 1 if it is busy and 0 otherwise.
* ReadUARTx();
* -Reads the value in the UART receive buffer.
* putcUARTx(data);
* -The same function as WriteUARTx(data). The [data] parameter is an unsigned int.
* putsUARTx(string);
* -Function places a string (or array of characters) into the transmit buffer.
*
* The difference between putsUARTx(string) and putcUARTx(data) is that putcUARTx(data) can only
* send out one character at a time, while putsUARTx(string) allows you to transmit a string of
* such characters.
*
*
* ========================== NOTES on XBee Wireless Transciever Modules ========================
*
* A very informative tutorial on initially setting up the XBee wireless modules can be found in
* . XBee’s can be connected to the computer
* using either using a USB to serial base unit like Sparkfun’s “XBee Explorer USB” or making the
* circuit specified in XBee-FTDI.pdf. This tutorial supposes using Hyperterminal in Windows, but
* the XBee modules can be configured using Digi’s XCTU program. On Mac and Linux systems, serial
* terminal emulators like “screen” and “minicom” will work.
*
* *** NOTE that in order for this system to work, both XBee’s must be set exactly the same: ***
* 8 bits of data, no parity bit, 1 stop bit, at 9600 baud or (8N1 @ 9600 baud)
*
* ================================= NOTES on #pragma directive ================================
*
* #pragma directives are often used in embedded systems to provide information to the compiler
* specific to the device being programmed. The directive is machine specific in the sense that
* it may not apply to all machines that can be programmed. Note that #pragma’s are compiler
* dependent and non-portable. Fortunately, all of our programming is done with MPLAB…
*
* =========================================================================
*/
#include <p32xxxx.h>
#include <plib.h>

#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_1
 
#define DESIRED_BAUDRATE (9600)
#define SYSCLK (80000000L)
 
#define CONFIG1 (ADC_MODULE_ON | ADC_FORMAT_INTG32 | ADC_CLK_AUTO | ADC_AUTO_SAMPLING_ON)
#define CONFIG2 (ADC_VREF_AVDD_AVSS | ADC_OFFSET_CAL_DISABLE | ADC_SCAN_ON | ADC_SAMPLES_PER_INT_1 | ADC_ALT_BUF_OFF | ADC_ALT_INPUT_OFF)
#define CONFIG3 (ADC_CONV_CLK_INTERNAL_RC | ADC_SAMPLE_TIME_15)
#define CONFIGPORT (ENABLE_AN15_ANA)
#define CONFIGSCAN (SKIP_SCAN_AN0 | SKIP_SCAN_AN1 | SKIP_SCAN_AN2 | SKIP_SCAN_AN3 | SKIP_SCAN_AN4 | SKIP_SCAN_AN5 | SKIP_SCAN_AN6 | SKIP_SCAN_AN7 | SKIP_SCAN_AN8 | SKIP_SCAN_AN9 | SKIP_SCAN_AN10 | SKIP_SCAN_AN11 | SKIP_SCAN_AN12 | SKIP_SCAN_AN13 | SKIP_SCAN_AN14)
 
unsigned char data;
int pbClk;
 
void initializeUART(){
        // Optimize PIC32 performance and return peripheral bus frequency
        pbClk=SYSTEMConfig( SYSCLK, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);
 
        // Enable UART2 and set baud rate to DESIRED_BAUDRATE=9600
        OpenUART2( UART_EN, UART_RX_ENABLE | UART_TX_ENABLE, pbClk/16/DESIRED_BAUDRATE-1);
 
        while( BusyUART2()); // Wait until the UART module is free
        putsUART2( "Turning the potentiometer will change characters on screen!");
}
 
int main(){
        initializeUART(); // Initialize the UART module

        mPORTESetBits(0xF);
        mPORTEClearBits(0x1);

        data = 0xF;
 
        while( 1){
 
                while( BusyUART2()); // Wait until UART2 module is not busy
                putsUART2( "testing" ); // Transmit ‘data’ through UART
                //putsUART2( ’13'); // Transmit ’13' (carriage return)
        }
 
        return 1;
}

