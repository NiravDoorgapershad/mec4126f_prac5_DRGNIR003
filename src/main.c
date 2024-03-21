// Written by: Nirav Doorgapershad
// Date created: 17/03/24
// Programmed in: STM32CubeIDE
// Target: STM32F051
// Description: Practical 5

// DEFINES AND INCLUDES
#define STM32F051
#include "stm32f0xx.h"
#include "lcd_stm32f0.h"
#define HIGH 1
#define LOW 0

// GLOBAL VARIABLES
enum motion{back=-1, stop, forward, right, left};
struct robot_status{
	enum motion movement;
	_Bool ir_left;
	_Bool ir_right;
} robot_state;


// GLOBAL CONSTANTS


// FUNCTION DECLARATIONS
void main(void);

// MAIN FUNCTION
void main(void)
{
	// Power on phase
	// Robot_state initialised
	robot_state.movement = stop;
	robot_state.ir_left = LOW;
	robot_state.ir_right = LOW;
	display();
	init_GPIOA();


	while(1){

	}
}

void display(void){

	int state_int = robot_state.movement;
	char state_string[20];
	char ir_state[20];

	    switch(state_int) {
	        case -1:
	            strcpy(state_string, "back");
	            break;
	        case 0:
	            strcpy(state_string, "stop");
	            break;
	        case 1:
	            strcpy(state_string, "forward");
	            break;
	        case 2:
	            strcpy(state_string, "right");
	            break;
	        case 3:
	            strcpy(state_string, "left");
	            break;
	        default:
	            strcpy(state_string, "unknown");
	            break;
	    }
	sprintf(ir_state, "IR0:%d-IR1:%d", robot_state.ir_left, robot_state.ir_right);
	init_LCD();
	lcd_command(CLEAR);
	lcd_command(TWOLINE_MODE);

	lcd_putstring(state_string);
	lcd_command(LINE_TWO);
	lcd_putstring(ir_state);

}

void init_GPIOA(void){
	RCC->AHBENR  |=  RCC_AHBENR_GPIOAEN;
	GPIOA -> MODER &= ~GPIO_MODER_MODER0_0;
	GPIOA -> MODER &= ~GPIO_MODER_MODER1_0;
	GPIOA -> PUPDR |= GPIO_PUPDR_PUPDR0_1;
	GPIOA -> PUPDR |= GPIO_PUPDR_PUPDR1_1;
}

void sensor_polling(void){

	if(GPIOA->IDR & GPIO_IDR_0 ){// if its high
		robot_state.ir_left = HIGH;
	}
	else{
		robot_state.ir_left = LOW;

	}
	if(GPIOA->IDR & GPIO_IDR_1 ){//if its high
		robot_state.ir_right = HIGH;
    }
	else{
		robot_state.ir_right = LOW;
	}

}
