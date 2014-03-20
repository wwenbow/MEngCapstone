#include <plib.h>
#include <stdio.h>
#include <stdlib.h>  
#include "HardwareProfile.h"

#define SYS_FREQ 				(80000000L)
#define MIN_SERVO_DUTY       3000
#define MAX_SERVO_DUTY      25000

int RCservo[5]; // Desired high durations here 3000 - 25000 values give 0.3 mS
            // to 2.5 mS.
int RCcount = 0;

int main(void)
{
   // Configure the proper PB frequency and the number of wait states
   SYSTEMConfigPerformance(SYS_FREQ);

   INTEnableSystemMultiVectoredInt();
   
   // init the output compare modules
   // comment out the servos you do not want to use
   OpenOC1( OC_ON | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE, 0, 0);
   
   // init Timer2 mode and period (PR2)
   // Fpb = SYS_FREQ = 80Mhz (From configuration in bootloader code)
   // Timer Prescale = 8
   // PR2 = 0x9C3F = 39,999
   // interrupts every 4 ms
   // 4 ms = (PR2 + 1) * TMR Prescale / Fpb = (39999 + 1) * 8 / 80000000
   CloseTimer2();
   OpenTimer2( T2_ON | T2_PS_1_8 | T2_SOURCE_INT, 0x9C3F);
   ConfigIntTimer2(T2_INT_ON | T2_INT_PRIOR_7);
   
   mT2SetIntPriority( 7);    // set Timer2 Interrupt Priority
   mT2ClearIntFlag();       // clear interrupt flag
   mT2IntEnable(1);      // enable timer2 interrupts

   int counter;
   while(1)
   {
      // This code ramps up each servo then starts over
      for (RCservo[0] = MIN_SERVO_DUTY; RCservo[0] < MAX_SERVO_DUTY; RCservo[0]+=1)
      {
         int i;
         for (i = 1; i < 5; i++)
         {
            RCservo[i] = RCservo[i-1] + 200;
            if (RCservo[i] > MAX_SERVO_DUTY) RCservo[i] -= MAX_SERVO_DUTY - MIN_SERVO_DUTY;
            
            //Put in short pause
            counter = 500;
            while(counter-- != 0);
         }
      }
   
   }
   CloseTimer2();
   CloseOC1();

} //end main

void __ISR( _TIMER_2_VECTOR, ipl7) T2Interrupt( void)
{
   if (++RCcount >= 5) RCcount = 0;    // 20mS cycle --> 5 interrupts
   
   // Set all PWM pins low  -- comment out the pins you do not want
   SetDCOC1PWM(0);

   // Determine selected servo and set PWM   
   switch(RCcount)
   {
      case 0:            // comment out the pins you do not want
         SetDCOC1PWM(RCservo[RCcount]);
         break;
   }
   
   // clear interrupt flag and exit
   mT2ClearIntFlag();
} // T2 Interrupt
