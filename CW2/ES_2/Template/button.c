#include "button.h"
#include "MK70F12.h"
// IRQC to 0xc
void button_init()
{
	// Button 0 (IRQ0): PortD/0
	// Button 1 (IRQ1): PortE/26


	// Enable clock gating to the correct pin port
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;
	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;

	// // Configure the BUTTON pins to be GPIOs

	//TOP BTN
	PORTD_PCR0 = PORT_PCR_MUX(1) | (0xc << 19) | PORT_PCR_DSE_MASK | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;

	//BOT BTN
	PORTE_PCR26 = PORT_PCR_MUX(1) | (0xc << 19) | PORT_PCR_DSE_MASK | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;


	//TOP BTN
	GPIOD_PDDR &= ~(1 << 0);

	//BOTTOM BTN
	GPIOE_PDDR &= ~(1 << 26);

}

int button_value(int button_id)
{
	switch(button_id)
	{
		case BUTTON_TOP:
			// Returns a non-zero value, just & is enough
			// Of course, 0 is counted as high, so inverted for clarity
			return !(GPIOD_PDIR & BUTTON_TOP_BIT);
		case BUTTON_BOTTOM:
			return !(GPIOE_PDIR & BUTTON_BOTTOM_BIT);
	}
}
