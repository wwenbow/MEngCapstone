#ifndef __UBW32_HARDWARE__
#define __UBW32_HARDWARE__

enum Ubw32LEDs 
{
    LED3_PIN = 64, //Yellow
    LED2_PIN = 65, //Red
    LED1_PIN = 66, //White
    USB_PIN  = 67, //Green
};

enum Ubw32Pins 
{ 
    /* Important pins:
     * RF8 UART1TX
     */

    RA0, RA1, RA2, RA3, RA4, RA5, RA6, RA7, RA8, RA9, RA10, RA11, RA12, RA13,
    RA14, RA15,
    RB0, RB1, RB2, RB3, RB4, RB5, RB6, RB7, RB8, RB9, RB10, RB11, RB12, RB13,
    RB14, RB15,
    RC0, RC1, RC2, RC3, RC4, RC5, RC6, RC7, RC8, RC9, RC10, RC11, RC12, RC13,
    RC14, RC15,
    RD0, RD1, RD2, RD3, RD4, RD5, RD6, RD7, RD8, RD9, RD10, RD11, RD12, RD13,
    RD14, RD15,
    RE0, RE1, RE2, RE3, RE4, RE5, RE6, RE7, RE8, RE9, RE10, RE11, RE12, RE13,
    RE14, RE15,
    RF0, RF1, RF2, RF3, RF4, RF5, RF6, RF7, RF8, RF9, RF10, RF11, RF12, RF13,
    RF14, RF15,
    RG0, RG1, RG2, RG3, RG4, RG5, RG6, RG7, RG8, RG9, RG10, RG11, RG12, RG13,
    RG14, RG15,
};

#endif
