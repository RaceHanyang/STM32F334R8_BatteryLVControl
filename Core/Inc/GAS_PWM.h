/*
 * GAS_PWM.h
 *
 *  Created on: Nov 6, 2020
 *      Author: Suprhimp
 */

#ifndef INC_GAS_PWM_H_
#define INC_GAS_PWM_H_

#include "tim.h"
#include "stm32f3xx_hal.h"
#include "GAS_Can.h"

typedef struct{
	uint16_t RisingEdgeValue;
	uint16_t FallingEdgeValue;
	uint8_t Period;
	uint8_t Width;
	uint8_t DutyCycle;
	uint8_t Frequency;
}pwmIn_t;

extern pwmIn_t pwmIn15;
extern pwmIn_t pwmIn16;
extern pwmIn_t pwmIn17;

extern volatile uint8_t pwmChangeFlag;
extern volatile uint8_t fanSelect0;//230108
extern volatile uint8_t fanSelect1;//230108


extern float FanPulse_SegmentIntake70;
extern float FanPulse_SideIntake;
extern float FanPulse_SegmentExhaust60;
extern float FanPulse_SegmentExhaust80;

extern void GAS_PWM_inputInit(void);
extern void GAS_PWM_outputInit(void);
extern void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
void GAS_PWM_Check(TIM_HandleTypeDef *htim, pwmIn_t *pwmIn);//230108

extern void GAS_PWM_Fan_run();

#endif /* INC_GAS_PWM_H_ */
