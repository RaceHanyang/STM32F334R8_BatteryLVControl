/*
 * GAS_Can.h
 *
 *  Created on: 2020. 10. 27.
 *  Modified on: 2023.01.31.
 *      First Author: Suprhimp
 *      Second Author: legolas089
 *
 *  **comments of this file needs to be added
 */

#ifndef SRC_GETANDSEND_H_
#define SRC_GETANDSEND_H_

#include "stm32f3xx_hal.h"
#include "can.h"

/*
 * RX Data
 * Receive data from: BMS
 * RX ID: BMSID //FIXME BMSID
 * Data: Temperature data from BMS
 */

typedef union{
	uint8_t RxData[8];
	struct{ //230104: unsinged int => uint8_t

		uint8_t HighestTemp		;	//HighestTemp
		uint8_t HighestTempID	;	//LowestTemp
		uint8_t MeanTemp		; 	//Average
		uint8_t InternalTemp	; 	//Heatsink Temperature
		uint16_t LowcellVoltage	;	//not used in this code
		uint8_t reserved1		;
		uint8_t reserved2;
//		uint8_t reserved3		;

	}__attribute__((aligned(1),packed)) B;

}BatteryTemp_t;

/*
 * TX Data
 * Transmit to: VCU
 * TX ID: 0x334C01
 * Data: Precharge state1&2, relaycontact signal 1&2&3, TSAL signal, IMD status
 */
typedef union{
	uint8_t TxData[8];
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
 * TX Data
 * Transmit to: VCU
 * TX ID: 0x334C02
 * Data: Fan Flag, TIM 15, 16, 17 Dutycycle & Frequency
 *
 ***Notation
 * Ex) if fanflag == 1, send frequency&duty cycle of no.7,4,1 fans.
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
	struct{
		uint8_t FanFlag			;
		uint8_t TIM15_Dutycycle ;
		uint8_t TIM15_Frequency ;
		uint8_t TIM16_Dutycycle ;
		uint8_t TIM16_Frequency ;
		uint8_t TIM17_Dutycycle ;
		uint8_t TIM17_Frequency ;
		uint8_t desiredDuty			;
	}__attribute__((aligned(1), packed)) B;

}FanStatusData_t;

//230108: TC order
/*
 * RX Data
 * Receive from: VCU
 * RX ID: 0x275B01
 * Data: TCControl mode 0/1, one TCFanDutyOrder for each of 4 segments, in range of 0~100.
 */
typedef union{
	union{
		uint8_t RxData[8];
		uint8_t TxData[8];
	};
	struct{
		uint8_t TCControlMode	; //0: not TC control mode, 1: TC Control mode
		uint8_t TCFanDutyOrder_SideIntake ;
		uint8_t TCFanDutyOrder_SegmentIntake70;
		uint8_t TCFanDutyOrder_SegmentExhaust60;
		uint8_t TCFanDutyOrder_SegmentExhaust80;
		uint8_t Remain1		;
		uint8_t Remain2		;
		uint8_t Remain3		;

	}__attribute__((aligned(1), packed)) B;

}TC_order_t;
/*
typedef union{
	uint8_t TxData[8];
	struct{
		uint8_t TCControlModeEcho ; //0: not TC control mode, 1: TC Control mode
		uint8_t TCFanDutyOrder_SideIntake ;
		uint8_t TCFanDutyOrder_SegmentIntake70;
		uint8_t TCFanDutyOrder_SegmentExhaust60;
		uint8_t TCFanDutyOrder_SegmentExhaust80;
		uint8_t Remain1		;
		uint8_t Remain2		;
		uint8_t Remain3		;

	}__attribute__((aligned(1), packed)) B;

}TC_orderEcho_t;
*/
typedef union{
	uint8_t TxData[8];
	struct{
		uint8_t TargetDuty_SideIntake ;
		uint8_t TargetDuty_SegmentIntake70;
		uint8_t TargetDuty_SegmentExhaust60;
		uint8_t TargetDuty_SegmentExhaust80;
		uint8_t Remain1;
		uint8_t Remain2;
		uint8_t Remain3;
		uint8_t Remain4;
	};
}FanTargetDuty_t;




extern BatteryTemp_t R_BatteryTemp;
extern BatteryDiagnose_t T_BatteryDiagnose;
extern FanStatusData_t T_FanStatusData; //230108
extern TC_order_t R_TC_order; //230108: BC-10 TC RX
extern TC_order_t T_TC_orderEcho;
//extern TC_orderEcho_t T_TC_order; //230204: echo TC order
extern uint8_t per; //230204
extern FanTargetDuty_t T_FanTargetDuty;

extern void GAS_Can_init(void);
extern void GAS_Can_sendMessage();
extern void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
extern void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan);//230108 added
//extern void GAS_Can_recieveMessage(CAN_HandleTypeDef *hcan);
#endif /* SRC_GETANDSEND_H_ */
