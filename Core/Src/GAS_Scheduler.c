/*
 * GAS_Scheduler.c
 *
 *  Created on: Oct 30, 2020
 *      Author: Suprhimp
 */

#include "GAS_Scheduler.h"
#include "stdio.h"
#include "tim.h"

extern uint8_t canRx0Data[8];

uint16_t FANval;

uint8_t pin_state;
pwmIn_t pwmIn1;
pwmIn_t pwmIn2;

#define False 0
#define True 1

volatile int gTimerCnt;

//extern volatile uint8_t pwmChangeFlag;
//volatile uint8_t pwmChangeFlag;//1231_1619: original
Scheduler_task gTask;

void GAS_Scheduler_init(void);
void GAS_Scheduler_taskCounter_1ms(void);
void GAS_Scheduler_taskCounter_10ms(void);
void GAS_Scheduler_taskCounter_100ms(void);
void GAS_Scheduler_taskCounter_1000ms(void);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void GAS_Schduler(void);

void GAS_Scheduler_init(void)
{
//	if (HAL_TIM_Base_Start_IT(&htim3) != HAL_OK)
//	  {
//		  Error_Handler();
//	  }
	SysTick_Config(SystemCoreClock/1000);


//********PWM initialization********
	GAS_PWM_inputInit();
	GAS_PWM_outputInit();



	GAS_Can_init();

}



void GAS_Scheduler_taskCounter_1ms(void)
{

}

void GAS_Scheduler_taskCounter_10ms(void)
{
	GAS_Can_sendMessage();
//	GAS_Diagnose_run_10ms();
}

void GAS_Scheduler_taskCounter_100ms(void)
{
	GAS_PWM_Fan_run();
}
void GAS_Scheduler_taskCounter_1000ms(void)
{
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_13);
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_12);
//	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_15);
//	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_10);
}

void HAL_SYSTICK_Callback(void){
	gTimerCnt++;
			gTask.flag_1ms=True;
			if(gTimerCnt % 10 == 0)
			{
				gTask.flag_10ms=True;
			}
			if(gTimerCnt % 100 == 0)
			{
				gTask.flag_100ms=True;
			}

			if(gTimerCnt == 1000)
			{
				gTask.flag_1000ms=True;
				gTimerCnt = 0;
			}
}

//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//	gTimerCnt++;
//
//	gTask.flag_1ms=True;
//
//	if(gTimerCnt % 10 == 0)
//	{
//		gTask.flag_10ms=True;
//	}
//
//	if(gTimerCnt % 100 == 0)
//	{
//		gTask.flag_100ms=True;
//	}
//
//	if(gTimerCnt == 1000)
//	{
//		gTask.flag_1000ms=True;
//		gTimerCnt = 0;
//	}
//}

void GAS_Scheduler(void)
{


	if(gTask.flag_1ms == True)
	{
		gTask.flag_1ms = False;
		GAS_Scheduler_taskCounter_1ms();
	}

	if(gTask.flag_10ms == True)
	{
		gTask.flag_10ms = False;
		GAS_Scheduler_taskCounter_10ms();
	}

	if(gTask.flag_100ms == True)
	{
		gTask.flag_100ms = False;
		GAS_Scheduler_taskCounter_100ms();
	}

	if(gTask.flag_1000ms == True)
	{
		gTask.flag_1000ms = False;
		GAS_Scheduler_taskCounter_1000ms();
	}

//	GAS_Can_recieveMessage(&hcan);
//	HAL_CAN_IRQHandler(&hcan);
//	  pin_state=HAL_GPIO_ReadPin(GPIO_PIN_8);
}
