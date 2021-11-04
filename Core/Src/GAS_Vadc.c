/*
 * GAS_Vadc.c
 *
 *  Created on: 2020. 10. 30.
 *      Author: Suprhimp
 */
#include <GAS_Can.h>
#include <GAS_Vadc.h>
#include "stdio.h"

uint32_t NumberOfVadc = 2;
volatile uint32_t ValueOfADC[2];

void GAS_Vadc_dmaIn(void);
void GAS_Vadc_getValue(void);


void GAS_Vadc_dmaIn(void)
{
	/*
	 * In F405R No need initialize for ADC DMA
	 * Just Start DMA only
	 * Analog data received in to array ValueOfADC
	 */
	if(HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ValueOfADC, NumberOfVadc) != HAL_OK)
	{
		Error_Handler();
	}


}

//void GAS_Vadc_getValue(void)
//{
//	/*
//	 *
//	 */
//	stm32_1.B.sensor0=ValueOfADC[0];
//	stm32_1.B.sensor1=ValueOfADC[1];
//	stm32_1.B.sensor2=ValueOfADC[2];
//	stm32_1.B.sensor3=ValueOfADC[3];
//	stm32_1.B.sensor4=ValueOfADC[4];
//}
