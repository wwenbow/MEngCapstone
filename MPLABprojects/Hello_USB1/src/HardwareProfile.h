#ifndef HARDWARE_PROFILE_H
#define HARDWARE_PROFILE_H

	#include "Compiler.h"
	#include "GenericTypeDefs.h"

	#define DEMO_BOARD PIC32_USB_STARTER_KIT

	#define SYS_FREQ				(80e6)
	#define GetSystemClock()		SYS_FREQ
	#define CORE_TIMER_TICK_RATE	(SYS_FREQ/2)

	//#define USE_SELF_POWER_SENSE_IO
	#define tris_self_power     TRISAbits.TRISA2
	#define self_power          1
	
	//#define USE_USB_BUS_SENSE_IO
	#define tris_usb_bus_sense  TRISBbits.TRISB5
	#define USB_BUS_SENSE       1 
	
	
	#define mInitAllLEDs()      LATE |= 0x000F; TRISE &= 0xFFF0;
	   
	#define mLED_1              LATEbits.LATE3
	#define mLED_2              LATEbits.LATE2
	#define mLED_3              LATEbits.LATE1
	#define mLED_4              LATEbits.LATE0
	
	#define mGetLED_1()         mLED_1
	#define mGetLED_USB()       mLED_1
	#define mGetLED_2()         mLED_2
	#define mGetLED_3()         mLED_3
	#define mGetLED_4()         mLED_4
	
	#define mLED_1_On()         mLED_1 = 0;
	#define mLED_USB_On()       mLED_1 = 0;
	#define mLED_2_On()         mLED_2 = 0;
	#define mLED_3_On()         mLED_3 = 0;
	#define mLED_4_On()         mLED_4 = 0;
	    
	#define mLED_1_Off()        mLED_1 = 1;
	#define mLED_USB_Off()      mLED_1 = 1;
	#define mLED_2_Off()        mLED_2 = 1;
	#define mLED_3_Off()        mLED_3 = 1;
	#define mLED_4_Off()        mLED_4 = 1;
	    
	#define mLED_1_Toggle()     mLED_1 = !mLED_1;
	#define mLED_USB_Toggle()   mLED_1 = !mLED_1;
	#define mLED_2_Toggle()     mLED_2 = !mLED_2;
	#define mLED_3_Toggle()     mLED_3 = !mLED_3;
	#define mLED_4_Toggle()     mLED_4 = !mLED_4;

	#define mLED_Both_Off()     { mLED_1_Off(); mLED_2_Off(); }
	#define mLED_Both_On()      { mLED_1_On();  mLED_2_On();  }
	#define mLED_Only_1_On()    { mLED_1_On();  mLED_2_Off(); }
	#define mLED_Only_2_On()    { mLED_1_Off(); mLED_2_On();  }
	    
	#define mInitSwitch2()      TRISEbits.TRISE7=1;
	#define mInitSwitch3()      TRISEbits.TRISE6=1;
	#define msg_switch          PORTDbits.RD6
	#define mInitAllSwitches()  mInitSwitch2();mInitSwitch3();
	#define swProgram           PORTEbits.RE7
	#define swUser              PORTEbits.RE6

#endif
