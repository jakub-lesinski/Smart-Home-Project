#include "stm32f7xx_hal.h"
#include "alarm.h"

void alarm_config()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	  /*Configure GPIO pin Output Level */
	  HAL_GPIO_WritePin(Alarm_LED_GPIO_Port, Alarm_LED_Pin, GPIO_PIN_RESET);

	  /*Configure GPIO pin Output Level */
	  HAL_GPIO_WritePin(Alarm_Buzzer_GPIO_Port, Alarm_Buzzer_Pin, GPIO_PIN_RESET);

	  /*Configure GPIO pins : PEPin PEPin PEPin */
	    GPIO_InitStruct.Pin = PIR_Living_room_Pin|PIR_Garage_Pin|PIR_Kitchen_Pin;
	    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	    GPIO_InitStruct.Pull = GPIO_NOPULL;
	    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	   /*Configure GPIO pin : PtPin */
	   GPIO_InitStruct.Pin = Alarm_LED_Pin;
	   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	   GPIO_InitStruct.Pull = GPIO_NOPULL;
	   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	   HAL_GPIO_Init(Alarm_LED_GPIO_Port, &GPIO_InitStruct);

	   /*Configure GPIO pin : PtPin */
	   GPIO_InitStruct.Pin = Alarm_Buzzer_Pin;
	   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	   GPIO_InitStruct.Pull = GPIO_NOPULL;
	   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	   HAL_GPIO_Init(Alarm_Buzzer_GPIO_Port, &GPIO_InitStruct);
}
