/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SPI4_SCK_temperature_Pin GPIO_PIN_2
#define SPI4_SCK_temperature_GPIO_Port GPIOE
#define BMP2_CSB_Pin GPIO_PIN_4
#define BMP2_CSB_GPIO_Port GPIOE
#define SPI4_MISO_temperature_Pin GPIO_PIN_5
#define SPI4_MISO_temperature_GPIO_Port GPIOE
#define SPI4_MOSI_temperature_Pin GPIO_PIN_6
#define SPI4_MOSI_temperature_GPIO_Port GPIOE
#define USER_Btn_Pin GPIO_PIN_13
#define USER_Btn_GPIO_Port GPIOC
#define USER_Btn_EXTI_IRQn EXTI15_10_IRQn
#define I2C2_SDA_INA219_Output_Pin GPIO_PIN_0
#define I2C2_SDA_INA219_Output_GPIO_Port GPIOF
#define I2C2_SCL_Output_Pin GPIO_PIN_1
#define I2C2_SCL_Output_GPIO_Port GPIOF
#define Battery_Pin GPIO_PIN_3
#define Battery_GPIO_Port GPIOF
#define Output_Pin GPIO_PIN_5
#define Output_GPIO_Port GPIOF
#define MCO_Pin GPIO_PIN_0
#define MCO_GPIO_Port GPIOH
#define TIM2_CH1_RGB_Garage_Pin GPIO_PIN_0
#define TIM2_CH1_RGB_Garage_GPIO_Port GPIOA
#define PowerSupply_Pin GPIO_PIN_3
#define PowerSupply_GPIO_Port GPIOA
#define TIM3_CH3_RGB_Kitchen_Pin GPIO_PIN_0
#define TIM3_CH3_RGB_Kitchen_GPIO_Port GPIOB
#define TIM1_CH1_Kitchen_Shutter_Pin GPIO_PIN_9
#define TIM1_CH1_Kitchen_Shutter_GPIO_Port GPIOE
#define PIR_Livingroom_Pin GPIO_PIN_10
#define PIR_Livingroom_GPIO_Port GPIOE
#define TIM1_CH2_LivingRoom_Shutter_Pin GPIO_PIN_11
#define TIM1_CH2_LivingRoom_Shutter_GPIO_Port GPIOE
#define PIR_Garage_Pin GPIO_PIN_12
#define PIR_Garage_GPIO_Port GPIOE
#define TIM3_CH3_Garage_Shutter_Pin GPIO_PIN_13
#define TIM3_CH3_Garage_Shutter_GPIO_Port GPIOE
#define PIR_Kitchen_Pin GPIO_PIN_14
#define PIR_Kitchen_GPIO_Port GPIOE
#define Alarm_LED_Pin GPIO_PIN_15
#define Alarm_LED_GPIO_Port GPIOE
#define TIM2_CH3_Alarm_Buzzer_Pin GPIO_PIN_10
#define TIM2_CH3_Alarm_Buzzer_GPIO_Port GPIOB
#define STLK_RX_Pin GPIO_PIN_8
#define STLK_RX_GPIO_Port GPIOD
#define STLK_TX_Pin GPIO_PIN_9
#define STLK_TX_GPIO_Port GPIOD
#define TIM4_CH1_RGB_LivingRoom_Pin GPIO_PIN_12
#define TIM4_CH1_RGB_LivingRoom_GPIO_Port GPIOD
#define TIM4_CH3_heating_Pin GPIO_PIN_14
#define TIM4_CH3_heating_GPIO_Port GPIOD
#define USB_PowerSwitchOn_Pin GPIO_PIN_6
#define USB_PowerSwitchOn_GPIO_Port GPIOG
#define USB_OverCurrent_Pin GPIO_PIN_7
#define USB_OverCurrent_GPIO_Port GPIOG
#define TIM3_CH4_Garage_Pin GPIO_PIN_9
#define TIM3_CH4_Garage_GPIO_Port GPIOC
#define TIM1_CH4_Door_Pin GPIO_PIN_11
#define TIM1_CH4_Door_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SW0_Pin GPIO_PIN_3
#define SW0_GPIO_Port GPIOB
#define I2C1_SCL_INA219_Solar_Pin GPIO_PIN_6
#define I2C1_SCL_INA219_Solar_GPIO_Port GPIOB
#define LD2_Pin GPIO_PIN_7
#define LD2_GPIO_Port GPIOB
#define I2C1_SDA_INA219_Solar_Pin GPIO_PIN_9
#define I2C1_SDA_INA219_Solar_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
