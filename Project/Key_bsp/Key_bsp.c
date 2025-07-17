#include "Key_bsp.h"

Button buttons[BTN_COUNT];
uint8_t TxMessage[1] = {0x01};

uint8_t Button_GetState(ButtonID id)
{
	if (id >= BTN_COUNT)
		return 0;
	return buttons[id].stableState;
}

void Button_Init(void)
{
	buttons[BTN_OFFENSIVE] = (Button){
		.port = GPIOC,
		.pin = GPIO_PIN_7,
		.state = BTN_STATE_IDLE,
		.lastChangeTime = 0,
		.stableState = 0};

	buttons[BTN_DEFENSIVE] = (Button){
		.port = GPIOC,
		.pin = GPIO_PIN_9,
		.state = BTN_STATE_IDLE,
		.lastChangeTime = 0,
		.stableState = 0};

	buttons[BTN_SELF_TEST] = (Button){
		.port = GPIOA,
		.pin = GPIO_PIN_9,
		.state = BTN_STATE_IDLE,
		.lastChangeTime = 0,
		.stableState = 0};

	buttons[BTN_CHALLENGE] = (Button){
		.port = GPIOA,
		.pin = GPIO_PIN_8,
		.state = BTN_STATE_IDLE,
		.lastChangeTime = 0,
		.stableState = 0};

	buttons[BTN_RESERVED1] = (Button){
		.port = GPIOA,
		.pin = GPIO_PIN_10,
		.state = BTN_STATE_IDLE,
		.lastChangeTime = 0,
		.stableState = 0};

	buttons[BTN_RESERVED2] = (Button){
		.port = GPIOC,
		.pin = GPIO_PIN_8,
		.state = BTN_STATE_IDLE,
		.lastChangeTime = 0,
		.stableState = 0};
}

void Refresh_Button_State(void)
{
	USER_sysTick++;
	for (int i = 0; i < BTN_COUNT; i++)
	{
		Button *btn = &buttons[i];
		uint8_t currentLevel = HAL_GPIO_ReadPin(btn->port, btn->pin);

		switch (btn->state)
		{
		case BTN_STATE_IDLE:
			if (currentLevel == GPIO_PIN_RESET)
			{
				btn->state = BTN_STATE_PRESS_DOWN;
				btn->lastChangeTime = USER_sysTick;
			}
			break;

		case BTN_STATE_PRESS_DOWN:
			if (USER_sysTick - btn->lastChangeTime >= BTN_DEBOUNCE_MS)
			{
				if (currentLevel == GPIO_PIN_RESET)
				{
					btn->state = BTN_STATE_PRESSED;
					btn->stableState = 1;
				}
				else
					btn->state = BTN_STATE_IDLE;
			}
			break;

		case BTN_STATE_PRESSED:
			if (currentLevel == GPIO_PIN_SET)
			{
				btn->state = BTN_STATE_RELEASE_DOWN;
				btn->lastChangeTime = USER_sysTick;
			}
			break;

		case BTN_STATE_RELEASE_DOWN:
			if (USER_sysTick - btn->lastChangeTime >= BTN_DEBOUNCE_MS)
			{
				if (currentLevel == GPIO_PIN_SET)
				{
					btn->state = BTN_STATE_IDLE;
					btn->stableState = 0;
				}
				else
					btn->state = BTN_STATE_PRESSED;
			}
			break;
		}
	}
}

void Button_Function_Execute(void)
{
	uint8_t Button_Send_Success[BTN_COUNT] = {0};
	if (Button_GetState(BTN_OFFENSIVE))
	{
		Button_IDLE[BTN_OFFENSIVE] = 0;
		if (buttons[BTN_OFFENSIVE].HaveTransed == false)
		{
			Button_Send_Success[BTN_OFFENSIVE] = OFFENSIVE_RESET();
			buttons[BTN_OFFENSIVE].HaveTransed = true;
		}
		if (Button_Send_Success[BTN_OFFENSIVE])
		{
			uint8_t leds[] = {1, 2, 3, 7, 8, 9};

			for (int i = 0; i < sizeof(leds) / sizeof(leds[0]); i++)
			{
				WS2812_Set_Single_Color(
					leds[i],
					Color_Table[COLOR_YELLOW].R,
					Color_Table[COLOR_YELLOW].G,
					Color_Table[COLOR_YELLOW].B,
					0.1);
			}
		}
	}
	else
	{
		buttons[BTN_OFFENSIVE].HaveTransed = false;
		Button_IDLE[BTN_OFFENSIVE] = 1;
	}

	if (Button_GetState(BTN_DEFENSIVE))
	{
		Button_IDLE[BTN_DEFENSIVE] = 0;
		if (buttons[BTN_DEFENSIVE].HaveTransed == false)
		{
			Button_Send_Success[BTN_DEFENSIVE] = DEFENSIVE_RESET();
			buttons[BTN_DEFENSIVE].HaveTransed = true;
		}
		if (Button_Send_Success[BTN_DEFENSIVE])
		{
			uint8_t leds[] = {4, 5, 6, 10, 11, 12};

			for (int i = 0; i < sizeof(leds) / sizeof(leds[0]); i++)
			{
				WS2812_Set_Single_Color(
					leds[i],
					Color_Table[COLOR_YELLOW].R,
					Color_Table[COLOR_YELLOW].G,
					Color_Table[COLOR_YELLOW].B,
					0.1);
			}
		}
	}
	else
	{
		buttons[BTN_DEFENSIVE].HaveTransed = false;
		Button_IDLE[BTN_DEFENSIVE] = 1;
	}

	if (Button_GetState(BTN_SELF_TEST))
	{
		Button_IDLE[BTN_SELF_TEST] = 0;
		if (buttons[BTN_SELF_TEST].HaveTransed == false)
		{
			Button_Send_Success[BTN_SELF_TEST] = SELF_TEST_BEGIN();
			buttons[BTN_SELF_TEST].HaveTransed = true;
		}
		if (Button_Send_Success[BTN_SELF_TEST])
		{
			uint8_t leds[] = {13, 14, 15, 19, 20, 21};

			for (int i = 0; i < sizeof(leds) / sizeof(leds[0]); i++)
			{
				WS2812_Set_Single_Color(
					leds[i],
					Color_Table[COLOR_YELLOW].R,
					Color_Table[COLOR_YELLOW].G,
					Color_Table[COLOR_YELLOW].B,
					0.1);
			}
		}
	}
	else
	{
		buttons[BTN_SELF_TEST].HaveTransed = false;
		Button_IDLE[BTN_SELF_TEST] = 1;
	}

	if (Button_GetState(BTN_CHALLENGE))
	{
		Button_IDLE[BTN_CHALLENGE] = 0;
		if (buttons[BTN_CHALLENGE].HaveTransed == false)
		{
			Button_Send_Success[BTN_CHALLENGE] = CHALLENGE_BEGIN();
			buttons[BTN_CHALLENGE].HaveTransed = true;
		}
		if (Button_Send_Success[BTN_CHALLENGE])
		{
			uint8_t leds[] = {16, 17, 18, 22, 23, 24};

			for (int i = 0; i < sizeof(leds) / sizeof(leds[0]); i++)
			{
				WS2812_Set_Single_Color(
					leds[i],
					Color_Table[COLOR_YELLOW].R,
					Color_Table[COLOR_YELLOW].G,
					Color_Table[COLOR_YELLOW].B,
					0.1);
			}
		}
	}
	else
	{
		buttons[BTN_CHALLENGE].HaveTransed = false;
		Button_IDLE[BTN_CHALLENGE] = 1;
	}

	if (Button_GetState(BTN_RESERVED1))
	{
		Button_IDLE[BTN_RESERVED1] = 0;
		if (buttons[BTN_RESERVED1].HaveTransed == false)
		{
			Button_Send_Success[BTN_RESERVED1] = RESERVED1_BEGIN();
			buttons[BTN_RESERVED1].HaveTransed = true;
		}
		if (Button_Send_Success[BTN_RESERVED1])
		{
			for (int i = 1; i <= 24; i++)
			{
				WS2812_Set_Single_Color(
					i,
					Color_Table[COLOR_YELLOW].R,
					Color_Table[COLOR_YELLOW].G,
					Color_Table[COLOR_YELLOW].B,
					0.1);
			}
		}
	}
	else
	{
		buttons[BTN_RESERVED1].HaveTransed = false;
		Button_IDLE[BTN_RESERVED1] = 1;
	}

	if (Button_GetState(BTN_RESERVED2))
	{
		Button_IDLE[BTN_RESERVED2] = 0;
		if (buttons[BTN_RESERVED2].HaveTransed == false)
		{
			Button_Send_Success[BTN_RESERVED2] = RESERVED2_BEGIN();
			buttons[BTN_RESERVED2].HaveTransed = true;
		}
		if (Button_Send_Success[BTN_RESERVED2])
		{
			for (int i = 1; i <= 24; i++)
			{
				WS2812_Set_Single_Color(
					i,
					Color_Table[COLOR_BLUE].R,
					Color_Table[COLOR_BLUE].G,
					Color_Table[COLOR_BLUE].B,
					0.1);
			}
		}
	}
	else
	{
		buttons[BTN_RESERVED2].HaveTransed = false;
		Button_IDLE[BTN_RESERVED2] = 1;
	}
}

uint8_t OFFENSIVE_RESET(void)
{
	return FDCAN_SendData(&hfdcan2, TxMessage, 0x20, 1);
}
uint8_t DEFENSIVE_RESET(void)
{
	return FDCAN_SendData(&hfdcan2, TxMessage, 0x30, 1);
}
uint8_t SELF_TEST_BEGIN(void)
{
	return FDCAN_SendData(&hfdcan2, TxMessage, 0x40, 1);
}
uint8_t CHALLENGE_BEGIN(void)
{
	return FDCAN_SendData(&hfdcan2, TxMessage, 0x50, 1);
}
uint8_t RESERVED1_BEGIN(void)
{
	return FDCAN_SendData(&hfdcan3, TxMessage, 0x60, 1);
}
uint8_t RESERVED2_BEGIN(void)
{
	return FDCAN_SendData(&hfdcan3, TxMessage, 0x70, 1);
}
