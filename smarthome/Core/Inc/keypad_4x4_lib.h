#define KEYPAD_4X4_LIB_H_

#define KEYPAD_4x4_GPIO GPIOD
//row lines
#define R1 GPIO_PIN_0
#define R2 GPIO_PIN_1
#define R3 GPIO_PIN_2
#define R4 GPIO_PIN_3

//column lines
#define C1 GPIO_PIN_4
#define C2 GPIO_PIN_5
#define C3 GPIO_PIN_6
#define C4 GPIO_PIN_7

//ASCII codes
#define R1C1 0x31 //1
#define R1C2 0x32 //2
#define R1C3 0x33 //3
#define R1C4 0x41 //A

#define R2C1 0x34 //4
#define R2C2 0x35 //5
#define R2C3 0x36 //6
#define R2C4 0x42 //B

#define R3C1 0x37 //7
#define R3C2 0x38 //8
#define R3C3 0x39 //9
#define R3C4 0x43 //C

#define R4C1 0x2A //*
#define R4C2 0x30 //0
#define R4C3 0x23 //#
#define R4C4 0x44 //D

void keypad_config(void);
char keypad_readkey(void);


