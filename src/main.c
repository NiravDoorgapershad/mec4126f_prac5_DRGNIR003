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


	while(1){

	}
}

void display(void){
	//this is a test

}
// FUNCTION DEFINITIONS
void lcd(void){


}
