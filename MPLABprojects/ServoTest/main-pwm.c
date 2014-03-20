#include <plib.h>
#include <stdio.h>
#include <stdlib.h>  
#include "HardwareProfile.h"

#define SYS_FREQ 				(80000000L)
#define MIN_SERVO_DUTY       3000
#define MAX_SERVO_DUTY      25000

int RCservo[5];

int RCcount;

int main(void)
{
    SYSTEMConfigPerformance(SYS_FREQ);

    CloseTimer2();
    OpenTimer2( T2_ON | T2_PS_1_8 | T2_SOURCE_INT, 0x9C3F);

    //Init output compare modules
    OpenOC1( OC_ON | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE, 0, 0);

    // init Timer2 mode and period
    // Fpb = SYS_FREQ = 80Mhz
    // Timer Prescale = 8
    // PR2 = 0x9C3F = 39,999
    // interrupts every 4 ms
    // 4 ms = (PR2 + 1) * TMR prescale / Fpb = (39,999 + 1) * 8 / 8Mega

    //ConfigIntTimer2( T2_INT_ON | T2_INT_PRIOR_7);
    //mT2SetIntPriority( 7 ); 
    //mT2ClearIntFlag();
    //mT2IntEnable(1);

    int counter = 0;
    RCservo[0] = MAX_SERVO_DUTY;
    while(1)
    {
        int deltaDuty = 1000;
        RCservo[0] += deltaDuty;
        if (RCservo[0] >= MAX_SERVO_DUTY
                || RCservo[0] <= MIN_SERVO_DUTY) {
            deltaDuty = -deltaDuty;
        }

        RCservo[0] = 15000;
        SetDCOC1PWM(RCservo[0]);

        counter--;
        if (counter <= 0)
            counter = 500;
    }

    CloseTimer2();
    CloseOCS1();
    
    return 0;
}

