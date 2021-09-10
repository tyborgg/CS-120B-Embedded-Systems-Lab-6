/*	Author: Tyler Pastor tpast001@ucr.edu
 *  Partner(s) Name:  N/A
 *	Lab Section: 023
 *	Assignment: Lab #6  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	
 *	Demo Link: https://www.youtube.com/watch?v=TzyEWCt2Ios
 */
#include <avr/io.h>
#include "timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char A;
unsigned char curr;
int count;
int prev;

enum SM1_STATES {SM1_START, SM1_B0, SM1_B1, SM1_B2, SM1_stop, SM1_wait, SM1_wait2} SM1_STATE;
void Tick_Toggle() {
	switch(SM1_STATE) {
     		case SM1_START:	
      			SM1_STATE = SM1_B0;
         		break;

		case SM1_B0:
                       if(A == 0x01){
                                SM1_STATE = SM1_stop;
			}
		       	else if(A == 0x00 && count == 2){
				count = 0;
				SM1_STATE = SM1_B1;
			}
		       	else{
			      	count++;
			       	SM1_STATE = SM1_B0;
		       	}
			break;

		case SM1_B1:
			if(A == 0x01){
                                SM1_STATE = SM1_stop;
                        }
			else if(A == 0x00 && count == 2){
				count = 0;

				if(prev == 2){
				SM1_STATE = SM1_B0;
				}
				else{		
					SM1_STATE = SM1_B2;
				}
			}
			else{
				count++;
				SM1_STATE = SM1_B1;
			}
			break;

		case SM1_B2:
			if(A == 0x01){
				SM1_STATE = SM1_stop;
			}
			else if(A == 0x00 && count == 2){
				count = 0;
				SM1_STATE = SM1_B1;
			}
			else{
				count++;
				SM1_STATE = SM1_B2;
			}
			break;

		case SM1_stop:
			if(A == 0x01){
				SM1_STATE = SM1_stop;
			}
			else if(A == 0x00){
				SM1_STATE = SM1_wait;
			}
			else{
				SM1_STATE = SM1_stop;
			}
			break;

		case SM1_wait:
			if(A == 0x01){
				SM1_STATE = SM1_wait2;
			}
			else{
				SM1_STATE = SM1_wait;
			}
			break;

		case SM1_wait2:
			if(A == 0x01){
				SM1_STATE = SM1_wait2;
			}
			else{
				SM1_STATE = SM1_B0;
			}
			break;

      		default:
         		SM1_STATE = SM1_B0;
         		break;
   	}

   	switch(SM1_STATE) {
      		case SM1_B0:
			PORTB = 0x01;
			prev = 0;
			curr = 0x01;
			break;

		case SM1_B1:
			PORTB = 0x02;
			curr = 0x02;
			break;

		case SM1_B2:
			PORTB = 0x04;
			prev = 2;
			curr = 0x04;
			break;

		case SM1_stop:
			PORTB = curr;
			break;

		case SM1_wait:
			break;

		case SM1_wait2:
			break;

		case SM1_START:
			break;
	}
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
    	DDRB = 0xFF; PORTB = 0x00;

	TimerSet(100);
	TimerOn();

	SM1_STATE = SM1_START;
	
    	while (1) {
		A = ~PINA;
		Tick_Toggle();
		while(!TimerFlag);
		TimerFlag = 0;
    	}

   	return 0;
}
