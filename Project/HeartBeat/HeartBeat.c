#include "HeartBeat.h"

uint64_t Last_HeartBeat_Time[DEVICE_NUM] = {0};
uint8_t HeartBeat_Error_Flags = 0;

void Check_HeartBeat_Timeout(void)
{
    for (int i = 0; i < DEVICE_NUM; i++)
    {
        if ((USER_sysTick - Last_HeartBeat_Time[i]) > HEARTBEAT_TIMEOUT)
        {
            HeartBeat_Error_Flags |= (1 << i);
        }
        else
        {
            HeartBeat_Error_Flags &= ~(1 << i);
        }
    }
}

void Refresh_HeartBeat_Time(FDCAN_HandleTypeDef *hfdcan)
{
    if (hfdcan == &hfdcan1)
    {
        if (CAN1_RxMessage.Identifier == VISION_CANID)
        {
            Last_HeartBeat_Time[0] = USER_sysTick;
        }
        else if (CAN1_RxMessage.Identifier == CHASSIS_CANID)
        {
            Last_HeartBeat_Time[1] = USER_sysTick;
        }
        else if (CAN1_RxMessage.Identifier == DRIBBLE_CANID)
        {
            Last_HeartBeat_Time[2] = USER_sysTick;
        }
        else if (CAN1_RxMessage.Identifier == PUSHSHOT_CANID)
        {
            Last_HeartBeat_Time[3] = USER_sysTick;
        }
    }
    else if (hfdcan == &hfdcan2)
    {
        if (CAN2_RxMessage.Identifier == VISION_CANID)
        {
            Last_HeartBeat_Time[0] = USER_sysTick;
        }
        else if (CAN2_RxMessage.Identifier == CHASSIS_CANID)
        {
            Last_HeartBeat_Time[1] = USER_sysTick;
        }
    }
    else if (hfdcan == &hfdcan3)
    {
        if (CAN3_RxMessage.Identifier == DRIBBLE_CANID)
        {
            Last_HeartBeat_Time[0] = USER_sysTick;
        }
        else if (CAN3_RxMessage.Identifier == PUSHSHOT_CANID)
        {
            Last_HeartBeat_Time[1] = USER_sysTick;
        }
    }
    
}
