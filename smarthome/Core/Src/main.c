/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "keypad_4x4_lib.h"
#include "string.h"
#include "lcd_hd44780_lib.h"
#include "bmp2.h"
#include "bmp2_config.h"



/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
unsigned char position=1;
unsigned char max_pos=4;

char symbol[1] = {"\0"};
char buff[5];
char result[50];

char *mainmenu[4] = {"1.Kitchen", "2.Living room", "3.Garage", "4.Alarm"};
char *menuKitchen[4] = {"1.Temperature", "2.Lighting","",""};
char *menuKitchenLighting[4] = {"1.On","2.Off","3.Set brightness",""};
char *menuKitchenTemperature[4] = {"1.Set Temp","2. off","",""};
char *menuLivingroom[4] = {"1.Temperature", "2.Lighting","",""};
char *menuLivingroomLighting[4] = {"1.On","2.Off","3.Set brightness",""};
char *menuLivingroomTemperature[4] = {"1.Set Temp","","",""};
char *menuGarage[4] = {"1.Lighting","","",""};
char *menuGarageLighting[4] = {"1.On","2.Off","3.Set brightness",""};
char *menuAlarm[4] = {"1.On","2.Off","3. Set Pin",""};
char **act_menu = mainmenu;

bool refreshLCD = false;
bool alarm = false;
bool alarmLED = false;
bool alarmBeep = false;
bool PIR_Garage, PIR_Livingroom, PIR_Kitchen, PIR_detected;
bool nextStep = false;

int i = 0;
int brightnessLivingroom = 500;
int brightnessKitchen = 500;
int brightnessGarage = 500;
int tempLivingroom;
int tempKitchen;
int pinKey;
int intPart;
int fracPart;

double temp = 0.0f;
double press = 0.0f;
double roundedValue;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void move_menu(char *a[])
  {
	symbol[0] = keypad_readkey();
	  if (symbol[0] == 'B' && (position<max_pos)) {
	  	   	  position++;
	  	   	  refreshLCD=true;
	  	   	}

	  	   	if (symbol[0] == 'A' && (position>1)) {
	  	   	  position--;
	  	   	  refreshLCD=true;
	  	   	}
	  	  if (refreshLCD){                           //Odswiezanie LCD tylko przy zmiane zawartosci - usuwa problem "migotania" LCD
	   	  LCD_WriteCommand(HD44780_CLEAR);
	   	  LCD_WriteText("->");
	   	  LCD_WriteText(a[position-1]);
	   	  if (position<4){
	   		LCD_WriteTextXY(a[position-1+1],2,1);
	   	  }
	   	  refreshLCD=false;
	   	}
  }

double roundToTwoDecimals(double value) {
	return floor(value * 100.0 + 0.5) / 100.0;
}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim == &htim2)
  {
	  if((PIR_Garage == true || PIR_Kitchen == true || PIR_Livingroom == true) && alarm == true)
	  {
		  PIR_detected = true, alarmLED = true;
	  }

	  if(alarmLED == true && PIR_detected == true)
	  {
		  HAL_GPIO_TogglePin(Alarm_LED_GPIO_Port, Alarm_LED_Pin);
		  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
	  }
	  else
	  {
		  HAL_GPIO_WritePin(Alarm_LED_GPIO_Port, Alarm_LED_Pin, GPIO_PIN_RESET);
	  };
  }
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */


  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_SPI4_Init();
  MX_SPI5_Init();
  /* USER CODE BEGIN 2 */
  //LOAD CONFIG
  //alarm_config();
  keypad_config();
  LCD_Initialize();
 //TIMERS AND INTERRUPTS
  HAL_TIM_Base_Start_IT(&htim2);
  BMP2_Init(&bmp2dev);
  //START SCREEN
  LCD_WriteCommand(HD44780_CLEAR);
  LCD_WriteText("Welcome to");
  LCD_WriteTextXY("Smarthome system",0,1);
  HAL_Delay(3000);
  refreshLCD = true;




  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if(HAL_GPIO_ReadPin(PIR_Garage_GPIO_Port, PIR_Garage_Pin) == GPIO_PIN_SET){PIR_Garage = true;}
	  	  else {PIR_Garage = false;};
	  if(HAL_GPIO_ReadPin(PIR_Kitchen_GPIO_Port, PIR_Kitchen_Pin) == GPIO_PIN_SET){PIR_Kitchen = true;}
	  	  else {PIR_Kitchen = false;};
	  if(HAL_GPIO_ReadPin(PIR_Livingroom_GPIO_Port, PIR_Livingroom_Pin) == GPIO_PIN_SET){PIR_Livingroom = true;}
	  	  else{PIR_Livingroom = false;};
	  move_menu(act_menu);
 	if (symbol[0] == '*' && act_menu == mainmenu){
 		refreshLCD = true;
 		switch (position){
			case 1: act_menu = menuKitchen, position = 1, max_pos = 2; break;
			case 2: act_menu = menuLivingroom, position = 1, max_pos = 2; break;
			case 3: act_menu = menuGarage, position = 1, max_pos = 1; break;
			case 4:
					i = 0;
					HAL_Delay(200);
					memset(buff, 0, sizeof(buff));
					LCD_WriteCommand(HD44780_CLEAR);
					LCD_WriteText("Write PIN");
					while(1) {
						refreshLCD = true;
						symbol[0] = keypad_readkey();
						if(symbol[0] >= '0' && symbol[0] <= '9' && i < sizeof(buff) - 1) {
							buff[i] = symbol[0];
							i++;
							buff[i] = '\0';
							LCD_WriteCommand(HD44780_CLEAR);
							LCD_WriteText("PIN: ");
							LCD_WriteTextXY(buff, 0, 1);
						}
						HAL_Delay(300);
						symbol[0] = keypad_readkey();
						if(symbol[0] == '*') {
							pinKey = atoi(buff);
							if(pinKey == 1234) {
								act_menu = menuAlarm, position = 1, max_pos = 2; break;
							}
							break;
						}
						HAL_Delay(100);
					}
					break;
			default: act_menu = menuKitchen, position = 1, max_pos = 2; break;
 		}
 	}
 	else if(symbol[0] == '*' && act_menu == menuAlarm){
 		refreshLCD = true;
 		switch (position){
			case 1: alarm = true;
				PIR_detected = false;
				alarmLED = false;
				LCD_WriteCommand(HD44780_CLEAR);
				LCD_WriteText("Alarm");
				LCD_WriteTextXY("turned on",0,1);
				HAL_TIM_Base_Start_IT(&htim2);
				act_menu = mainmenu, position = 1, max_pos = 4;
				break;

 	 		case 2: alarm = false;
				PIR_detected = false;
				alarmLED = false;
				LCD_WriteCommand(HD44780_CLEAR);
				LCD_WriteText("Alarm");
				LCD_WriteTextXY("turned off",0,1);
				HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_3);
				HAL_TIM_Base_Stop_IT(&htim2);
				act_menu = mainmenu, position = 1, max_pos = 4;
				break;

 	 		default: act_menu = menuAlarm, position = 1, max_pos = 2; break;
 		}
 	}
	else if(symbol[0] == '*' && act_menu == menuKitchen){
		refreshLCD = true;
		switch (position){
			case 1: act_menu = menuKitchenTemperature, position = 1, max_pos = 2; break;
			case 2: act_menu = menuKitchenLighting, position = 1, max_pos = 3; break;
			default: act_menu = menuKitchen, position = 1, max_pos = 2; break;
		}
	}
	else if(symbol[0] == '*' && act_menu == menuKitchenLighting){
 		 	 		refreshLCD = true;
 		 	 	  switch (position){
 		 	 		case 1: LCD_WriteCommand(HD44780_CLEAR);
 		 	 				LCD_WriteText("Light");
 		 	 				LCD_WriteTextXY("turned on",0,1);
 		 	 				__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, brightnessKitchen);
 		 	 				HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
 		 	 				break;

 		 	 		case 2: LCD_WriteCommand(HD44780_CLEAR);
 				 	 		LCD_WriteText("Light");
 				 	 		LCD_WriteTextXY("turned off",0,1);
 				 	 		HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_3);
 				 	 		break;

 		 	 		case 3:
							i = 0;
							HAL_Delay(200);
							memset(buff, 0, sizeof(buff));
							LCD_WriteCommand(HD44780_CLEAR);
							LCD_WriteText("Write Bright");
							while(1) {
								refreshLCD = true;
								symbol[0] = keypad_readkey();
								if(symbol[0] >= '0' && symbol[0] <= '9' && i < sizeof(buff) - 1) {
									buff[i] = symbol[0];
									i++;
									buff[i] = '\0';
									LCD_WriteCommand(HD44780_CLEAR);
									LCD_WriteText("Brightness: ");
									LCD_WriteTextXY(buff, 0, 1);
								}
								HAL_Delay(200);
								symbol[0] = keypad_readkey();
								if(symbol[0] == '*') {
									brightnessKitchen = atoi(buff)*10;
									if(brightnessKitchen >= 1000) {
										brightnessKitchen = 999;
									}
								   __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, brightnessKitchen);
									act_menu = menuKitchenLighting;
									position = 1;
									max_pos = 3;
									break;
								}
								HAL_Delay(100);
							}
							break;
 		 	 		default: act_menu = menuKitchen, position = 1, max_pos = 2; break;
 		 	 	  	  }
	}
	else if(symbol[0] == '*' && act_menu == menuKitchenTemperature){
		refreshLCD = true;
		temp = 0.0f;
		switch (position){
			case 1:
					while(1) {
						HAL_Delay(100);
						BMP2_ReadData(&bmp2dev, &press, &temp);
						roundedValue = roundToTwoDecimals(temp);
						intPart = (int)roundedValue;
						fracPart = (int)((roundedValue - intPart) * 100);
						snprintf(result, sizeof(result), "Temp: %d.%04d", intPart, abs(fracPart));
						HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
						__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 100);
						LCD_WriteCommand(HD44780_CLEAR);
						LCD_WriteText(result);
						i++;
					}
					break;

			case 2: LCD_WriteCommand(HD44780_CLEAR);
					LCD_WriteText("grzanie off");
					LCD_WriteTextXY("",0,1);
					__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 0);
					HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_3);
					break;
			case 3:
					i = 0;
					HAL_Delay(200);
					memset(buff, 0, sizeof(buff));
					LCD_WriteCommand(HD44780_CLEAR);
					LCD_WriteText("Write Temp");
					while(1) {
						refreshLCD = true;
						symbol[0] = keypad_readkey();
						if(symbol[0] >= '0' && symbol[0] <= '9' && i < sizeof(buff) - 1) {
							buff[i] = symbol[0];
							i++;
							buff[i] = '\0';
							LCD_WriteCommand(HD44780_CLEAR);
							LCD_WriteText("Temp: ");
							LCD_WriteTextXY(buff, 0, 1);
						}
						HAL_Delay(200);
						symbol[0] = keypad_readkey();
						if(symbol[0] == '*') {
							tempKitchen = atoi(buff)*10;
							if(tempKitchen >= 1000) {
								tempKitchen = 999;
							}
							act_menu = menuKitchenLighting;
							position = 1;
							max_pos = 3;
							break;
						}
						HAL_Delay(100);
					}
					break;
			default: act_menu = menuKitchen, position = 1, max_pos = 2; break;
		}
	}
	else if(symbol[0] == '*' && act_menu == menuLivingroom){
					refreshLCD = true;
				  switch (position){
						case 1: act_menu = menuLivingroomTemperature, position = 1, max_pos = 2; break;
						case 2: act_menu = menuLivingroomLighting, position = 1, max_pos = 3; break;
					default: act_menu = menuLivingroom, position = 1, max_pos = 2; break;
					  }
	}
	else if(symbol[0] == '*' && act_menu == menuLivingroomLighting){
 		 	 		refreshLCD = true;
 		 	 	  switch (position){
 		 	 		case 1: LCD_WriteCommand(HD44780_CLEAR);
 		 	 				LCD_WriteText("Light");
 		 	 				LCD_WriteTextXY("turned on",0,1);
 		 	 				__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, brightnessLivingroom);
							HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
 		 	 				break;

 		 	 		case 2: LCD_WriteCommand(HD44780_CLEAR);
 				 	 		LCD_WriteText("Light");
 				 	 		LCD_WriteTextXY("turned off",0,1);
 				 	 		HAL_TIM_PWM_Stop(&h , TIM_CHANNEL_1);
 				 	 		break;
 		 	 		case 3:
							i = 0;
							refreshLCD = true;
							HAL_Delay(200);
							memset(buff, 0, sizeof(buff));
							LCD_WriteCommand(HD44780_CLEAR);
							LCD_WriteText("Write Bright");
							while(1) {
								refreshLCD = true;
								symbol[0] = keypad_readkey();
								if(symbol[0] >= '0' && symbol[0] <= '9' && i < sizeof(buff) - 1) {
									buff[i] = symbol[0];
									i++;
									buff[i] = '\0';
									LCD_WriteCommand(HD44780_CLEAR);
									LCD_WriteText("Brightness: ");
									LCD_WriteTextXY(buff, 0, 1);
								}
								HAL_Delay(200);
								symbol[0] = keypad_readkey();
								if(symbol[0] == '*') {
									brightnessLivingroom = atoi(buff)*10;
									if(brightnessLivingroom >= 1000) {
										brightnessLivingroom = 999;
									}
								   __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, brightnessLivingroom);
									act_menu = menuLivingroomLighting;
									position = 1;
									max_pos = 3;
									break;
								}
								HAL_Delay(100);
							}
							break;
 		 	 		default: act_menu = menuLivingroom, position = 1, max_pos = 2; break;
 		 	 	  	  }
 		 	 	}
 			else if(symbol[0] == '*' && act_menu == menuLivingroomTemperature){
 			 		 	 		refreshLCD = true;
 			 		 	 	  switch (position){
 			 		 	 		case 1: LCD_WriteCommand(HD44780_CLEAR);
 			 		 	 				LCD_WriteText("");
 			 		 	 				LCD_WriteTextXY("",0,1);
 			 		 	 				break;
 			 		 	 		case 2: LCD_WriteCommand(HD44780_CLEAR);
 			 				 	 		LCD_WriteText("");
 			 				 	 		LCD_WriteTextXY("",0,1);; break;
 			 		 	 		default: act_menu = menuKitchen, position = 1, max_pos = 2; break;
 			 		 	 	  	  }
 			 		 	 	}
 			else if(symbol[0] == '*' && act_menu == menuGarage){
 		 		 	 		refreshLCD = true;
 		 		 	 	  switch (position){
 		 		 	 	  	  	case 1: act_menu = menuGarageLighting, position = 1, max_pos = 3; break;
 		 		 	 		default: act_menu = menuGarage, position = 1, max_pos = 1; break;
 		 		 	 	  	  }
 		 		 	 	}
 			else if(symbol[0] == '*' && act_menu == menuGarageLighting){
 		 	 		refreshLCD = true;
 		 	 	  switch (position){
 		 	 		case 1: LCD_WriteCommand(HD44780_CLEAR);
 		 	 				LCD_WriteText("Light");
 		 	 				LCD_WriteTextXY("turned on",0,1);
 		 	 				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, brightnessGarage);
 		 	 				HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
 		 	 				break;

 		 	 		case 2: LCD_WriteCommand(HD44780_CLEAR);
 				 	 		LCD_WriteText("Light");
 				 	 		LCD_WriteTextXY("turned off",0,1);
 				 	 		HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
 				 	 		break;

 		 	 		case 3:
							i = 0;
							refreshLCD = true;
							HAL_Delay(200);
							memset(buff, 0, sizeof(buff));
							LCD_WriteCommand(HD44780_CLEAR);
							LCD_WriteText("Write Bright");
							while(1) {
								refreshLCD = true;
								symbol[0] = keypad_readkey();
								if(symbol[0] >= '0' && symbol[0] <= '9' && i < sizeof(buff) - 1) {
									buff[i] = symbol[0];
									i++;
									buff[i] = '\0';
									LCD_WriteCommand(HD44780_CLEAR);
									LCD_WriteText("Brightness: ");
									LCD_WriteTextXY(buff, 0, 1);
								}
								HAL_Delay(200);
								symbol[0] = keypad_readkey();
								if(symbol[0] == '*') {
									brightnessGarage = atoi(buff)*10;
									if(brightnessGarage >= 1000) {
										brightnessGarage = 999;
									}
								   __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, brightnessGarage);
									act_menu = menuGarageLighting;
									position = 1;
									max_pos = 3;
									break;
								}
								HAL_Delay(100);
							}
							break;
 		 	 		default: act_menu = menuGarage, position = 1, max_pos = 2; break;
 		 	 	  	  }
 		 	 	}

 	if (symbol[0] == '#')
 	{
 		refreshLCD = true;
 		act_menu = mainmenu;
 		position = 1;
 		max_pos = 4;
 	}

 	HAL_Delay(500);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
