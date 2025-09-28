#ifndef  __LCD_TEST_H
#define	__LCD_TEST_H

#include "stm32h7xx_hal.h"
#include "lcd_rgb.h"
#include	"lcd_image.h"


void LCD_Test_Clear(void); // Clear screen test
void LCD_Test_Text(void); //Text display test
void LCD_Test_Variable (void); // Variable display, including integers and decimals
void LCD_Test_Color(void); // Color test
void LCD_Test_GrahicTest(void); // 2D graphics drawing
void LCD_Test_FillRect(void); // Rectangular filling test
void LCD_Test_Image(void); // Image display test
void LCD_Test_Vertical(void); // Vertical display test
void LCD_Test_DoubleLayer(void); //Double-layer display

#endif //__LCD_TEST_H
