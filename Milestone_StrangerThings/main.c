#include <msp430.h> 

volatile int position = 0;

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
	TB0CTL = ( MC__UP  + TBSSEL__SMCLK + ID__8);    //Set to up mode, run at SMCLK DIVIDE BY 8
	TB0EX0 = TBIDEX_3;      // DIVIDE BY 4
	TB0CCTL0 = CCIE;
	TB0CCR0 = 256;      //set PWM frequency to 1 KHz
	TB0CCR1 = 125;        //initialize red Duty cycle to CCR1
	TB0CCTL1 = CCIE;
	TB0CCR2 = 125;        //initialize green PWM to CCR2
	TB0CCTL2 = CCIE;

//initialize timer B1
	TB1CTL = ( MC__UP  + TBSSEL__SMCLK + ID__8);    //Set to up mode, run at SMCLK0 DIVIDE BY 8
    TB1EX0 = TBIDEX_3;      // DIVIDE BY 4
	TB1CCTL0 = CCIE;
	TB1CCR0 = 256;      //set PWM frequency to 2 Khz
	TB1CCR1 = 50;        //initialize blue PWM to B1CCR1
	TB1CCTL1 = CCIE;

	/* UART INIT */
	  __bis_SR_register(SCG0);                 // disable FLL
	  CSCTL3 |= SELREF__REFOCLK;               // Set REFO as FLL reference source
	  CSCTL1 = DCOFTRIMEN_1 | DCOFTRIM0 | DCOFTRIM1 | DCORSEL_3;// DCOFTRIM=3, DCO Range = 8MHz
	  CSCTL2 = FLLD_0 + 243;                  // DCODIV = 8MHz
	  __delay_cycles(3);
	  __bic_SR_register(SCG0);                // enable FLL

	  CSCTL4 = SELMS__DCOCLKDIV | SELA__REFOCLK; // set default REFO(~32768Hz) as ACLK source, ACLK = 32768Hz
	                                           // default DCODIV as MCLK and SMCLK source

	  // Configure UART pins
	  P1SEL0 |= BIT6 | BIT7;                    // set 2-UART pin as second function

	  // Configure UART
	  UCA0CTLW0 |= UCSWRST;
	  UCA0CTLW0 |= UCSSEL__SMCLK;

	  // Baud Rate calculation
	  // 8000000/(16*9600) = 52.083
	  // Fractional portion = 0.083
	  // User's Guide Table 17-4: UCBRSx = 0x49
	  // UCBRFx = int ( (52.083-52)*16) = 1
	  UCA0BR0 = 52;                             // 8000000/16/9600
	  UCA0BR1 = 0x00;
	  UCA0MCTLW = 0x4900 | UCOS16 | UCBRF_1;

	  UCA0CTLW0 &= ~UCSWRST;                    // Initialize eUSCI
	  UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt

	  __bis_SR_register(LPM3_bits|GIE);         // Enter LPM3, interrupts enabled
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
/* ISR FOR UART RX */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A0_VECTOR))) USCI_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(UCA0IV,USCI_UART_UCTXCPTIFG))
  {
    case USCI_NONE: break;
    case USCI_UART_UCRXIFG:
      while(!(UCA0IFG&UCTXIFG));
      UCA0TXBUF = UCA0RXBUF;
      __no_operation();
      break;
    case USCI_UART_UCTXIFG: break;
    case USCI_UART_UCSTTIFG: break;
    case USCI_UART_UCTXCPTIFG: break;
    default: break;
  }
}
