#include "clock.h"
#include <p33FJ256GP710A.h>


static long int time = 0;

void __attribute__((__interrupt__, no_auto_psv)) _T2Interrupt(void){
    //Timer Interrupt used for the control loops and data link
    time += 20;
    IFS0bits.T2IF = 0;
}

long int getTime() {
    return time;
}

void InitClock()
{
	PLLFBD = 38;	// M = 40
	CLKDIVbits.PLLPOST = 0;	// N1 = 2
	CLKDIVbits.PLLPRE = 0;	// N2 = 2
	OSCTUN = 0;
	RCONbits.SWDTEN = 0;

// Clock switch to incorporate PLL
	__builtin_write_OSCCONH(0x01);		// Initiate Clock Switch to
													// FRC with PLL (NOSC=0b001)
	__builtin_write_OSCCONL(0x01);		// Start clock switching
	while (OSCCONbits.COSC != 0b001);	// Wait for Clock switch to occur

	while(OSCCONbits.LOCK != 1) {};
}
