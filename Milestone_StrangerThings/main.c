#include <msp430.h> 

int main(void)
{
//enable GPIO
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	PM5CTL0 &= ~LOCKLPM5; // Disable the GPIO power-on default high-impedance mode

/*set RGB outputs*/
	P1DIR |= BIT0|BIT1|BIT2;            //SETS P1.0,1,2 AS OUTPUT
	P1SEL0 &= ~(BIT0|BIT1|BIT2);        //SETS P1.0,1,2 MUX AS OUTPUT
    P1SEL1 &= ~(BIT0|BIT1|BIT2);        //SETS P1.0,1,2 MUX AS OUTPUT

//initialize timer B0
	TB0CTL = ( MC__UP  + TBSSEL__SMCLK);    //Set to up mode, run at SMCLK
	TB0CCTL0 = CCIE;
	TB0CCR0 = 500;      //set PWM frequency to 1 KHz
	TB0CCR1 = 100;        //initialize red Duty cycle to CCR1
	TB0CCTL1 = CCIE;
	TB0CCR2 = 200;        //initialize green PWM to CCR2
	TB0CCTL2 = CCIE;

//initialize timer B1
	TB1CTL = ( MC__UP  + TBSSEL__SMCLK);    //Set to up mode, run at SMCLK0
	TB1CCTL0 = CCIE;
	TB1CCR0 = 500;      //set PWM frequency to 2 Khz
	TB1CCR1 = 300;        //initialize blue PWM to B1CCR1
	TB1CCTL1 = CCIE;

    _BIS_SR(CPUOFF + GIE);                  // Enter LPM0 w/ interrupt
}
/*ISR FOR TIMERB0 CCR0*/
#pragma vector=TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR (void){
    P1OUT &= ~(BIT0|BIT1);

    TB0CCTL0 &= ~BIT0;    //clears flags
}
/*ISR FOR TIMERB0 CCR1 and CCR2*/
#pragma vector=TIMER0_B1_VECTOR
__interrupt void Timer0_B1_ISR (void){

    switch (TB0IV){
    case 0x02:      //CCR1
        P1OUT |= BIT0;

        TB0CCTL1 &= ~BIT0;    //clears flags
        break;
    case 0x04:      //CCR2
        P1OUT |= BIT1;

        TB0CCTL2 &= ~BIT0;    //clears flags
        break;
    }
}

/*ISR FOR TIMERB1 CCR0*/
#pragma vector=TIMER1_B0_VECTOR
__interrupt void Timer1_B0_ISR (void){
    P1OUT &= ~BIT2;

    TB1CCTL0 &= ~BIT0;    //clears flags
}
/*ISR FOR TIMERB1 CCR1*/
#pragma vector=TIMER1_B1_VECTOR
__interrupt void Timer1_B1_ISR (void){
    P1OUT |= BIT2;

    TB1CCTL1 &= ~BIT0;    //clears flags
}
