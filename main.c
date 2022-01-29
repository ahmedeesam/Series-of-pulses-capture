/*
 * IR-Receiver_Transmitter_UPDATED.c
 *
 * Created: 29/01/2022 06:49:32 م
 * Author : ahmed essam
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "var.h"
#include "ext_int.h"
#include "timer0.h"
#include "timer1.h"
#include "uart.h"
#include "send_fun.h"



ISR (INT0_vect)
{
	if(puls_num > 0)
	{
		serial_debug("0");
		puls_time[puls_num] = read_tim1();
		recieved_ir = (recieved_ir << 1)|(0x01);	/* then its logic HIGH */
		puls_num++;
	}
	start_tim1_normal();
}

ISR (INT1_vect)
{
	serial_debug("1");
	puls_time[puls_num] = read_tim1();
	recieved_ir = (recieved_ir << 1);                /* then its logic low */
	puls_num++;
	start_tim1_normal();
}

int main(void)
{
	_delay_ms(3000);
	DDRD |= (1<<PORTD6);
	uart_init();
	int0_START();
	int1_init();
	
	
	while (1)
	{
		sei();             //open global interrupts                        
		_delay_ms(300);
		
		serial_debug("signal");
		signal_num++;
		itoa(signal_num,buffer2,9);
		serial_debug(buffer2);
		serial_debug(":");
		
		while(!(TIFR1 & (1<<TOV1)));     //wait until stop receiving, timer1 overflow works good
		cli();                            //disable global interrupts ,to not disrupting sending
		TIFR1 = (1<<TOV1);          //clear timer1 overflow flag
		trash = read_tim1();                //stop timer1
		_delay_ms(100);
		          //start 38461khz with dutycycle 50% ,time interval 13us

		serial_debug("sending:");
		serial_debug("puln:");
		itoa(puls_num,buffer1,17);
		serial_debug(buffer1);
		serial_debug(":");
		cpuls_num = puls_num;
		for (int i =0;i <= puls_num;i++)
		{
			if ((recieved_ir & (1<<cpuls_num)) == 0)   //send one
			{
				serial_debug("1");
				start_tim0_38khz();
				start_tim1_ctc(puls_time[i]);   //start timer1 to count by the same time for same pulse
				see_compare();
				stop_tim0_38khz();
			}
			else if ((recieved_ir & (1<<cpuls_num)) != 0)
			{
				serial_debug("0");
				stop_tim0_38khz();
				start_tim1_ctc(puls_time[i]);
				see_compare();
			}
			cpuls_num--;
		}
		stop_tim0_38khz();
		puls_num = 0;
		recieved_ir = 0;
		cpuls_num = 0;
	}
}

