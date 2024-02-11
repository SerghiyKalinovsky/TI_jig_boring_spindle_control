 /* gpinint.c
 *
 *  Created on: 12 may  2022
 *      Author: Sergey Kalinovskiy
 */


#include <stdio.h>
#include <stdint.h>
#include "gpioinit.h"
#include "tm4c123gh6pm.h"

void EncConf(){

	SYSCTL->RCGCQEI |= 0x3;
	SYSCTL->RCGCGPIO |= 0x3F;

	
	// PC5 init PhA1 PC6 init PhB1
	GPIOC->AFSEL |= 0x60;
	GPIOC->DEN |= 0x60;
	GPIOC->PCTL |= 0x06600000;
		
	QEI1->CTL |=0x10;
	QEI1->MAXPOS = 0xC8; // 200 pulses per round
	QEI1->CTL |=0x01;
}


void GpioConf(){
	
	SYSCTL->RCGCGPIO |= 0xFF;

	while (!(SYSCTL->PRGPIO &= 0xFF)); // w8ing for GPIO clock ready

	// GPIOE INIT
	GPIOE->DIR |= 0x01;
	GPIOE->ODR = 0x00;
	GPIOE->DEN |= 0x3F;
	GPIOE->PUR |= 0x3F;
	
	// GPIOD Init
	GPIOD->DIR |= 0x0F;
	GPIOD->ODR |= 0x0F;
	GPIOD->DEN |= 0x0F;
	GPIOD->PUR |= 0x0F;
	GPIOD->DATA |= 0x0F; // Turn off PD0 - PD3 pins

	//GPIOB Init
	GPIOB->DIR |= 0x2c;
	GPIOB->ODR |= 0x20;
	GPIOB->DEN |= 0x3f;
	GPIOB->PUR |= 0x3f;
	GPIOB->DATA |= 0x20; // Turn off PB5 pin
	
	//GPIOA Init
	GPIOA->DIR |= 0xf0;
	GPIOA->ODR |= 0xe0;
	GPIOA->DEN |= 0xf0;
	GPIOA->PUR |= 0xf0;
	GPIOA->DATA |= 0xe0; //turn off PA5 - PA7 pins
	
	//GPIOC Init
	GPIOC->DIR |= 0x90;
	GPIOC->ODR = 0x00;
	GPIOC->DEN |= 0xf0;
	GPIOC->PUR |= 0x90;
	
	
}

void AdcInit(){
	
	// ADC0 - PD3,  ADC1 - PE1
	SYSCTL->RCGCADC = 0x3; // ADC clock enable
	while (!(SYSCTL->PRADC &= 0x3)); // w8ing for ADC module ready 
	
	// PE1 init
	GPIOE->AFSEL |=0x02;
	GPIOE->DEN &= ~0x02UL;
	GPIOE->AMSEL |=0x02;
	
	// PD3 init
	GPIOD->AFSEL |= 0x08;
	GPIOD->DEN &= ~0x08UL;
	GPIOD->AMSEL |= 0x08;
	
	// ADC0 init  - Y axis speed and direction
	ADC0->ACTSS &= ~0x08UL; // Disable SS3 during Configuration
	ADC0->EMUX &= ~0xF000UL;  // Software triger
	ADC0->SSMUX3 =4;      // channel selection
	ADC0->SSCTL3 |= 0x06;  // Take 1 sample at time, set flag at 1st sample
	ADC0->ACTSS |=0x8;  	// Enable ADC seq3
	
	// ADC1 init - X axis speed and direction
	ADC1->ACTSS &= ~0x08UL;
	ADC1->EMUX &= ~0xF000UL;
	ADC1->SSMUX3 = 1;
	ADC1->SSCTL3 |= 0x6;
	ADC1->ACTSS |=0x8;
}

void TimerConf(void){
	
	SYSCTL->RCGCTIMER = 0x3F; // Clock for Timer 0,1 enabled
	while (!(SYSCTL->PRTIMER &=0xF)); 
	
	// =========== timer 0 intit  =================
	TIMER0->CTL = 0;
	TIMER0->CFG = 0x00; // 32 bit timer
	TIMER0->TAMR |= 0x01; // 1 shot timer mode
	TIMER0->TAPR = 0x00; // prescale	
	TIMER0->TAILR = 0x017fffff; // 0x017fffff - 1 sec delay 50 mHz mcu freq
	// TIMER0->CTL = 0x1;
	
	// ============= timer 1 init ===========
	TIMER1->CTL = 0;
	TIMER1->CFG = 0x00; // 32 bit timer
	TIMER1->TAMR |= 0x01; // 1 shot timer mode
	TIMER1->TAPR = 0x00; // prescale	
	TIMER1->TAILR = 0x017fffff; // 0x017fffff - 1 sec delay 50 mHz mcu freq
	
	// ================== timer 2 intit ==========
	TIMER2->CTL = 0;
	TIMER2->CFG = 0x00; // 32 bit timer
	TIMER2->TAMR |= 0x01; // 1 shot timer mode
	TIMER2->TAPR = 0x00; // prescale	
	//TIMER1->TAILR = 0x017fffff; // 0x017fffff - 1 sec delay 50 mHz mcu freq
	
	// ================== timer 3 intit ==========
	TIMER3->CTL = 0;
	TIMER3->CFG = 0x00; // 32 bit timer
	TIMER3->TAMR |= 0x01; // 1 shot timer mode
	TIMER3->TAPR = 0x00; // prescale	
	//TIMER1->TAILR = 0x017fffff; // 0x017fffff - 1 sec delay 50 mHz mcu freq
	
	// ================== timer 3 intit ==========
	TIMER4->CTL = 0;
	TIMER4->CFG = 0x00; // 32 bit timer
	TIMER4->TAMR |= 0x01; // 1 shot timer mode
	TIMER4->TAPR = 0x00; // prescale	
	//TIMER1->TAILR = 0x017fffff; // 0x017fffff - 1 sec delay 50 mHz mcu freq
	
	
	
	
}

void PwmConf(void){
	
	SYSCTL->RCGCPWM = 0x1 ; // PWM clock en
	while (!(SYSCTL->PRPWM)); // w8 for PWM module readiness 
	// PB6 PWM pin config
	GPIOB->DIR |=0x40;
	GPIOB->DEN |=0x40;
	GPIOB->AFSEL |=0x40;
	GPIOB->PCTL |=0x04000000;
	
	SYSCTL->RCC |=  0x00100000;
	SYSCTL->RCC |=  0x000E0000; 
	
	PWM0->_0_CTL |=0x0;
	PWM0->_0_GENA |= 0x8c;
	PWM0->_0_LOAD = 0x018f; // set PWM period
	
	PWM0->_0_CMPA = 0x00c7; // set puls width 25%
	PWM0->_0_CTL |=0x1;
	//PWM0->ENABLE |=0x1;
	
}
