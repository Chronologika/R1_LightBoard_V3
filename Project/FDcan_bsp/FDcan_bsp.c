#include "FDcan_bsp.h"

FDCAN_RxHeaderTypeDef CAN1_RxMessage, CAN2_RxMessage, CAN3_RxMessage;
uint8_t CAN1_RxData[64] = {0}, CAN2_RxData[64] = {0}, CAN3_RxData[64] = {0};

// uint8_t FDCAN1_Init(FDCAN_HandleTypeDef *hfdcan)
//{
//     FDCAN_FilterTypeDef RXFilter;

//    RXFilter.IdType = FDCAN_STANDARD_ID;
//    RXFilter.FilterIndex = 0;
//    RXFilter.FilterType = FDCAN_FILTER_MASK;
//    RXFilter.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
//    RXFilter.FilterID1 = VISION_CANID;
//    RXFilter.FilterID2 = 0x7FF;
//    if (HAL_FDCAN_ConfigFilter(hfdcan, &RXFilter) != HAL_OK) return 2;

//    RXFilter.FilterIndex = 1;
//    RXFilter.FilterID1 = CHASSIS_CANID;
//    RXFilter.FilterID2 = 0x7FF;
//    if (HAL_FDCAN_ConfigFilter(hfdcan, &RXFilter) != HAL_OK) return 2;

//    RXFilter.FilterIndex = 2;
//    RXFilter.FilterID1 = DRIBBLE_CANID;
//    RXFilter.FilterID2 = 0x7FF;
//    if (HAL_FDCAN_ConfigFilter(hfdcan, &RXFilter) != HAL_OK) return 2;

//    RXFilter.FilterIndex = 3;
//    RXFilter.FilterID1 = PUSHSHOT_CANID;
//    RXFilter.FilterID2 = 0x7FF;
//    if (HAL_FDCAN_ConfigFilter(hfdcan, &RXFilter) != HAL_OK) return 2;

//    if (HAL_FDCAN_ConfigGlobalFilter(hfdcan,
//                                     FDCAN_REJECT,
//                                     FDCAN_REJECT,
//                                     DISABLE,
//                                     DISABLE) != HAL_OK)
//    {
//        Error_Handler();
//    }

//    if (HAL_FDCAN_ActivateNotification(hfdcan,
//                                       FDCAN_IT_RX_FIFO0_NEW_MESSAGE | FDCAN_IT_BUS_OFF,
//                                       0) != HAL_OK)
//    {
//        Error_Handler();
//    }

//    if (HAL_FDCAN_Start(hfdcan) != HAL_OK)
//    {
//        Error_Handler();
//    }

//    return 0;
//}

uint8_t FDCAN2_Init(FDCAN_HandleTypeDef *hfdcan)
{
    FDCAN_FilterTypeDef RXFilter;

    RXFilter.IdType = FDCAN_STANDARD_ID;
    RXFilter.FilterType = FDCAN_FILTER_MASK;
    RXFilter.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;

    RXFilter.FilterIndex = 0;
    RXFilter.FilterID1 = VISION_CANID;
    RXFilter.FilterID2 = 0x7FF;
    if (HAL_FDCAN_ConfigFilter(hfdcan, &RXFilter) != HAL_OK)
        return 2;

    RXFilter.FilterIndex = 1;
    RXFilter.FilterID1 = CHASSIS_CANID;
    RXFilter.FilterID2 = 0x7FF;
    if (HAL_FDCAN_ConfigFilter(hfdcan, &RXFilter) != HAL_OK)
        return 2;

    if (HAL_FDCAN_ConfigGlobalFilter(hfdcan,
                                     FDCAN_REJECT,
                                     FDCAN_REJECT,
                                     DISABLE,
                                     DISABLE) != HAL_OK)
    {
        Error_Handler();
    }

    if (HAL_FDCAN_ActivateNotification(hfdcan,
                                       FDCAN_IT_RX_FIFO0_NEW_MESSAGE | FDCAN_IT_BUS_OFF,
                                       0) != HAL_OK)
    {
        Error_Handler();
    }

    if (HAL_FDCAN_Start(hfdcan) != HAL_OK)
    {
        Error_Handler();
    }

    return 0;
}

uint8_t FDCAN3_Init(FDCAN_HandleTypeDef *hfdcan)
{
    FDCAN_FilterTypeDef RXFilter;

    RXFilter.IdType = FDCAN_STANDARD_ID;
    RXFilter.FilterType = FDCAN_FILTER_MASK;
    RXFilter.FilterConfig = FDCAN_FILTER_TO_RXFIFO1;

    RXFilter.FilterIndex = 0;
    RXFilter.FilterID1 = DRIBBLE_CANID;
    RXFilter.FilterID2 = 0x7FF;
    if (HAL_FDCAN_ConfigFilter(hfdcan, &RXFilter) != HAL_OK)
        return 2;

    RXFilter.FilterIndex = 1;
    RXFilter.FilterID1 = PUSHSHOT_CANID;
    RXFilter.FilterID2 = 0x7FF;
    if (HAL_FDCAN_ConfigFilter(hfdcan, &RXFilter) != HAL_OK)
        return 2;

    if (HAL_FDCAN_ConfigGlobalFilter(hfdcan,
                                     FDCAN_REJECT,
                                     FDCAN_REJECT,
                                     DISABLE,
                                     DISABLE) != HAL_OK)
    {
        Error_Handler();
    }

    if (HAL_FDCAN_ActivateNotification(hfdcan,
                                       FDCAN_IT_RX_FIFO1_NEW_MESSAGE | FDCAN_IT_BUS_OFF,
                                       0) != HAL_OK)
    {
        Error_Handler();
    }

    if (HAL_FDCAN_Start(hfdcan) != HAL_OK)
    {
        Error_Handler();
    }

    return 0;
}

uint8_t FDCAN_SendData(FDCAN_HandleTypeDef *hfdcan, uint8_t *TxData, uint32_t StdId, uint8_t Length)
{
    FDCAN_TxHeaderTypeDef TxHeader = {0};

    TxHeader.Identifier = StdId;
    TxHeader.IdType = FDCAN_STANDARD_ID;
    TxHeader.TxFrameType = FDCAN_DATA_FRAME;

    switch (Length)
    {
    case 0:
        TxHeader.DataLength = FDCAN_DLC_BYTES_0;
        break;
    case 1:
        TxHeader.DataLength = FDCAN_DLC_BYTES_1;
        break;
    case 2:
        TxHeader.DataLength = FDCAN_DLC_BYTES_2;
        break;
    case 3:
        TxHeader.DataLength = FDCAN_DLC_BYTES_3;
        break;
    case 4:
        TxHeader.DataLength = FDCAN_DLC_BYTES_4;
        break;
    case 5:
        TxHeader.DataLength = FDCAN_DLC_BYTES_5;
        break;
    case 6:
        TxHeader.DataLength = FDCAN_DLC_BYTES_6;
        break;
    case 7:
        TxHeader.DataLength = FDCAN_DLC_BYTES_7;
        break;
    case 8:
        TxHeader.DataLength = FDCAN_DLC_BYTES_8;
        break;
    case 12:
        TxHeader.DataLength = FDCAN_DLC_BYTES_12;
        break;
    case 16:
        TxHeader.DataLength = FDCAN_DLC_BYTES_16;
        break;
    case 20:
        TxHeader.DataLength = FDCAN_DLC_BYTES_20;
        break;
    case 24:
        TxHeader.DataLength = FDCAN_DLC_BYTES_24;
        break;
    case 32:
        TxHeader.DataLength = FDCAN_DLC_BYTES_32;
        break;
    case 48:
        TxHeader.DataLength = FDCAN_DLC_BYTES_48;
        break;
    case 64:
        TxHeader.DataLength = FDCAN_DLC_BYTES_64;
        break;
    default:
        return 0;
    }

    TxHeader.ErrorStateIndicator = FDCAN_ESI_PASSIVE;
    TxHeader.BitRateSwitch = FDCAN_BRS_ON;
    TxHeader.FDFormat = FDCAN_FD_CAN;
    TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    TxHeader.MessageMarker = 0;

    if (HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &TxHeader, TxData) != HAL_OK)
        return 0;

    return 1;
}

uint8_t FDCAN_SendData_Ext(FDCAN_HandleTypeDef *hfdcan, uint8_t *TxData, uint32_t ExtId, uint8_t Length, uint32_t Data_type)
{
    FDCAN_TxHeaderTypeDef TxHeader = {0};

    TxHeader.Identifier = ExtId;
    TxHeader.IdType = FDCAN_EXTENDED_ID;
    TxHeader.TxFrameType = Data_type;

    switch (Length)
    {
    case 0:
        TxHeader.DataLength = FDCAN_DLC_BYTES_0;
        break;
    case 1:
        TxHeader.DataLength = FDCAN_DLC_BYTES_1;
        break;
    case 2:
        TxHeader.DataLength = FDCAN_DLC_BYTES_2;
        break;
    case 3:
        TxHeader.DataLength = FDCAN_DLC_BYTES_3;
        break;
    case 4:
        TxHeader.DataLength = FDCAN_DLC_BYTES_4;
        break;
    case 5:
        TxHeader.DataLength = FDCAN_DLC_BYTES_5;
        break;
    case 6:
        TxHeader.DataLength = FDCAN_DLC_BYTES_6;
        break;
    case 7:
        TxHeader.DataLength = FDCAN_DLC_BYTES_7;
        break;
    case 8:
        TxHeader.DataLength = FDCAN_DLC_BYTES_8;
        break;
    case 12:
        TxHeader.DataLength = FDCAN_DLC_BYTES_12;
        break;
    case 16:
        TxHeader.DataLength = FDCAN_DLC_BYTES_16;
        break;
    case 20:
        TxHeader.DataLength = FDCAN_DLC_BYTES_20;
        break;
    case 24:
        TxHeader.DataLength = FDCAN_DLC_BYTES_24;
        break;
    case 32:
        TxHeader.DataLength = FDCAN_DLC_BYTES_32;
        break;
    case 48:
        TxHeader.DataLength = FDCAN_DLC_BYTES_48;
        break;
    case 64:
        TxHeader.DataLength = FDCAN_DLC_BYTES_64;
        break;
    default:
        return 0;
    }

    TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    TxHeader.BitRateSwitch = FDCAN_BRS_ON;
    TxHeader.FDFormat = FDCAN_FD_CAN;
    TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    TxHeader.MessageMarker = 0;

    if (HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &TxHeader, TxData) != HAL_OK)
        return 0;

    return 1;
}
