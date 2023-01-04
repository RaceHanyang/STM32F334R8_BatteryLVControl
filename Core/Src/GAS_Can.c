/*
 * CAN.c
 *
 *  Created on: 2020. 10. 27.
 *      Author: Suprhimp
 */

#include <GAS_Can.h>
#include <stdio.h>

CAN_FilterTypeDef sFilterConfig;
CAN_FilterTypeDef sFilterConfig2;
CAN_RxHeaderTypeDef canRxHeader;
CAN_TxHeaderTypeDef canTxHeader;

uint8_t canRx0Data[8];
uint32_t TxMailBox;
BatteryTemp_t R_BatteryTemp;
BatteryDiagnose_t T_BatteryDiagnose;



uint32_t BatteryInsideID = 0x405DB; //230104: need to be change to BMS temperature message id. Fix when BMS setted
uint32_t BatteryFanCnt = 0x334C;	//221228_0338: THIS stm's CAN ID //testest///testtest


/*-------------------------Function Prototypes--------------------------------*/

void GAS_Can_rxSetting(void);
void GAS_Can_init(void);
void GAS_Can_sendMessage();
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
void HAL_CAN_ErrorCallback(CAN_HandleTypeDef* hcan);


void GAS_Can_txSetting(void)
{

//	  canTxHeader.StdId = (0x283>>18)&0x7ff;
	  canTxHeader.ExtId = BatteryFanCnt;
	  canTxHeader.IDE	= CAN_ID_EXT;
	  canTxHeader.RTR	= CAN_RTR_DATA;
	  canTxHeader.DLC	=	8;

}

void GAS_Can_rxSetting(void){

	sFilterConfig.FilterIdHigh = (BatteryInsideID<<3)>>16;				/*first 2byte in 29bit (shift need to IED,RTR,0)*/
	sFilterConfig.FilterIdLow = (0xffff & (BatteryInsideID << 3)) | (1<<2);	/*second 2byte in 29bit + IDE (shift need to IED,RTR,0/)*/
	sFilterConfig.FilterMaskIdHigh = (0x0ffffff0<<3)>>16;
	sFilterConfig.FilterMaskIdLow =(0xffff & (0x0FFFFFF0 << 3)) | (1<<2);
	sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
	sFilterConfig.FilterBank = 0;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterActivation = ENABLE;
	sFilterConfig.SlaveStartFilterBank = 0;


	 if (HAL_CAN_ConfigFilter(&hcan, &sFilterConfig) != HAL_OK)
	 {
	     /* Filter configuration Error */
	     Error_Handler();
	 }
}

void GAS_Can_init(void)
{

	GAS_Can_txSetting();
	GAS_Can_rxSetting();
//

	HAL_CAN_Start(&hcan);

	if (HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
	{
	  Error_Handler();
	}


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

	TxMailBox = HAL_CAN_GetTxMailboxesFreeLevel(&hcan);
	HAL_CAN_AddTxMessage(&hcan, &canTxHeader, &T_BatteryDiagnose.TxData[0], &TxMailBox);

}



void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	if(hcan->Instance == CAN)
	{
		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &canRxHeader, R_BatteryTemp.RxData);

	}
}
//
//void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
//{
//	if(hcan->Instance == CAN1)
//	{
//		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &canRxHeader, AMK_Setpoint2.RxData);
//
//	}
//}

//void GAS_Can_recieveMessage(CAN_HandleTypeDef *hcan)
//{
//		if(HAL_CAN_GetRxFifoFillLevel(hcan, CAN_RX_FIFO1) != 0)
//	{
//		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &canRxHeader, stm32_2.RxData);
//	}
//
//}
