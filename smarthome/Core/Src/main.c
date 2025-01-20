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
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
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
#include "INA219.h"


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
#define RX_BUFFER_SIZE 4
unsigned char position=1;
unsigned char max_pos=4;

char symbol[1] = {"\0"};
char buff[5];
char result[50];
char buffer[10];
uint8_t rxBuffer[10];
uint8_t rxIndex = 0;
char received[RX_BUFFER_SIZE + 1];
char lastMessage[RX_BUFFER_SIZE + 1] = "";// +1 na znak '\0' kończący string

char *mainmenu[4] = {"1.Kitchen", "2.Living room", "3.Garage", "4.Alarm"};
char *menuKitchen[4] = {"1.Lighting", "2.Shutter","",""};
char *menuKitchenLighting[4] = {"1.On","2.Off","3.Set brightness",""};
char *menuKitchenShutter[4] = {"1.Down", "2.Up","",""};
//char *menuKitchenTemperature[4] = {"1.On","2.Off","3.Set Temp",""};
char *menuLivingroom[4] = {"1.Temperature", "2.Lighting","3.Shutter",""};
char *menuLivingroomLighting[4] = {"1.On","2.Off","3.Set brightness",""};
char *menuLivingroomTemperature[4] = {"1.Set Temp","","",""};
char *menuLivingroomShutter[4] = {"1.Down", "2.Up","",""};
char *menuGarage[4] = {"1.Lighting","2.Shutter","3.Gate",""};
char *menuGarageLighting[4] = {"1.On","2.Off","3.Set brightness",""};
char *menuGarageShutter[4] = {"1.Down", "2.Up","",""};
char *menuGarageGate[4] = {"1.Down", "2.Up","",""};
char *menuAlarm[4] = {"1.On","2.Off","3. Set Pin",""};
char **act_menu = mainmenu;

bool refreshLCD = false;
bool alarm = false;
bool alarmLED = false;
bool alarmBeep = false;
bool PIR_Garage, PIR_Livingroom, PIR_Kitchen, PIR_detected;
bool nextStep = false;
bool kitchenShutter = false;
bool livingroomShutter = false;
bool garageShutter = false;
bool garageGate = false;

int i = 0;
int brightnessLivingroom = 500;
int brightnessKitchen = 500;
int brightnessGarage = 500;
int tempLivingroom;
int tempKitchen;
int pinKey;
int intPart;
int fracPart;
int expectedTemp=30;

double temp = 0.0f;
double press = 0.0f;
double roundedValue;

//ENERGY SYSTEM
INA219_t ina219, ina219_2;

uint16_t vbus, vshunt, current, power, vbus2, vshunt2, current2, power2;
uint16_t energymode = 1;
bool PowerSupply, Battery, Output, Solar;
float solar_volts, vshunt_volts, solar_amps, output_volts, vshunt_volts2, current_amps2, solar_power;

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
//ENERGY SYSTEM

// Wył/Wł PowerSupply
void PowerSupply_SetState(GPIO_PinState state) {
    HAL_GPIO_WritePin(PowerSupply_GPIO_Port, PowerSupply_Pin, state);
}

bool PowerSupply_Off(void) {
    PowerSupply_SetState(GPIO_PIN_SET);
    sendBluetoothData("PS00");
    return PowerSupply = false;
}

bool PowerSupply_On(void) {
    PowerSupply_SetState(GPIO_PIN_RESET);
    sendBluetoothData("PS01");
    return PowerSupply = true;
}

// Wył/Wł baterie
void Battery_SetState(GPIO_PinState state) {
    HAL_GPIO_WritePin(Battery_GPIO_Port, Battery_Pin, state);
}

bool Battery_Off(void) {
    Battery_SetState(GPIO_PIN_SET);
    sendBluetoothData("BT00");
    return Battery = false;
}

bool Battery_On(void) {
    Battery_SetState(GPIO_PIN_RESET);
    sendBluetoothData("BT01");
    return Battery = true;
}

// Wył/Wł wyjście
void Output_SetState(GPIO_PinState state) {
    HAL_GPIO_WritePin(Output_GPIO_Port, Output_Pin, state);
}

bool Output_Off(void) {
    Output_SetState(GPIO_PIN_SET);
    return Output = false;
}

bool Output_On(void) {
    Output_SetState(GPIO_PIN_RESET);
    return Output = true;
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void sendBluetoothData(const char* data){
	snprintf(buffer, sizeof(buffer), "%s\n", data);
	HAL_UART_Transmit(&huart6, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
	HAL_Delay(500);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART6) {
        rxIndex++;

        if (rxIndex >= RX_BUFFER_SIZE+2) {
            rxIndex = 0;
        }
        HAL_UART_Receive_IT(huart, &rxBuffer[rxIndex], 1);
    }
}

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
  MX_I2C1_Init();
  MX_USART6_UART_Init();
  MX_TIM1_Init();
  MX_I2C2_Init();
  /* USER CODE BEGIN 2 */
  //LOAD CONFIG
  keypad_config();
  LCD_Initialize();
 //TIMERS AND INTERRUPTS
  HAL_TIM_Base_Start_IT(&htim2);
  BMP2_Init(&bmp2dev);
  //ENERGY SYSTEM
  INA219_Init(&ina219, &hi2c1, INA219_ADDRESS);
  INA219_Init(&ina219_2, &hi2c2, INA219_ADDRESS);
  Output_On();
  //START SCREEN
  LCD_WriteCommand(HD44780_CLEAR);
  LCD_WriteText("Welcome to");
  LCD_WriteTextXY("Smarthome system",0,1);
  HAL_Delay(3000);
  refreshLCD = true;
  HAL_UART_Receive_IT(&huart6, &rxBuffer[rxIndex], 1);  // Rozpocznij odbiór


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
			case 2: act_menu = menuLivingroom, position = 1, max_pos = 3; break;
			case 3: act_menu = menuGarage, position = 1, max_pos = 3; break;
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
			case 1: act_menu = menuKitchenLighting, position = 1, max_pos = 3; break;
			case 2: act_menu = menuKitchenShutter, position = 1, max_pos = 2; break;
			default: act_menu = menuKitchen, position = 1, max_pos = 2; break;
		}
	}
	else if(symbol[0] == '*' && act_menu == menuKitchenLighting){
 		 	 		refreshLCD = true;
 		 	 	  switch (position){
 		 	 		case 1: LCD_WriteCommand(HD44780_CLEAR);
 		 	 				LCD_WriteText("Light");
 		 	 				LCD_WriteTextXY("turned on",0,1);
 		 	 				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, brightnessKitchen);
 		 	 				HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
 		 	 				break;

 		 	 		case 2: LCD_WriteCommand(HD44780_CLEAR);
 				 	 		LCD_WriteText("Light");
 				 	 		LCD_WriteTextXY("turned off",0,1);
 				 	 		HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_4);
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
								   __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, brightnessKitchen);
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

	else if(symbol[0] == '*' && act_menu == menuKitchenShutter) {
			refreshLCD = true;
			HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
			switch(position) {
				case 1:
					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1, 500);
					HAL_Delay(500);
				   __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1, 0);
				   kitchenShutter = true;
				   LCD_WriteText("Shutter");
				   LCD_WriteTextXY("is lowering",0,1);
				   break;
				case 2:
					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1, 2000);
					HAL_Delay(500);
					__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
					kitchenShutter = false;
					LCD_WriteText("Shutter");
					LCD_WriteTextXY("go up",0,1);
			}
	}

	else if(symbol[0] == '*' && act_menu == menuLivingroom){
					refreshLCD = true;
				  switch (position){
						case 1: act_menu = menuLivingroomTemperature, position = 1, max_pos = 2; break;
						case 2: act_menu = menuLivingroomLighting, position = 1, max_pos = 3; break;
						case 3: act_menu = menuLivingroomShutter, position = 1, max_pos = 2; break;
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
 				 	 		HAL_TIM_PWM_Stop(&htim4 , TIM_CHANNEL_1);
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
			case 1:
				while(1) {
					HAL_Delay(500);
					BMP2_ReadData(&bmp2dev, &press, &temp);
					roundedValue = roundToTwoDecimals(temp);
					intPart = (int)roundedValue;
					fracPart = (int)((roundedValue - intPart) * 100);
					snprintf(result, sizeof(result), "Temp: %d.%04d", intPart, abs(fracPart));
					__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 990);
					HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
					LCD_WriteCommand(HD44780_CLEAR);
					LCD_WriteText(result);
					if(symbol[0] == '*') {
						act_menu = menuLivingroomTemperature;
						position = 1;
						max_pos = 3;
						break;
					}
					i++;
				}
				break;
			case 2: LCD_WriteCommand(HD44780_CLEAR);
					LCD_WriteText("Heat");
					LCD_WriteTextXY("Turned off",0,1);; break;
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
						tempLivingroom = atoi(buff)*10;
						if(tempLivingroom >= 1000) {
							tempLivingroom = 999;
						}
						act_menu = menuLivingroomTemperature;
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
	else if(symbol[0] == '*' && act_menu == menuLivingroomShutter) {
			refreshLCD = true;
			HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
			switch(position) {
				case 1:
					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2, 500);
					HAL_Delay(500);
				   __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2, 0);
				   livingroomShutter = true;
				   LCD_WriteText("Shutter");
				   LCD_WriteTextXY("is lowering",0,1);
				   break;
				case 2:
					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2, 2000);
					HAL_Delay(500);
					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2, 0);
					livingroomShutter = false;
					LCD_WriteText("Shutter");
					LCD_WriteTextXY("go up",0,1);
			}
	}
	else if(symbol[0] == '*' && act_menu == menuGarage){
 		 		 	 		refreshLCD = true;
 		 		 	 	  switch (position){
 		 		 	 	  	  	case 1: act_menu = menuGarageLighting, position = 1, max_pos = 3; break;
 		 		 	 	  	  	case 2: act_menu = menuGarageShutter, position = 1, max_pos = 2; break;
 		 		 	 	  	  	case 3: act_menu = menuGarageGate, position = 1, max_pos = 2; break;
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
 			else if(symbol[0] == '*' && act_menu == menuGarageShutter) {
 						refreshLCD = true;
 						HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
 						switch(position) {
 							case 1:
 								__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3, 500);
 								HAL_Delay(500);
 							   __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3, 0);
 							  garageShutter= true;
 							   LCD_WriteText("Shutter");
 							   LCD_WriteTextXY("is lowering",0,1);
 							   break;
 							case 2:
 								__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3, 2000);
 								HAL_Delay(500);
 								__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3, 0);
 								garageShutter = false;
 								LCD_WriteText("Shutter");
 								LCD_WriteTextXY("go up",0,1);
 						}
 				}
 			else if(symbol[0] == '*' && act_menu == menuGarageGate) {
 			 						refreshLCD = true;
 			 						HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
 			 						switch(position) {
 			 							case 1:
 			 								__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4, 500);
 			 								HAL_Delay(500);
 			 							   __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4, 0);
 			 							  garageGate= true;
 			 							   LCD_WriteText("Gate");
 			 							   LCD_WriteTextXY("is lowering",0,1);
 			 							   break;
 			 							case 2:
 			 								__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4, 2000);
 			 								HAL_Delay(500);
 			 								__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4, 0);
 			 								garageGate = false;
 			 								LCD_WriteText("Gate");
 			 								LCD_WriteTextXY("go up",0,1);
 			 						}
 			 				}

 	if (symbol[0] == '#')
 	{
 		refreshLCD = true;
 		act_menu = mainmenu;
 		position = 1;
 		max_pos = 4;
 	}

 	HAL_Delay(200);
 	///////////////SYSTEM ZASILANIA///////////////////////////////////////////////////
 	vbus = INA219_ReadBusVoltage(&ina219);
 		 	   vshunt = INA219_ReadShuntVolage(&ina219);
 		 	   current = INA219_ReadCurrent(&ina219);
 		 	   power = vbus * current;

 		 	   // Konwersja na jednostki podstawowe (V i A)
 		 	    solar_volts = vbus / 1000.0;  // mV -> V
 		 	    vshunt_volts = vshunt / 1000.0; // mV -> V
 		 	    solar_amps = current / 1000.0; // mA -> A

 		 	//        Zaokrąglanie do 3 miejsc po przecinku
 		 	       solar_volts = round(solar_volts * 1000) / 1000;
 		 	       vshunt_volts = round(vshunt_volts * 1000) / 1000;
 		 	       solar_amps = round(solar_amps * 1000) / 1000;
 		 	       solar_power = solar_volts * solar_amps;

 		  vbus2 = INA219_ReadBusVoltage(&ina219_2);
 		   vshunt2 = INA219_ReadShuntVolage(&ina219_2);
 			   current2 = INA219_ReadCurrent(&ina219_2);
 			   power2 = vbus2 * current2;

 			   // Konwersja na jednostki podstawowe (V i A)
 			   	 	    output_volts = vbus2 / 1000.0;  // mV -> V
 			   	 	    vshunt_volts2 = vshunt2 / 1000.0; // mV -> V
 			   	 	    current_amps2 = current2 / 1000.0; // mA -> A

 			   	 	      //  Zaokrąglanie do 3 miejsc po przecinku
 			   	 	       output_volts = round(output_volts * 1000) / 1000;
 			   	 	       vshunt_volts2 = round(vshunt_volts2 * 1000) / 1000;
 			   	 	       current_amps2 = round(current_amps2 * 1000) /  1000;
 	    if(Output == true)
 	    {
 		if(solar_volts >= 4.6) energymode = 1;
 		else if(PowerSupply == false && solar_volts < 4.5 && energymode != 2) energymode = 2;
 		if(output_volts < 4) energymode = 3;
 	    }
 		if(Output == false) energymode = 4; //wylaczenie zasilania, panel jesli jest w stanie to ładuje akumulator
 		switch (energymode) {
 		case 1:
 			PowerSupply_Off();
 			Battery_Off();
 			Solar = true;
 			break;
 		case 2:
 			Battery_On();
 			Solar = false;
 			break;

 		case 3:
 			Battery_Off();
 			PowerSupply_On();
 			Solar = false;

 			break;
 		case 4:
 			PowerSupply_Off();
 			Battery_Off();
 			Solar = false;
 			break;
 		}
 	////////////////////////////////////////////////////////////////////////////////////
 	for (int i = 0; i < RX_BUFFER_SIZE; i++) {
 	    received[i] = rxBuffer[i]; // Kopiuj dane
 	}
 	received[RX_BUFFER_SIZE] = '\0'; // Dodaj znak końca stringa
 	//Wyłączenie światła w kuchnii
 	if (strcmp(received, "LK00") == 0 && strcmp(lastMessage, "LK00") != 0) {
 	    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_3);
 	    strcpy(lastMessage, "LK00");
 	}
 	//Włączenie światła w kuchnii
 	if (strcmp(received, "LK01") == 0 && strcmp(lastMessage, "LK01") != 0) {
 	 	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, brightnessKitchen);
 	 	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
 	 	strcpy(lastMessage, "LK01");
 	}
 	//Wyłączenie światła w salonie
 	if (strcmp(received, "LL00") == 0 && strcmp(lastMessage, "LL00") != 0) {
 	 	HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);
 	 	strcpy(lastMessage, "LL00");
 	}
 	//Włączenie światła w salonie
 	if (strcmp(received, "LL01") == 0 && strcmp(lastMessage, "LL01") != 0) {
 	 	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, brightnessLivingroom);
 	 	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
 	 	strcpy(lastMessage, "LL01");
 	}

 	if (strcmp(received, "LG00") == 0 && strcmp(lastMessage, "LG00") != 0) {
 	 	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
 	 	strcpy(lastMessage, "LG00");
 	}

 	if (strcmp(received, "LG01") == 0 && strcmp(lastMessage, "LG01") != 0) {
 	 	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, brightnessGarage);
 	 	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
 	 	strcpy(lastMessage, "LG01");
 	}

 	if(rxBuffer[0]=='1')  //Suwak jasności kuchnia
 	{
 		brightnessKitchen = (received[1] - '0') * 1000
 		                  + (received[2] - '0') * 100
 		                  + (received[3] - '0')* 10;
 		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, brightnessKitchen);
 	}

 	if(received[0]=='2')	//Suwak jasności salon
 	{
 		brightnessLivingroom = (received[1] - '0') * 1000
                 + (received[2] - '0') * 100
                 + (received[3] - '0') * 10;
 		__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, brightnessLivingroom);
 	}
 	if(received[0]=='3')	//Suwak jasności garaż
 	{
 		brightnessGarage = (received[1] - '0') * 1000
                 + (received[2] - '0') * 100
                 + (received[3] - '0') * 10;
 		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, brightnessGarage);
 	}
 	//Włączenie alarmu
 	if (strcmp(received, "AL01") == 0 && strcmp(lastMessage, "AL01") != 0)
 	{
 		alarm = true;
		PIR_detected = false;
		alarmLED = false;
		HAL_TIM_Base_Start_IT(&htim2);
 	}
 	//Wyłączenie alarmu
 	if (strcmp(received, "AL00") == 0 && strcmp(lastMessage, "AL00") != 0)
 	{
 		alarm = false;
		PIR_detected = false;
		alarmLED = false;
		HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_3);
		HAL_TIM_Base_Stop_IT(&htim2);
 	}
 	//Zamknięcie drzwi
	if (strcmp(received, "DM00") == 0 && strcmp(lastMessage, "DM00") != 0)
	{
		//Logika do napisanie

		strcpy(lastMessage, "DM00");
	}

	//Otwarcie drzwi
	 if (strcmp(received, "DM01") == 0 && strcmp(lastMessage, "DM01") != 0)
	 {
		//Logika do napisania

		strcpy(lastMessage, "DM01");
	 }

	 //Zamknięcie bramy
	if (strcmp(received, "GM00") == 0 && strcmp(lastMessage, "GM00") != 0)
	{
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4, 500);
		HAL_Delay(500);
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4, 0);
		garageGate= true;

		strcpy(lastMessage, "GM00");
	}

	//Otwarcie bramy
	if (strcmp(received, "GM01") == 0 && strcmp(lastMessage, "GM01") != 0)
	{
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4, 2000);
		HAL_Delay(500);
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4, 0);
		garageGate = false;

		strcpy(lastMessage, "GM01");
	}

	//Zamknięcie rolety w kuchnii
	if (strcmp(received, "SK00") == 0 && strcmp(lastMessage, "SK00") != 0)
	{

		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1, 500);
		HAL_Delay(500);
	   __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1, 0);
	   kitchenShutter = true;
		strcpy(lastMessage, "SK00");
	}

	//Otwarcie rolety w kuchnii
	if (strcmp(received, "SK01") == 0 && strcmp(lastMessage, "SK01") != 0)
	{
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1, 2000);
		HAL_Delay(500);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
		kitchenShutter = false;
		strcpy(lastMessage, "SK01");
	}

	//Zamknięcie rolety w salonie
	if (strcmp(received, "SL00") == 0 && strcmp(lastMessage, "SL00") != 0)
	{

		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2, 500);
		HAL_Delay(500);
	   __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2, 0);
	   livingroomShutter = true;
		strcpy(lastMessage, "SL00");
	}

	//Otwarcie rolety w salonie
	if (strcmp(received, "SL01") == 0 && strcmp(lastMessage, "SL01") != 0)
	{
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2, 2000);
		HAL_Delay(500);
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2, 0);
		livingroomShutter = false;
		strcpy(lastMessage, "SL01");
	}


	//Zamknięcie rolety w garażu
	if (strcmp(received, "SG00") == 0 && strcmp(lastMessage, "SG00") != 0)
	{
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3, 500);
		HAL_Delay(500);
	    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3, 0);
	   	garageShutter= true;
		strcpy(lastMessage, "SG00");
	}

	//Otwarcie rolety w garażu
	if (strcmp(received, "SG01") == 0 && strcmp(lastMessage, "SG01") != 0)
	{
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3, 2000);
		HAL_Delay(500);
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3, 0);
		garageShutter = false;
		strcpy(lastMessage, "SG01");
	}

	//Wyłączenie systemu zasilania
	if (strcmp(received, "PW00") == 0 && strcmp(lastMessage, "PW00") != 0)
	{
		OutputOff();

		strcpy(lastMessage, "PW00");
	}

	//Włączenie systemu zasilania
	if (strcmp(received, "PW01") == 0 && strcmp(lastMessage, "PW01") != 0)
	{
		OutputOn();

		strcpy(lastMessage, "PW01");
	}

	//Odbieranie zadanej temperatury
	if(received[0]=='T' && received[1]=='M' )	//Suwak jasności garaż
	{
		//expectedTemp = (received[2] - '0') * 10 + (received[3] - '0');

		//Logika do napisania
	}

 	////////////////////////////////////////////////////////////////////////////////////
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

 	sendBluetoothData("70");
 	sendBluetoothData("23");

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
