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

/*
 * 230108: Fan data TX message
 * Fanflag = 1: tim15: 7
 * 			 tim16: 4
 * 			 tim17: 1
 * Fanflag = 2: tim15: 8
 * 			 tim16: 5
 * 			 tim17: 2
 * Fanflag = 3: tim15: 9
 * 			 tim16: 6
 * 			 tim17: 3
 */
typedef union{
	uint8_t TxData[8];
	uint8_t RxData[8];
	struct{
		uint8_t FanFlag			;
		uint8_t TIM15_Dutycycle ;
		uint8_t TIM15_Frequency ;
		uint8_t TIM16_Dutycycle ;
		uint8_t TIM16_Frequency ;
		uint8_t TIM17_Dutycycle ;
		uint8_t TIM17_Frequency ;
		uint8_t Remain			;
	}__attribute__((aligned(1), packed)) B;

}FanStatusData_t;

//230108: TC order
typedef union{
	uint8_t RxData[8];
	struct{
		uint8_t TCControlMode	;
		uint8_t TCFanDutyOrder  ;
		uint8_t Remain1			;
		uint8_t Remain2			;
		uint8_t Remain3			;
		uint8_t Remain4			;
		uint8_t Remain5			;
		uint8_t Remain6			;
	}__attribute__((aligned(1), packed)) B;

}TC_order_t;

extern BatteryTemp_t R_BatteryTemp;
extern BatteryDiagnose_t T_BatteryDiagnose;
extern FanStatusData_t T_FanStatusData; //230108
extern TC_order_t R_TC_order; //230108: BC-10 TC RX

extern void GAS_Can_init(void);
extern void GAS_Can_sendMessage();
extern void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan);
//extern void GAS_Can_recieveMessage(CAN_HandleTypeDef *hcan);
#endif /* SRC_GETANDSEND_H_ */
