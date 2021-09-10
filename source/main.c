/*	Author: Tyler Pastor tpast001@ucr.edu
 *  Partner(s) Name:  N/A
 *	Lab Section: 023
 *	Assignment: Lab #6  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	
 *	Demo Link: 
 */
#include <avr/io.h>
#include "timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char A;
unsigned char B;
int count;

enum SM1_STATES {SM1_SMStart, SM1_up, SM1_down, SM1_Wait, SM1_reset, SM1_Waitup, SM1_Waitdown} SM1_STATE;
void Tick_Toggle() {
	switch(SM1_STATE) {
     		case SM1_SMStart:
      			SM1_STATE = SM1_Wait;
         		break;

		case SM1_Wait:
			if(A == 0x01 && B != 9){
				SM1_STATE = SM1_up;
			}
			else if(A == 0x02 && B != 0){
				SM1_STATE = SM1_down;
			}
			else if(A == 0x03){
				SM1_STATE = SM1_reset;
			}
			else{
				SM1_STATE = SM1_Wait;
			}
			break;

		case SM1_up:
			SM1_STATE = SM1_Waitup;
			break;

		case SM1_down:
			SM1_STATE = SM1_Waitdown;
			break;

		case SM1_reset:
			SM1_STATE = SM1_Wait;
			break;

		case SM1_Waitdown:
			if(A == 0x02 && count != 9){
				count++;
				SM1_STATE = SM1_Waitdown;
			}
			else if(A == 0x02 && count == 9){
				if(B > 0){
					PORTB--;
				}
				count = 0;
				SM1_STATE = SM1_Waitdown;
			}
			else if(A == 0x00){
				SM1_STATE = SM1_Wait;
			}
			else if(A == 0x03){
				SM1_STATE = SM1_reset;
			}
			break;

		case SM1_Waitup:
			if(A == 0x01 && count != 9){
				count++;
				SM1_STATE = SM1_Waitup;
			}
			else if(A == 0x01 && count == 9){
				if(B < 9){
                                	PORTB++;
				}
                                count = 0;
                                SM1_STATE = SM1_Waitup;
                        }
			else if(A == 0x00){
				SM1_STATE = SM1_Wait;
			}
			else if(A == 0x03){
				SM1_STATE = SM1_reset;
			}
			break;

      		default:
         		SM1_STATE = SM1_Wait;
         		break;
   	}

   	switch(SM1_STATE) {
      		case SM1_SMStart:
         		break;

		case SM1_Wait:
         		break;

		case SM1_up:
         		PORTB++;
			count = 0;
         		break;

      		case SM1_down:
         		PORTB--;
			count = 0;
        		break;

		case SM1_reset:
         		PORTB = 0x00;
        		break;

		case SM1_Waitdown:
        		break;

		case SM1_Waitup:
        		break;
	}
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;

	PORTB = 0x07;

	SM1_STATE = SM1_SMStart;
	
	TimerSet(100);
	TimerOn();

	while(1){
		A = ~PINA;
		B = PORTB;
		Tick_Toggle();
		while(!TimerFlag);
		TimerFlag = 0;
	}

	return 0;
}
