/* ==================================================================================
 *  PIC32 Tutorial 1 - Blinking an LED with a button: Digital Input and Outputs
 * ==================================================================================
 *	Rodrigo S. Bismonte II
 *	with info from the Northwestern Mechatronics Wiki page on the NU32v2:
 *	<http://hades.mech.northwestern.edu/index.php/Microchip_PICs>
 *
 *	Objective:
 *	-Blink an LED attached to pin RD8 when the PROG (RE7) button
 *	 on the UBW32 is pressed.
 *
 *	Tutorial covers:
 *	-Including neccesary header files.
 *	-Setup for best performance using the peripheral library.
 *	-Setup I/O ports for digital input and output.
 *
 *	==================== NOTES on Digital I/O setup ===================================
 *
 *	Each I/O port (Port A - Port F) has three registers: PORTx, TRISx, LATx.
 *		They are short for Port, Tri-State, and Latch registers.
 *
 *	Note that each pin on the PIC32 is grouped into a certain port. That is, 
 *		you wouldn't refer to "pin 4" but might refer to "pin B4" or "pin RB4"
 *		(pin 4 on Port B). The "R" in "RB4" is convention.
 *
 *	TRISx sets the direction of the pin. Corresponding port pin is an input
 *		when the bit is set (1), output when the bit is cleared (0).
 *		Think 1=I(nput) and 0=O(utput).
 *			"TRISA = 0x000C;" sets pins RA2 and RA3 to inputs, rest of PortA as outputs
 *			"TRISC = 0xA000;" sets pins RC13 and RC15 as inputs, all else outputs
 *		Individual pins can be addressed with this convention:
 *		TRISxbits.TRISxy (x is the port letter, y is the number of the pin)
 *	
 *	LATx sets the value at that particular port.
 *			"LATA = 0x003E;" sets RE1 through RE5 HIGH, all else on PortA LOW
 *		Individual bits can be addressed with this convention:
 *		LATxbits.LATxy (x is the port letter, y is the number of the pin)
 *
 *	PORTx holds the value at that port's pin, which can then be read.
 *		A write to PORTx is the same as a write to the LATx register.
 *			"short Value = PORTA;" writes the values of all pins on PortA to the variable 'Value'
 *		Individual pins can be addressed with this convention:
 *		PORTxbits.Rxy (x is the port letter, y is the number of the pin)
 *
 *	Each of the three special function registers (SFR) also have three
 *	registers that allow for atomic updating (which takes fewer clock cycles).
 *
 *	TRISxSET: sets the specified bits of Port x, also works for PORTx and LATx.
 *	TRISxCLR: clears the specified bits of Port x, also works for PORTx and LATx.
 *	TRISxINV: inverts the specified bits of Port x, also works for PORTx and LATx.
 *
 *	For alternative methods to those above, Peripheral Library Functions exist
 *	although are usually not necessary for these simple tasks. 
 *
 *	Note about Port B:	If you want to use the analog pins (PORTB / ANx),
 *		as digital inputs, you must first SET the
 *		corresponding bits in the AD1PCFG register.
 *			"AD1PCFG = 0x0003;" sets pins RB0 and RB1 to analog inputs, all else digital inputs
 *	=================================================================================
 */

#define SYSCLK 80000000L // Give the System's clock frequency (80MHz for PIC32MX440F512H)
			 // This is general practice in order to get the best performance

#include <p32xxxx.h>	// Include PIC32 specifics header file.
#include <plib.h>	// Include the PIC32 Peripheral Library.
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "i2c_functions.h"
#include "shared.h"

void i2c_dly(void) {}

void i2c_start(void) {
  SDA = 1;             // i2c start bit sequence
  i2c_dly();
  SCL = 1;
  i2c_dly();
  SDA = 0;
  i2c_dly();
  SCL = 0;
  i2c_dly();
}

void i2c_stop(void) {
  SDA = 0;             // i2c stop bit sequence
  i2c_dly();
  SCL = 1;
  i2c_dly();
  SDA = 1;
  i2c_dly();
}

unsigned char i2c_rx(char ack) {
  char x, d=0;
  SDA = 1; 
  for(x=0; x<8; x++) {
    d <<= 1;
    do {
      SCL = 1;
    }
    while(SCL_IN==0);    // wait for any SCL clock stretching
    i2c_dly();
    if(SDA_IN) d |= 1;
    SCL = 0;
  } 
  if(ack) SDA = 0;
  else SDA = 1;
  SCL = 1;
  i2c_dly();             // send (N)ACK bit
  SCL = 0;
  SDA = 1;
  return d;
}

int i2c_tx(unsigned char d) {
  char x;
  static int b;
  for(x=8; x; x--) {
    if(d) SDA = 1;
    else SDA = 0;
    SCL = 1;
    d <<= 1;
    SCL = 0;
  }
  SDA = 1;
  SCL = 1;
  i2c_dly();
  b = SDA_IN;          // possible ACK bit
  SCL = 0;
  return b;
} 

void LDByteWriteI2C(char address, char command_address, char command) {
	i2c_start();              // send start sequence
	i2c_tx(0x68);             // SRF08 I2C address with R/W bit clear
	i2c_tx(command_address);             // SRF08 command register address
	i2c_tx(command);             // command to start ranging in cm
	i2c_stop();               // send stop sequence	
}

void LDByteReadI2C(char address, char read_address, char * info, int test) {
	i2c_start();              // send start sequence
	i2c_tx(0x68);             // SRF08 I2C address with R/W bit clear
	i2c_tx(read_address);             // SRF08 light sensor register address
	i2c_start();              // send a restart sequence
	i2c_tx(0x69);             // SRF08 I2C address with R/W bit set
	* info = i2c_rx(1);
	//char lightsensor = i2c_rx(1);  // get light sensor and send acknowledge. Internal register address will increment automatically.
	//char rangehigh = i2c_rx(1);    // get the high byte of the range and send acknowledge.
	//char rangelow = i2c_rx(0);     // get low byte of the range - note we don't acknowledge the last byte.
	i2c_stop();               // send stop sequence
}


/** EOF i2c_functions.c ***********************************************/

