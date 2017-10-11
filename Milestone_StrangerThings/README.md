# Milestone 1: Communicating with Will Byers
For this Milestone, the MSP430FR2311 was used to receive a UART string, set an RGB LED based on 3 bytes of the string, then transmit the string without the RGB bytes used on this node. 
##Introduction
As a leader in developing programs for Embedded Systems, Juice Crew LLC. has and will continue to develop solutions to many customer needs. In this specific product, our customer has requested that this solution be as cost effective as possible, without sacrificing performance. The specific need is to blink a RGB led from an array of 80 or less characters.
###Why the FR2311?
While FR2311 lacks the functionality compared to other MSP430 family controllers, it is currently available for $1.53 as compared to $2.66 for the next alternative: the MSP430G2553. 

One feature the FR2311 lack is 4 Capture Compare Registers on the same timer. One would control the PWM frequency and the other three would control each color of the RGB LED.  Because of this, the time needed to develop a working program will increase and thus increase engineering costs. 
Another feature lacking on the FR2311 is that the UART and some of the Capture Compare Registers hardware outputs are on the same pin so a hardware implementation of the software would not be viable. This would also increase necessary engineering hours and thus increase engineering costs.
In total, the program needed an estimated 2 hours of extra engineering compared to solutions of other chips. This overall hardware savings would pay off after only a small amount were sold.



