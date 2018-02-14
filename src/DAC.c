// use PB5-0 to implement DAC

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
// Code files contain the actual implemenation for public functions
// this file also contains an private functions and private data


// **************DAC_Init*********************
// Initialize 6-bit DAC, called once 
// Input: none
// Output: none
void DAC_Init(void){
  GPIO_PORTB_AMSEL_R &= ~0xFF;      // no analog
  GPIO_PORTB_PCTL_R &= ~0xFF; // regular GPIO function
  GPIO_PORTB_DIR_R |= 0x3F;      // make PB5-0 out
	GPIO_PORTB_DIR_R &= ~0xC0;			 // make PB6-7 input
  GPIO_PORTB_AFSEL_R &= ~0xFF;   // disable alt funct on PB7-0
  GPIO_PORTB_DEN_R |= 0xFF;      // enable digital I/O on PB7-0
	
}

// **************DAC_Out*********************
// output to DAC
// Input: 6-bit data, 0 to 63
// Output: none
void DAC_Out(uint32_t data){
	GPIO_PORTB_DATA_R &= ~0x3F;
	GPIO_PORTB_DATA_R |= data;
}
