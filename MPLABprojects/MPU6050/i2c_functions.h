
#ifndef I2C_FUNCTIONS_H
#define I2C_FUNCTIONS_H

#define SCL TRISAbits.TRISA14 //
#define SDA TRISAbits.TRISA15 //
#define SCL_IN PORTAbits.RA14 //
#define SDA_IN PORTAbits.RA15 //


void i2c_dly(void);
void i2c_start(void);
void i2c_stop(void);
unsigned char i2c_rx(char ack);
int i2c_tx(unsigned char d);
void LDByteWriteI2C(char address, char command_address, char command);
void LDByteReadI2C(char addess, char read_address, char* info, int test);

#endif

