/*
 * external-INT-timer0.c
 *
 * Created: 14/12/2021 04:04:28 م
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
#include "lcd.h"



ISR (INT0_vect)
{
	if(puls_num > 0)
	{
		puls_time[puls_num] = read_tim1();
		recieved_ir = (recieved_ir << 1)|(0x01);	/* then its logic HIGH */
		puls_num++;
	}
	start_tim1_normal();
}

ISR (INT1_vect)
{
	puls_time[puls_num] = read_tim1(); 
	recieved_ir = (recieved_ir << 1);                /* then its logic low */
	puls_num++;
	start_tim1_normal();
}

ISR (TIMER0_COMPA_vect)
{
	g = ~g;
	if (g == 0)
	{
		PORTC &= ~(1<<PORTC0);
	}
	else if (g != 0)
	{
		PORTC |= (1<<PORTC0);
	}
	
}

int main(void)
{
	DDRC |= ((1<<PORTC0)|(1<<PORTC1)); 
	lcd_init();
	lcd_send_command(LCD_CMD_DISPLAY_NO_CURSOR);	
	int0_init();
	int1_init();
	 
    
    while (1) 
    {
		sei();  
		lcd_clear();                         //open global interrupts 
		lcd_goto_xy(0,0);
		lcd_write_word("signal:");
	
		
		signal_num++;
		itoa(signal_num,buffer2,9);	/* int to string conversion */ 
		lcd_write_word(buffer2); 
		
		while(!(TIFR1 & (1<<TOV1)));     //wait until stop receiving, timer1 overflow works good 
		cli();                            //disable global interrupts ,to not disrupting sending
		TIFR1 = (1<<TOV1);          //clear timer1 overflow flag
		trash = read_tim1();                //stop timer1
		_delay_ms(500);
		start_tim0_38khz();          //start 38461khz with dutycycle 50% ,time interval 13us
		lcd_clear();
		lcd_goto_xy(0,0);
		lcd_write_word("sending");
		cpuls_num = puls_num;
		for (int i =0;i <= puls_num;i++)
		{
			if ((recieved_ir & (1<<cpuls_num)) == 0)   //send zero
			{
				PORTC |= (1<<PORTC1);
				start_tim1_ctc(puls_time[i]);   //start timer1 to count by the same time for same pulse 
				see_compare();
			}
			else if ((recieved_ir & (1<<cpuls_num)) == 1)
			{
				PORTC &= ~(1<<PORTC1);
				start_tim1_ctc(puls_time[i]);
				see_compare();
			}
			cpuls_num -= 1;
			}
		PORTC &= ~(1<<PORTC1);
		stop_tim0_38khz();
		puls_num = 0;
		recieved_ir = 0;
    }
}
