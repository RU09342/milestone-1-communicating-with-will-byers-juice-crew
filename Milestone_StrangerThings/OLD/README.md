# Milestone 1: Communicating with Will Byers
For this Milestone, the MSP430FR2311 was used to recieve a UART string, set an RGB LED based on 3 bytes of the string, then transmit the string without the RGB bytes used on this node. 
##Why the FR2311?
While FR2311 lacks the functionality of 4 Capture compare registers on the same timer, it is currently avaliable for $1.53 as compared to $2.66 for the MSP430G2553. To work around this, 
