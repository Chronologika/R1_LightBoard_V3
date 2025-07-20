#ifndef _HEARTBEAT_H
#define _HEARTBEAT_H

#include "FDcan_bsp.h"

#define DEVICE_NUM 4
#define HEARTBEAT_TIMEOUT 500

extern uint64_t Last_HeartBeat_Time[DEVICE_NUM];
extern uint8_t HeartBeat_Error_Flags;
extern uint8_t Event_Flag[4];
extern uint8_t HeartBeat_Display;

void Check_HeartBeat_Timeout(void);
void Refresh_HeartBeat_Time(FDCAN_HandleTypeDef *hfdcan);

#endif
