#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "../inc/I_O.h"

void I_O_Init(){
	
}

void Buttons_Init(void) {
	SYSCTL_RCGCGPIO_R |= 0x10;            // activate port E
	while ((SYSCTL_RCGCGPIO_R & 0x10) == 0x00);  //allow time to finish activating
	GPIO_PORTE_DIR_R &= ~0x2F;            // make PE3-0 in for buttons
  GPIO_PORTE_AFSEL_R &= ~0x2F;          // disable alt funct on PE3-0
  GPIO_PORTE_DEN_R |= 0x2F;             // enable digital I/O PE3-0
  GPIO_PORTE_AMSEL_R = 0;               // disable analog functionality on PE
	GPIO_PORTE_PCTL_R &= ~0x00F0FFFF; 		// configure PE3-0 as GPIO

	//Interrupt Enable for Stop Button PE0
	GPIO_PORTE_IS_R &= ~0x01;     				// PE0 is edge-sensitive
  GPIO_PORTE_IBE_R &= ~0x01;     				// PE0 is not both edges
	GPIO_PORTE_IEV_R |= 0x01;     				// PE0 is rising edges
	
	GPIO_PORTE_ICR_R = 0x01;      				// clear flag0
  GPIO_PORTE_IM_R |= 0x01;      				// arm interrupt on PE0
  NVIC_PRI11_R |= 0x00000080; 					// priority 4
  NVIC_EN0_R |= 0x0010;      						// enable interrupt 4 in NVIC  book page 269
}

void GPIOPortE_Handler(void){
  GPIO_PORTE_IM_R &= ~0x01;     // disarm interrupt on PE0 
	
}
