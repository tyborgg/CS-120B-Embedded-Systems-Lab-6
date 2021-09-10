/*	Author: Tyler Pastor tpast001@ucr.edu
 *  Partner(s) Name:  N/A
 *	Lab Section: 023
 *	Assignment: Lab #6  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	
 *	Demo Link: https://www.youtube.com/watch?v=evDZEkpsZ7c
 */
#include <avr/io.h>
#include "timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum SM1_STATES {SM1_START, SM1_B0, SM1_B1, SM1_B2} SM1_STATE;
void Tick_Toggle() {
	switch(SM1_STATE) {
     		case SM1_START:
      			SM1_STATE = SM1_B0;
         		break;

		case SM1_B0:
			SM1_STATE = SM1_B1;
			break;

		case SM1_B1:
			SM1_STATE = SM1_B2;
			break;

		case SM1_B2:
			SM1_STATE = SM1_B0;
			break;

      		default:
         		SM1_STATE = SM1_B0;
         		break;
   	}

   	switch(SM1_STATE) {
      		case SM1_B0:
			PORTB = 0x01;
			break;

		case SM1_B1:
			PORTB = 0X02;
			break;

		case SM1_B2:
			PORTB = 0X04;
			break;

		case SM1_START:
			break;
	}
}

int main(void) {
    	DDRB = 0xFF; PORTB = 0x00;

	TimerSet(1000);
	TimerOn();

	SM1_STATE = SM1_START;
	
    	while (1) {
		Tick_Toggle();
		while(!TimerFlag);
		TimerFlag = 0;
    	}

   	return 0;
}
