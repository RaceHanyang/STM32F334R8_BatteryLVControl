/*
 * GAS_PWM.c
 *
 *  Created on: Nov 6, 2020
 *      Author: Suprhimp
 *
 *      must calibrate the SeraRisingTime and Maxwidth by debugging.
 *      Check the Falling edge to find them!!!
 *      pwm1->R, pwm2->L
 */

#include "GAS_PWM.h"
#include "stdio.h"


BatteryTemp_t R_BatteryTemp;

pwmIn_t pwmIn15;
pwmIn_t pwmIn16;
pwmIn_t pwmIn17;

volatile uint8_t pwmChangeFlag = 0;

void GAS_PWM_inputInit(void);
void GAS_PWM_outputInit(void);
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
void GAS_PWM_Fan_run();
void GAS_PWM_Check(TIM_HandleTypeDef *htim, pwmIn_t *pwmIn);

void GAS_PWM_inputInit(void)
{

	HAL_TIM_IC_Start_IT(&htim15, TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&htim16, TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&htim17, TIM_CHANNEL_1);


}

void GAS_PWM_outputInit(void)
{
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);

	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);

	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);

	/*Turn on the fan!*/
	HAL_GPIO_WritePin(Fan_On_GPIO_Port, Fan_On_Pin, GPIO_PIN_SET);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	/*
	 * For PWM input function
	 * One PWM input per one htim channel(because of TIM counter)
	 * Period, width, duty cycle, frequency can be calculated.
	 */
	if(htim ->Instance == TIM15)
	{
		GAS_PWM_Check(htim, &pwmIn15);
	}
	else if(htim ->Instance == TIM16){
		GAS_PWM_Check(htim, &pwmIn16);
	}
	else if(htim ->Instance == TIM17){
		GAS_PWM_Check(htim, &pwmIn17);
	}
//	if(htim ->Instance == TIM4){
//		GAS_PWM_Check2(htim, &pwmIn3);
//		pwmChangeFlag = 1;
//	}
}

void GAS_PWM_Fan_run()
{
	/*
	 * PWM change duty cycle function
	 * Change duty cycle by changing register CCR directly
	 */

	uint16_t T = R_BatteryTemp.B.HighestTemp/10;

	uint16_t per = T*4-60;
	if (per<=20) per=20;
	else if (per>=100) per = 100;

	uint16_t duty = (per*287)/100;

	TIM1->CCR1=duty;
	TIM1->CCR2=duty;
	TIM1->CCR3=duty;

	TIM2->CCR1=duty;
	TIM2->CCR2=duty;
	TIM2->CCR3=duty;

	TIM3->CCR1=duty;
	TIM3->CCR2=duty;
}


void GAS_PWM_Check(TIM_HandleTypeDef *htim, pwmIn_t *pwmIn){

			if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
			{
				pwmIn->RisingEdgeValue = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
				htim->Instance->CNT = 0;
				pwmIn->Period=pwmIn->RisingEdgeValue;
			}
			else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
			{
				pwmIn->FallingEdgeValue=HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
				pwmIn->Width=pwmIn->FallingEdgeValue;
			}

}
