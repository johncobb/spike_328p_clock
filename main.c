/*
 * main.c
 *
 *  Created on: Dec 11, 2014
 *      Author: jcobb
 */


#define F_CPU	8000000

#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include "util/clock.h"
#include "util/log.h"




// log debugging
static const char _tag[] PROGMEM = "main: ";
volatile char term_in = 0;

// timeout helper
volatile clock_time_t future = 0;
bool timeout();
void set_timer(clock_time_t timeout);


void terminal_in_cb(uint8_t c)
{
	term_in = c;
	LOG("input=%c\r\n", c);
}

void main()
{
	debug_init(terminal_in_cb);
	clock_init();

	LOG("\r\n\r\spike_328p_clock starting...\r\n");
	sei();



	while(1) {
		if(timeout()) {

			LOG("time : %d\r\n", clock_time());
			set_timer(1000);
		}
	}
}

void set_timer(clock_time_t timeout)
{
	future = clock_time() + timeout;
}

// timeout routine to demonstrate clock_time
// being kept by pwm isr interrupt
bool timeout()
{
	bool timeout = false;

	if(clock_time() >= future)
	{
		set_timer(1000);
		timeout = true;

	}

	return timeout;
}


