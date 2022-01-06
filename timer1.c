/*
 * timer1.c
 *
 * Created: 16/12/2021 10:30:04 م
 *  Author: ahmed essam
 */ 

#include "timer1.h"

void start_tim1_normal()
{
	TCNT1 = 0x00;
	TCCR1B |= (1<<CS12);
}

void start_tim1_ctc(int q)
{
	TCNT1 = 0x00;
	OCR1A = q;
	TCCR1B |= ((1<<CS12)|(1<<WGM12));
}

void see_compare()
{
	while(!(TIFR1 & (1<<OCF1A))); //wait until match
	TIFR1 |= (1<<OCF1A); //clear flag
	TCCR1B = 0; //stop timer
}

int read_tim1()
{
	int t;
	t = TCNT1;
	TCCR1B = 0;
	return t;
}

void tim1_interrupt_init()
{
	TIMSK1 = (1<<TOIE1);
}