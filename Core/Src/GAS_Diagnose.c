/*
 * GAS_Diagnose.c
 *
 *  Created on: Oct 30, 2021
 *      Author: aswww
 */

#include "GAS_Diagnose.h"
#include "GAS_Can.h"///
//diagonoseSet_t diagnoseSet; //221231_0136: Not used in this file: maybe GAS_Diagnose.h not used

BatteryDiagnose_t T_BatteryDiagnose; ///branchtest 1231_1558

volatile uint32_t ValueOfADC[2];
extern BatteryDiagnose_t T_BatteryDiagnose; ///T_BatteryDiagnose

void GAS_Diagnose_run_10ms(){

	T_BatteryDiagnose.B.prechargeStateSignal1 = HAL_GPIO_ReadPin(PrechargeStateSignal1_GPIO_Port,PrechargeStateSignal1_Pin);
	T_BatteryDiagnose.B.prechargeStateSignal2 = HAL_GPIO_ReadPin(PrechargeStateSignal2_GPIO_Port,PrechargeStateSignal2_Pin);
	T_BatteryDiagnose.B.RelayContactSignal1 = HAL_GPIO_ReadPin(RelayContactSignal1_GPIO_Port,RelayContactSignal1_Pin);
	T_BatteryDiagnose.B.RelayContactSignal2 = HAL_GPIO_ReadPin(RelayContactSignal2_GPIO_Port,RelayContactSignal2_Pin);
	T_BatteryDiagnose.B.RelayContactSignal3 = HAL_GPIO_ReadPin(RelayContactSignal3_GPIO_Port,RelayContactSignal3_Pin);
	T_BatteryDiagnose.B.TsalSignal = HAL_GPIO_ReadPin(TSALSignal_GPIO_Port,TSALSignal_Pin);
	T_BatteryDiagnose.B.IMDStatusFrequency = HAL_GPIO_ReadPin(IMDStatusSignal_GPIO_Port,IMDStatusSignal_Pin);
	T_BatteryDiagnose.B.Reserved			= 123;

	GAS_Can_sendMessage();

}
