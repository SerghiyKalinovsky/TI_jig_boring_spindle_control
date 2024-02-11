#include "tm4c123gh6pm.h"
#include <stdio.h>
#include <stdint.h>

//     My includes
#include "gpioinit.h"
#include "controlfunc.h"

int main(){
	
	GpioConf();
//	AdcInit();
	//TimerConf();
	//PwmConf();
	EncConf();
	
	volatile int x_val=0, y_val=0, result_x=0, result_y=0;
	int  x_motion_flag=0, y_motion_flag=0;
	
	
	//while (GPIOB->DATA & 0x10);					//w8 for press X clamp button
	//while (!(GPIOB->DATA & 0x10));
	
	while (1){
		
		
		
	//	clampX();
		//clampY();
	 	//result_y = ADCread(1);

		//x_val = GetMoveX(result_x, &x_motion_flag, &y_motion_flag);
    //y_val = GetMoveY(result_y, &x_motion_flag, &y_motion_flag);
		//Commutation(x_val, x_motion_flag, y_val, y_motion_flag); 
		//if ((!x_motion_flag)&&(y_motion_flag))
		//	CommutationY(y_val, y_motion_flag);
		
	
	}
	return 1;
	
}