#ifndef  __LCD_RGB_H
#define	__LCD_RGB_H

#include "stm32h7xx_hal.h"


#include "lcd_fonts.h"
#include <stdio.h>

// 1. If only a single layer is used, this parameter can be defined as 1. If a double layer is used, it needs to be modified to 2.
// 2. The FK743M1-IIT6 core board uses external SDRAM as video memory, with a starting address of 0xC0000000 and a size of 32MB.
// 3. The space required for video memory = resolution * the number of bytes occupied by each pixel. For example, a 480*272 screen using 16-bit color (RGB565 or AEGB1555) requires 480*272*2 = 261120 bytes of video memory.
//
#define LCD_NUM_LAYERS 1 // Define the number of display layers, 750 can drive two layers of display

#define ColorMode_0 LTDC_PIXEL_FORMAT_RGB565 //Define the color format of layer0
//#define	ColorMode_0   LTDC_PIXEL_FORMAT_ARGB1555  
//#define	ColorMode_0    LTDC_PIXEL_FORMAT_ARGB4444  
//#define	ColorMode_0   LTDC_PIXEL_FORMAT_RGB888
//#define	ColorMode_0   LTDC_PIXEL_FORMAT_ARGB8888   


#if LCD_NUM_LAYERS == 2 //If dual-layer is enabled, define the color format of layer1 here			

//	#define	ColorMode_1   LTDC_PIXEL_FORMAT_RGB565   	
// #define ColorMode_1 LTDC_PIXEL_FORMAT_ARGB1555 // ARGB1555 only supports one transparent color, that is, there are only two states: transparent and opaque
// #define ColorMode_1 LTDC_PIXEL_FORMAT_ARGB4444 // ARGB4444 supports 4-bit transparent color and has 16 transparent states
// #define	ColorMode_1   LTDC_PIXEL_FORMAT_RGB888   
	#define ColorMode_1 LTDC_PIXEL_FORMAT_ARGB8888 // ARGB8888 supports 8-bit transparent color and has 256 transparent states	

#endif


//Display direction parameters
// Usage example: LCD_DisplayDirection(Direction_H), set the screen to display horizontally
// Usage example: LCD_DisplayDirection(Direction_V), set the screen to display vertically

#define Direction_H 0 //LCD horizontal screen display
#define Direction_V 1 //LCD vertical screen display

//Set whether to add 0 or spaces when extra bits are displayed in variables.
// Only the two functions LCD_DisplayNumber() to display integers and LCD_DisplayDecimals() to display decimals are used
// Usage example: LCD_ShowNumMode(Fill_Zero) Set the extra bits to fill with 0, for example, 123 can be displayed as 000123
//
#define Fill_Zero 0 //Fill 0
#define Fill_Space 1 //Fill the spaces


/*----------------------------------------Commonly used colors----- --------------------------------------------------------

 1. For the convenience of users, 32-bit colors are defined here, and then automatically converted into the colors required by the corresponding color format through code.
 2. Among the 32-bit colors, from high to low, they correspond to the four color channels A, R, G, and B respectively, where A represents the transparent channel.
 3. Up to 255 levels of transparent color can be set, ff means opaque, 0 means completely transparent
 4. Transparent colors will not work unless you use color formats such as ARGB1555 and ARGB8888 that support transparent colors. ARGB1555 only supports one bit.
	 Transparent color, that is, there are only two states: transparent and opaque. ARGB4444 has 16 levels of transparency, and ARGB8888 supports 255 levels of transparency.
 5. Users can use the palette on the computer to obtain the 24-bit RGB color, then add the transparent channel to obtain the 32-bit color, and then input the 32-bit color
	 LCD_SetColor() or LCD_SetBackColor() can display the corresponding color.
 6. Usage example: The RGB value of pure blue is 0x0000FF. If no transparent color is required, the corresponding 32-bit color value is 0xff0000FF.
 7. The colors defined below are all set to opaque, and users can define the corresponding colors according to their needs */

#define LCD_WHITE 0xffFFFFFF // pure white
#define LCD_BLACK 0xff000000 // Pure black
                           
#define LCD_BLUE 0xff0000FF // Pure blue color
#define LCD_GREEN 0xff00FF00 // Pure green
#define LCD_RED 0xffFF0000 // Pure red
#define LCD_CYAN 0xff00FFFF // Cyan
#define LCD_MAGENTA 0xffFF00FF // Fuchsia
#define LCD_YELLOW 0xffFFFF00 // Yellow
#define LCD_GREY 0xff2C2C2C // Gray
													
#define LIGHT_BLUE 0xff8080FF // Light blue
#define LIGHT_GREEN 0xff80FF80 // Light green color
#define LIGHT_RED 0xffFF8080 // Light red color
#define LIGHT_CYAN 0xff80FFFF // Bright blue color
#define LIGHT_MAGENTA 0xffFF80FF // bright purple-red
#define LIGHT_YELLOW 0xffFFFF80 // bright yellow
#define LIGHT_GREY 0xffA3A3A3 // Light gray
													
#define DARK_BLUE 0xff000080 // Dark blue
#define DARK_GREEN 0xff008000 // dark green
#define DARK_RED 0xff800000 // dark red
#define DARK_CYAN 0xff008080 // Dark teal
#define DARK_MAGENTA 0xff800080 // Dark purple-purple
#define DARK_YELLOW 0xff808000 // dark yellow
#define DARK_GREY 0xff404040 // dark gray

/*------------------------------------------------ ---------- Function declaration----------------------------------------- ------------------*/
	
//void MX_LTDC_Init(void); // LTDC and layer initialization
void LCD_Clear(void); // Clear screen function
void LCD_ClearRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height); // Partial screen clearing function

void LCD_SetLayer(uint8_t Layerx); // Set layer
void LCD_SetColor(uint32_t Color); //Set the brush color
void LCD_SetBackColor(uint32_t Color); //Set the background color
void LCD_DisplayDirection(uint8_t direction); // Set the display direction

//>>>>> Display ASCII characters
void LCD_SetFont(pFONT *fonts); // Set ASCII font
void LCD_DisplayChar(uint16_t x, uint16_t y,uint8_t c); // Display a single ASCII character
void LCD_DisplayString( uint16_t x, uint16_t y, char *p); // Display ASCII string

//>>>>> Display Chinese characters, including ASCII code
void LCD_SetTextFont(pFONT *fonts); //Set text fonts, including Chinese and ASCII fonts
void LCD_DisplayChinese(uint16_t x, uint16_t y, char *pText); // Display a single Chinese character
void LCD_DisplayText(uint16_t x, uint16_t y, char *pText); // Display string, including Chinese and ASCII characters

//>>>>> Display integers or decimals
void LCD_ShowNumMode(uint8_t mode); // Set the display mode, fill the extra bits with spaces or 0
void LCD_DisplayNumber( uint16_t x, uint16_t y, int32_t number, uint8_t len) ; // Display integer
void LCD_DisplayDecimals( uint16_t x, uint16_t y, double number,uint8_t len,uint8_t decs); // Display decimals

//>>>>> Draw pictures

void 	LCD_DrawImage(uint16_t x,uint16_t y,uint16_t width,uint16_t height,const uint8_t *pImage)  ;

//>>>>> 2D graphics drawing function

void  LCD_DrawPoint(uint16_t x,uint16_t y,uint32_t color);   								//����
uint32_t 	LCD_ReadPoint(uint16_t x,uint16_t y);												//����
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2); //Draw line
void LCD_DrawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height); //drawing rectangle
void  LCD_DrawCircle(uint16_t x, uint16_t y, uint16_t r);									//��Բ
void LCD_DrawEllipse(int x, int y, int r1, int r2); //Draw an ellipse

//>>>>> Area filling function

void LCD_FillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height); //Fill the rectangle
void LCD_FillCircle(uint16_t x, uint16_t y, uint16_t r); //Fill circle

/*------------------------------------------------ -------- LCD related parameters--------------------------------------- ----------------*/

#define HBP 43 // Set according to the screen manual
#define VBP  12
#define HSW 1
#define VSW  1
#define HFP  8
#define VFP  8

#define LCD_Width 480 // LCD pixel length
#define LCD_Height 272 // LCD pixel width
#define LCD_MemoryAdd 0xC0000000 // Starting address of video memory

//The bytes occupied by each pixel of layer0

#if ( ColorMode_0 == LTDC_PIXEL_FORMAT_RGB565 || ColorMode_0 == LTDC_PIXEL_FORMAT_ARGB1555 || ColorMode_0 ==LTDC_PIXEL_FORMAT_ARGB4444 )
	#define BytesPerPixel_0 2 //16-bit color mode occupies 2 bytes per pixel
#elif ColorMode_0 == LTDC_PIXEL_FORMAT_RGB888
	#define BytesPerPixel_0 3 //24-bit color mode occupies 3 bytes per pixel
#elif ColorMode_0 == LTDC_PIXEL_FORMAT_ARGB8888
	#define BytesPerPixel_0 4 //32-bit color mode occupies 4 bytes per pixel
#endif	

//The bytes occupied by each pixel of layer1

#if LCD_NUM_LAYERS == 2

	#if ( ColorMode_1 == LTDC_PIXEL_FORMAT_RGB565 || ColorMode_1 == LTDC_PIXEL_FORMAT_ARGB1555 || ColorMode_1 == LTDC_PIXEL_FORMAT_ARGB4444 )
		#define BytesPerPixel_1 2 //16-bit color mode occupies 2 bytes per pixel
	#elif ColorMode_1 == LTDC_PIXEL_FORMAT_RGB888	
		#define BytesPerPixel_1 3 //24-bit color mode occupies 3 bytes per pixel
	#else	
		#define BytesPerPixel_1 4 //32-bit color mode occupies 4 bytes per pixel
	#endif	

	#define LCD_MemoryAdd_OFFSET LCD_Width * LCD_Height * BytesPerPixel_0 //The offset address of the second layer of video memory

#endif

/*------------------------------------------------ -------- LCD backlight pin----------------------------------------- ------------------*/

#define  LCD_Backlight_PIN								GPIO_PIN_5								
#define	LCD_Backlight_PORT							GPIOB								
#define 	GPIO_LDC_Backlight_CLK_ENABLE        	__HAL_RCC_GPIOB_CLK_ENABLE()	 	

#define	LCD_Backlight_OFF		HAL_GPIO_WritePin(LCD_Backlight_PORT, LCD_Backlight_PIN, GPIO_PIN_RESET);	// �رձ���
#define 	LCD_Backlight_ON		HAL_GPIO_WritePin(LCD_Backlight_PORT, LCD_Backlight_PIN, GPIO_PIN_SET);		// ��������



#endif //__LCD_RGB_H
