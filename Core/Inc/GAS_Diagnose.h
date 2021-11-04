/*
 * GAS_Diagnose.h
 *
 *  Created on: Oct 30, 2021
 *      Author: aswww
 */

#ifndef INC_GAS_DIAGNOSE_H_
#define INC_GAS_DIAGNOSE_H_

#include "gpio.h"
#include "GAS_Vadc.h"
#include "GAS_Can.h"

typedef struct{
	uint8_t prechargeStateSignal1 : 1;
	uint8_t prechargeStateSignal2 : 1;
	uint8_t RelayContactSignal1   : 1;
	uint8_t RelayContactSignal2	  : 1;
	uint8_t RelayContactSignal3	  : 1;
	uint8_t TsalSignal			  : 1;
	uint8_t IMDStatusFrequency	  : 1;
	uint16_t FanPowerVoltage;
	uint16_t ImdOkVoltage ;

}diagonoseSet_t;

extern diagonoseSet_t diagnoseSet;

extern void GAS_Diagnose_run_10ms();

#endif /* INC_GAS_DIAGNOSE_H_ */
