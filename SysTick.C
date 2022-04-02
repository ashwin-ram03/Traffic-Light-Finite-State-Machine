// SysTick.c
// Runs on TM4C123
// ASHWIN RAM 
// Last Modified: 1/11/2022
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"

// Initialize SysTick with a busy wait running at bus clock.
void SysTick_Init(void){
        	NVIC_ST_CTRL_R = 0;
        	NVIC_ST_RELOAD_R = 0x1387F; //initialize for 1ms
	NVIC_ST_CURRENT_R = 0;
        	NVIC_ST_CTRL_R = 0x05;
	 
}
// The delay parameter is in units of the 80 MHz core clock. (12.5 ns)
void SysTick_Wait(uint32_t delay){
  // EE319K students write this function
   NVIC_ST_RELOAD_R = delay-1;
   NVIC_ST_CURRENT_R = 0;
 
  while((NVIC_ST_CTRL_R&0x10000)==0){ //executes until flag bit is set
}

}
// 10000us equals 10ms




void SysTick_Wait10ms(uint32_t delay){
      	uint32_t i;
     	for(i=0; i<delay; i++){
         	SysTick_Wait(800000); //wait 10m/s

   	}
}
