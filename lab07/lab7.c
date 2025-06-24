/*****************************************************************
 * ledsw2.c
 * Adapted by Peter Loux Sr, from the original ledsw1.c by
 * Robert B. Reese, Bryan A. Jones, and J. W. Bruce (2008)
 * Mississippi State University, ECE3724 class usage only 
 *****************************************************************
 * Demonstrates using an FSM for a LED/switch/button IO problem.
 *****************************************************************
 * Start: The LED is off 
 * After a button press and release, the LED is turned on. 
 * After another press and release:
 *  if switch = 0, go back to start
 *  if switch = 1, blink
 * After 5 blinks (10 toggles), go to start
 * If button is pressed before 5 blinks, turn LED on
 * When button is released, go back to start 
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
	PRESSED_1,
	RELEASED_2,
	PRESSED_2,
	RELEASED_3,
	PRESSED_3
} state_t;

// state descriptions
const char* apsz_state_names[] = {		
	"RELEASED_1 - LED is off",
	"PRESSED_1 - LED is still off",
	"RELEASED_2 - LED is on",
	"PRESSED_2 - LED is still on",
	"RELEASED_3 - LED blinks 5x or until PB press",
	"PRESSED_3 - LED is on",
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
			LED1 = 0;
			if (PB_PRESSED()) {
				e_state = PRESSED_1;
			}
			break;

		case PRESSED_1:
			if (PB_RELEASED()) {
				e_state = RELEASED_2;
			}
			break;

		case RELEASED_2:
			LED1 = 1;
			if (PB_PRESSED()) {
				e_state = PRESSED_2;
			}
			break;

		case PRESSED_2:
			if (PB_RELEASED() && SW) {
				u16_led_toggles = 0;	// zero counter before blinking state
				e_state = RELEASED_3;
			}
			if (PB_RELEASED() && !SW) {
				e_state = RELEASED_1;
			}
			break;

		case RELEASED_3:
			LED1 = !LED1;		// blink
			u16_led_toggles++;
			printf("toggles = %d\n", u16_led_toggles);
			DELAY_MS(250);		// Delay to make LED blinks visible
      		if (u16_led_toggles >= 10) {
				e_state = RELEASED_1;
			}
			if (PB_PRESSED()) {
				e_state = PRESSED_3;
			}
			break;

		case PRESSED_3:
			LED1 = 1;
			if (PB_RELEASED()) {
				e_state = RELEASED_1;
			}
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
