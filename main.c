#include "tm4c123gh6pm.h"
#include <stdio.h>
#include <stdint.h>

//     My includes
#include "gpioinit.h"
#include "controlfunc.h"

//volatile uint32_t inc;  // Machine feed velocity variable

int main(){
	
	GpioConf();
	EncConf();
	TimerInit();
	PwmConf();
	
	
	
	//while (GPIOB->DATA & 0x10);					//w8 for press X clamp button
	//while (!(GPIOB->DATA & 0x10));
	
	
	while (1){
		
		//GPIOA->DATA &= ~0x80; // y relay
//    GPIOB->DATA &= ~0x20;
		clampX();
		clampY();
		Indication();
	  Commutation();
		PwmCom();
	
	}
	
	return 1;
}
