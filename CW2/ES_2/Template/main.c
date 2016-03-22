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

char buff;

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

	// Configure the MCG - set up clock dividers on
	SIM_CLKDIV1 = SIM_CLKDIV1_OUTDIV1(0) | SIM_CLKDIV1_OUTDIV2(0) | SIM_CLKDIV1_OUTDIV3(1) | SIM_CLKDIV1_OUTDIV4(1);
	MCG_C1 = MCG_C1_CLKS(2);

	led_init();
	button_init();
	uartInit(UART2_BASE_PTR, 50000, 115200);
}

void main()
{
	// led_on(LED_RED);
	while(1)
	{
		// orginal: 10000000
		int timer = 1000000;
		while(timer--) ;

		// led_toggle(LED_RED);
		// led_toggle(LED_BLUE);

		if(button_value(0)) {
			led_on(LED_YELLOW);
			uartPuts(UART2_BASE_PTR, "Hello ");
		}

		if(button_value(1)) {
			led_off(LED_YELLOW);
			uartPuts(UART2_BASE_PTR, "World\n");
		}

		// echo character sent to device
		if(uartGetcharPresent(UART2_BASE_PTR)) {
			buff = uartGetchar(UART2_BASE_PTR);
			uartPutchar(UART2_BASE_PTR, buff);
		}

	}
}
