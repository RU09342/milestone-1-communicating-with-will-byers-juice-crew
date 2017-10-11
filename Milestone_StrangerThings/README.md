# Milestone 1: Communicating with Will Byers
## Introduction
As a leader in developing programs for Embedded Systems, Juice Crew LLC. has and will continue to develop solutions to many customer needs. In this specific product, our customer has requested that this solution be as cost effective as possible, without sacrificing performance. The specific need is to blink a RGB led from an array of 80 or less characters.
## Project Overview
The goal of this project was to create addressable RGB LEDs. This project stems from Will Byers' mother's Christmas Light Wall in the Netflix's "Stranger Things." The addressable LEDs are made of many nodes in series. Each Node in the string is a team's microprocessor running their own individual program. Because of this, if anyone's code does not work, the LEDs will not work properly. Another limitation is that the groups do not know where they will place in the series, therefore, the code must be independent of position.
## Program Overview
### Initialization
The program developed for this project first starts by initializing the GPIO Pins and control registers. The LED outputs were configure to be red at P1.0, green at P1.1, blue at P1.2. Timer B0 was set to up mode, 1MHz clock, and internal divider set to 8. The Timer B0 Capture Compare register 0 (TB0CCR0) and TB1CCR0 were set to 256 (sets maximum timer value) to set a PWM (pulse width modulation) frequency of 1 kHz. TB0CCTL1, TB0CCTL2, and TB1CCTL1 were set to enable interupts. This allows for TB0CCR1, TB0CCR2, and TB1CCR1's values of 125, to set the duty cycle to 50%?????????????. This works by setting the red, green, and blue LEDs when the timer is 125, and turning them off when the timer is 256.
### Testing the LED
Once initialization is completed, the program runs a small test program to verify the red, green and blue parts of the LED are working correctly. The funciton first sets the red LED (P1OUT bit 0) directly to 1 for 50,000 clock cycles then turns sets it back to 0. The funciton then sets the green LED (P1OUT bit 1) directly to 1 for 50,000 clock cycles then turns sets it back to 0. The function finally sets the blue LED (P1OUT bit 2) directly to 1 for 50,000 clock cycles then turns sets it back to 0. All the code for this can be found in xxxx.
### Recieving from UART
This program recieves the 3 RGB LED's output values from UART. The UART package can be anywhere between 5 and 80 bytes long, depending on the position of the node. ???
### UART to Output
The UART code is read from the array and the Duty Cycle values are saved in the capture compare registers. Because of this and the fact that the timer interupts are enabled, the LED can be controlled by timer interupts. For example, the UART input for the red duty cycle will be saved directly to Timer B0  capture compare register 1 (TB0CCR1). The timer interupt on TB0CCR1 will then change the red duty cycle output directly. This code can be found on line ? in the milestone 1 program.
### Why the FR2311?
While FR2311 lacks the functionality compared to other MSP430 family controllers, it is currently available for $1.53 as compared to $2.66 for the next alternative: the MSP430G2553. 

One feature the FR2311 lack is 4 Capture Compare Registers on the same timer. One would control the PWM frequency and the other three would control each color of the RGB LED.  Because of this, the time needed to develop a working program will increase and thus increase engineering costs. 
Another feature lacking on the FR2311 is that the UART and some of the Capture Compare Registers hardware outputs are on the same pin so a hardware implementation of the software would not be viable. This would also increase necessary engineering hours and thus increase engineering costs.
In total, the program needed an estimated 2 hours of extra engineering compared to solutions of other chips. This overall hardware savings would pay off after only a small amount were sold.
