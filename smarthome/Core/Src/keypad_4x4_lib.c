#include "stm32f7xx_hal.h"
#include "keypad_4x4_lib.h"

const char KeyMap[4][4]={{R1C1, R1C2, R1C3, R1C4},
		{R2C1, R2C2, R2C3, R2C4},
		{R3C1, R3C2, R3C3, R3C4},
		{R4C1, R4C2, R4C3, R4C4}
};

const char Rows[4]={R1, R2, R3, R4};
const char Cols[4]={C1, C2, C3, C4};

void keypad_config(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  if (KEYPAD_4x4_GPIO == GPIOA) __HAL_RCC_GPIOA_CLK_ENABLE(); else
  if (KEYPAD_4x4_GPIO == GPIOB) __HAL_RCC_GPIOB_CLK_ENABLE(); else
  if (KEYPAD_4x4_GPIO == GPIOC) __HAL_RCC_GPIOC_CLK_ENABLE(); else
  if (KEYPAD_4x4_GPIO == GPIOD) __HAL_RCC_GPIOD_CLK_ENABLE(); else
  if (KEYPAD_4x4_GPIO == GPIOE) __HAL_RCC_GPIOE_CLK_ENABLE();


  GPIO_InitStruct.Pin = R1 | R2 | R3 | R4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(KEYPAD_4x4_GPIO, &GPIO_InitStruct);


  GPIO_InitStruct.Pin = C1 | C2 | C3 | C4;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(KEYPAD_4x4_GPIO, &GPIO_InitStruct);

}

char keypad_readkey(void)
{ //returns ASCII code of a pressed key
  unsigned char r, c;
  char pressedKey=0x00;

  r=0;
  while (r<4) {
    HAL_GPIO_WritePin(KEYPAD_4x4_GPIO, Rows[r], GPIO_PIN_SET);        //activate row
    c=0;
    while (c<4) {
      if (HAL_GPIO_ReadPin(KEYPAD_4x4_GPIO, Cols[c])==GPIO_PIN_SET){  //check columns
        pressedKey=KeyMap[r][c];                                   //if column active - find key value
      }
      c++;
    }
    HAL_GPIO_WritePin(KEYPAD_4x4_GPIO, Rows[r], GPIO_PIN_RESET);      //deactivate row
    r++;
  }
  return pressedKey;                                               //return ASCII key value
}

