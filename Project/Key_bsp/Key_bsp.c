#include "Key_bsp.h"

Button buttons[BTN_COUNT];
uint8_t TxMessage[1] = {0x01};

uint8_t Button_GetState(ButtonID id)
{
    return (id < BTN_COUNT) ? buttons[id].stableState : 0;
}

void Button_Init(void)
{
    const GPIO_TypeDef *ports[BTN_COUNT] = {
        GPIOC, GPIOC, GPIOA, GPIOA, GPIOA, GPIOC};
    const uint16_t pins[BTN_COUNT] = {
        GPIO_PIN_7, GPIO_PIN_9, GPIO_PIN_9, GPIO_PIN_8, GPIO_PIN_10, GPIO_PIN_8};

    for (int i = 0; i < BTN_COUNT; ++i)
    {
        buttons[i] = (Button){
            .port = (GPIO_TypeDef *)ports[i],
            .pin = pins[i],
            .state = BTN_STATE_IDLE,
            .lastChangeTime = 0,
            .stableState = 0,
            .HaveTransed = false};
    }
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
                btn->state = (currentLevel == GPIO_PIN_RESET) ? BTN_STATE_PRESSED : BTN_STATE_IDLE;
                btn->stableState = (currentLevel == GPIO_PIN_RESET);
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
                btn->state = (currentLevel == GPIO_PIN_SET) ? BTN_STATE_IDLE : BTN_STATE_PRESSED;
                if (currentLevel == GPIO_PIN_SET)
                    btn->stableState = 0;
            }
            break;
        }
    }
}

static const struct
{
    uint8_t (*sendFunc)(void);
    const uint8_t *leds;
    size_t ledCount;
    ColorName_t color;
} buttonActions[BTN_COUNT] = {
    {OFFENSIVE_RESET, (uint8_t[]){1, 2, 3, 7, 8, 9}, 6, COLOR_YELLOW},
    {DEFENSIVE_RESET, (uint8_t[]){4, 5, 6, 10, 11, 12}, 6, COLOR_YELLOW},
    {SELF_TEST_BEGIN, (uint8_t[]){13, 14, 15, 19, 20, 21}, 6, COLOR_YELLOW},
    {CHALLENGE_BEGIN, (uint8_t[]){16, 17, 18, 22, 23, 24}, 6, COLOR_YELLOW},
    {RESERVED1_BEGIN, NULL, 24, COLOR_YELLOW},
    {RESERVED2_BEGIN, NULL, 24, COLOR_BLUE},
};

void Button_Function_Execute(void)
{
    for (int i = 0; i < BTN_COUNT; i++)
    {
        if (Button_GetState(i) == 1)
        {
            Button_IDLE[i] = 0;
            if (!buttons[i].HaveTransed)
            {
                uint8_t success = buttonActions[i].sendFunc();
                buttons[i].HaveTransed = true;

                if (success)
                {
                    if (buttonActions[i].leds != NULL)
                    {
                        for (size_t j = 0; j < buttonActions[i].ledCount; j++)
                        {
                            WS2812_Set_Single_Color(
                                buttonActions[i].leds[j],
                                Color_Table[buttonActions[i].color].R,
                                Color_Table[buttonActions[i].color].G,
                                Color_Table[buttonActions[i].color].B,
                                0.1);
                        }
                    }
                    else
                    {
                        for (int j = 0; j <= 23; j++)
                        {
                            WS2812_Set_Single_Color(
                                j,
                                Color_Table[buttonActions[i].color].R,
                                Color_Table[buttonActions[i].color].G,
                                Color_Table[buttonActions[i].color].B,
                                0.1);
                        }
                    }
                }
            }
        }
        else
        {
            buttons[i].HaveTransed = false;
            Button_IDLE[i] = 1;
        }
    }
}

uint8_t OFFENSIVE_RESET(void) { return FDCAN_SendData(&hfdcan2, TxMessage, 0x20, 1); }
uint8_t DEFENSIVE_RESET(void) { return FDCAN_SendData(&hfdcan2, TxMessage, 0x30, 1); }
uint8_t SELF_TEST_BEGIN(void) { return FDCAN_SendData(&hfdcan2, TxMessage, 0x40, 1); }
uint8_t CHALLENGE_BEGIN(void) { return FDCAN_SendData(&hfdcan2, TxMessage, 0x50, 1); }
uint8_t RESERVED1_BEGIN(void) { return FDCAN_SendData(&hfdcan2, TxMessage, 0x60, 1); }
uint8_t RESERVED2_BEGIN(void) { return FDCAN_SendData(&hfdcan2, TxMessage, 0x70, 1); }
