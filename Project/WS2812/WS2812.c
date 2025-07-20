#include "WS2812.h"

uint32_t Send_Buffer[24 * LED_number + 50] = {0};
uint8_t High_CRR = 112;
uint8_t Low_CRR = 56;
uint8_t Button_IDLE[6] = {1, 1, 1, 1, 1, 1};
uint8_t flowing_index = 0;
int8_t flowing_direction = 1;

void WS2812_Set_Single_Color(uint16_t led_index, uint8_t R, uint8_t G, uint8_t B, float brightness)
{
	if (led_index >= LED_number)
		return;

	if (brightness < 0.0f)
		brightness = 0.0f;
	if (brightness > 1.0f)
		brightness = 1.0f;

	uint16_t index = led_index * 24;
	uint8_t g_scaled = G * brightness;
	uint8_t r_scaled = R * brightness;
	uint8_t b_scaled = B * brightness;

	for (int i = 7; i >= 0; i--)
		Send_Buffer[index++] = (g_scaled >> i) & 0x01 ? High_CRR : Low_CRR;

	for (int i = 7; i >= 0; i--)
		Send_Buffer[index++] = (r_scaled >> i) & 0x01 ? High_CRR : Low_CRR;

	for (int i = 7; i >= 0; i--)
		Send_Buffer[index++] = (b_scaled >> i) & 0x01 ? High_CRR : Low_CRR;
}

void WS2812_Append_Reset(void)
{
	uint32_t index = LED_number * 24;
	for (int i = 0; i < 50; i++)
	{
		Send_Buffer[index++] = 0;
	}
}

void WS2812_NewRound_Send(TIM_HandleTypeDef *htim, uint32_t TIM_CHANNEL)
{
	HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL);
	WS2812_Append_Reset();
	HAL_TIM_PWM_Start_DMA(htim, TIM_CHANNEL, Send_Buffer, sizeof(Send_Buffer) / sizeof(uint32_t));
}

void WS2812_DisPlay_HeartBeat_Error(void)
{
	if (HeartBeat_Error_Flags == 0)
	{
		for (int i = 1; i <= LED_number; i++)
		{
			WS2812_Set_Single_Color(i, Color_Table[COLOR_GREEN].R,
									Color_Table[COLOR_GREEN].G,
									Color_Table[COLOR_GREEN].B, 0.1);
		}
		return;
	}

	uint8_t Spark_Number = ((USER_sysTick & 0x1FF) > 256) ? 1 : 0;

	// --- BTN_OFFENSIVE ---
	if (Button_IDLE[0])
	{
		for (uint8_t i = 0; i <= 3; i++)
		{
			if (Event_Flag[i] == 1)
				HeartBeat_Display = 0;
		}
		if (HeartBeat_Display == 1)
		{
			if (HeartBeat_Error_Flags & 0x01)
			{
				uint8_t color = Spark_Number ? COLOR_RED : COLOR_BLACK;
				uint8_t leds[] = {1, 2, 3, 7, 8, 9};

				for (int i = 0; i < sizeof(leds) / sizeof(leds[0]); i++)
				{
					WS2812_Set_Single_Color(
						leds[i],
						Color_Table[color].R,
						Color_Table[color].G,
						Color_Table[color].B,
						0.1);
				}
			}
			else
			{
				uint8_t leds[] = {1, 2, 3, 7, 8, 9};

				for (int i = 0; i < sizeof(leds) / sizeof(leds[0]); i++)
				{
					WS2812_Set_Single_Color(
						leds[i],
						Color_Table[COLOR_GREEN].R,
						Color_Table[COLOR_GREEN].G,
						Color_Table[COLOR_GREEN].B,
						0.1);
				}
			}
		}
	}

	// --- BTN_DEFENSIVE ---
	if (Button_IDLE[1])
	{
		for (uint8_t i = 0; i <= 3; i++)
		{
			if (Event_Flag[i] == 1)
				HeartBeat_Display = 0;
		}
		if (HeartBeat_Display == 1)
		{
			if (HeartBeat_Error_Flags & 0x02)
			{
				uint8_t color = Spark_Number ? COLOR_RED : COLOR_BLACK;
				uint8_t leds[] = {4, 5, 6, 10, 11, 12};

				for (int i = 0; i < sizeof(leds) / sizeof(leds[0]); i++)
				{
					WS2812_Set_Single_Color(
						leds[i],
						Color_Table[color].R,
						Color_Table[color].G,
						Color_Table[color].B,
						0.1);
				}
			}
			else
			{
				uint8_t leds[] = {4, 5, 6, 10, 11, 12};

				for (int i = 0; i < sizeof(leds) / sizeof(leds[0]); i++)
				{
					WS2812_Set_Single_Color(
						leds[i],
						Color_Table[COLOR_GREEN].R,
						Color_Table[COLOR_GREEN].G,
						Color_Table[COLOR_GREEN].B,
						0.1);
				}
			}
		}
	}

	// --- BTN_SELF_TEST ---
	if (Button_IDLE[2])
	{
		for (uint8_t i = 0; i <= 3; i++)
		{
			if (Event_Flag[i] == 1)
				HeartBeat_Display = 0;
		}
		if (HeartBeat_Display == 1)
		{
			if (HeartBeat_Error_Flags & 0x04)
			{
				uint8_t color = Spark_Number ? COLOR_RED : COLOR_BLACK;
				uint8_t leds[] = {13, 14, 15, 19, 20, 21};

				for (int i = 0; i < sizeof(leds) / sizeof(leds[0]); i++)
				{
					WS2812_Set_Single_Color(
						leds[i],
						Color_Table[color].R,
						Color_Table[color].G,
						Color_Table[color].B,
						0.1);
				}
			}
			else
			{
				uint8_t leds[] = {13, 14, 15, 19, 20, 21};

				for (int i = 0; i < sizeof(leds) / sizeof(leds[0]); i++)
				{
					WS2812_Set_Single_Color(
						leds[i],
						Color_Table[COLOR_GREEN].R,
						Color_Table[COLOR_GREEN].G,
						Color_Table[COLOR_GREEN].B,
						0.1);
				}
			}
		}
	}

	// --- BTN_CHALLENGE ---
	if (Button_IDLE[3])
	{
		for (uint8_t i = 0; i <= 3; i++)
		{
			if (Event_Flag[i] == 1)
				HeartBeat_Display = 0;
		}
		if (HeartBeat_Display == 1)
		{
			if (HeartBeat_Error_Flags & 0x08)
			{
				uint8_t color = Spark_Number ? COLOR_RED : COLOR_BLACK;
				uint8_t leds[] = {16, 17, 18, 22, 23, 24, 25};

				for (int i = 0; i < sizeof(leds) / sizeof(leds[0]); i++)
				{
					WS2812_Set_Single_Color(
						leds[i],
						Color_Table[color].R,
						Color_Table[color].G,
						Color_Table[color].B,
						0.1);
				}
			}
			else
			{
				uint8_t leds[] = {16, 17, 18, 22, 23, 24};

				for (int i = 0; i < sizeof(leds) / sizeof(leds[0]); i++)
				{
					WS2812_Set_Single_Color(
						leds[i],
						Color_Table[COLOR_GREEN].R,
						Color_Table[COLOR_GREEN].G,
						Color_Table[COLOR_GREEN].B,
						0.1);
				}
			}
		}
	}
}

void WS2812_Display_Now_Event(void)
{
	int8_t Now_Event = -1;
	for (uint8_t i = 0; i <= 3; i++)
	{
		if (Event_Flag[i] == 1)
		{
			Now_Event = i;
			break;
		}
		if (i == 3)
		{
			return;
		}
	}
	for (uint8_t i = 0; i <= 3; i++)
	{
		if (Button_IDLE[i] == 0)
		{
			return;
		}
	}
	switch (Now_Event)
	{
	case 0:
		for (uint8_t i = 0; i < LED_number; i++)
        {
            WS2812_Set_Single_Color(i, 0, 0, 0, 0.0f);
        }

        WS2812_Set_Single_Color(
            flowing_index,
            Color_Table[COLOR_GREEN].R,
            Color_Table[COLOR_GREEN].G,
            Color_Table[COLOR_GREEN].B,
            0.1f);

        flowing_index += flowing_direction;

        if (flowing_index == LED_number - 1 || flowing_index == 0)
        {
            flowing_direction = -flowing_direction; 
        }
		break;
	case 1:
		uint8_t leds[] = {16, 17, 18, 22, 23, 24};

		for (int i = 0; i < sizeof(leds) / sizeof(leds[0]); i++)
		{
			WS2812_Set_Single_Color(
				leds[i],
				Color_Table[COLOR_GREEN].R,
				Color_Table[COLOR_GREEN].G,
				Color_Table[COLOR_GREEN].B,
				0.1);
		}
		break;
	case 2:
		uint8_t leds[] = {16, 17, 18, 22, 23, 24};

		for (int i = 0; i < sizeof(leds) / sizeof(leds[0]); i++)
		{
			WS2812_Set_Single_Color(
				leds[i],
				Color_Table[COLOR_GREEN].R,
				Color_Table[COLOR_GREEN].G,
				Color_Table[COLOR_GREEN].B,
				0.1);
		}
		break;
	case 3:
		uint8_t leds[] = {16, 17, 18, 22, 23, 24};

		for (int i = 0; i < sizeof(leds) / sizeof(leds[0]); i++)
		{
			WS2812_Set_Single_Color(
				leds[i],
				Color_Table[COLOR_GREEN].R,
				Color_Table[COLOR_GREEN].G,
				Color_Table[COLOR_GREEN].B,
				0.1);
		}
		break;
	default:
		break;
	}
}