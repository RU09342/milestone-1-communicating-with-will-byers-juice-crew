# Milestone 1: Communicating with Will Byers
For this Milestone, the MSP430FR2311 was used to receive a UART string, set an RGB LED based on 3 bytes of the string, then transmit the string without the RGB bytes used on this node. 
## Introduction
As a leader in developing programs for Embedded Systems, Juice Crew LLC. has and will continue to develop solutions to many customer needs. In this specific product, our customer has requested that this solution be as cost effective as possible, without sacrificing performance. The specific need is to blink a RGB led from an array of 80 or less characters.
## Project Overview
The goal of this project was to recreate an addressable string RGB LEDs. This project stems from Will Byers' mother's Christmas Light Wall in the Netflix's "Stranger Things." The addressable string is made of many nodes in series. Each Node in the string is an indidual group with their own individual program. Because of this, the code must work no matter where and what comes before and after it.
## Program Overview
This program first starts by initializing the GPIO Pins and control registers. This allows for the program to run and function as properly intended. More detail can be found within the code's main function. From here, the LED is tested. The pro the processor goes into low power mode, then waits for a UART message. What happens during the 
### Recieving from UART
This program recieves the 3 RGB LED's output values from UART. The UART package can be anywhere between 5 and 80 bytes long, depending on the position of the node. ???
### UART to PWM
The UART code is read from the array and the Duty Cycle values are saved in the capture compare registers. Because of this and the fact that the timer interupts are enabled, the LED can be controlled by timer interupts. For example, the UART input for the red duty cycle will be saved directly to Timer B0  capture compare register 1 (TB0CCR1). The timer interupt on TB0CCR1 will then change the red duty cycle output directly. This code can be found on line ? in the milestone 1 program.
### Why the FR2311?
While FR2311 lacks the functionality compared to other MSP430 family controllers, it is currently available for $1.53 as compared to $2.66 for the next alternative: the MSP430G2553. 

One feature the FR2311 lack is 4 Capture Compare Registers on the same timer. One would control the PWM frequency and the other three would control each color of the RGB LED.  Because of this, the time needed to develop a working program will increase and thus increase engineering costs. 
Another feature lacking on the FR2311 is that the UART and some of the Capture Compare Registers hardware outputs are on the same pin so a hardware implementation of the software would not be viable. This would also increase necessary engineering hours and thus increase engineering costs.
In total, the program needed an estimated 2 hours of extra engineering compared to solutions of other chips. This overall hardware savings would pay off after only a small amount were sold.
