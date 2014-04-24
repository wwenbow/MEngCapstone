/********************************************************************
Super simple HelloWorld (blink some LEDs) for use with UBW32 hardware
see http://www.schmalzhaus.com/UBW32 for more info
********************************************************************/

/* 06/01/2010 - updated to use latest MPLAB and C32 compiler */
/* 08/28/2010 - Changed to use Microchip folder - from Microchip Applications Library USB
				stack v2.7 - updated project to use unchanged MAL Microchip folder - no code changes */
/* 09/02/2011 - Tested with USB stack v2.9a, C32 v2.01, MPLAB v8.76 - no code changes */

//#include "GenericTypeDefs.h"
#include "plib.h"
#include "HardwareProfile.h"

// NOTE THAT BECAUSE WE USE THE BOOTLOADER, NO CONFIGURATION IS NECESSARY
// THE BOOTLOADER PROJECT ACTUALLY CONTROLS ALL OF OUR CONFIG BITS

// Let compile time pre-processor calculate the CORE_TICK_PERIOD
#define SYS_FREQ 				(80000000L)
#define TOGGLES_PER_SEC			1000
#define CORE_TICK_RATE	       (SYS_FREQ/2/TOGGLES_PER_SEC)

// Decriments every 1 ms.
volatile static uint32 OneMSTimer;

int main(void)
{   
	// This is in ms, and is how long we wait between blinks
	uint32	BlinkTime = 1000;

	// Set all analog pins to be digital I/O
    AD1PCFG = 0xFFFF;
    
    // Configure the proper PB frequency and the number of wait states
	SYSTEMConfigPerformance(80000000L);

	// Turn off JTAG so we get the pins back
 	mJTAGPortEnable(0);

    //Initialize all of the LED pins
	mInitAllLEDs();

	// Open up the core timer at our 1ms rate
	OpenCoreTimer(CORE_TICK_RATE);

    // set up the core timer interrupt with a prioirty of 2 and zero sub-priority
	mConfigIntCoreTimer((CT_INT_ON | CT_INT_PRIOR_2 | CT_INT_SUB_PRIOR_0));

    // enable multi-vector interrupts
	INTEnableSystemMultiVectoredInt();

    while(1)
    {
	    // What we're going to do is blink an LED on and off
	    // The rate we use will be determind by the user pushing
	    // the User button.
	    mLED_2_On();
	    
	    OneMSTimer = BlinkTime;
	    while (OneMSTimer);
	    
	    mLED_2_Off();
	    
	    OneMSTimer = BlinkTime;
	    while (OneMSTimer);
	    
	    // Look for a button press to switch between the two speeds
	    if (swUser)
	    {
		    BlinkTime = 1000;		// 1second
		}
		else
		{
			BlinkTime = 100;		// 100ms
		} 
    }
}

void __ISR(_CORE_TIMER_VECTOR, ipl2) CoreTimerHandler(void)
{
    // clear the interrupt flag
    mCTClearIntFlag();

	if (OneMSTimer)
	{
		OneMSTimer--;
	}

    // update the period
    UpdateCoreTimer(CORE_TICK_RATE);
}

