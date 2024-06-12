#include "stm32f7xx_hal.h"
#include "lcd_hd44780_lib.h"

GPIO_InitTypeDef GPIO_InitStruct;

//-----------------------------------------------------------------------------
void LCD_WriteNibble(char nibbleToWrite)
{
  HAL_GPIO_WritePin(LCD_GPIO, LCD_EN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LCD_GPIO, LCD_D4, (GPIO_PinState)(nibbleToWrite & 0x01));
  HAL_GPIO_WritePin(LCD_GPIO, LCD_D5, (GPIO_PinState)(nibbleToWrite & 0x02));
  HAL_GPIO_WritePin(LCD_GPIO, LCD_D6, (GPIO_PinState)(nibbleToWrite & 0x04));
  HAL_GPIO_WritePin(LCD_GPIO, LCD_D7, (GPIO_PinState)(nibbleToWrite & 0x08));
  HAL_GPIO_WritePin(LCD_GPIO, LCD_EN, GPIO_PIN_RESET);
}


//-----------------------------------------------------------------------------
unsigned char LCD_ReadNibble(void)
{
  unsigned char tmp = 0;
  HAL_GPIO_WritePin(LCD_GPIO, LCD_EN, GPIO_PIN_SET);
  tmp |= (HAL_GPIO_ReadPin(LCD_GPIO, LCD_D4) << 0);
  tmp |= (HAL_GPIO_ReadPin(LCD_GPIO, LCD_D5) << 1);
  tmp |= (HAL_GPIO_ReadPin(LCD_GPIO, LCD_D6) << 2);
  tmp |= (HAL_GPIO_ReadPin(LCD_GPIO, LCD_D7) << 3);
  HAL_GPIO_WritePin(LCD_GPIO, LCD_EN, GPIO_PIN_RESET);
  return tmp;
}


//-----------------------------------------------------------------------------
unsigned char LCD_ReadStatus(void)
{
  unsigned char status = 0;
  
  GPIO_InitStruct.Pin =  LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  
  HAL_GPIO_WritePin(LCD_GPIO, LCD_RW, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LCD_GPIO, LCD_RS, GPIO_PIN_RESET);
  
  status |= (LCD_ReadNibble() << 4);
  status |= LCD_ReadNibble();
  

  GPIO_InitStruct.Pin = LCD_D4|LCD_D5|LCD_D6|LCD_D7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  return status;
}


//-----------------------------------------------------------------------------
void LCD_WriteData(char dataToWrite)
{
  HAL_GPIO_WritePin(LCD_GPIO, LCD_RW, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LCD_GPIO, LCD_RS, GPIO_PIN_SET);
  
  LCD_WriteNibble(dataToWrite >> 4);
  LCD_WriteNibble(dataToWrite & 0x0F);
  
  while(LCD_ReadStatus() & 0x80);
}


//-----------------------------------------------------------------------------
void LCD_WriteCommand(char commandToWrite)
{
  HAL_GPIO_WritePin(LCD_GPIO, LCD_RW | LCD_RS, GPIO_PIN_RESET);
  LCD_WriteNibble(commandToWrite >> 4);
  LCD_WriteNibble(commandToWrite & 0x0F);
  
  while(LCD_ReadStatus() & 0x80);
}


//-----------------------------------------------------------------------------
void LCD_WriteText(char * text)
{
  while(*text)
    LCD_WriteData(*text++);
}


//-----------------------------------------------------------------------------
void LCD_GoTo(unsigned char x, unsigned char y)
{
  LCD_WriteCommand(HD44780_DDRAM_SET | (x + (0x40 * y)));
}


//-----------------------------------------------------------------------------
void LCD_WriteTextXY(char * text, unsigned char x, unsigned char y)
{
  LCD_GoTo(x,y);
  while(*text)
    LCD_WriteData(*text++);
}


//-----------------------------------------------------------------------------
void LCD_WriteBinary(unsigned int var, unsigned char bitCount)
{
  signed char i;
  
  for(i = (bitCount - 1); i >= 0; i--) {
     LCD_WriteData((var & (1 << i))?'1':'0');
  }
}


//-----------------------------------------------------------------------------
void LCD_ShiftLeft(void)
{
  LCD_WriteCommand(HD44780_DISPLAY_CURSOR_SHIFT | HD44780_SHIFT_LEFT | HD44780_SHIFT_DISPLAY);
}


//-----------------------------------------------------------------------------
void LCD_ShiftRight(void)
{
  LCD_WriteCommand(HD44780_DISPLAY_CURSOR_SHIFT | HD44780_SHIFT_RIGHT | HD44780_SHIFT_DISPLAY);
}


//-----------------------------------------------------------------------------
void LCD_Initialize(void)
{
  volatile unsigned char i = 0;
  volatile unsigned int delayCnt = 0;
  
  if (LCD_GPIO == GPIOA) __HAL_RCC_GPIOA_CLK_ENABLE(); else
  if (LCD_GPIO == GPIOB) __HAL_RCC_GPIOB_CLK_ENABLE(); else
  if (LCD_GPIO == GPIOC) __HAL_RCC_GPIOC_CLK_ENABLE(); else
  if (LCD_GPIO == GPIOD) __HAL_RCC_GPIOD_CLK_ENABLE(); else
  if (LCD_GPIO == GPIOE) __HAL_RCC_GPIOE_CLK_ENABLE();


  GPIO_InitStruct.Pin = LCD_D4|LCD_D5|LCD_D6|LCD_D7|LCD_RS|LCD_RW|LCD_EN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(LCD_GPIO, &GPIO_InitStruct);
  
  HAL_GPIO_WritePin(LCD_GPIO, LCD_RS | LCD_EN | LCD_RW, GPIO_PIN_RESET);
  

  for(delayCnt = 0; delayCnt < 300000; delayCnt++);

  for(i = 0; i < 3; i++) {
    LCD_WriteNibble(0x03);            
    for(delayCnt = 0; delayCnt < 30000; delayCnt++);
  }
  
  LCD_WriteNibble(0x02);             
  
  for(delayCnt = 0; delayCnt < 6000; delayCnt++);
     
  LCD_WriteCommand(HD44780_FUNCTION_SET | 
                   HD44780_FONT5x7 | 
                   HD44780_TWO_LINE | 
                   HD44780_4_BIT);
  
  LCD_WriteCommand(HD44780_DISPLAY_ONOFF | 
                   HD44780_DISPLAY_OFF); 
  
  LCD_WriteCommand(HD44780_CLEAR); 
  
  LCD_WriteCommand(HD44780_ENTRY_MODE | 
                   HD44780_EM_SHIFT_CURSOR | 
                   HD44780_EM_INCREMENT);
  
  LCD_WriteCommand(HD44780_DISPLAY_ONOFF | 
                   HD44780_DISPLAY_ON |
                   HD44780_CURSOR_OFF | 
                   HD44780_CURSOR_NOBLINK);
}


//-----------------------------------------------------------------------------
void LCD_SetUserChar (char chrNum, unsigned char n, const char *p)
{         //chrNum  - character number (code) to be registered (0..7)
          //n       - number of characters to register
          //*p      - pointer to the character pattern (8 * n bytes)
	LCD_WriteCommand(HD44780_CGRAM_SET | chrNum * 8);
	n *= 8;
	do
		LCD_WriteData(*p++);
	while (--n);
}


//-----------------------------------------------------------------------------
