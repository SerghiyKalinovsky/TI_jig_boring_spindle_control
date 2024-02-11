 /* controlfunc.c
 *
 *  Created on: 12 may  2022 
 *      Author: Sergey Kalinovskiy
 */


#include <stdio.h>
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "controlfunc.h"






int GetMoveX (uint32_t DcVal, int *axis_flag_x,int *axis_flag_y){

    int speed_val=0;

    // X speed equation
    if (!(*axis_flag_y)){
        if (1600<=DcVal<2460)
            *axis_flag_x = 0;

            if (DcVal<1600){

            *axis_flag_x = 1;
            speed_val =  DcVal/4;

						}

        if (DcVal>=2496){
            *axis_flag_x = 1;
            speed_val = ((4096- DcVal) / 4)*(-1);

        }
    }
		else 
			*axis_flag_x = 0;
		return speed_val;
	}

int GetMoveY(uint32_t DcVal, int *axis_flag_x, int *axis_flag_y){
	
	// Y speed equation
    int speed_val=0;
       if (!(*axis_flag_x)){
           if (1600<=DcVal<2460)
               *axis_flag_y = 0;

           if (DcVal<1600){

               *axis_flag_y = 1;
               speed_val =  DcVal/4;

           }

           if (DcVal>=2496){
               *axis_flag_y = 1;
               speed_val = ((4096- DcVal) / 4)*(-1);

           }
       }
			 else 
				 *axis_flag_y = 0;
       return speed_val;
}


void clampX(){

    static int status;

    switch (status)
    {
        case 1:
            if (GPIOB->DATA  & 0x10)
                status = 2;
            break;
        case 2:
            if (!(GPIOE->DATA & 0x20)){      // If PE5 = 0
                GPIOD->DATA &= (~0x04);          // turn on PD2 free
                status = 3;
                }
            if (GPIOE->DATA & 0x20 && status != 3) {        // If PE5 = 1
                GPIOD->DATA &= (~0x02);       // turn on PD1 fix
                status = 4;
            }
            break;

        case 3:                                 // Release X axis sequence
            if (GPIOE->DATA & 0x20) {    // W8 for PE5=1
							TIMER0->TAILR = 0x017fffff;
              TIMER0->CTL |=0x1;             // 1 sec delay timer on
              status = 31;
            }
            break;
        case 31:
            if (!(TIMER0->CTL & 0x1)){           // 1 sec delay timer check
                GPIOD->DATA |= 0x4;       //Turn off PD2
							  TIMER0->TAILR = 0x027fffff;
								TIMER0->CTL |=0x1;        
                status = 5;
            }
            break;


        case 4:                                 // Fixing X axis sequence
            if (!(GPIOE->DATA & 0x20)){  //W8 for PE5 = 0
								TIMER0->TAILR = 0x017fffff;
                TIMER0->CTL |=0x1;              // 1 sec delay timer on

                status = 41;
            }

            break;
        case 41:
            if (!(TIMER0->CTL & 0x1)){           // 1 sec delay timer check
                GPIOD->DATA |= 0x2;       // Turn off PD1
                status = 5;
								TIMER0->TAILR = 0x027fffff;
							  TIMER0->CTL |=0x1;
            }
            break;
						
				case 5:
						if (!(TIMER0->CTL & 0x1))
							status =0;
						break;
						
        default :
            if (!(GPIOB->DATA  & 0x10))
                status = 1;

    }

}



void clampY () {

    static int status;

    switch (status)
        {
            case 1:
                if (GPIOB->DATA  & 0x01)  // If PB0 = 1 Releasing button check
                    status = 2;
                break;

            case 2:
                       if (!(GPIOE->DATA & 0x08)){      // If PE3 = 0
                           GPIOA->DATA &= ~0x40;          // turn on PA6
                           status = 3;
                           }
                       if (GPIOE->DATA & 0x08 && status !=3) {        // If PE3 = 1
                           GPIOA->DATA &= ~0x20;       // turn on PA5
                           status = 4;
                       }
                       break;

            case 3:                                 // Release Y axis sequence
                    if (GPIOE->DATA & 0x08) {    // W8 for PE3=1
											  TIMER1->TAILR = 0x017fffff;
                        TIMER1->CTL |=0x1;             // 1 sec delay timer on


                        status = 31;
                    }
                    break;
            case 31:
                    if (!(TIMER1->CTL & 0x1)){           // 1 sec delay timer check
                        GPIOA->DATA |= 0x40;       //Turn off PA6
												TIMER1->TAILR = 0x027fffff;
                        status = 5;
                    }
                    break;

           case 4:                                 // Fixing Y axis sequence
                    if (!(GPIOE->DATA & 0x08)){  //W8 for PE3 = 0
                        TIMER1->TAILR = 0x017fffff;
												TIMER1->CTL |=0x1;              // 1 sec delay timer on

                        status = 41;
                        }

                    break;
            case 41:
                    if (!(TIMER1->CTL & 0x1)){           // 1 sec delay timer check
                        GPIOA->DATA |= 0x20;       // Turn off PA5
												TIMER1->TAILR = 0x027fffff;
                        status = 5;
                        }
                    break;

						case 5:
										if (!(TIMER1->CTL & 0x1))
											status =0;
										break;
            default :
                   if (!(GPIOB->DATA  & 0x01))   // If PB0 = 0 Pressing button check
                       status = 1;


        }
}



//====================================================
// ======================PWM  ===============
//====================================================
void pwm_com (int speed, int x_flag, int y_flag){
	
	static int status_x, status_y;
	int speed_rev=0;
	
	if (x_flag){
		if (speed >0 &&(!(GPIOB->DATA & 0x02)))
			GPIOD->DATA &= ~0x80;   // Turn on invereter forward PD7-> gnd
	
		if (speed <0 && (!(GPIOA->DATA & 0x10))) 
			GPIOD->DATA &= ~0xC0;  // Turn on inverterd reverse PD7, PD6 -> gnd 
		switch (status_x){
			case (11):
				if (!(GPIOB->DATA & 0x02))	
					status_x=0;
				break;
			default:
				if (GPIOB->DATA & 0x02 || GPIOA->DATA & 0x10 ){  // Turn off inverter when limit switch pressed
					GPIOD->DATA |= 0xC0;
					status_x = 11;
				}
								
		}
	}
	
	if (y_flag){
		if (speed >0 &&(!(GPIOE->DATA & 0x10)))
			GPIOD->DATA &= ~0x80;   // Turn on invereter forward PD0-> gnd
	
		if (speed <0 && (!(GPIOE->DATA & 0x02))) 
			GPIOD->DATA &= ~0xC0;  // Turn on inverterd reverse PD0, PD6 -> gnd  
		switch (status_y){
			case (11):
				if (!(GPIOE->DATA & 0x10) || !(GPIOE->DATA & 0x02))	
					status_y=0;
				break;
			default:
				if (GPIOE->DATA & 0x10 || GPIOE->DATA & 0x02 ){  // Turn off inverter when limit switch pressed
					GPIOD->DATA |= 0xC0;
					status_y = 11;
				}
								
		}
	}
	
	
	if (speed){
  
        if  (0>speed)
            speed_rev = speed *(-1);
        else
            speed_rev = speed;

        PWM0->_0_CTL &= ~0x00000001;

        PWM0->_0_CMPA = 0x00000000;
        PWM0->_0_CMPA |= speed_rev;

        PWM0->_0_CTL  |= 0x000000001;
        PWM0->ENABLE |= 0x000000001;


    }
	else {
		GPIOD->DATA |= 0xC0; // Turn off inverter 
		PWM0->_0_CTL &= ~0x01;
		PWM0->ENABLE &= ~0x01;
	}
}

//====================================================
// ======================Commutation  ===============
//====================================================
void Commutation (int x_speed, int x_motion_flag, int y_speed, int y_motion_flag){
	
	int speed_rev=0;
	static int status;
	switch (status){
// ================== X Axis commutation =================
		case 11:
			if (!(TIMER3->CTL & 0x1)){
				//GPIOB->DATA &= ~0x20;
				status=12;
			}
			break;
		case 12:
			if ((x_motion_flag)&&(GPIOE->DATA & 0x20)){
				pwm_com(x_speed, 1, 0);
			}
			else {
				pwm_com(0,0,0);
				TIMER3->TAILR = 0x077FFFFB;
				TIMER3->CTL |=0x1;
				status = 13;
				}
			break;
		case 13:
			if 	(!(TIMER3->CTL & 0x1)){
				GPIOB->DATA |= 0x20;
				status = 0;
			}
		break;

// ================== Y Axis commutation =================
			
		case 21:
			if (!(TIMER3->CTL & 0x1)){
				status=22;
			}
			break;
		case 22:
			if ((y_motion_flag)&&(GPIOE->DATA & 0x08)){
				pwm_com(y_speed, 0, 1);
			}
			else {
				pwm_com(0,0,0);
				TIMER4->TAILR = 0x077FFFFB;
				TIMER4->CTL |=0x1;
				status = 23;
				}
			break;
		case 23:
			if 	(!(TIMER4->CTL & 0x1)){
				GPIOA->DATA |= 0x80;
				status = 0;
			}
		break;
			
// ================== Default =================
		default:
			if ((x_motion_flag)&&(GPIOE->DATA & 0x20) && (!y_motion_flag)){
				GPIOB->DATA &= ~0x20;
				TIMER3->TAILR = 0x017fffff; // 1  sec delay
				TIMER3->CTL |=0x1;
				status=11;
			}
			if ( (!x_motion_flag) && y_motion_flag && (GPIOE->DATA & 0x08)){
				GPIOA->DATA &= ~0x80;
				TIMER4->TAILR = 0x017fffff; // 1 sec delay
				TIMER4->CTL |=0x1;
				status=21;
			}
		}
				
}
	


