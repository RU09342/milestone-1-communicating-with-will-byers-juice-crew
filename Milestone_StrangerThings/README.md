# Milestone 1: Communicating with Will Byers
## Introduction
As a leader in developing programs for Embedded Systems, Juice Crew LLC. has and will continue to develop solutions to many customer needs. In this specific product, our customer has requested that this solution be as cost effective as possible, without sacrificing performance. The specific need is to control a RGB led from an array of 80 or less characters.
## Project Overview
The goal of this project was to create addressable RGB LED node. The idea is to be able to have a string of rgb led nodes with a cascading uart signal to control them all. This project stems from Will Byers' mother's Christmas Light Wall in the Netflix's "Stranger Things." The addressable LEDs are made of many nodes in series. Each Node in the string is a team's microprocessor running their own individual program. Because of this, if anyone's code does not work, the LEDs will not work properly. Another limitation is that the groups do not know where they will place in the series, therefore, the code must be independent of position.
### The Package
The package is made up of 80 bytes. The 80 byte package is sent to the first node, this node removes 3 bytes for its RGB values, then sends the new package of 77 bytes to the next node. This is why there is a limit of 26 nodes in the addressable LEDs. The most significant byte in the package is always the end of message check byte (0x0D). The data in the middle of the string is RGB duty cycle values in groups of 3. Then least significant byte holds the value for the total number of bytes in the string. 
## Program Overview
### Initialization
The program developed for this project first starts by initializing the GPIO Pins and control registers. The LED outputs were configured to be red at P1.0, green at P1.1, blue at P1.2. Because the RGB LED is in a common anode configuration the corresponding color of the LED is only on when the port output is low. Timer B0 was set to up mode, SMCLK, and internal divider set to 8, and then the expanded divider set to 4. The Timer B0 Capture Compare register 0 (TB0CCR0) and TB1CCR0 were set to 255 (sets maximum timer value) to set a PWM (pulse width modulation) frequency about 1 kHz. TB0CCTL0, TB0CCTL1, TB0CCTL2, TB1CCTL0, and TB1CCTL1 were set to enable interrupts. This allows for TB0CCR1, TB0CCR2, and TB1CCR1's values of 127, to set the duty cycle to 50%. This works by setting the P1.0, P1.1, and P1.2 (turning off red, green, and blue LEDs) when the timer is 127, and reseting P1.0, P1.1, and P1.2 (turning on red, green, and blue LEDs) when the timer is 256.

![alt text](https://github.com/RU09342/milestone-1-communicating-with-will-byers-juice-crew/blob/master/Milestone_StrangerThings/Schematic.JPG)


### Testing the LED
Once initialization is completed, the program runs a small test program before going into low power mode to verify the red, green and blue parts of the LED are working correctly. The code cycles the LED through the following colors with a delay inbetween:Red, yellow, green, light blue, blue, purple, and back to red. It does this simply by toggling the ports.
### Software PWM
The code uses software PWM instead of the onbard hardware PWM because the hardware PWM outputs share pins with the uart rx and tx pins. Software PWM is implemented by resetting the pins on the CCR0 interrupt and setting the pins on the CCR1 and CCR2 interrupts. While also using switch statements in the interrupt vectors to see what CCR triggered the interrupt. 
### UART to Output
After the intialization the processor is dropped into low power mode 3, and waits for a uart transmission with a uart rx interrupt. Upon receiving the first byte it subtracts 3 and writes the value to the uart tx buffer. It then returns to low power until the next re interrupt is triggered. On the next 3 rx interrupts code writes the byte received to the TB0CCR1, TB0CCR2, and TB1CCR1 received to set the duty cyle of the outputs. After that all bytes received are mirrored on the tx line until the end character is received at which point the processor is ready to receive another string of data.
### Why the FR2311?
While FR2311's pwm outputs are on the same pins as the UART recieve and transmit pins Because of this only software pwm can be used to have pwm outputs on other pins. this was favorable compared to other MSP430 family controllers, because it is currently available for $1.53 as compared to $2.66 for the next alternative: the MSP430G2553. The other processors cost upwards of $7 so they were not considered at all.

#### Working Around Limited Features
One feature the FR2311 lack is 4 Capture Compare Registers on the same timer. One would control the PWM frequency and the other three would control each color of the RGB LED.  Because of this, the time needed to develop a working program will increase and thus increase engineering costs. 
Another feature lacking on the FR2311 is that the UART and some of the Capture Compare Registers hardware outputs are on the same pin so a hardware implementation of the software would not be viable. This would also increase necessary engineering hours and thus increase engineering costs.
In total, the program needed an estimated 2 hours of extra engineering compared to solutions of other chips. This overall hardware savings would pay off after only a small amount were sold.
