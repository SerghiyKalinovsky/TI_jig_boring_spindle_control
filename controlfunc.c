 /* controlfunc.c
 *
 *  Created on: 12 may  2022 
 *      Author: Sergey Kalinovskiy
 */


#include <stdio.h>
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "controlfunc.h"


void clampX(){

    static int status;

    switch (status)
    {
				case 1: 			// Program filter
					if (!(TIMER0->CTL & 0x1) && !(GPIOB->DATA  & 0x10))
						status = 11;
					if (!(TIMER0->CTL & 0x1) && (GPIOB->DATA  & 0x10))
						status = 0;	
					break;
						
				case 11:
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
                GPIOD->DATA |= 0x04;       //Turn off PD2
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
                GPIOD->DATA |= 0x02;       // Turn off PD1
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
            if (!(GPIOB->DATA  & 0x10)){
							TIMER0->TAILR = 0xbfffff;
							TIMER0->CTL |= 0x1;
                status = 1;

						}
    }

}



void clampY () {

    static int status;

    switch (status)
        {
						case 1:	// Program filter
							if (!(TIMER1->CTL & 0x1) && !(GPIOB->DATA  & 0x01))
								status = 11;
					
							if (!(TIMER1->CTL & 0x1) && (GPIOB->DATA  & 0x01))
								status = 0;
							break;
							
						case 11:
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
												TIMER1->CTL |=0x1;
                        status = 5;
                        }
                    break;

						case 5:
										if (!(TIMER1->CTL & 0x1))
											status =0;
										break;
            default :
                   if (!(GPIOB->DATA  & 0x01)) {										 // If PB0 = 0 Pressing button check
											TIMER1->TAILR = 0xbfffff;
											TIMER1->CTL |= 0x1;
											status = 1;
									 
									 }

        }
}

void Commutation (void){
	static int status;
	uint32_t time;
	if (QEI1->POS < 100)
		time = 0x02FFFFFE;
	if ((QEI1->POS > 100) && QEI1->POS <=150)
		time = 0x5FFFFFC;
	if ((QEI1->POS > 150)&& (QEI1->POS <=190))
		time = 0xBFFFFF8;
	
	switch (status)
	{
		//  =================  X Left Moving=======================
		case 11:
			if (!(GPIOD->DATA &0x40) && !(TIMER2->CTL & 0x1))
				status = 111;
			if ((GPIOD->DATA &0x40) && !(TIMER2->CTL & 0x1))
				status = 0;
		break;
		case 111:   
			GPIOB->DATA &= ~0x20;
			TIMER2->TAILR = 0xBFFFF;
			TIMER2->CTL |= 0x1;
			status = 12;
		break;
		case 12:
			if (!(TIMER2->CTL & 0x1)){
				GPIOB->DATA |= 0x04;
				status = 13;
			}
			break;
		case 13:
			if ((GPIOD->DATA & 0x40) ||(GPIOE->DATA & 0x04)){
				TIMER2->TAILR = 0x5FFF;
				TIMER2->CTL |= 0x1;
				status = 14;
			}
			break;
		case 14:
			if ( ( (GPIOD->DATA & 0x40) || (GPIOE->DATA & 0x04) ) && (!(TIMER2->CTL & 0x1)) ){
				TIMER2->TAILR = time;
				TIMER2->CTL |= 0x1;
				GPIOB->DATA &= ~0x04;
				status = 15;
			}
			if (!(GPIOD->DATA & 0x40) && (!(TIMER2->CTL & 0x1)) && !(GPIOE->DATA & 0x04) )
				status = 13;
			break;
		case 15:
			if (!(TIMER2->CTL & 0x1)){
				GPIOB->DATA |= 0x20;
				status =0;
			}
			break;
			
			//  =================  X right Moving=======================
		case 21:
			if (!(GPIOA->DATA & 0x08) && !(TIMER2->CTL & 0x1))
				status = 211;
			if ((GPIOA->DATA & 0x08) && !(TIMER2->CTL & 0x1))
				status = 0;
		break;
		case 211:   
			GPIOB->DATA &= ~0x20;
			TIMER2->TAILR = 0xBFFFF;
			TIMER2->CTL |= 0x1;
			status = 22;
		break;
		case 22:
			if (!(TIMER2->CTL & 0x1)){
				GPIOB->DATA |= 0x0C;
				status = 23;
			}
			break;
		case 23:
			if ((GPIOA->DATA & 0x08) || (GPIOB->DATA & 0x02)){
				TIMER2->TAILR = 0x5FFF;
				TIMER2->CTL |= 0x1;
				status = 24;
			}
			break;
		case 24:
			if ( ((GPIOA->DATA & 0x08)||(GPIOB->DATA & 0x02)) && (!(TIMER2->CTL & 0x1))){
				TIMER2->TAILR = time;
				TIMER2->CTL |= 0x1;
				GPIOB->DATA &= ~0x0C;
				status = 25;
			}
			if ((!(GPIOA->DATA & 0x08)) && (!(TIMER2->CTL & 0x1)) && (!(GPIOB->DATA & 0x02)))
				status = 23;
			break;
		case 25:
			if (!(TIMER2->CTL & 0x1)){
				GPIOB->DATA |= 0x20;
				status =0;
			}
			break;
		
			// ==================   Y upward motion ====================================
			
		case 31:   
			GPIOA->DATA &= ~0x80;
			TIMER2->TAILR = 0xBFFFF;
			TIMER2->CTL |= 0x1;
			status = 32;
		break;
		case 32:
			if (!(TIMER2->CTL & 0x1)){
				GPIOB->DATA |= 0x0C;
				status = 33;
			}
			break;
		case 33:
			if ((GPIOF->DATA & 0x01) || (GPIOE->DATA & 0x10)){
				TIMER2->TAILR = 0x5FFF;
				TIMER2->CTL |= 0x1;
				status = 34;
			}
			break;
		case 34:
			if ( ((GPIOF->DATA & 0x01)||(GPIOE->DATA & 0x10)) && (!(TIMER2->CTL & 0x1))){
				TIMER2->TAILR = time;
				TIMER2->CTL |= 0x1;
				GPIOB->DATA &= ~0x0C;
				status = 35;
			}
			if ((!(GPIOF->DATA & 0x01)) && (!(TIMER2->CTL & 0x1)) && (!(GPIOE->DATA & 0x10)))
				status = 33;
			break;
		case 35:
			if (!(TIMER2->CTL & 0x1)){
				GPIOA->DATA |= 0x80;
				status =0;
			}
			break;
			
			// ==================   Y downward motion ====================================
			
		case 41:   
			GPIOA->DATA &= ~0x80;
			TIMER2->TAILR = 0xBFFFF;
			TIMER2->CTL |= 0x1;
			status = 42;
		break;
		case 42:
			if (!(TIMER2->CTL & 0x1)){
				GPIOB->DATA |= 0x04;
				status = 43;
			}
			break;
		case 43:
			if ((GPIOD->DATA & 0x80) || (GPIOE->DATA & 0x02)){
				TIMER2->TAILR = 0x5FFF;
				TIMER2->CTL |= 0x1;
				status = 44;
			}
			break;
		case 44:
			if ( ((GPIOD->DATA & 0x80)||(GPIOE->DATA & 0x02)) && (!(TIMER2->CTL & 0x1))){
				TIMER2->TAILR = time;
				TIMER2->CTL |= 0x1;
				GPIOB->DATA &= ~0x04;
				status = 45;
			}
			if ((!(GPIOD->DATA & 0x80)) && (!(TIMER2->CTL & 0x1)) && (!(GPIOE->DATA & 0x02)))
				status = 43;
			break;
		case 45:
			if (!(TIMER2->CTL & 0x1)){
				GPIOA->DATA |= 0x80;
				status =0;
			}
			break;
			
			// ================== Default =========================
		default:
					   
			if ((!(GPIOD->DATA &0x40)) &&(!(GPIOE->DATA & 0x04)) && (GPIOE->DATA & 0x08)){  //X left moving GPIOD->DATA &0x40 - selector, GPIOE->DATA & 0x04 - limit sensor
				status = 11;
				TIMER2->TAILR = 0xBFFFFF;
				TIMER2->CTL = 0x01;
			}
			if (!(GPIOA->DATA & 0x08) &&(!(GPIOB->DATA & 0x02)) && (GPIOE->DATA & 0x08)){ //X Rigth moving GPIOA->DATA & 0x08 - selector, GPIOB->DATA & 0x02 - limit sensor
				status = 21;
				TIMER2->TAILR = 0xBFFFFF;
				TIMER2->CTL = 0x01;
			}
			
			if ((!(GPIOF->DATA & 0x01)) &&(!(GPIOE->DATA & 0x10)) && (GPIOE->DATA & 0x20)) // Y upward motion GPIOF->DATA &0x01 - selector,  GPIOE->DATA & 0x10  - limit sensor
				status =31;
			if ((!(GPIOD->DATA & 0x80)) && (!(GPIOE->DATA & 0x02)) && (GPIOE->DATA & 0x20)) // Y downward motion GPIOD->DATA & 0x80 - selector,  GPIOE->DATA & 0x02 - limit sensor
				status = 41;
			
	}
}	
	
 void Indication (void){
			
			if (!(GPIOE->DATA & 0x08)) // X clamping indication ON
					GPIOE->DATA |= 0x01;
			if (GPIOE->DATA & 0x08)    // X clamping Indication OFF
					GPIOB->DATA &= ~0x01;
			
			if (!(GPIOE->DATA & 0x20))   // Y clamping indication On
				GPIOC->DATA |= 0x10;
			if (GPIOE->DATA & 0x20)				// Y clamping indicaation Off
				GPIOB->DATA &= ~0x10;
			
	}	
 
	void PwmCom(){
		int period = 0, half_per=0;
		double t=0;
		volatile uint32_t inc;
		inc= QEI1->POS;
		
		if (inc<=100){
			t= inc *2.1;
			period = 790 - t;
			half_per = period/2;
		}
		if ((inc>100)&&(inc<=150)){
			t= (inc - 100) *8.4;
			period = 582 - t;
			half_per = period/2;
		}
		if (inc>150 && inc<=190){
			period = 158;
			half_per = period/2;
		}
		
		PWM0->_0_CTL |=0x0;
		PWM0->_0_LOAD = period;
		PWM0->_0_CMPA = half_per;
		PWM0->_0_CTL |=0x01;
		PWM0->ENABLE |=0x01;
	}

/*	extern void TestPwm(void){
		static int status, cnt;

		switch (status){
			case 10:
				PwmCom(cnt);
				TIMER3->TAILR = 0x02FFFFFE;
				TIMER3->CTL |= 0x01;
				status =20;
			break;
			
			case 20:
				if (!(TIMER3->CTL & 0x01)){
					cnt=cnt+10;
					status = 10;
				}
			break;
					
			default:
				cnt = 10;
				status = 10;
		}
		if (cnt == 190)
			status = 0;
			
		
	}
*/
