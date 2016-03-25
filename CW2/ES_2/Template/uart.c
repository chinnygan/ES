// Full disclosure this is heavily based on uart.c written by James L. Frankel
// Found here: http://sites.fas.harvard.edu/~libe251/fall2015/K70F120M/uart.c

#include <stdint.h>
#include "MK70F12.h"
#include "uart.h"

void uartInit() {
    uint16_t sbr, brfa;
    uint8_t temp;
    UART_MemMapPtr uartChannel;
    int clockInKHz, baud;

    uartChannel = UART2_BASE_PTR;
    clockInKHz = 50000;
    baud = 115200;

    /* Enable the clock to the selected Port & UART and set the pin MUX */
  	/* Enable clock for PORTE */
  	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;

  	/* Enable clock for UART2 */
  	SIM_SCGC4 |= SIM_SCGC4_UART2_MASK;

  	/* Enable the UART2_TXD function on PTE16 (Pin J3) */
  	PORTE_PCR16 = PORT_PCR_MUX(0x3); // UART is ALT3 function for this pin

  	/* Enable the UART2_RXD function on PTE17 (Pin K2) */
  	PORTE_PCR17 = PORT_PCR_MUX(0x3); // UART is ALT3 function for this pin

    /* Make sure that the transmitter and receiver are disabled while we
     * change settings.
     */
    UART_C2_REG(uartChannel) &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK );

    /* Configure the UART for 8-bit mode, no parity */
    UART_C1_REG(uartChannel) = 0;    	/* We need all default settings, so
    	    	    	    	    	   entire register is cleared */

    /* Calculate baud settings */
    sbr = (uint16_t)((clockInKHz * 1000)/(baud * 16));

    /* Save off the current value of the UARTx_BDH except for the SBR field */
    temp = UART_BDH_REG(uartChannel) & ~(UART_BDH_SBR(0x1F));

    UART_BDH_REG(uartChannel) = temp |  UART_BDH_SBR(((sbr & 0x1F00) >> 8));
    UART_BDL_REG(uartChannel) = (uint8_t)(sbr & UART_BDL_SBR_MASK);

    /* Determine if a fractional divider is needed to get closer to the baud
       rate */
    brfa = ((((uint32_t)clockInKHz*32000)/(baud * 16)) - (sbr * 32));

    /* Save off the current value of the UARTx_C4 register except for the BRFA
       field */
    temp = UART_C4_REG(uartChannel) & ~(UART_C4_BRFA(0x1F));

    UART_C4_REG(uartChannel) = temp |  UART_C4_BRFA(brfa);

    /* Enable receiver and transmitter */
    UART_C2_REG(uartChannel) |= (UART_C2_TE_MASK | UART_C2_RE_MASK );
}


char uartGetchar(UART_MemMapPtr uartChannel) {
    /* Wait until character has been received */
    while(!(UART_S1_REG(uartChannel) & UART_S1_RDRF_MASK)) {
    }

    /* Return the 8-bit data from the receiver */
    return UART_D_REG(uartChannel);
}


void uartPutchar(UART_MemMapPtr uartChannel, char ch) {
    /* Wait until space is available in the FIFO */
    while(!(UART_S1_REG(uartChannel) & UART_S1_TDRE_MASK)) {
    }

    /* Send the character */
    UART_D_REG(uartChannel) = (uint8_t)ch;
}


int uartGetcharPresent(UART_MemMapPtr uartChannel) {
    return (UART_S1_REG(uartChannel) & UART_S1_RDRF_MASK) != 0;
}
