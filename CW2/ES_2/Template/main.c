/*
 * Bare metal template created by Harry Wagstaff
 * Based on Freescale Codewarrior Bareboard project template
 * Edited by Stan Manilov
 */

#include "MK70F12.h"

#include "led.h"
// Files we have added
#include "button.h"
#include "uart.h"
#include "filter0.h"
#include "filter1.h"
#include "filter2.h"
#include "filter3.h"

short byte_in, byte_out;
float filter_in, filter_out;
int filter_state, button_pressed[2];

filter0Type *filter0;
filter1Type *filter1;
filter2Type *filter2;
filter3Type *filter3;

// General function for filters
// All filters are first order, so we don't expect fantastic results
float filter_function(float in) {
	switch(filter_state) {
		case 0: // 500-1000Hz Band Pass
			filter0_writeInput( filter0, in );
			return filter0_readOutput( filter0 );
			break;
		case 1: // 1500-1750Hz Band Pass
			filter1_writeInput( filter1, in );
			return filter1_readOutput( filter1 );
			break;
		case 2: // 2000-2500Hz Band Pass
			filter2_writeInput( filter2, in );
			return filter2_readOutput( filter2 );
			break;
		case 3: // 3000-3750Hz Band Pass
			filter3_writeInput( filter3, in );
			return filter3_readOutput( filter3 );
			break;
	}
}

// __init_hardware is called by the Freescale __thumb_startup function (see
// vectors.c)
void __init_hardware()
{
	// Disable the Watchdog module. This module is designed to reset
	// the board in case it crashes for some reason. We don't need it,
	// so we disable it here.
	WDOG_UNLOCK = 0xC520;
	WDOG_UNLOCK = 0xD928;
	WDOG_STCTRLH = 0xD2;

	// Turn on FPU capability on processor
	SCB_CPACR = SCB_CPACR_CP10(1) | SCB_CPACR_CP11(1);

	// Configure the MCG - set up clock dividers on
	SIM_CLKDIV1 = SIM_CLKDIV1_OUTDIV1(0) | SIM_CLKDIV1_OUTDIV2(0) | SIM_CLKDIV1_OUTDIV3(1) | SIM_CLKDIV1_OUTDIV4(1);
	MCG_C1 = MCG_C1_CLKS(2);

	led_init();
	button_init();
	uartInit();

	// These will check whether we've hit the button (so we don't hit it millions
	// of times per second)
	button_pressed[0] = 0;
	button_pressed[1] = 0;

	// Current filter to be processed, initially 0
	filter_state = 0;
}

void main()
{

	filter0 = filter0_create();
	filter1 = filter1_create();
	filter2 = filter2_create();
	filter3 = filter3_create();

	while(1)
	{

		// top button
		if(button_value(0)) {
			// If we've not pressed it recently
			if (!button_pressed[0]) {
				button_pressed[0] = 1;

				filter_state += 1;

				// resets the filter state if we hit the end
				if (filter_state > 3) {
					filter_state = 0;
				}
			}

		} else {
			if(button_pressed[0]) {
				button_pressed[0] = 0;
			}
		}

		// bottom button
		if(button_value(1)) {
			// If we've not pressed it recently
			if (!button_pressed[1]) {
				button_pressed[1] = 1;

				filter_state -= 1;

				// resets the filter state if we hit the end
				if (filter_state < 0) {
					filter_state = 3;
				}
			}

		} else {
			if(button_pressed[1]) {
				button_pressed[1] = 0;
			}
		}

		// LED control based on state
		switch(filter_state) {
			case 0:  // Filter 0
				led_on(LED_BLUE);
				led_off(LED_GREEN);
				led_off(LED_YELLOW);
				led_off(LED_RED);
				break;
			case 1: // Filter 1
				led_off(LED_BLUE);
				led_on(LED_GREEN);
				led_off(LED_YELLOW);
				led_off(LED_RED);
				break;
			case 2: // Filter 2
				led_off(LED_BLUE);
				led_off(LED_GREEN);
				led_on(LED_YELLOW);
				led_off(LED_RED);
				break;
			case 3: // Filter 3
				led_off(LED_BLUE);
				led_off(LED_GREEN);
				led_off(LED_YELLOW);
				led_on(LED_RED);
				break;

		}

		// If we have some data, we want to filter it
		if(uartGetcharPresent(UART2_BASE_PTR)) {

			// Get the data
			byte_in = uartGetchar(UART2_BASE_PTR);

			// Convert unsigned 8 bit number (0-255) to a signed float between -1.0
			// and 1.0 so the filter can read it.
			filter_in = (byte_in > 127) ? (byte_in-256.0)/128.0 : (byte_in)/128.0;

			filter_out = filter_function(filter_in);

			// We don't have to convert it back to signed here as it should be the
			// same. Fun quirk of the size of the data we work with here.
			byte_out = (short)(filter_out*128.0);

			uartPutchar(UART2_BASE_PTR, byte_out);
		}

	}

	// Filter cleanup (not used in typical operation)
	filter0_destroy(filter0);
	filter1_destroy(filter1);
	filter2_destroy(filter2);
	filter3_destroy(filter3);
}
