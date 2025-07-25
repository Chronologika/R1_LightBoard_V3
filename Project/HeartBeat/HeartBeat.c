#include "HeartBeat.h"

uint8_t SYSTEM_INTERRUPT_FLAG = 0;
uint64_t Last_HeartBeat_Time[DEVICE_NUM] = {0};
uint64_t Last_Status_Switch_Time = 0;
uint8_t Event_Flag[4] = {0};
uint8_t HeartBeat_Error_Flags = 0;
uint8_t HeartBeat_Display = 1;

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
    if (USER_sysTick - Last_Status_Switch_Time > 2000)
    {
        Event_Flag[2] = 0;Event_Flag[3] = 0;
    }
}

void Refresh_HeartBeat_Time(FDCAN_HandleTypeDef *hfdcan)
{
    uint32_t id = 0;

    if (hfdcan == &hfdcan1)
        id = CAN1_RxMessage.Identifier;
    else if (hfdcan == &hfdcan2)
        id = CAN2_RxMessage.Identifier;
    else if (hfdcan == &hfdcan3)
        id = CAN3_RxMessage.Identifier;
    else
        return;

    switch (id)
    {
        case VISION_CANID:
            Last_HeartBeat_Time[0] = USER_sysTick;
            break;
        case CHASSIS_CANID:
            Last_HeartBeat_Time[1] = USER_sysTick;
            break;
        case DRIBBLE_CANID:
            Last_HeartBeat_Time[2] = USER_sysTick;
            break;
        case PUSHSHOT_CANID:
            Last_HeartBeat_Time[3] = USER_sysTick;
            break;
        case 0x105:
            Event_Flag[0] = 1;
            break;
        case 0x106:
            Event_Flag[0] = 0;
            break;
        case 0x107:
            Event_Flag[1] = 1;
            break;
        case 0x108:
            Event_Flag[1] = 0;
            break;
        case 0x109:
            Event_Flag[2] = 1;
            Last_Status_Switch_Time = USER_sysTick;
            break;
        case 0x10A:
            Event_Flag[3] = 1;
            Last_Status_Switch_Time = USER_sysTick;
            break;
        default:
            break;
    }
}
