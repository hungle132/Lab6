/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif


volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn(){

TCCR1B = 0x0B;

OCR1A = 125;

TIMSK1 = 0x02;

TCNT1 = 0;

_avr_timer_cntcurr = _avr_timer_M;

SREG |= 0x80;

}

void TimerOff(){
	TCCR1B = 0x00;
}

void TimerISR(){
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect){
	_avr_timer_cntcurr--;
	if(_avr_timer_cntcurr == 0){
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M){
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}


enum Light_States { Start,init,one,wait1,move1,two,three,four,five,six,seven,eight } state;

unsigned char but1 = 0x00;

void fsm(){
	
switch(state){

	but1 = ~PINA & 0x01;

	case Start:
		state = init;
		break;
	case init:
		state = one;
		break;
	case one:
		if(but1){
		state = wait1;
		}
		else if (!but1){
		state = two;
		}
		break;

	case wait1:
		if(!but1){
		state = move1;
		}
		else if (but1){
		state = wait1;
		}

		break;

	case move1:
		if(but1){

		state = two;
		}

		else{
		state = move1;
		}

		break;
	case two:
		state = three;
		break;
	case three:
		state = four;
		break;
	case four:
		state = five;
		break;

	case five:
		state = six;
		break;
	case six:
		state = seven;
		break;

	case seven:
		state = eight;
		break;
	
	case eight:
		state = one;
		break;

	default:
		break;
}

switch(state){

	case Start:
		break;
	case init:
		PORTB = 0;
		break;
	case one:
		PORTB = 1;
		break;
	case two:
		PORTB = 0x02;
		break;
	case three:
		PORTB = 0x04;
		break;
	case four:
		PORTB  = 0x02;
		break;
	case five:
		PORTB = 0x01;
		break;
	case six:
		PORTB = 0x02;
		break;
	case seven:
		PORTB = 0x04;
		break;
	case eight:
		PORTB = 0x02;
		break;

	default:
		break;
}

}


int main(void) {
    /* Insert DDR and PORT initializations */
DDRB = 0xFF; PORTB = 0x00;
TimerSet(100);
TimerOn();
//unsigned char tmpB = 0x00;
//TimerSet(1000);
//TimerOn();

state = init;
    /* Insert your solution below */
    while (1) {
//	tmpB = ~tmpB;
//	PORTB = tmpB;
	fsm();
	while(!TimerFlag);
	TimerFlag = 0;	
		
    }
    return 1;
}
