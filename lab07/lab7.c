/****************************************************************
* Name
* ECE3724 - Section __
* Lab #7 - lab7.c
* Date
*****************************************************************
* Uses n state machine for a LED/switch/button IO problem.
*****************************************************************
* Start: 
* 
* 
* 
* 
* 
*		 
*****************************************************************/

#include <stdio.h>
#include "pic24_all.h"

/* LED config */
#define config_LED1() CONFIG_RB14_AS_DIG_OUTPUT()
#define LED1 (_LATB14)

/* Pushbutton (PB) config */
void config_PB()  {
	CONFIG_RB13_AS_DIG_INPUT();
	ENABLE_RB13_PULLUP();
	DELAY_US(1);			// Give the pullup some time to take effect.
}
# define PB_PRESSED()   (_RB13 == 0)
# define PB_RELEASED()  (_RB13 == 1)

/* Switch config */
void config_SW()  {
	CONFIG_RB12_AS_DIG_INPUT();
	ENABLE_RB12_PULLUP();
	DELAY_US(1);			// Give the pullup some time to take effect.
}
#define SW (_RB12)

/* State machine config */
// names of states
typedef enum  { 
	RELEASED_1,

} state_t;

// state descriptions
const char* apsz_state_names[] = {		
	"RELEASED_1 - LED is ____",

};

// prints state descriptions
void print_state(state_t e_state) {		
	static state_t e_last_state = 0xFFFF;	// forces initial print
	if (e_state != e_last_state) {			// only print if the state changes
		e_last_state = e_state;
		printf("%s\n", apsz_state_names[e_state]);
	}
}

/* State Machine */
void update_state(void) {
	static state_t e_state = RELEASED_1;	// starting state
	static uint16_t u16_led_toggles;		// 2 toggles = 1 blink

	switch (e_state) {
		case RELEASED_1:

			break;
		

			
		default:
			ASSERT(0);		// something's wrong - we're not in a state
	}
	print_state(e_state);
}

int main(void) {
	/* Hardware config */
	configBasic(HELLO_MSG);
	config_PB();
	config_SW();
	config_LED1();

	while (1) {
		update_state();
		DELAY_MS(DEBOUNCE_DLY); // debounce the switch 
	}
}
