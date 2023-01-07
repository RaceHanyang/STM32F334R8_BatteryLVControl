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


//BatteryTemp_t R_BatteryTemp; //230104: defined in GAS_Can.c

pwmIn_t pwmIn15;
pwmIn_t pwmIn16;
pwmIn_t pwmIn17;
/*230108_0300: added for fan selection*/
volatile uint8_t fanSelect0 = 0;
volatile uint8_t fanSelect1 = 0;

volatile uint8_t pwmChangeFlag; //230104: not used in this file//230108: use again

void GAS_PWM_inputInit(void);
void GAS_PWM_outputInit(void);
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
void GAS_PWM_Fan_run();
void GAS_PWM_Check(TIM_HandleTypeDef *htim, pwmIn_t *pwmIn);

void GAS_PWM_inputInit(void)
{

	HAL_TIM_IC_Start_IT(&htim15, TIM_CHANNEL_1);	//Fan_Tach_789
	HAL_TIM_IC_Start_IT(&htim16, TIM_CHANNEL_1);	//Fan_Tach_456
	HAL_TIM_IC_Start_IT(&htim17, TIM_CHANNEL_1);	//Fan_Tach_123


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

	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);//230105: why not existed?????

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
	 * 220108
	 * write 9 duties every time
	 * read only 3 fan duties at a time
	 */

	uint16_t T = R_BatteryTemp.B.HighestTemp; //230104: FIXED(/10 deleted)


	uint16_t per = T*4-60;
	if (per<=20) per=20;					//230105: under 20 degree C, min duty
	else if (per>=100) per = 100;			//230105: over 40 degree C, Max duty

	uint16_t fanPulse = 287*(per/100); 		//230105: not duty... its pulse!!!! => name change(duty->fanPulse)

	TIM1->CCR1=fanPulse;					//TIM1_CHANNEL1: fan control 3
	TIM1->CCR2=fanPulse;					//TIM1_CHANNEL2: fan control 2
	TIM1->CCR3=fanPulse;					//TIM1_CHANNEL3: fan control 1

	TIM2->CCR1=fanPulse;					//TIM1_CHANNEL1: fan control 4
	TIM2->CCR2=fanPulse;					//TIM1_CHANNEL2: fan control 5
	TIM2->CCR3=fanPulse;					//TIM1_CHANNEL3: fan control 6

	TIM3->CCR1=fanPulse;					//TIM1_CHANNEL1: fan control 7
	TIM3->CCR2=fanPulse;					//TIM1_CHANNEL2: fan control 8

	TIM3->CCR3=fanPulse;					//230105: why not existed???: fan control 9


	/*
	 * PWM input: select fan and calculate duty: 230108
	 */
	switch(pwmChangeFlag){
	case 1:
		HAL_GPIO_WritePin(Fan_Tach_S0_GPIO_Port, Fan_Tach_S0_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(Fan_Tach_S1_GPIO_Port, Fan_Tach_S1_Pin, GPIO_PIN_SET);
		GAS_PWM_Check(htim15, pwmIn15); //read fan7
		GAS_PWM_Check(htim16, pwmIn16); //read fan4
		GAS_PWM_Check(htim17, pwmIn17); //read fan1
		pwmChangeFlag ++;
		break;

	case 2:
		HAL_GPIO_WritePin(Fan_Tach_S0_GPIO_Port, Fan_Tach_S0_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(Fan_Tach_S1_GPIO_Port, Fan_Tach_S1_Pin, GPIO_PIN_SET);
		GAS_PWM_Check(htim15, pwmIn15); //read fan8
		GAS_PWM_Check(htim16, pwmIn16); //read fan5
		GAS_PWM_Check(htim17, pwmIn17); //read fan2
		pwmChangeFlag ++;
		break;

	case 3:
		HAL_GPIO_WritePin(Fan_Tach_S0_GPIO_Port, Fan_Tach_S0_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(Fan_Tach_S1_GPIO_Port, Fan_Tach_S1_Pin, GPIO_PIN_RESET);
		GAS_PWM_Check(htim15, pwmIn15); //read fan9
		GAS_PWM_Check(htim16, pwmIn16); //read fan6
		GAS_PWM_Check(htim17, pwmIn17); //read fan3
		pwmChangeFlag ++;
		break;
	default:
		pwmChangeFlag = 1;
	}



}


void GAS_PWM_Check(TIM_HandleTypeDef *htim, pwmIn_t *pwmIn){

			if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
			{
				pwmIn->RisingEdgeValue = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1); //CCR1: pulse
				htim->Instance->CNT = 0;
				pwmIn->Period=pwmIn->RisingEdgeValue;
			}
			else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
			{
				pwmIn->FallingEdgeValue=HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
				pwmIn->Width=pwmIn->FallingEdgeValue;
				pwmIn->DutyCycle = (float)(pwmIn->FallingEdgeValue)/(float)(pwmIn->RisingEdgeValue)*100;
			}

}

void GAS_FanSelect(uint8_t S0, uint8_t S1){

	uint8_t SS = S0+S1;

	if(SS == 0){
		pwmChangeFlag = 0;
	}else if(SS == 2){
		pwmChangeFlag = 1;
	}else if(S1 > S0){
		pwmChangeFlag = 2;
	}else if(S1 < S0){
		pwmChangeFlag = 3;
	}
}
