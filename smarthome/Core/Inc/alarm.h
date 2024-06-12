#define ALARM_H_

#define PIR_Living_room_Pin GPIO_PIN_10
#define PIR_Living_room_GPIO_Port GPIOE
#define PIR_Garage_Pin GPIO_PIN_12
#define PIR_Garage_GPIO_Port GPIOE
#define PIR_Kitchen_Pin GPIO_PIN_14
#define PIR_Kitchen_GPIO_Port GPIOE
#define Alarm_LED_Pin GPIO_PIN_15
#define Alarm_LED_GPIO_Port GPIOE
#define Alarm_Buzzer_Pin GPIO_PIN_11
#define Alarm_Buzzer_GPIO_Port GPIOB

void alarm_config();
