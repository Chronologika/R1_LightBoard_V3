#include "WS2812.h"

uint32_t Send_Buffer[24 * LED_number + 50] = {0};
const uint8_t High_CRR = 112;
const uint8_t Low_CRR = 56;
const uint8_t Skill_LED_Hz = 10;
const uint8_t SelfTest_LED_Hz = 30;
uint8_t Button_IDLE[6] = {1, 1, 1, 1, 1, 1};
uint8_t flowing_index = 0;
uint8_t flowing_col_index = 0;
int8_t flowing_direction = 1;
int8_t flowing_col_direction = 1;
uint64_t Last_Skill_LED_Refresh_Time = 0;
uint64_t Last_SelfTest_LED_Refresh_Time = 0;

void WS2812_Set_Single_Color(uint16_t led_index, uint8_t R, uint8_t G, uint8_t B, float brightness)
{
    if (led_index >= LED_number)
        return;
    brightness = brightness < 0.0f ? 0.0f : (brightness > 1.0f ? 1.0f : brightness);

    uint16_t index = led_index * 24;
    uint8_t g = G * brightness, r = R * brightness, b = B * brightness;
    for (int i = 7; i >= 0; i--)
        Send_Buffer[index++] = (g >> i) & 0x01 ? High_CRR : Low_CRR;
    for (int i = 7; i >= 0; i--)
        Send_Buffer[index++] = (r >> i) & 0x01 ? High_CRR : Low_CRR;
    for (int i = 7; i >= 0; i--)
        Send_Buffer[index++] = (b >> i) & 0x01 ? High_CRR : Low_CRR;
}

void WS2812_Append_Reset(void)
{
    for (int i = 0; i < 50; i++)
        Send_Buffer[LED_number * 24 + i] = 0;
}

void WS2812_NewRound_Send(TIM_HandleTypeDef *htim, uint32_t TIM_CHANNEL)
{
    HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL);
    WS2812_Append_Reset();
    HAL_TIM_PWM_Start_DMA(htim, TIM_CHANNEL, Send_Buffer, sizeof(Send_Buffer) / sizeof(uint32_t));
}

static void Set_LED_Group_Color(const uint8_t *leds, uint8_t count, ColorName_t color, float brightness)
{
    for (uint8_t i = 0; i < count; i++)
    {
        WS2812_Set_Single_Color(
            leds[i],
            Color_Table[color].R,
            Color_Table[color].G,
            Color_Table[color].B,
            brightness);
    }
}

void WS2812_DisPlay_HeartBeat_Error(void)
{
    HeartBeat_Display = 1;
    for (uint8_t j = 0; j < 4; j++)
    {
        if (Event_Flag[j])
        {
            HeartBeat_Display = 0;
            break;
        }
    }

    uint8_t Spark_Number = ((USER_sysTick & 0x1FF) > 256);
    struct
    {
        uint8_t button;
        uint8_t flag;
        const uint8_t *leds;
        uint8_t count;
    } configs[] = {
        {Button_IDLE[0], 0x01, (uint8_t[]){1, 2, 3, 7, 8, 9}, 6},
        {Button_IDLE[1], 0x02, (uint8_t[]){4, 5, 6, 10, 11, 12}, 6},
        {Button_IDLE[2], 0x04, (uint8_t[]){13, 14, 15, 19, 20, 21}, 6},
        {Button_IDLE[3], 0x08, (uint8_t[]){16, 17, 18, 22, 23, 24}, 6},
    };

    for (uint8_t i = 0; i < 4; i++)
    {
        if (!configs[i].button)
            continue;

        if (!HeartBeat_Display)
            continue;

        ColorName_t color;

        if (HeartBeat_Error_Flags & configs[i].flag)
            color = Spark_Number ? COLOR_RED : COLOR_BLACK;
        else
            color = COLOR_GREEN;

        Set_LED_Group_Color(configs[i].leds, configs[i].count, color, 0.1);
    }
}

void WS2812_Display_Now_Event(void)
{
    int8_t Now_Event = -1;
    for (uint8_t i = 0; i < 4; i++)
    {
        if (Event_Flag[i])
        {
            Now_Event = i;
            break;
        }
    }
    for (uint8_t i = 0; i < 4; i++)
        if (!Button_IDLE[i])
            return;
    if (Now_Event == -1)
        return;

    for (uint8_t i = 0; i < LED_number; i++)
        WS2812_Set_Single_Color(i, 0, 0, 0, 0.0f);

    switch (Now_Event)
    {
    case 0:
        if (USER_sysTick - Last_SelfTest_LED_Refresh_Time > 1000/SelfTest_LED_Hz)
        {
        WS2812_Set_Single_Color(flowing_index, Color_Table[COLOR_GREEN].R, Color_Table[COLOR_GREEN].G, Color_Table[COLOR_GREEN].B, 0.1);
        flowing_index += flowing_direction;
        if (flowing_index == LED_number - 1 || flowing_index == 0)
            flowing_direction = -flowing_direction;
            Last_SelfTest_LED_Refresh_Time = USER_sysTick;
        }
        break;

    case 1:
        if (USER_sysTick - Last_Skill_LED_Refresh_Time > 1000/Skill_LED_Hz)
        {
        for (uint8_t i = flowing_col_index; i < LED_number; i += 6)
            WS2812_Set_Single_Color(i, Color_Table[COLOR_BLUE].R, Color_Table[COLOR_BLUE].G, Color_Table[COLOR_BLUE].B, 0.1);
        flowing_col_index += flowing_col_direction;
        if (flowing_col_index == LED_COLS - 1 || flowing_col_index == 0)
            flowing_col_direction = -flowing_col_direction;
            Last_Skill_LED_Refresh_Time = USER_sysTick;
        }
        break;

    case 2:
    case 3:
    {
        uint16_t mask = Now_Event == 2 ? 0x3FF : 0x1FF;
        uint16_t threshold = Now_Event == 2 ? 512 : 256;
        ColorName_t color = ((USER_sysTick & mask) > threshold) ? (Now_Event == 2 ? COLOR_GREEN : COLOR_RED) : COLOR_BLACK;
        for (int i = 0; i < LED_number; i++)
            WS2812_Set_Single_Color(i, Color_Table[color].R, Color_Table[color].G, Color_Table[color].B, 0.1);
        break;
    }
    default:
        break;
    }
}
