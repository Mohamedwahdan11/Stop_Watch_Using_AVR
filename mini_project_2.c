/*
 * mini_project_2.c
 *
 *  Created on: May 4, 2022
 *      Author: DELL
 */


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "timer.h"

unsigned char seconds=0;
unsigned char minutes=0;
unsigned char hours=0;

/*structure to hold timer conf*/
struct timer1_conf timer_1_a={0,15625,scale_64,COMPARE_A};

/*
 * Description :
 * Increment the number of seconds.
 */
void time_inc(void)
{
	seconds++;
}


/*
 * Description :
 * Intialize INT0.
 */

void INT0_int(void)
{
	DDRD&=~(1<<2);
	PORTD|=(1<<2);
	GICR|=(1<<INT0);
	MCUCR|=(1<<ISC01);

}

/*
 * Description :
 * Intialize INT1.
 */


void INT1_int(void)
{
	DDRD&=~(1<<3);
	GICR|=(1<<INT1);
	MCUCR|=(1<<ISC11)|(1<<ISC10);

}

/*
 * Description :
 * Intialize INT2.
 */

void INT2_int(void)
{
	DDRB&=~(1<<2);
	PORTB|=(1<<2);
	GICR|=(1<<INT2);

}


int main(void)
{
	DDRC=0xff;
	DDRA=0xff;

	INT0_int();
	INT1_int();
	INT2_int();

	timer1_init(&timer_1_a);
	timer_setCallBack_timer1(time_inc);

	SREG|=(1<<7);

//	TCCR1B=(1<<WGM12)|(1<<CS11)|(1<<CS10);


	while(1)
	{
		if(seconds==60)
		{
			minutes++;
			seconds=0;

		}
		if(seconds==60)
		{
			hours++;
			minutes=0;
		}
		if(hours>99)
		{
			hours=0;
			minutes=0;
			seconds=0;
		}


		PORTA=0x01;
		PORTC=(seconds%10);
		_delay_us(150);
		PORTA=0x02;
		PORTC=((seconds/10)%10);
		_delay_us(150);

		PORTA=0x04;
		PORTC=(minutes%10);
		_delay_us(150);
		PORTA=0x08;
		PORTC=((minutes/10)%10);
		_delay_us(150);

		PORTA=0x10;
		PORTC=(hours%10);
		_delay_us(150);
		PORTA=0x20;
		PORTC=((hours/10)%10);
		_delay_us(150);

	}

}




ISR(INT0_vect)
{
	seconds=0;
	minutes=0;
	hours=0;

}

ISR(INT1_vect)
{
	timer_timer1_deinit();
}


ISR(INT2_vect)
{
	timer1_init(&timer_1_a);
}
