#ifndef _FDCAN_BSP_H_
#define _FDCAN_BSP_H_

#include "fdcan.h"

#define VISION_CANID 0X101
#define CHASSIS_CANID 0X102
#define DRIBBLE_CANID 0X103
#define PUSHSHOT_CANID 0X104

//uint8_t FDCAN1_Init(FDCAN_HandleTypeDef *hfdcan);
uint8_t FDCAN2_Init(FDCAN_HandleTypeDef *hfdcan);
uint8_t FDCAN3_Init(FDCAN_HandleTypeDef *hfdcan);
uint8_t FDCAN_SendData(FDCAN_HandleTypeDef *hfdcan, uint8_t *TxData, uint32_t StdId, uint8_t Length);
uint8_t FDCAN_SendData_Ext(FDCAN_HandleTypeDef *hfdcan, uint8_t *TxData, uint32_t ExtId, uint8_t Length, uint32_t Data_type);

extern uint8_t CAN1_RxData[64], CAN2_RxData[64], CAN3_RxData[64];
extern FDCAN_RxHeaderTypeDef CAN1_RxMessage, CAN2_RxMessage, CAN3_RxMessage;

#endif
