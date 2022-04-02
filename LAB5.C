//FINITE STATE MACHINE TRAFFIC LIGHT
// Runs on TM4C123
// ASHWIN RAM 
// Last Modified: MARCH 6, 2022

/* Option A1, connect LEDs to PB5-PB0, switches to PA5-3, walk LED PF321
   Option A2, connect LEDs to PB5-PB0, switches to PA4-2, walk LED PF321
   Option A6, connect LEDs to PB5-PB0, switches to PE3-1, walk LED PF321
   Option A5, connect LEDs to PB5-PB0, switches to PE2-0, walk LED PF321
   Option B4, connect LEDs to PE5-PE0, switches to PC7-5, walk LED PF321
   Option B3, connect LEDs to PE5-PE0, switches to PC6-4, walk LED PF321
   Option B1, connect LEDs to PE5-PE0, switches to PA5-3, walk LED PF321
   Option B2, connect LEDs to PE5-PE0, switches to PA4-2, walk LED PF321
  */
// east/west red light connected to bit 5
// east/west yellow light connected to bit 4
// east/west green light connected to bit 3
// north/south red light connected to bit 2
// north/south yellow light connected to bit 1
// north/south green light connected to bit 0
// pedestrian detector connected to most significant bit (1=pedestrian present)
// north/south car detector connected to middle bit (1=car present)
// east/west car detector connected to least significant bit (1=car present)
// "walk" light connected to PF3-1 (built-in white LED)
// "don't walk" light connected to PF3-1 (built-in red LED)
#include <stdint.h>
#include "SysTick.h"
#include "Lab5grader.h"
#include "../inc/tm4c123gh6pm.h"
// put both EIDs in the next two lines
char EID1[] = "ar67262"; //  ;replace abc123 with your EID
char EID2[] = "ar72248"; //  ;replace abc123 with your EID

void DisableInterrupts(void);
void EnableInterrupts(void);






struct State{ // define struct parameters
	uint16_t out;
	int32_t wait;
	uint16_t next[8]; // there are 8 different inputs so therefore this array is size 8
};

typedef struct State State_t; //define the struct

#define GoS 0
#define YlwS 1
#define AO1 2
#define GoW 3
#define YlwW 4
#define AO2 5
#define GoWalk 6
#define Flash1 7
#define Flash2 8
#define Flash3 9
#define Flash4 10
#define Flash5 11
#define Flash6 12
#define Flash7 13
#define Flash8 14
#define AO3 15

State_t FSM[16] = {
  {0x109, 160000000, {GoS, YlwS, GoS, YlwS, YlwS, YlwS, YlwS, YlwS}},
  {0x111, 80000000, {AO1, AO1, AO1, AO1, AO1, AO1, AO1, AO1}},
  {0x121, 40000000, {GoW, GoW, GoS, GoW, GoWalk, GoW, GoWalk, GoW}},
  {0x061, 160000000, {GoW, GoW, YlwW, YlwW, YlwW, YlwW, YlwW, YlwW}},
  {0x0A1, 80000000, {AO2, AO2, AO2, AO2, AO2, AO2, AO2, AO2}},
  {0x121, 40000000, {GoWalk, GoW, GoS, GoS, GoWalk, GoWalk, GoWalk, GoWalk}},
  {0x127, 160000000, {GoWalk, Flash1, Flash1, Flash1, GoWalk, Flash1, Flash1, Flash1}},
  {0x121, 8000000, {Flash2, Flash2, Flash2, Flash2, Flash2, Flash2, Flash2, Flash2}},
  {0x127, 8000000, {Flash3, Flash3, Flash3, Flash3, Flash3, Flash3, Flash3, Flash3}},
  {0x121, 8000000, {Flash4, Flash4, Flash4, Flash4, Flash4, Flash4, Flash4, Flash4}},
  {0x127, 8000000, {Flash5, Flash5, Flash5, Flash5, Flash5, Flash5, Flash5, Flash5}},
  {0x121, 8000000, {Flash6, Flash6, Flash6, Flash6, Flash6, Flash6, Flash6, Flash6}},
  {0x127, 8000000, {Flash7, Flash7, Flash7, Flash7, Flash7, Flash7, Flash7, Flash7}},
  {0x121, 8000000, {Flash8, Flash8, Flash8, Flash8, Flash8, Flash8, Flash8, Flash8}},
  {0x127, 8000000, {AO3, AO3, AO3, AO3, AO3, AO3, AO3, AO3}},
  {0x121, 40000000, {GoS, GoW, GoS, GoS, GoWalk, GoW, GoS, GoS}}
};






int main(void){
  DisableInterrupts();
  TExaS_Init(5); // initialize lab 5 grader
  SysTick_Init();   // Initialize SysTick for software waits
  // initialize system
uint16_t input;
uint16_t cState = GoS; //current state is initially south

SYSCTL_RCGCGPIO_R|=0x23; //TURN ON THE CLOCK FOR PORT A, B, F
SysTick_Wait10ms(2); //INITIALIZE CLOCK

GPIO_PORTA_DEN_R|=0x1C; //DIGITALLY ENABLE PINS 4, 3, 2
GPIO_PORTA_DIR_R&=0xE3  ; //we need to clear pins 4, 3, and 2 for inputs

GPIO_PORTB_DEN_R|=0x3F; //SET PINS 5, 4, 3, 2, 1, 0 TO DIGITALLY ENABLE
GPIO_PORTB_DIR_R|=0x3F; //SET PINS TO BE OUTPUTS (1S)

GPIO_PORTF_DEN_R|=0x0E; //DIGITALLY ENABLE PINS 1, 2, 3
GPIO_PORTF_DIR_R|=0X0E; //SET THOSE PINS TO 1, 2, 3


  EnableInterrupts();
  while(1){
	// 1) output
         	GPIO_PORTB_DATA_R = FSM[cState].out >>3; //shift right by 3
         	GPIO_PORTF_DATA_R = FSM[cState].out<<1; //shift left by 1
         	GPIO_PORTF_DATA_R&=0xF; //clear all bits besides PF3, PF2, and PF1
	// 2) wait
               	SysTick_Wait(FSM[cState].wait);
	// 3) input
            	input = (GPIO_PORTA_DATA_R &0x1C)>>2; //get bits 4-2, shift by 2
            	 
	// 4) next
           	cState = FSM[cState].next[input];

  }
}
