/*
 * CAN.c
 *
 *  Created on: 2020. 10. 27.
 *      Author: Suprhimp
 */

#include <GAS_Can.h>
#include <GAS_PWM.h>
#include <stdio.h>
#include <string.h>

CAN_FilterTypeDef sFilterConfig;
CAN_FilterTypeDef sFilterConfig2;
CAN_RxHeaderTypeDef canRxHeader;
CAN_RxHeaderTypeDef canRxHeader2;
CAN_TxHeaderTypeDef canTxHeader;
CAN_TxHeaderTypeDef canTxHeader2;

//uint8_t canRx0Data[8];
uint32_t TxMailBox;
BatteryTemp_t R_BatteryTemp;
BatteryDiagnose_t T_BatteryDiagnose;
FanStatusData_t T_FanStatusData;//230108: added
TC_order_t R_TC_order;

uint32_t BMSID = 0x1F02; //230130: BMSID fixed
uint32_t stm32BattInfoTX1 = 0x334C01;	//221228_0338: THIS stm's CAN ID //testest///testtest
uint32_t stm32BattFanInfoTX2 = 0x334C02;	//230108_2100: Cooling fan duty cycles

uint32_t TC_order_ID = 0x275B01;


/*-------------------------Function Prototypes--------------------------------*/

void GAS_Can_rxSetting(void);
void GAS_Can_init(void);
void GAS_Can_sendMessage();
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan);//230108 added
void HAL_CAN_ErrorCallback(CAN_HandleTypeDef* hcan);


void GAS_Can_txSetting(void)
{

//	  canTxHeader.StdId = (0x283>>18)&0x7ff;
	  canTxHeader.ExtId = stm32BattInfoTX1;
	  canTxHeader.IDE	= CAN_ID_EXT;
	  canTxHeader.RTR	= CAN_RTR_DATA;
	  canTxHeader.DLC	=	8;

	  //canTxHeader.StdId = (0x283>>18)&0x7ff;
	  canTxHeader2.ExtId = stm32BattFanInfoTX2;
	  canTxHeader2.IDE	= CAN_ID_EXT;
	  canTxHeader2.RTR	= CAN_RTR_DATA;
	  canTxHeader2.DLC	=	8;

}

void GAS_Can_rxSetting(void){

	sFilterConfig.FilterIdHigh = (BMSID<<3)>>16;				/*first 2byte in 29bit (shift need to IED,RTR,0)*/
	sFilterConfig.FilterIdLow =(0xffff & (BMSID << 3)) | (1<<2);	/*second 2byte in 29bit + IDE (shift need to IED,RTR,0/)*/
	sFilterConfig.FilterMaskIdHigh = (BMSID<<3)>>16;
	sFilterConfig.FilterMaskIdLow = (0xffff & (BMSID << 3)) | (1<<2);
	sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
	sFilterConfig.FilterBank = 5;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterActivation = ENABLE;
	sFilterConfig.SlaveStartFilterBank = 15;


	 if (HAL_CAN_ConfigFilter(&hcan, &sFilterConfig) != HAL_OK)
	 {
	     /* Filter configuration Error */
	     Error_Handler();
	 }


	 //220108_2223 TC control mode RX, from STM32F450_Cooling_Control-FanVar GAS_Can.c
	 sFilterConfig2.FilterIdHigh = (TC_order_ID<<3)>>16;				/*first 2byte in 29bit (shift need to IED,RTR,0)*/
	 sFilterConfig2.FilterIdLow = (0xffff & (TC_order_ID << 3)) | (1<<2);	/*second 2byte in 29bit + IDE (shift need to IED,RTR,0/)*/
	 sFilterConfig2.FilterMaskIdHigh = (0x0fffffff<<3)>>16;
	 sFilterConfig2.FilterMaskIdLow =(0xffff & (0x0FFFFFFF << 3)) | (1<<2);
	 sFilterConfig2.FilterFIFOAssignment = CAN_RX_FIFO1;
	 sFilterConfig2.FilterBank = 2;   /* YOU MUST USE FILTERBANK over 14 if YOU USE CAN2!!!!!!!!!!!!!!! */
	 sFilterConfig2.FilterMode = CAN_FILTERMODE_IDMASK;
	 sFilterConfig2.FilterScale = CAN_FILTERSCALE_32BIT;
	 sFilterConfig2.FilterActivation = ENABLE;
	 sFilterConfig2.SlaveStartFilterBank = 15;
	 if (HAL_CAN_ConfigFilter(&hcan, &sFilterConfig2) != HAL_OK)
	 {
		 /* Filter configuration Error */
		 Error_Handler();
	 }

}

void GAS_Can_init(void)
{

	GAS_Can_txSetting();
	GAS_Can_rxSetting();


	HAL_CAN_Start(&hcan);

	if (HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
	{
	  Error_Handler();
	}
	//230108 added ///
	/*
	if(HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO1_MSG_PENDING)!= HAL_OK){
		Error_Handler();
	}
*/

}

void GAS_Can_sendMessage()
{
	T_BatteryDiagnose.B.prechargeStateSignal1 = HAL_GPIO_ReadPin(PrechargeStateSignal1_GPIO_Port,PrechargeStateSignal1_Pin);
	T_BatteryDiagnose.B.prechargeStateSignal2 = HAL_GPIO_ReadPin(PrechargeStateSignal2_GPIO_Port,PrechargeStateSignal2_Pin);
	T_BatteryDiagnose.B.RelayContactSignal1 = HAL_GPIO_ReadPin(RelayContactSignal1_GPIO_Port,RelayContactSignal1_Pin);
	T_BatteryDiagnose.B.RelayContactSignal2 = HAL_GPIO_ReadPin(RelayContactSignal2_GPIO_Port,RelayContactSignal2_Pin);
	T_BatteryDiagnose.B.RelayContactSignal3 = HAL_GPIO_ReadPin(RelayContactSignal3_GPIO_Port,RelayContactSignal3_Pin);
	T_BatteryDiagnose.B.TsalSignal = HAL_GPIO_ReadPin(TSALSignal_GPIO_Port,TSALSignal_Pin);
	T_BatteryDiagnose.B.IMDStatusFrequency = HAL_GPIO_ReadPin(IMDStatusSignal_GPIO_Port,IMDStatusSignal_Pin);
	T_BatteryDiagnose.B.Reserved			= 123;

	//230108: add TX sending message: T_FanStatusData
	T_FanStatusData.B.FanFlag = pwmChangeFlag;
	T_FanStatusData.B.TIM15_Dutycycle = pwmIn15.DutyCycle;
	T_FanStatusData.B.TIM15_Frequency = pwmIn15.Frequency;
	T_FanStatusData.B.TIM16_Dutycycle = pwmIn16.DutyCycle;
	T_FanStatusData.B.TIM16_Frequency = pwmIn16.Frequency;
	T_FanStatusData.B.TIM17_Dutycycle = pwmIn17.DutyCycle;
	T_FanStatusData.B.TIM17_Frequency = pwmIn17.Frequency;

	TxMailBox = HAL_CAN_GetTxMailboxesFreeLevel(&hcan);
	HAL_CAN_AddTxMessage(&hcan, &canTxHeader, &T_BatteryDiagnose.TxData[0], &TxMailBox);

	//230108: add TX send message sending part
	TxMailBox = HAL_CAN_GetTxMailboxesFreeLevel(&hcan);
	HAL_CAN_AddTxMessage(&hcan, &canTxHeader2, &T_FanStatusData.TxData[0], &TxMailBox);
}


//230108: fixed based on STM32F405R_Cooling_Control-FanVar
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	if(hcan->Instance == CAN)
	{
		uint8_t temp[8];
		//230108: get R_BatteryTemp from BMS
		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &canRxHeader, temp);
		if(canRxHeader.ExtId == BMSID){ //230203 ExtID -> stdID
			memcpy(R_BatteryTemp.RxData, temp, sizeof(uint8_t)*8);///
		}
	}

}

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	if(hcan->Instance == CAN){
		uint8_t temp[8];
		//230108: get R_TC_order from VCU
		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &canRxHeader2, temp);
		if(canRxHeader2.ExtId == TC_order_ID){
			memcpy(R_TC_order.RxData, temp, sizeof(uint16_t)*4);
	}

	}

}


//void GAS_Can_recieveMessage(CAN_HandleTypeDef *hcan)
//{
//		if(HAL_CAN_GetRxFifoFillLevel(hcan, CAN_RX_FIFO1) != 0)
//	{
//		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &canRxHeader, stm32_2.RxData);
//	}
//
//}
