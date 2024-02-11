#include <stdint.h>

#ifndef CONTROL_FUNCTION

#define CONTROL_FUNCTION



extern int GetMoveX(uint32_t DcVal, int *axis_flag_x, int *axis_flag_y); // Get axis direction & speed
extern int GetMoveY(uint32_t DcVal, int *axis_flag_x, int *axis_flag_y); // Get axis direction & speed
extern void clampX(); // Clamping / releasing X axis
extern void clampY();

extern void Commutation (int x_speed, int x_motion_flag, int y_speed, int y_motion_flag);

extern void pwm_com (int speed, int x_flag  , int y_flag); // PWM control function

//extern void CommutationY (int x_speed, int x_motion_flag);

#endif