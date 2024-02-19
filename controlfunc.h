#include <stdint.h>

#ifndef CONTROL_FUNCTION

#define CONTROL_FUNCTION



extern int GetMoveX(uint32_t DcVal, int *axis_flag_x, int *axis_flag_y); // Get axis direction & speed
extern int GetMoveY(uint32_t DcVal, int *axis_flag_x, int *axis_flag_y); // Get axis direction & speed
extern void clampX(void); // Clamping / releasing X axis
extern void clampY(void);

extern void Commutation (void);

extern void PwmCom (void); // PWM control function
extern void Indication (void);
//extern void CommutationY (int x_speed, int x_motion_flag);

extern void TestPwm(void);

#define X_SEL_LEFT 		0x40
#define X_SEL_RIGHT 	0x08

//volatile uint32_t inc;  // Machine feed velocity variable


#endif