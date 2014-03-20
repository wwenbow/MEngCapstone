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
#include "MPU6050.h"
#include "shared.h"

#define SCL TRISAbits.TRISA14 //
#define SDA TRISAbits.TRISA15 //
#define SCL_IN PORTAbits.RA14 //
#define SDA_IN PORTAbits.RA15 //

char GYRO_XOUT_H;
char GYRO_XOUT_L;
char GYRO_YOUT_H;
char GYRO_YOUT_L;
char GYRO_ZOUT_H;
char GYRO_ZOUT_L;
char GYRO_XOUT_OFFSET_1000SUM;
char GYRO_YOUT_OFFSET_1000SUM;
char GYRO_ZOUT_OFFSET_1000SUM;
char GYRO_XOUT_OFFSET;
char GYRO_YOUT_OFFSET;
char GYRO_ZOUT_OFFSET;
char ACCEL_XOUT_H;
char ACCEL_XOUT_L;
char ACCEL_YOUT_H;
char ACCEL_YOUT_L;
char ACCEL_ZOUT_H;
char ACCEL_ZOUT_L;
char ACCEL_XOUT;
char ACCEL_YOUT;
char ACCEL_ZOUT;
char ACCEL_XANGLE;
char ACCEL_YANGLE;
char ACCEL_ZANGLE;
char ACCEL_XOUT;
char ACCEL_YOUT;
char ACCEL_ZOUT;
char GYRO_XOUT_H;
char GYRO_XOUT_L;
char GYRO_YOUT_H;
char GYRO_YOUT_L;
char GYRO_ZOUT_H;
char GYRO_ZOUT_L;
char GYRO_XOUT;
char GYRO_XOUT_OFFSET;
char GYRO_YOUT;
char GYRO_YOUT_OFFSET;
char GYRO_ZOUT;
char GYRO_ZOUT_OFFSET;
char GYRO_XRATE;
char gyro_xsensitivity;
char GYRO_YRATE;
char gyro_ysensitivity;
char GYRO_ZRATE;
char gyro_zsensitivity;

//SDA = 1;
//SCL = 1;
//SCL_IN = 0;
//SDA_IN = 0;

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

void Setup_MPU6050()
{
    //Sets sample rate to 8000/1+7 = 1000Hz
    LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_SMPLRT_DIV, 0x07);
    //Disable FSync, 256Hz DLPF
    LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_CONFIG, 0x00);
    //Disable gyro self tests, scale of 500 degrees/s
    LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_GYRO_CONFIG, 0b00001000);
    //Disable accel self tests, scale of +-2g, no DHPF
    LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_ACCEL_CONFIG, 0x00);
    //Freefall threshold of |0mg|
    LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_FF_THR, 0x00);
    //Freefall duration limit of 0
    LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_FF_DUR, 0x00);
    //Motion threshold of 0mg
    LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_MOT_THR, 0x00);
    //Motion duration of 0s
    LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_MOT_DUR, 0x00);
    //Zero motion threshold
    LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_ZRMOT_THR, 0x00);
    //Zero motion duration threshold
    LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_ZRMOT_DUR, 0x00);
    //Disable sensor output to FIFO buffer
    LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_FIFO_EN, 0x00);
 
    //AUX I2C setup
    //Sets AUX I2C to single master control, plus other config
    LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_MST_CTRL, 0x00);
    //Setup AUX I2C slaves
    LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV0_ADDR, 0x00);
    LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV0_REG, 0x00);
    LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV0_CTRL, 0x00);
    LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV1_ADDR, 0x00);
    LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV1_REG, 0x00);
    LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV1_CTRL, 0x00);
    LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV2_ADDR, 0x00);
    LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV2_REG, 0x00);
    LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV2_CTRL, 0x00);
    LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV3_ADDR, 0x00);
    LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV3_REG, 0x00);
    LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV3_CTRL, 0x00);
    LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV4_ADDR, 0x00);
    LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV4_REG, 0x00);
    LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV4_DO, 0x00);
    LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV4_CTRL, 0x00);
    LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV4_DI, 0x00);
 
    //MPU6050_RA_I2C_MST_STATUS //Read-only
    //Setup INT pin and AUX I2C pass through
    LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_INT_PIN_CFG, 0x00);
    //Enable data ready interrupt
    LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_INT_ENABLE, 0x00);
 
    //MPU6050_RA_DMP_INT_STATUS        //Read-only
    //MPU6050_RA_INT_STATUS 3A        //Read-only
    //MPU6050_RA_ACCEL_XOUT_H         //Read-only
    //MPU6050_RA_ACCEL_XOUT_L         //Read-only
    //MPU6050_RA_ACCEL_YOUT_H         //Read-only
    //MPU6050_RA_ACCEL_YOUT_L         //Read-only
    //MPU6050_RA_ACCEL_ZOUT_H         //Read-only
    //MPU6050_RA_ACCEL_ZOUT_L         //Read-only
    //MPU6050_RA_TEMP_OUT_H         //Read-only
    //MPU6050_RA_TEMP_OUT_L         //Read-only
    //MPU6050_RA_GYRO_XOUT_H         //Read-only
    //MPU6050_RA_GYRO_XOUT_L         //Read-only
    //MPU6050_RA_GYRO_YOUT_H         //Read-only
    //MPU6050_RA_GYRO_YOUT_L         //Read-only
    //MPU6050_RA_GYRO_ZOUT_H         //Read-only
    //MPU6050_RA_GYRO_ZOUT_L         //Read-only
    //MPU6050_RA_EXT_SENS_DATA_00     //Read-only
    //MPU6050_RA_EXT_SENS_DATA_01     //Read-only
    //MPU6050_RA_EXT_SENS_DATA_02     //Read-only
    //MPU6050_RA_EXT_SENS_DATA_03     //Read-only
    //MPU6050_RA_EXT_SENS_DATA_04     //Read-only
    //MPU6050_RA_EXT_SENS_DATA_05     //Read-only
    //MPU6050_RA_EXT_SENS_DATA_06     //Read-only
    //MPU6050_RA_EXT_SENS_DATA_07     //Read-only
    //MPU6050_RA_EXT_SENS_DATA_08     //Read-only
    //MPU6050_RA_EXT_SENS_DATA_09     //Read-only
    //MPU6050_RA_EXT_SENS_DATA_10     //Read-only
    //MPU6050_RA_EXT_SENS_DATA_11     //Read-only
    //MPU6050_RA_EXT_SENS_DATA_12     //Read-only
    //MPU6050_RA_EXT_SENS_DATA_13     //Read-only
    //MPU6050_RA_EXT_SENS_DATA_14     //Read-only
    //MPU6050_RA_EXT_SENS_DATA_15     //Read-only
    //MPU6050_RA_EXT_SENS_DATA_16     //Read-only
    //MPU6050_RA_EXT_SENS_DATA_17     //Read-only
    //MPU6050_RA_EXT_SENS_DATA_18     //Read-only
    //MPU6050_RA_EXT_SENS_DATA_19     //Read-only
    //MPU6050_RA_EXT_SENS_DATA_20     //Read-only
    //MPU6050_RA_EXT_SENS_DATA_21     //Read-only
    //MPU6050_RA_EXT_SENS_DATA_22     //Read-only
    //MPU6050_RA_EXT_SENS_DATA_23     //Read-only
    //MPU6050_RA_MOT_DETECT_STATUS     //Read-only
 
    //Slave out, dont care
    LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV0_DO, 0x00);
    LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV1_DO, 0x00);
    LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV2_DO, 0x00);
    LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV3_DO, 0x00);
    //More slave config
    LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_MST_DELAY_CTRL, 0x00);
    //Reset sensor signal paths
    LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_SIGNAL_PATH_RESET, 0x00);
    //Motion detection control
    LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_MOT_DETECT_CTRL, 0x00);
    //Disables FIFO, AUX I2C, FIFO and I2C reset bits to 0
    LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_USER_CTRL, 0x00);
    //Sets clock source to gyro reference w/ PLL
    LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_PWR_MGMT_1, 0b00000010);
    //Controls frequency of wakeups in accel low power mode plus the sensor standby modes
    LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_PWR_MGMT_2, 0x00);
    //MPU6050_RA_BANK_SEL            //Not in datasheet
    //MPU6050_RA_MEM_START_ADDR        //Not in datasheet
    //MPU6050_RA_MEM_R_W            //Not in datasheet
    //MPU6050_RA_DMP_CFG_1            //Not in datasheet
    //MPU6050_RA_DMP_CFG_2            //Not in datasheet
    //MPU6050_RA_FIFO_COUNTH        //Read-only
    //MPU6050_RA_FIFO_COUNTL        //Read-only
    //Data transfer to and from the FIFO buffer
    LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_FIFO_R_W, 0x00);
    //MPU6050_RA_WHO_AM_I             //Read-only, I2C address
 
    printf("\nMPU6050 Setup Complete");
}

void Calibrate_Gyros()
{
	int x = 0;
	for(x = 0; x<1000; x++)
	{
		LDByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_GYRO_XOUT_H, &GYRO_XOUT_H, 1);
		LDByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_GYRO_XOUT_L, &GYRO_XOUT_L, 1);
		LDByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_GYRO_YOUT_H, &GYRO_YOUT_H, 1);
		LDByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_GYRO_YOUT_L, &GYRO_YOUT_L, 1);
		LDByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_GYRO_ZOUT_H, &GYRO_ZOUT_H, 1);
		LDByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_GYRO_ZOUT_L, &GYRO_ZOUT_L, 1);
 
		GYRO_XOUT_OFFSET_1000SUM += ((GYRO_XOUT_H<<8)|GYRO_XOUT_L);
		GYRO_YOUT_OFFSET_1000SUM += ((GYRO_YOUT_H<<8)|GYRO_YOUT_L);
		GYRO_ZOUT_OFFSET_1000SUM += ((GYRO_ZOUT_H<<8)|GYRO_ZOUT_L);
 
		//delay(1);
	}	
	GYRO_XOUT_OFFSET = GYRO_XOUT_OFFSET_1000SUM/1000;
	GYRO_YOUT_OFFSET = GYRO_YOUT_OFFSET_1000SUM/1000;
	GYRO_ZOUT_OFFSET = GYRO_ZOUT_OFFSET_1000SUM/1000;
 
	printf("\nGyro X offset sum: %ld Gyro X offset: %d", GYRO_XOUT_OFFSET_1000SUM, GYRO_XOUT_OFFSET);
	printf("\nGyro Y offset sum: %ld Gyro Y offset: %d", GYRO_YOUT_OFFSET_1000SUM, GYRO_YOUT_OFFSET);
	printf("\nGyro Z offset sum: %ld Gyro Z offset: %d", GYRO_ZOUT_OFFSET_1000SUM, GYRO_ZOUT_OFFSET);
}	
 
//Gets raw accelerometer data, performs no processing
void Get_Accel_Values()
{
	LDByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_ACCEL_XOUT_H, &ACCEL_XOUT_H, 1);
	LDByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_ACCEL_XOUT_L, &ACCEL_XOUT_L, 1);
	LDByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_ACCEL_YOUT_H, &ACCEL_YOUT_H, 1);
	LDByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_ACCEL_YOUT_L, &ACCEL_YOUT_L, 1);
	LDByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_ACCEL_ZOUT_H, &ACCEL_ZOUT_H, 1);
	LDByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_ACCEL_ZOUT_L, &ACCEL_ZOUT_L, 1);
 
	ACCEL_XOUT = ((ACCEL_XOUT_H<<8)|ACCEL_XOUT_L);
	ACCEL_YOUT = ((ACCEL_YOUT_H<<8)|ACCEL_YOUT_L);
	ACCEL_ZOUT = ((ACCEL_ZOUT_H<<8)|ACCEL_ZOUT_L);		
}	
 
//Converts the already acquired accelerometer data into 3D euler angles
void Get_Accel_Angles()
{
	ACCEL_XANGLE = 57.295*atan((float)ACCEL_YOUT/ sqrt(pow((float)ACCEL_ZOUT,2)+pow((float)ACCEL_XOUT,2)));
	ACCEL_YANGLE = 57.295*atan((float)-ACCEL_XOUT/ sqrt(pow((float)ACCEL_ZOUT,2)+pow((float)ACCEL_YOUT,2)));	
}	
 
//Function to read the gyroscope rate data and convert it into degrees/s
void Get_Gyro_Rates()
{
	LDByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_GYRO_XOUT_H, &GYRO_XOUT_H, 1);
	LDByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_GYRO_XOUT_L, &GYRO_XOUT_L, 1);
	LDByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_GYRO_YOUT_H, &GYRO_YOUT_H, 1);
	LDByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_GYRO_YOUT_L, &GYRO_YOUT_L, 1);
	LDByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_GYRO_ZOUT_H, &GYRO_ZOUT_H, 1);
	LDByteReadI2C(MPU6050_ADDRESS, MPU6050_RA_GYRO_ZOUT_L, &GYRO_ZOUT_L, 1);
 
	GYRO_XOUT = ((GYRO_XOUT_H<<8)|GYRO_XOUT_L) - GYRO_XOUT_OFFSET;
	GYRO_YOUT = ((GYRO_YOUT_H<<8)|GYRO_YOUT_L) - GYRO_YOUT_OFFSET;
	GYRO_ZOUT = ((GYRO_ZOUT_H<<8)|GYRO_ZOUT_L) - GYRO_ZOUT_OFFSET;
 
 
	GYRO_XRATE = (float)GYRO_XOUT/gyro_xsensitivity;
	GYRO_YRATE = (float)GYRO_YOUT/gyro_ysensitivity;
	GYRO_ZRATE = (float)GYRO_ZOUT/gyro_zsensitivity;
}

int main(void)
{
	SYSTEMConfigPerformance(SYSCLK);
	// Peripheral function that configures for best performance given the system's
	// clock frequency--this is for general practice.

	// This part is analogous to the void setup(){}; in Arduino.
	TRISDCLR = 0x0100; 	// Set pin RD8 as an output, could be written as TRISD = 0xFEFF;
				// but takes more clock cycles to perform.
	TRISESET = 0x0080;	// Set pin RE7 (PROG button) as an input, could be written as TRISE = 0x0080;
				// but takes more clock cycles to perform.
	
	// This part is analogous to the void loop(){}; in Arduino.
        SDA = SCL = 1;
        SCL_IN = SDA_IN = 0;
	Setup_MPU6050();
	Calibrate_Gyros();
	while(1) 		// Loop forever...
	{
		
		// Reads the state of pin RE7, and latches RD8 accordingly.
		// Note that RE7 is normally set HIGH (3.3V) through an internal pull-up
		//	resistor and will actually be set LOW when the PROG button is pressed. 
		// LATDbits.LATD8 = PORTEbits.RE7;		
		Get_Accel_Angles();
		
	}

	return 0; // Included because main returns an int which is being expected.
} // Make sure the program ends in a new line

// Further excercises:
// - Have the button toggle a second LED.
// - Have the button toggle a third LED every two button presses.
