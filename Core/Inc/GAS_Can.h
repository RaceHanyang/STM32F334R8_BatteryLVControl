/*
 * CAN.h
 *
 *  Created on: 2020. 10. 27.
 *      Author: Suprhimp
 */

#ifndef SRC_GETANDSEND_H_
#define SRC_GETANDSEND_H_

#include "stm32f3xx_hal.h"
#include "can.h"


typedef union{
	uint8_t TxData[8];
	uint8_t RxData[8];
	struct{ //230104: unsinged int => uint8_t
		uint8_t HighestTemp		;	//HighestTemp
		uint8_t LowestTemp		;	//LowestTemp
		uint8_t MeanTemp		; 	//Average
		uint8_t HighestTempID	; 	//highestTemperature id
		uint8_t LowestTempID	;	//LowestTemperature id
		uint8_t reserved2		;

	}__attribute__((aligned(1),packed)) B;

}BatteryTemp_t;


typedef union{
	uint8_t TxData[8];
	uint8_t RxData[8];
	struct{
		uint8_t prechargeStateSignal1 ;
		uint8_t prechargeStateSignal2 ;
		uint8_t RelayContactSignal1   ;
		uint8_t RelayContactSignal2	  ;
		uint8_t RelayContactSignal3	  ;
		uint8_t TsalSignal			  ;
		uint8_t IMDStatusFrequency	  ;
		uint8_t Reserved			  ;


	}__attribute__((aligned(1),packed)) B;
}BatteryDiagnose_t;

extern BatteryTemp_t R_BatteryTemp;
extern BatteryDiagnose_t T_BatteryDiagnose;
//extern stm32_msg_t stm32_1;

extern void GAS_Can_init(void);
extern void GAS_Can_sendMessage();
extern void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan);
//extern void GAS_Can_recieveMessage(CAN_HandleTypeDef *hcan);
#endif /* SRC_GETANDSEND_H_ */
