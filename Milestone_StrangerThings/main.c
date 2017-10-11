/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2014, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************
 *
 *                       MSP430 CODE EXAMPLE DISCLAIMER
 *
 * MSP430 code examples are self-contained low-level programs that typically
 * demonstrate a single peripheral function or device feature in a highly
 * concise manner. For this the code may rely on the device's power-on default
 * register values and settings such as the clock configuration and care must
 * be taken when combining code from several examples to avoid potential side
 * effects. Also see www.ti.com/grace for a GUI- and www.ti.com/msp430ware
 * for an API functional library-approach to peripheral configuration.
 *
 * --/COPYRIGHT--*/
//******************************************************************************

#include <msp430.h> 

volatile int position = 0;
void Software_Trim();                       // Software Trim to get the best DCOFTRIM value
#define MCLK_FREQ_MHZ 8                     // MCLK = 8MHz

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
	TB0CCTL0 = CCIE;    // enable interrupts
	TB0CCR0 = 255;      //set PWM frequency to 1 KHz
	TB0CCR1 = 0;        //initialize red Duty cycle to CCR1
	TB0CCTL1 = CCIE;    // enable interrupts
	TB0CCR2 = 0;        //initialize green PWM to CCR2
	TB0CCTL2 = CCIE;    // enable interrupts

//initialize timer B1
	TB1CTL = ( MC__UP  + TBSSEL__SMCLK + ID__8);    //Set to up mode, run at SMCLK0 DIVIDE BY 8
    TB1EX0 = TBIDEX_3;      // DIVIDE BY 4
	TB1CCTL0 = CCIE;    // enable interrupts
	TB1CCR0 = 255;      //set PWM frequency to 2 Khz
	TB1CCR1 = 0;        //initialize blue PWM to B1CCR1
	TB1CCTL1 = CCIE;    // enable interrupts

	/* UART INIT */
	  __bis_SR_register(SCG0);                 // disable freqrency level lock
	  CSCTL3 |= SELREF__REFOCLK;               // Set REFO as FLL reference source
	  CSCTL1 = DCOFTRIMEN_1 | DCOFTRIM0 | DCOFTRIM1 | DCORSEL_3;// DCOFTRIM=3, DCO Range = 8MHz
	  CSCTL2 = FLLD_0 + 243;                  // DCODIV = 8MHz
	  __delay_cycles(3);
	  __bic_SR_register(SCG0);                // enable freqrency level lock
	  Software_Trim();                        // TI's code to get accurate uart clock

	  CSCTL4 = SELMS__DCOCLKDIV | SELA__REFOCLK; // set default REFO(~32768Hz) as ACLK source, ACLK = 32768Hz
	                                           // default DCODIV as MCLK and SMCLK source

	  // Configure UART pins
	  P1SEL0 |= BIT6 | BIT7;                    // set 2-UART pin as second function

	  // Configure UART
	  UCA0CTLW0 |= UCSWRST;
	  UCA0CTLW0 |= UCSSEL__SMCLK;           // use smclk

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
    if(TB0CCR1 != 0)          //if duty cycle is 0 keep output high
        P1OUT &= ~BIT0;

    if(TB0CCR2 != 0)        //if duty cycle is 0 keep output high
        P1OUT &= ~BIT1;

    TB0CCTL0 &= ~BIT0;    //clears flags
}
/*ISR FOR TIMERB0 CCR1 and CCR2*/
#pragma vector=TIMER0_B1_VECTOR
__interrupt void Timer0_B1_ISR (void){

    switch (TB0IV){
    case 0x02:      //CCR1
        if(TB0CCR1 != 255)      //if duty cycle is 100%  keep output low
            P1OUT |= BIT0;

        TB0CCTL1 &= ~BIT0;    //clears flags
        break;
    case 0x04:      //CCR2
        if(TB0CCR2 != 255)      //if duty cycle is 100%  keep output low
            P1OUT |= BIT1;

        TB0CCTL2 &= ~BIT0;    //clears flags
        break;
    }
}

/*ISR FOR TIMERB1 CCR0*/
#pragma vector=TIMER1_B0_VECTOR
__interrupt void Timer1_B0_ISR (void){
    if(TB1CCR1 != 0)            //if duty cycle is 0 keep output high
        P1OUT &= ~BIT2;

    TB1CCTL0 &= ~BIT0;    //clears flags
}
/*ISR FOR TIMERB1 CCR1*/
#pragma vector=TIMER1_B1_VECTOR
__interrupt void Timer1_B1_ISR (void){
    if(TB1CCR1 != 255)      //if duty cycle is 100%  keep output low
        P1OUT |= BIT2;

    TB1CCTL1 &= ~BIT0;    //clears flags
}
/* ISR FOR UART RX */
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void){

  switch(__even_in_range(UCA0IV,USCI_UART_UCTXCPTIFG))
  {
    case USCI_NONE: break;
    case USCI_UART_UCRXIFG:
       switch(position){            //SWITCHES ON BYTE RECIEVED
       case 0:                      // byte 0 received
           while(!(UCA0IFG&UCTXIFG));    //waits for current transmission to finish
           UCA0TXBUF = UCA0RXBUF - 3;        // transmits the package total length -3
           __no_operation();
           break;
       case 1:
           TB0CCR1 = UCA0RXBUF;     // SET RED DUTY CYCLE
           break;
       case 2:
           TB0CCR2 = UCA0RXBUF;     // SET GREEN DUTY CYCLE
           break;
       case 3:
           TB1CCR1 = UCA0RXBUF;     // SET BLUE DUTY CYCLE
           break;
       default:                     // TRANSMITS THE REST OF THE DATA AS IT IS RECIEVED
          while(!(UCA0IFG&UCTXIFG));    //waits for current transmission to finish
          UCA0TXBUF = UCA0RXBUF;
          __no_operation();
           break;
       }

      position += 1;        //ADDS ONE TO POSITION
      break;
    case USCI_UART_UCTXIFG: break;
    case USCI_UART_UCSTTIFG: break;
    case USCI_UART_UCTXCPTIFG: break;
    default: break;
  }
}

void Software_Trim()
{
    unsigned int oldDcoTap = 0xffff;
    unsigned int newDcoTap = 0xffff;
    unsigned int newDcoDelta = 0xffff;
    unsigned int bestDcoDelta = 0xffff;
    unsigned int csCtl0Copy = 0;
    unsigned int csCtl1Copy = 0;
    unsigned int csCtl0Read = 0;
    unsigned int csCtl1Read = 0;
    unsigned int dcoFreqTrim = 3;
    unsigned char endLoop = 0;

    do
    {
        CSCTL0 = 0x100;                         // DCO Tap = 256
        do
        {
            CSCTL7 &= ~DCOFFG;                  // Clear DCO fault flag
        }while (CSCTL7 & DCOFFG);               // Test DCO fault flag

        __delay_cycles((unsigned int)3000 * MCLK_FREQ_MHZ);// Wait FLL lock status (FLLUNLOCK) to be stable
                                                           // Suggest to wait 24 cycles of divided FLL reference clock
        while((CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1)) && ((CSCTL7 & DCOFFG) == 0));

        csCtl0Read = CSCTL0;                   // Read CSCTL0
        csCtl1Read = CSCTL1;                   // Read CSCTL1

        oldDcoTap = newDcoTap;                 // Record DCOTAP value of last time
        newDcoTap = csCtl0Read & 0x01ff;       // Get DCOTAP value of this time
        dcoFreqTrim = (csCtl1Read & 0x0070)>>4;// Get DCOFTRIM value

        if(newDcoTap < 256)                    // DCOTAP < 256
        {
            newDcoDelta = 256 - newDcoTap;     // Delta value between DCPTAP and 256
            if((oldDcoTap != 0xffff) && (oldDcoTap >= 256)) // DCOTAP cross 256
                endLoop = 1;                   // Stop while loop
            else
            {
                dcoFreqTrim--;
                CSCTL1 = (csCtl1Read & (~DCOFTRIM)) | (dcoFreqTrim<<4);
            }
        }
        else                                   // DCOTAP >= 256
        {
            newDcoDelta = newDcoTap - 256;     // Delta value between DCPTAP and 256
            if(oldDcoTap < 256)                // DCOTAP cross 256
                endLoop = 1;                   // Stop while loop
            else
            {
                dcoFreqTrim++;
                CSCTL1 = (csCtl1Read & (~DCOFTRIM)) | (dcoFreqTrim<<4);
            }
        }

        if(newDcoDelta < bestDcoDelta)         // Record DCOTAP closest to 256
        {
            csCtl0Copy = csCtl0Read;
            csCtl1Copy = csCtl1Read;
            bestDcoDelta = newDcoDelta;
        }

    }while(endLoop == 0);                      // Poll until endLoop == 1

    CSCTL0 = csCtl0Copy;                       // Reload locked DCOTAP
    CSCTL1 = csCtl1Copy;                       // Reload locked DCOFTRIM
    while(CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1)); // Poll until FLL is locked
}
