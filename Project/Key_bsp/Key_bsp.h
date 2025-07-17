#ifndef _KEY_BSP_H
#define _KEY_BSP_H

#include "WS2812.h"

#define BTN_COUNT 6
#define BTN_DEBOUNCE_MS 5

typedef enum
{
  BTN_OFFENSIVE,
  BTN_DEFENSIVE,
  BTN_SELF_TEST,
  BTN_CHALLENGE,
  BTN_RESERVED1,
  BTN_RESERVED2
} ButtonID;

typedef enum
{
  BTN_STATE_IDLE,
  BTN_STATE_PRESS_DOWN,
  BTN_STATE_PRESSED,
  BTN_STATE_RELEASE_DOWN,
} ButtonState;

typedef struct
{
  GPIO_TypeDef *port;
  uint16_t pin;
  ButtonState state;
  uint32_t lastChangeTime;
  uint8_t stableState;
  bool HaveTransed;
} Button;

extern Button buttons[BTN_COUNT];

uint8_t Button_GetState(ButtonID id);
void Button_Init(void);
void Refresh_Button_State(void);
void Button_Function_Execute(void);
uint8_t OFFENSIVE_RESET(void);
uint8_t DEFENSIVE_RESET(void);
uint8_t SELF_TEST_BEGIN(void);
uint8_t CHALLENGE_BEGIN(void);
uint8_t RESERVED1_BEGIN(void);
uint8_t RESERVED2_BEGIN(void);

#endif
