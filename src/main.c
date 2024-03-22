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
	init_SW3();
	init_external_interrupts();
	init_GPIOB();


	while(1){
		sensor_polling();
		delay(100000);
		display();

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

void init_SW3(void){
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA -> MODER &= ~GPIO_MODER_MODER3;// set pushbuttons to input mode
	GPIOA -> PUPDR |= GPIO_PUPDR_PUPDR3_0;//enable the pull up resistor for button three
}

void init_external_interrupts(void){
	RCC-> APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN;//enable clock for sysconfig
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI3_PA;//route PA3 to EXTI3
	EXTI->IMR |= EXTI_IMR_MR3;//unmask the interrupt for pa3
	EXTI->FTSR |= EXTI_FTSR_TR3;//falling edge trigger
	NVIC_EnableIRQ(EXTI2_3_IRQn);//enable the ext interrupt on nvic
}

void init_GPIOB(void){
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    GPIOB->MODER |= (GPIO_MODER_MODER2_0 | GPIO_MODER_MODER6_0); // Set MODER bits to 01 (output mode)
}

void EXTI2_3_IRQnHandler (void){

	if(EXTI->PR &= EXTI_PR_PR3){
		if (robot_state.movement == stop){//if pushbutton 3 is pressed and state is stop
			robot_state.movement = forward;
			GPIOB->ODR = 0x44;
			display();
		}

		else if(robot_state.movement == forward){//if pushbutton 3 is pressed and state is stop
			robot_state.movement = stop;
			GPIOB->ODR = 0x00;
			display();
		}

	}

	EXTI->PR |= EXTI_PR_PR3;
}




