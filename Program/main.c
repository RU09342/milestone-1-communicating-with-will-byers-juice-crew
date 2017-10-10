#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
//enable GPIO
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	PM5CTL0 &= ~LOCKLPM5; // Disable the GPIO power-on default high-impedance mode

//set red output as P1.6
	P1DIR|=BIT6;//sets P1.6 as output
	P1SEL0|=0;//selects TB0.1 as output
	P1SEL1|=BIT6;//selects TB0.1 as output

//set green output as P1.7
    P1DIR|=BIT7;//sets P1.7 as output
    P1SEL0|=0;//selects TB0.2 as output for P1.7
    P1SEL1|=BIT7;//selects TB0.2 as output for P1.7

//set blue output as P2.0
    P2DIR|=BIT0;//sets P2.0 as output
    P2SEL0|=BIT0;//selects TB1.1 as output for P2.0
    P2SEL1|=0;//selects TB1.1 as output for P2.0

//initialize timer B0
	TB0CTL= ( MC__UP  + TBSSEL__SMCLK);//Set to up mode, run at SMCLK
	TB0CCR0=500;//set PWM frequency to 2 Khz
	TB0CCR1=0;//initialize red Duty cycle to CCR1
	TB0CCTL1=(OUTMOD_7);//toggle red on CCR0, and reset red on CCR1
	TB0CCR2=0;//initialize green PWM to CCR2
	TB0CCTL2=(OUTMOD_7);//toggle green on CCR0, and reset green on CCR2

//initialize timer B1
	TB1CTL= ( MC__UP  + TBSSEL__SMCLK);//Set to up mode, run at SMCLK
	TB1CCR0=500;//set PWM frequency to 2 Khz
	TB1CCR1=0;//initialize blue PWM to B1CCR1
	TB1CCTL1=(OUTMOD_7);//toggle blue on CCR0, and reset blue on CCR1
}
