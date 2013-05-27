/*
  sample code for MSP430 UART
  transmit serial data at 9600,N,8,1

  code for msp430-gcc
  MSP430G2553 at 1 MHz
*/

#include <msp430.h>
#include <legacymsp430.h>
#include <stdio.h>

/* some prototypes */

/* some vars */
unsigned int count;

/*
 * main routines
 */
int main(void)
{
  // init watchdog for millis()
  // set watchdog timer interval to 32ms, enable interrupt
  WDTCTL  = WDT_MDLY_32;
  IE1    |= WDTIE;
  // init internal digitally controlled oscillator DCO
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL  = CALDCO_1MHZ;
  // IO setup (launchpad : bit 0 -> red led, bit 6 -> green led)
  P1DIR  |= BIT0 | BIT6;
  P1OUT  &= ~(BIT0 | BIT6);
  // start interrupt
  __enable_interrupt();

  // program loop
  while(1) {
    // go to lowpower mode 1
    __bis_SR_register(LPM1_bits);
    // wake up from lowpower
  }
}

/*
 * Watchdog (in timer mode) interrupt routine
 * use to emulate Arduino millis()
 */
interrupt(WDT_VECTOR) wd_timer_isr(void) {
  count++;
  if(count >= 100) {    // 10 * 32 ms = 320 ms, ≈.3 s
    P1OUT |= BIT0;   // P1.0 red LED on
    count = 0;
  } else {
    P1OUT &= ~BIT0;   // P1.0 red LED on
  }
  // wake up processor for job handling
  //__bic_SR_register_on_exit(LPM0_bits);
}
