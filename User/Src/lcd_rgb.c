/***
	************************************************************************************************************************************************************************************************
	*	@file  	lcd_rgb.c
	*	@version V1.0
	*  @date    2021-7-20
	* @author anti-customer technology
	* @brief drives RGB display for display	
   **********************************************************************************************************************************************************************************************
   *  @description
	*
	* Experimental platform: FK STM32H743IIT6 core board (model: FK743M2-IIT6) + 4.3-inch RGB LCD screen (screen model RGB043M1-480*272)
	* Eshop: https://kavirelectronic.ir/eshop
	* TELEGRAM ID:   @kavirsupport
	*
>>>>> Important Note:
	*
	* 1. The FK743M2-IIT6 core board uses external SDRAM as video memory, with a starting address of 0xC0000000 and a size of 32MB.
	* 2. When the program has just been downloaded, it is normal for the screen to jitter and flicker slightly. Wait for a moment or power on again to return to normal.
	* 3. The LTDC clock is set in the SystemClock_Config() function in the main.c file. It is configured to 10MHz, that is, the refresh rate is around 60 frames. If it is too high or too low, it will cause flickering.
	* 
>>>>> Other instructions:
	*
	* 1. The Chinese character library uses a small character library, that is, the corresponding Chinese characters are used and then the model is taken. Users can add or delete them according to their needs.
	* 2. For the function and use of each function, please refer to the description of the function and the test function in the lcd_test.c file
	*
	****************************************************** ****************************************************** ****************************************************** *******************************************FAN*****
***/

#include "lcd_rgb.h"

extern DMA2D_HandleTypeDef hdma2d; // DMA2D handle
extern LTDC_HandleTypeDef hltdc; // LTDC handle

static pFONT *LCD_Fonts; // English font
static pFONT *LCD_CHFonts; // Chinese font

//LCD related parameter structure
struct	
{
	uint32_t Color; //LCD current brush color
	uint32_t BackColor; //Background color
	uint32_t ColorMode; // Color format
	uint32_t LayerMemoryAdd; //Layer memory address
	uint8_t Layer; // current layer
	uint8_t Direction; // Display direction
	uint8_t BytesPerPixel; //The number of bytes occupied by each pixel
	uint8_t ShowNum_Mode; //Number display mode
}LCD;

/*************************************************************************************************
* Function name: HAL_LTDC_MspInit
* Entry parameters: None
* Return value: None
* Function: Initialize the IO port of the LTDC pin
* Note: Will be called by HAL_LTDC_Init() function			
*************************************************************************************************/

void HAL_LTDC_MspInit_(LTDC_HandleTypeDef* hltdc)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if(hltdc->Instance==LTDC)
	{
		__HAL_RCC_LTDC_CLK_ENABLE(); // Enable LTDC clock

		__HAL_RCC_GPIOE_CLK_ENABLE(); // Enable IO port clock
		__HAL_RCC_GPIOI_CLK_ENABLE();
		__HAL_RCC_GPIOF_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_GPIOH_CLK_ENABLE();
		__HAL_RCC_GPIOG_CLK_ENABLE();
		__HAL_RCC_GPIOD_CLK_ENABLE();
		GPIO_LDC_Backlight_CLK_ENABLE; // Enable backlight pin IO port clock

/*------------------------------LTDC GPIO Configuration--------------------------

    PG13    ------> LTDC_R0		PE5     ------> LTDC_G0			PG14    ------> LTDC_B0
    PA2     ------> LTDC_R1      PE6     ------> LTDC_G1       PG12    ------> LTDC_B1
    PH8     ------> LTDC_R2      PH13    ------> LTDC_G2       PD6     ------> LTDC_B2
    PH9     ------> LTDC_R3      PH14    ------> LTDC_G3       PA8     ------> LTDC_B3
    PH10    ------> LTDC_R4      PH15    ------> LTDC_G4       PI4     ------> LTDC_B4
    PH11    ------> LTDC_R5	   PI0     ------> LTDC_G5       PI5     ------> LTDC_B5
    PH12    ------> LTDC_R6      PI1     ------> LTDC_G6       PI6     ------> LTDC_B6
    PG6     ------> LTDC_R7	   PI2     ------> LTDC_G7       PI7     ------> LTDC_B7

    PG7     ------> LTDC_CLK
    PF10 ------> LTDC_DE
    PI9     ------> LTDC_VSYNC
    PI10    ------> LTDC_HSYNC

----------------------------------------------------------------------------------*/


		GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
		HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_0|GPIO_PIN_1
								  |GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6
								  |GPIO_PIN_7;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
		HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_10;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
		HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_2;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
								  |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
		HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_12|GPIO_PIN_13
								  |GPIO_PIN_14;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
		HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_8;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF13_LTDC;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_6;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
		HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

		//Initialize backlight pin

		GPIO_InitStruct.Pin = LCD_Backlight_PIN; // Backlight pin
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // Push-pull output mode
		GPIO_InitStruct.Pull = GPIO_NOPULL; // No pull-down
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; // Speed ??level is low
		HAL_GPIO_Init(LCD_Backlight_PORT, &GPIO_InitStruct); // Initialization

		LCD_Backlight_OFF; // Turn off the backlight pin first, and then turn it on after initializing LTDC
	}
}


/*************************************************************************************************
* Function name: MX_LTDC_Init
* Entry parameters: None
* Return value: None
* Function: Initialize the IO port, global parameters, layer settings, etc. of the LTDC pin
* Description: None			
*************************************************************************************************/

void MX_LTDC_Init(void)
{
	LTDC_LayerCfgTypeDef pLayerCfg = {0}; // layer0 related parameters
	__HAL_RCC_DMA2D_CLK_ENABLE(); // Enable DMA2D clock

	hltdc.Instance 		 = LTDC;
	hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL; // Active low level
	hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL; // Active low level
	hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL; // Low level is effective. It should be noted that many panels are high level, but 743 needs to be set to low level to display normally.
	hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC; //Normal clock signal

	hltdc.Init.HorizontalSync = HSW - 1; // Just set parameters according to the screen
	hltdc.Init.VerticalSync 		= VSW	-1 ;
	hltdc.Init.AccumulatedHBP		= HBP + HSW -1;
	hltdc.Init.AccumulatedVBP 		= VBP + VSW -1;
	hltdc.Init.AccumulatedActiveW = LCD_Width  + HSW + HBP -1;
	hltdc.Init.AccumulatedActiveH = LCD_Height + VSW + VBP -1;
	hltdc.Init.TotalWidth = LCD_Width + HSW + HBP + HFP - 1;
	hltdc.Init.TotalHeigh 			= LCD_Height + VSW + VBP + VFP - 1;

	hltdc.Init.Backcolor.Red = 0; //Initial background color, R
	hltdc.Init.Backcolor.Green = 0; //Initial background color, G
	hltdc.Init.Backcolor.Blue = 0; //Initial background color, B

	HAL_LTDC_Init(&hltdc); //Initialize LTDC parameters

/*---------------------------------- layer0 display configuration---------- ---------------------*/

	pLayerCfg.WindowX0 = 0; // Horizontal starting point
	pLayerCfg.WindowX1 = LCD_Width; // Horizontal end point
	pLayerCfg.WindowY0 = 0; // Vertical starting point
	pLayerCfg.WindowY1 = LCD_Height; // Vertical end point
	pLayerCfg.ImageWidth = LCD_Width; //Display area width
	pLayerCfg.ImageHeight = LCD_Height; // Display area height
	pLayerCfg.PixelFormat = ColorMode_0; // Color format

//Configure the constant transparency of layer0 and finally write to the LTDC_LxCACR register
// It should be noted that this parameter directly configures the transparency of the entire layer0. Here it is set to 255, which is opaque.
	pLayerCfg.Alpha = 255; //The value range is 0~255, 255 means opaque, 0 means completely transparent

//Set the layer mixing coefficient of layer0 and finally write it to the LTDC_LxBFCR register
// This parameter is used to set the color mixing coefficient between layer0 and the underlying background. The calculation formula is:
// Mixed color = BF1 * color of layer0 + BF2 * color of underlying background
// If layer0 uses a transparent color, it must be configured as LTDC_BLENDING_FACTOR1_PAxCA and LTDC_BLENDING_FACTOR2_PAxCA, otherwise the A channel in ARGB will not work
// For more information, please refer to the reference manual for the introduction of the LTDC_LxBFCR register.
	pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA; // Blending factor 1
	pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA; // Blending factor 2

	pLayerCfg.FBStartAdress = LCD_MemoryAdd; //Video memory address

//Configure the initial default color of layer0, including the values ????of A, R, G, and B, and finally write it to the LTDC_LxDCCR register
	pLayerCfg.Alpha0 = 0; // Initial color, A
	pLayerCfg.Backcolor.Blue = 0; // Initial color, R
	pLayerCfg.Backcolor.Green = 0; // Initial color, G
	pLayerCfg.Backcolor.Red = 0; // Initial color, B

	HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, 0);		// ����layer0

#if ( ( ColorMode_0 == LTDC_PIXEL_FORMAT_RGB888 )||( ColorMode_0 == LTDC_PIXEL_FORMAT_ARGB8888 ) ) // Determine whether to use 24-bit or 32-bit color

// Because the low bits of each channel of 743 use pseudo-random dither output, if color dithering is not turned on, 24-bit or 32-bit colors cannot be displayed normally.

	HAL_LTDC_EnableDither(&hltdc); // Enable color dithering

#endif


/*---------------------------------- layer1 display configuration---------- ---------------------*/

#if ( LCD_NUM_LAYERS == 2 ) //If double layers are defined

	LTDC_LayerCfgTypeDef pLayerCfg1 = {0};

	pLayerCfg1.WindowX0 = 0; // Horizontal starting point
	pLayerCfg1.WindowX1 = LCD_Width; // Horizontal end point
	pLayerCfg1.WindowY0 = 0; //Vertical starting point
	pLayerCfg1.WindowY1 = LCD_Height; // Vertical end point
	pLayerCfg1.ImageWidth = LCD_Width; //Display area width
	pLayerCfg1.ImageHeight = LCD_Height; // Display area height
	pLayerCfg1.PixelFormat = ColorMode_1; // Color format, layer1 should be configured as a format with transparent color, such as ARGB8888 or ARGB1555

// Configure the constant transparency of layer1 and finally write to the LTDC_LxCACR register
// It should be noted that this parameter directly configures the transparency of the entire layer1. Here it is set to 255, which is opaque.
	pLayerCfg1.Alpha = 255; //The value range is 0~255, 255 means opaque, 0 means completely transparent

//Set the layer mixing coefficient of layer1 and finally write it to the LTDC_LxBFCR register
// This parameter is used to set the color mixing coefficient between layer1 and (layer0+background). The calculation formula is:
// Mixed color = BF1 * color of layer1 + BF2 * (layer0 + background mixed color)
// If layer1 uses a transparent color, it must be configured as LTDC_BLENDING_FACTOR1_PAxCA and LTDC_BLENDING_FACTOR2_PAxCA, otherwise the A channel in ARGB will not work
// For more information, please refer to the reference manual for the introduction of the LTDC_LxBFCR register.
	pLayerCfg1.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA; // Blending factor 1
	pLayerCfg1.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA; // Blending factor 2

	pLayerCfg1.FBStartAdress = LCD_MemoryAdd + LCD_MemoryAdd_OFFSET; //Video memory address


//Configure the initial default color of layer1, including the values ????of A, R, G, and B, and finally write it to the LTDC_LxDCCR register
	pLayerCfg1.Alpha0 = 0; // Initial color, A
	pLayerCfg1.Backcolor.Red = 0; // Initial color, R
	pLayerCfg1.Backcolor.Green = 0; // Initial color, G
	pLayerCfg1.Backcolor.Blue = 0; // Initial color, B

	HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg1, 1); // Initialize the configuration of layer1

	#if ( ( ColorMode_1 == LTDC_PIXEL_FORMAT_RGB888 )||( ColorMode_1 == LTDC_PIXEL_FORMAT_ARGB8888 ) ) // Determine whether to use 24-bit or 32-bit color

	// Because the low bits of each channel of 743 use pseudo-random dither output, if color dithering is not turned on, 24-bit or 32-bit colors cannot be displayed normally.

		HAL_LTDC_EnableDither(&hltdc); // Enable color dithering

	#endif

#endif

/*----------------------------------Initialize some default configuration---------- -----------------------*/

	LCD_DisplayDirection(Direction_H); //Set horizontal screen display
	LCD_SetFont(&Font24); //Set the default font
	LCD_ShowNumMode(Fill_Space); //Set number display to fill spaces by default

	LCD_SetLayer(0); //switch to layer0
	LCD_SetBackColor(LCD_BLACK); //Set the background color
	LCD_SetColor(LCD_WHITE); //Set the brush color
	LCD_Clear(); // Clear the screen and refresh the background color

/*---------------------------------- If two-layer display is enabled-------- --------------------------*/

#if LCD_NUM_LAYERS == 2

	LCD_SetLayer(1); // Switch to the foreground layer layer1, layer1 is always above layer0
	LCD_SetBackColor(LCD_BLACK); //Set the background color
	LCD_SetColor(LCD_WHITE); //Set the brush color
	LCD_Clear(); // Clear the screen and refresh the background color

#endif

// After LTDC is initialized, there will be a brief white screen at the moment of power-on.
// Even if the backlight pin is pulled low at the beginning and the screen backlight pin is pulled down with a resistor, this phenomenon will still occur.
// If you need to eliminate this phenomenon, you can perform a short delay before turning on the backlight after initialization.
//
// HAL_Delay(200); // Delay 200ms

	LCD_Backlight_ON; // Turn on backlight

}

/*************************************************************************************************
* Function name: LCD_SetLayer
* Entry parameter: layer - the layer to be displayed and operated, which can be set to 0 or 1, that is, select layer0 or layer1
* Return value: None
* Function: Set the layer to be displayed and operated, switch the corresponding video memory address, color format, etc.
* Note: The LTDC layer order of 743 is fixed, layer1 is above layer0, that is, when two-layer display is turned on,
* layer1 is the foreground layer, usually using a color format with transparent color, layer0 is the background layer,
* When only a single layer is enabled, only layer0 is operated by default.
*************************************************************************************************/

void LCD_SetLayer(uint8_t layer)
{
#if LCD_NUM_LAYERS == 2 // If double layer is turned on
	
	if (layer == 0) // If layer0 is set
	{
		LCD.LayerMemoryAdd = LCD_MemoryAdd; // Get the video memory address of layer0
		LCD.ColorMode = ColorMode_0; // Get the color format of layer0
		LCD.BytesPerPixel = BytesPerPixel_0; // Get the size of the bytes required for each pixel of layer0
	}
	else if(layer == 1) // If layer1 is set
	{
		LCD.LayerMemoryAdd = LCD_MemoryAdd + LCD_MemoryAdd_OFFSET; // Get the video memory address of layer1
		LCD.ColorMode = ColorMode_1; // Get the color format of layer1
		LCD.BytesPerPixel = BytesPerPixel_1; // Get the size of bytes required for each pixel of layer1
	}
	LCD.Layer = layer; //Record the current layer
	
#else // If only a single layer is enabled, the default operation is layer0
	
	LCD.LayerMemoryAdd = LCD_MemoryAdd; // Get the video memory address of layer0
	LCD.ColorMode = ColorMode_0; // Get the color format of layer0
	LCD.BytesPerPixel = BytesPerPixel_0; // Get the size of the bytes required for each pixel of layer0
	LCD.Layer = 0; //The layer mark is set to layer0
	
#endif
}  

/***************************************************************************************************************
* Function name: LCD_SetColor
*
* Entry parameter: Color - the color to be displayed, example: 0xff0000FF represents opaque blue, 0xAA0000FF represents blue with 66.66% transparency
*
* Function: This function is used to set the color of display characters, drawing points and lines, and drawing
*
* Note: 1. In order to facilitate users to use custom colors, the entry parameter Color uses 32-bit color format, and users do not need to worry about color format conversion.
* 2. Among the 32-bit colors, from high to low, they correspond to the 4 color channels A, R, G, and B respectively.
* 3. In the upper 8 bits of the transparent channel, ff means opaque and 0 means completely transparent.
* 4. Transparent colors will not work unless you use color formats such as ARGB1555 and ARGB8888 that support transparent colors. ARGB1555 only supports one bit.
* Transparent color, that is, there are only two states: transparent and opaque. ARGB8888 supports 255 levels of transparency.
* 5. The transparency mentioned here refers to the transparency between the background layer, layer0 and layer1
*
***************************************************************************************************************/

void LCD_SetColor(uint32_t Color)
{
	uint16_t Alpha_Value = 0, Red_Value = 0, Green_Value = 0, Blue_Value = 0; //The value of each color channel

	if( LCD.ColorMode == LTDC_PIXEL_FORMAT_RGB565 ) //Convert 32-bit color to 16-bit color
	{
		Red_Value   = (uint16_t)((Color&0x00F80000)>>8);
		Green_Value = (uint16_t)((Color&0x0000FC00)>>5);
		Blue_Value  = (uint16_t)((Color&0x000000F8)>>3);
		LCD.Color = (uint16_t)(Red_Value | Green_Value | Blue_Value);		
	}
	else if( LCD.ColorMode == LTDC_PIXEL_FORMAT_ARGB1555 ) //Convert 32-bit color to ARGB1555 color
	{
		if( (Color & 0xFF000000) == 0 ) //Determine whether to use transparent color
			Alpha_Value = 0x0000;
		else
			Alpha_Value = 0x8000;

		Red_Value   = (uint16_t)((Color&0x00F80000)>>9);	
		Green_Value = (uint16_t)((Color&0x0000F800)>>6);
		Blue_Value  = (uint16_t)((Color&0x000000F8)>>3);
		LCD.Color = (uint16_t)(Alpha_Value | Red_Value | Green_Value | Blue_Value);	
	}
	else if( LCD.ColorMode == LTDC_PIXEL_FORMAT_ARGB4444 ) //Convert 32-bit color to ARGB4444 color
	{

		Alpha_Value = (uint16_t)((Color&0xf0000000)>>16);
		Red_Value   = (uint16_t)((Color&0x00F00000)>>12);	
		Green_Value = (uint16_t)((Color&0x0000F000)>>8);
		Blue_Value  = (uint16_t)((Color&0x000000F8)>>4);
		LCD.Color = (uint16_t)(Alpha_Value | Red_Value | Green_Value | Blue_Value);	
	}	
	else
		LCD.Color = Color; //24-bit color or 32-bit color does not require conversion
}

/***************************************************************************************************************
* Function name: LCD_SetBackColor
*
* Entry parameter: Color - the color to be displayed, example: 0xff0000FF represents opaque blue, 0xAA0000FF represents blue with 66.66% transparency
*
* Function: Set the background color. This function is used to clear the screen and display the background color of characters.
*
* Note: 1. In order to facilitate users to use custom colors, the entry parameter Color uses 32-bit color format, and users do not need to worry about color format conversion.
* 2. Among the 32-bit colors, from high to low, they correspond to the 4 color channels A, R, G, and B respectively.
* 3. In the upper 8 bits of the transparent channel, ff means opaque and 0 means completely transparent.
* 4. Transparent colors will not work unless you use color formats such as ARGB1555 and ARGB8888 that support transparent colors. ARGB1555 only supports one bit.
* Transparent color, that is, there are only two states: transparent and opaque. ARGB8888 supports 255 levels of transparency.
* 5. The transparency mentioned here refers to the transparency between the background layer, layer0 and layer1
*
***************************************************************************************************************/

void LCD_SetBackColor(uint32_t Color)
 {
	uint16_t Alpha_Value = 0, Red_Value = 0, Green_Value = 0, Blue_Value = 0; //The value of each color channel

	if( LCD.ColorMode == LTDC_PIXEL_FORMAT_RGB565 ) //Convert 32-bit color to 16-bit color
	{
		Red_Value   	= (uint16_t)((Color&0x00F80000)>>8);
		Green_Value 	= (uint16_t)((Color&0x0000FC00)>>5);
		Blue_Value  	= (uint16_t)((Color&0x000000F8)>>3);
		LCD.BackColor	= (uint16_t)(Red_Value | Green_Value | Blue_Value);	
	}
	else if( LCD.ColorMode == LTDC_PIXEL_FORMAT_ARGB1555 ) //Convert 32-bit color to ARGB1555 color
	{
		if( (Color & 0xFF000000) == 0 ) //Determine whether to use transparent color
			Alpha_Value = 0x0000;
		else
			Alpha_Value = 0x8000;

		Red_Value   	= (uint16_t)((Color&0x00F80000)>>9);
		Green_Value 	= (uint16_t)((Color&0x0000F800)>>6);
		Blue_Value  	= (uint16_t)((Color&0x000000F8)>>3);
		LCD.BackColor 	= (uint16_t)(Alpha_Value | Red_Value | Green_Value | Blue_Value);	
	}
	else if( LCD.ColorMode == LTDC_PIXEL_FORMAT_ARGB4444 ) //Convert 32-bit color to ARGB4444 color
	{

		Alpha_Value 	= (uint16_t)((Color&0xf0000000)>>16);
		Red_Value   	= (uint16_t)((Color&0x00F00000)>>12);	
		Green_Value 	= (uint16_t)((Color&0x0000F000)>>8);
		Blue_Value  	= (uint16_t)((Color&0x000000F8)>>4);
		LCD.BackColor 	= (uint16_t)(Alpha_Value | Red_Value | Green_Value | Blue_Value);	
	}		
	
	else	
		LCD.BackColor = Color; //24-bit color or 32-bit color does not require conversion
	
}

/***************************************************************************************************************
* Function name: LCD_SetFont
*
* Entry parameters: *fonts - ASCII font to be set
*
* Function: Set ASCII font, you can choose to use 3216/2412/2010/1608/1206 five sizes of fonts
*
* Instructions: 1. Use the example LCD_SetFont(&Font24) to set the ASCII font of 2412
* 2. Related fonts are stored in lcd_fonts.c 			
*
***************************************************************************************************************/

void LCD_SetFont(pFONT *fonts)
{
  LCD_Fonts = fonts;
}

/***************************************************************************************************************
* Function name: LCD_DisplayDirection
*
* Entry parameters: direction - the direction to be displayed
*
* Function: Set the direction to be displayed. You can enter the parameter Direction_H to represent horizontal screen display, and Direction_V to represent vertical display.
*
* Description: Use the example LCD_DisplayDirection(Direction_H), that is, set the screen to display horizontally
*
***************************************************************************************************************/

void LCD_DisplayDirection(uint8_t direction)
{
	LCD.Direction = direction;
}

/***************************************************************************************************************
* Function name: LCD_Clear
*
* Function: Clear screen function, clear the LCD to the color of LCD.BackColor, implemented using DMA2D
*
* Note: First use LCD_SetBackColor() to set the background color to be cleared, and then call this function to clear the screen.
*
***************************************************************************************************************/

void LCD_Clear(void)
{
	
	DMA2D->CR &= ~(DMA2D_CR_START); // Stop DMA2D
	DMA2D->CR = DMA2D_R2M; // Register to SDRAM
	DMA2D->OPFCCR = LCD.ColorMode; // Set color format
	DMA2D->OOR = 0; // Set row offset
	DMA2D->OMAR = LCD.LayerMemoryAdd; // address
	DMA2D->NLR = (LCD_Width<<16)|(LCD_Height); // Set length and width
	DMA2D->OCOLR = LCD.BackColor; // Color
	
/******	
Wait for the vertical data enable display state, that is, when LTDC is about to refresh the entire screen of data.
Because if the screen is refreshed before one frame is refreshed, there will be tearing.
Users can also use the register reload interrupt to make judgments. However, in order to ensure the simplicity of the routine and the convenience of transplantation, the method of judging the registers is directly used here.
	
If no judgment is made, the DMA2D screen refresh speed is as follows:
	
Color format RGB565 RGB888 ARGB888
Time taken 1.2ms 2.0ms 2.7ms


After adding it, no matter which format, it takes 15.5ms to refresh the screen, but the refresh rate of the screen itself is only 60 frames.
The speed of 15.5ms is enough. Unless it is an occasion with particularly high speed requirements, it is recommended to add a statement to judge vertical waiting to avoid the tearing effect.
	
******/
	while( LTDC->CDSR != 0X00000001); // Determine bit 0 VDES of the display status register LTDC_CDSR: vertical data enable display status
	
	DMA2D->CR |= DMA2D_CR_START; // Start DMA2D
		
	while (DMA2D->CR & DMA2D_CR_START); // Wait for the transfer to complete
	 
}

/***************************************************************************************************************
* Function name: LCD_ClearRect
*
* Entry parameter: x - starting horizontal coordinate, value range 0~479
* y - starting vertical coordinate, value range 0~271
* width - the horizontal length of the area to be cleared
* height - the vertical width of the area to be cleared
*
* Function: Partial clear screen function, clear the area corresponding to the specified position to the color of LCD.BackColor
*
* Note: 1. First use LCD_SetBackColor() to set the background color to be cleared, and then call this function to clear the screen.
* 2. Use the example LCD_ClearRect(10, 10, 100, 50) to clear the area 100 long and 50 wide starting from coordinates (10,10)
*
***************************************************************************************************************/

void LCD_ClearRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{

	DMA2D->CR &= ~(DMA2D_CR_START); // Stop DMA2D
	DMA2D->CR = DMA2D_R2M; // Register to SDRAM
	DMA2D->OPFCCR = LCD.ColorMode; // Set color format
	DMA2D->OCOLR = LCD.BackColor; // Color
	
	if(LCD.Direction == Direction_H) //Horizontal screen filling
	{		
		DMA2D->OOR = LCD_Width - width; // Set line offset
		DMA2D->OMAR		=	LCD.LayerMemoryAdd + LCD.BytesPerPixel*(LCD_Width * y + x);	// ��ַ;
		DMA2D->NLR = (width<<16)|(height); // Set length and width		
	}
	else //Vertical screen filling
	{		
		DMA2D->OOR = LCD_Width - height; // Set row offset
		DMA2D->OMAR		=	LCD.LayerMemoryAdd + LCD.BytesPerPixel*((LCD_Height - x - 1 - width)*LCD_Width + y);	// ��ַ
		DMA2D->NLR = (width)|(height<<16); // Set length and width		
	}		

	DMA2D->CR |= DMA2D_CR_START; // Start DMA2D
		
	while (DMA2D->CR & DMA2D_CR_START); // Wait for the transfer to complete

}


/***************************************************************************************************************
* Function name: LCD_DrawPoint
*
* Entry parameter: x - starting horizontal coordinate, value range 0~479
* y - starting vertical coordinate, value range 0~271
* color - the color to be drawn, using 32-bit color format, users do not need to care about color format conversion
*
* Function: Draw points of specified color at specified coordinates
*
* Note: 1. Directly write the color value in the corresponding video memory location to realize the function of drawing points
* 2. Use the example LCD_DrawPoint(10, 10, 0xff0000FF) to draw a blue point at coordinates (10,10)
*
***************************************************************************************************************/

void LCD_DrawPoint(uint16_t x,uint16_t y,uint32_t color)
{

/*----------------------- 32-bit color ARGB8888 mode-------------------- --*/
		
	if( LCD.ColorMode == LTDC_PIXEL_FORMAT_ARGB8888 ) 
	{
		if (LCD.Direction == Direction_H) //Horizontal direction
		{
			*(__IO uint32_t*)( LCD.LayerMemoryAdd + 4*(x + y*LCD_Width) ) = color ; 	
		}
		else if(LCD.Direction == Direction_V) //Vertical direction
		{
			*(__IO uint32_t*)( LCD.LayerMemoryAdd + 4*((LCD_Height - x - 1)*LCD_Width + y) ) = color ;
		}
	}
/*----------------------------- 24-bit color RGB888 mode-------------- -----------*/	
	
	else if ( LCD.ColorMode == LTDC_PIXEL_FORMAT_RGB888 )
	{		
		if (LCD.Direction == Direction_H) //Horizontal direction
		{
			*(__IO uint16_t*)( LCD.LayerMemoryAdd + 3*(x + y*LCD_Width) ) = color ; 
			*(__IO uint8_t*)( LCD.LayerMemoryAdd + 3*(x + y*LCD_Width) + 2 ) = color>>16 ; 	
		}
		else if(LCD.Direction == Direction_V) //Vertical direction
		{
			*(__IO uint16_t*)( LCD.LayerMemoryAdd + 3*((LCD_Height - x - 1)*LCD_Width + y) ) = color ; 
			*(__IO uint8_t*)( LCD.LayerMemoryAdd + 3*((LCD_Height - x - 1)*LCD_Width + y) +2) = color>>16 ; 	
		}	
	}

/*----------------------- 16-bit color ARGB1555, RGB565 or ARGB4444 mode---------------- ------*/	
	else		
	{
		if (LCD.Direction == Direction_H) //Horizontal direction
		{
			*(__IO uint16_t*)( LCD.LayerMemoryAdd + 2*(x + y*LCD_Width) ) = color ; 	
		}
		else if(LCD.Direction == Direction_V) //Vertical direction
		{
			*(__IO uint16_t*)( LCD.LayerMemoryAdd + 2*((LCD_Height - x - 1)*LCD_Width + y) ) = color ;
		}	
	}
}  

/***************************************************************************************************************
* Function name: LCD_ReadPoint
*
* Entry parameter: x - starting horizontal coordinate, value range 0~479
* y - starting vertical coordinate, value range 0~271
*
* Return value: read color
*
* Function: Read the color of the specified coordinate point. When using the 16-bit or 24-bit color mode, the read color data corresponds to 16-bit or 24-bit.
*
* Description: 1. Directly read the corresponding video memory value to realize the function of reading points
* 2. Use example color = LCD_ReadPoint(10, 10), color is the color of the read coordinate point (10,10)
*
***************************************************************************************************************/

uint32_t LCD_ReadPoint(uint16_t x,uint16_t y)
{
	uint32_t color = 0;

/*----------------------- 32-bit color ARGB8888 mode-------------------- --*/
	if( LCD.ColorMode == LTDC_PIXEL_FORMAT_ARGB8888 ) 
	{
		if (LCD.Direction == Direction_H) //Horizontal direction
		{
			color = *(__IO uint32_t*)( LCD.LayerMemoryAdd + 4*(x + y*LCD_Width) ); 	
		}
		else if(LCD.Direction == Direction_V) //Vertical direction
		{
			color = *(__IO uint32_t*)( LCD.LayerMemoryAdd + 4*((LCD_Height - x - 1)*LCD_Width + y) );
		}
	}
	
/*----------------------------- 24-bit color RGB888 mode-------------- -----------*/	
	else if ( LCD.ColorMode == LTDC_PIXEL_FORMAT_RGB888 )
	{
		if (LCD.Direction == Direction_H) //Horizontal direction
		{
			color = *(__IO uint32_t*)( LCD.LayerMemoryAdd + 3*(x + y*LCD_Width) ) &0x00ffffff; 	
		}
		else if(LCD.Direction == Direction_V) //Vertical direction
		{
			color = *(__IO uint32_t*)( LCD.LayerMemoryAdd + 3*((LCD_Height - x - 1)*LCD_Width + y) ) &0x00ffffff; 	
		}	
	}
	
/*----------------------- 16-bit color ARGB1555, RGB565 or ARGB4444 mode---------------- ------*/	
	else		
	{
		if (LCD.Direction == Direction_H) //Horizontal direction
		{
			color = *(__IO uint16_t*)( LCD.LayerMemoryAdd + 2*(x + y*LCD_Width) ); 	
		}
		else if(LCD.Direction == Direction_V) //Vertical direction
		{
			color = *(__IO uint16_t*)( LCD.LayerMemoryAdd + 2*((LCD_Height - x - 1)*LCD_Width + y) );
		}	
	}
	return color;
}  

/***************************************************************************************************************
* Function name: LCD_DisplayChar
*
* Entry parameter: x - starting horizontal coordinate, value range 0~479
* y - starting vertical coordinate, value range 0~271
*c - ASCII character
*
* Function: Display the specified characters at the specified coordinates
*
* Note: 1. You can set the font to be displayed, for example, use LCD_SetFont(&Font24) to set the ASCII font to 2412
* 2. You can set the color to be displayed, for example, use LCD_SetColor(0xff0000FF) to set it to blue
* 3. The corresponding background color can be set, for example, use LCD_SetBackColor(0xff000000) to set the background color to black
* 4. Use the example LCD_DisplayChar(10, 10, 'a') to display the character 'a' at coordinates (10,10)
*
***************************************************************************************************************/

void LCD_DisplayChar(uint16_t x, uint16_t y,uint8_t c)
{
	uint16_t index = 0, counter = 0; // Count variable
   uint8_t disChar; //The address of stored characters
	uint16_t Xaddress = x; //horizontal coordinate
	
	c = c - 32; // Calculate the offset of ASCII characters
	
	for(index = 0; index < LCD_Fonts->Sizes; index++)	
	{
		disChar = LCD_Fonts->pTable[c*LCD_Fonts->Sizes + index]; //Get the modulus value of the character
		for(counter = 0; counter < 8; counter++)
		{ 
			if(disChar & 0x01)	
			{		
				LCD_DrawPoint(Xaddress,y,LCD.Color); //When the current modulus value is not 0, use the brush color to draw the point
			}
			else		
			{		
				LCD_DrawPoint(Xaddress,y,LCD.BackColor); //Otherwise use the background color to draw the point
			}
			disChar >>= 1;
			Xaddress++; //Horizontal coordinates are added automatically
			
			if( (Xaddress - x)==LCD_Fonts->Width ) //If the horizontal coordinate reaches the character width, exit the current loop
			{ //Enter the drawing of the next line
				Xaddress = x;
				y++;
				break;
			}
		}	
	}
}

/***************************************************************************************************************
* Function name: LCD_DisplayString
*
* Entry parameter: x - starting horizontal coordinate, value range 0~479
* y - starting vertical coordinate, value range 0~271
* p - the first address of the ASCII string
*
* Function: Display the specified string at the specified coordinates
*
* Note: 1. You can set the font to be displayed, for example, use LCD_SetFont(&Font24) to set the ASCII font to 2412
* 2. You can set the color to be displayed, for example, use LCD_SetColor(0xff0000FF) to set it to blue
* 3. The corresponding background color can be set, for example, use LCD_SetBackColor(0xff000000) to set the background color to black
* 4. Use the example LCD_DisplayString(10, 10, "FANKE") to display the string "FANKE" at the starting coordinates of (10,10)
*
***************************************************************************************************************/

void LCD_DisplayString( uint16_t x, uint16_t y, char *p) 
{  
	while ((x < LCD_Width) && (*p != 0)) //Determine whether the display coordinates exceed the display area and whether the character is a null character
	{
		 LCD_DisplayChar( x,y,*p);
		 x += LCD_Fonts->Width; //Display the next character
		 p++; //Get the next character address
	}
}

/***************************************************************************************************************
* Function name: LCD_SetTextFont
*
* Entry parameters: *fonts - the text font to be set
*
* Function: Set text font, including Chinese and ASCII characters,
*
* Note: 1. You can choose to use Chinese fonts in five sizes: 3232/2424/2020/1616/1212.
* And the corresponding ASCII font is set to 3216/2412/2010/1608/1206
* 2. Related fonts are stored in lcd_fonts.c
* 3. The Chinese character library uses a small character library, that is, the corresponding Chinese characters are used and then the model is taken.
* 4. Use the example LCD_SetTextFont(&CH_Font24), that is, set the Chinese font of 2424 and the ASCII character font of 2412
*
***************************************************************************************************************/

void LCD_SetTextFont(pFONT *fonts)
{
	LCD_CHFonts = fonts; // Set Chinese fonts
	switch(fonts->Width )
	{
		case 12: LCD_Fonts = &Font12; break; //Set the ASCII character font to 1206
		case 16: LCD_Fonts = &Font16; break; //Set the ASCII character font to 1608
		case 20: LCD_Fonts = &Font20; break; //Set the font of ASCII characters to 2010	
		case 24: LCD_Fonts = &Font24; break; //Set the font of ASCII characters to 2412
		case 32: LCD_Fonts = &Font32; break; //Set the ASCII character font to 3216		
		default: break;
	}

}
/***************************************************************************************************************
* Function name: LCD_DisplayChinese
*
* Entry parameter: x - starting horizontal coordinate, value range 0~479
* y - starting vertical coordinate, value range 0~271
* pText - Chinese characters
*
* Function: Display the specified single Chinese character at the specified coordinates
*
* Note: 1. You can set the font to be displayed, for example, use LCD_SetTextFont(&CH_Font24) to set the Chinese font to 2424 and the ASCII character font to 2412
* 2. You can set the color to be displayed, for example, use LCD_SetColor(0xff0000FF) to set it to blue
* 3. The corresponding background color can be set, for example, use LCD_SetBackColor(0xff000000) to set the background color to black
* 4. Use the example LCD_DisplayChinese(10, 10, "reverse") to display the Chinese character "reverse" at coordinates (10,10)
*
***************************************************************************************************************/

void LCD_DisplayChinese(uint16_t x, uint16_t y, char *pText) 
{
	uint16_t i=0,index = 0, counter = 0; // Count variable
	uint16_t addr; // Font address
   uint8_t disChar; //Value of font model
	uint16_t Xaddress = x; //horizontal coordinate

	while(1)
	{		
		// Compare the Chinese character codes in the array to locate the address of the Chinese character font		
		if ( *(LCD_CHFonts->pTable + (i+1)*LCD_CHFonts->Sizes + 0)==*pText && *(LCD_CHFonts->pTable + (i+1)*LCD_CHFonts->Sizes + 1)==*(pText+1) )	
		{   
			addr=i; // Font address offset
			break;
		}				
		i+=2; // Each Chinese character encoding occupies two bytes

		if(i >= LCD_CHFonts->Table_Rows) break; // There is no corresponding Chinese character in the font list	
	}	
	

	for(index = 0; index <LCD_CHFonts->Sizes; index++)
	{	
		disChar = *(LCD_CHFonts->pTable + (addr)*LCD_CHFonts->Sizes + index); // Get the corresponding font address
		
		for(counter = 0; counter < 8; counter++)
		{ 
			if(disChar & 0x01)	
			{		
				LCD_DrawPoint(Xaddress,y,LCD.Color); //When the current modulus value is not 0, use the brush color to draw the point
			}
			else		
			{		
				LCD_DrawPoint(Xaddress,y,LCD.BackColor); //Otherwise use the background color to draw the point
			}
			disChar >>= 1;
			Xaddress++; //Horizontal coordinates are added automatically
			
			if( (Xaddress - x)==LCD_CHFonts->Width ) // If the horizontal coordinate reaches the character width, exit the current loop
			{ // Enter the drawing of the next line
				Xaddress = x;
				y++;
				break;
			}
		}	
	}	

}

/***************************************************************************************************************
* Function name: LCD_DisplayText
*
* Entry parameter: x - starting horizontal coordinate, value range 0~479
* y - starting vertical coordinate, value range 0~271
* pText - string, can display Chinese or ASCII characters
*
* Function: Display the specified string at the specified coordinates
*
* Note: 1. You can set the font to be displayed, for example, use LCD_SetTextFont(&CH_Font24) to set the Chinese font to 2424 and the ASCII character font to 2412
* 2. You can set the color to be displayed, for example, use LCD_SetColor(0xff0000FF) to set it to blue
* 3. The corresponding background color can be set, for example, use LCD_SetBackColor(0xff000000) to set the background color to black
* 4. Use the example LCD_DisplayChinese(10, 10, "Anke Technology STM32") to display the string "Anti Ke Technology STM32" at coordinates (10,10)
*
***************************************************************************************************************/

void LCD_DisplayText(uint16_t x, uint16_t y, char *pText) 
{  
 	
	while(*pText != 0) // Determine whether it is a null character
	{
		if(*pText<=0x7F) // Determine whether it is ASCII code
		{
			LCD_DisplayChar(x,y,*pText); // Display ASCII
			x+=LCD_Fonts->Width; // Adjust the horizontal coordinate to the next character
			pText++; // String address +1
		}
		else // if the character is a Chinese character
		{			
			LCD_DisplayChinese(x,y,pText); // Display Chinese characters
			x+=LCD_CHFonts->Width; // Adjust the horizontal coordinate to the next character
			pText+=2; // String address + 2, the encoding of Chinese characters requires 2 bytes
		}
	}	
}


/***************************************************************************************************************
* Function name: LCD_ShowNumMode
*
* Entry parameter: mode - Set the display mode of the variable
*
* Function: Set whether to fill the extra bits with zeros or spaces when displaying variables. You can enter the parameter Fill_Space to fill the spaces, and Fill_Zero to fill the zeros.
*
* Note: 1. Only the two functions LCD_DisplayNumber() to display integers and LCD_DisplayDecimals() to display decimals are used
* 2. Use the example LCD_ShowNumMode(Fill_Zero) to set the extra bits to fill with 0, for example, 123 can be displayed as 000123
*
***************************************************************************************************************/

void LCD_ShowNumMode(uint8_t mode)
{
	LCD.ShowNum_Mode = mode;
}

/*****************************************************************************************************************************************
* Function name: LCD_DisplayNumber
*
* Entry parameter: x - starting horizontal coordinate, value range 0~479
* y - starting vertical coordinate, value range 0~271
* number - the number to be displayed, ranging from -2147483648~2147483647
*len - the number of digits in the number. If the number of digits exceeds len, it will be output according to its actual length, excluding the sign. But if you need to display negative numbers, please reserve one bit of symbol display space.
*
* Function: Display the specified integer variable at the specified coordinates
*
* Note: 1. You can set the font to be displayed, for example, use LCD_SetTextFont(&CH_Font24) to set the Chinese font to 2424 and the ASCII character font to 2412
* 2. You can set the color to be displayed, for example, use LCD_SetColor(0xff0000FF) to set it to blue
* 3. The corresponding background color can be set, for example, use LCD_SetBackColor(0xff000000) to set the background color to black
* 4. Use the example LCD_DisplayNumber(10, 10, a, 5) to display the specified variable a at coordinates (10,10), with a total of 5 digits. The remaining digits are filled with 0 or spaces.
* For example, when a=123, 123 (the first two spaces) or 00123 will be displayed according to the setting of LCD_ShowNumMode()
*						
*****************************************************************************************************************************************/

void  LCD_DisplayNumber( uint16_t x, uint16_t y, int32_t number, uint8_t len) 
{  
	char Number_Buffer[15]; // Used to store the converted string

	if( LCD.ShowNum_Mode == Fill_Zero) // Fill in excess bits with 0
	{
		sprintf(Number_Buffer, "%0.*d",len, number); //Convert number into a string for easy display		
	}
	else // Fill in extra bits with spaces
	{	
		sprintf(Number_Buffer, "%*d",len, number); // Convert number into a string for easy display		
	}
	
	LCD_DisplayString( x, y,(char *)Number_Buffer); // Display the converted string
	
}

/***************************************************************************************************************************************
* Function name: LCD_DisplayDecimals
*
* Entry parameter: x - starting horizontal coordinate, value range 0~479
* y - starting vertical coordinate, value range 0~271
* decimals - the number to be displayed, the double type value is 1.7 x 10^ (-308) ~ 1.7 x 10^ (+308), but it can ensure that the accurate number of significant digits is 15~16 digits
*
*len - the total number of digits in the entire variable (including decimal points and negative signs). If the actual total number of digits exceeds the specified total number of digits, the actual total number of digits will be output.
* Example 1: Decimal -123.123, if len <=8 is specified, -123.123 will actually be output as usual.
* Example 2: Decimal -123.123, if you specify len =10, the actual output is -123.123 (there will be two spaces before the negative sign)
* Example 3: Decimal -123.123, if len =10 is specified, when the function LCD_ShowNumMode() is called and set to fill 0 mode, the actual output is -00123.123
*
* decs - the number of decimal places to be retained. If the actual number of decimal places exceeds the specified decimal places, it will be rounded and output according to the specified width.
* Example: 1.12345, if decs is specified as 4 digits, the output result will be 1.1235
*
* Function: Display the specified variable, including decimals, at the specified coordinates
*
* Note: 1. You can set the font to be displayed, for example, use LCD_SetTextFont(&CH_Font24) to set the Chinese font to 2424 and the ASCII character font to 2412
* 2. You can set the color to be displayed, for example, use LCD_SetColor(0xff0000FF) to set it to blue
* 3. The corresponding background color can be set, for example, use LCD_SetBackColor(0xff000000) to set the background color to black
* 4. Use the example LCD_DisplayDecimals(10, 10, a, 5, 3) to display the word variable a at coordinates (10,10), with a total length of 5 digits, including 3 decimal places.
*						
*****************************************************************************************************************************************/

void  LCD_DisplayDecimals( uint16_t x, uint16_t y, double decimals, uint8_t len, uint8_t decs) 
{  
	char Number_Buffer[20]; // Used to store the converted string
	
	if( LCD.ShowNum_Mode == Fill_Zero) // Excess bits are filled with 0 mode
	{
		//sprintf(Number_Buffer, "%0*.*lf",len,decs, decimals); //Convert number into a string for easy display
	}
	else // Fill in spaces with extra bits
	{
	//	sprintf(Number_Buffer, "%*.*lf",len,decs, decimals); //Convert number into a string for easy display
	}
	
	LCD_DisplayString( x, y,(char *)Number_Buffer); // Display the converted string
}


/***************************************************************************************************************************************
* Function name: LCD_DrawImage
*
* Entry parameter: x - horizontal coordinate, value range 0~479
* y - vertical coordinate, value range 0~271
* width - the horizontal width of the image, the maximum value is 480
* height - the vertical width of the image, the maximum value is 272
* *pImage - the first address of the image data storage area
*
* Function: Display the picture at the specified coordinates
*
* Note: The picture to be displayed needs to be modulated in advance and can only display one color. Use the LCD_SetColor() function to set the brush color.
*						 
*****************************************************************************************************************************************/

void 	LCD_DrawImage(uint16_t x,uint16_t y,uint16_t width,uint16_t height,const uint8_t *pImage) 
{  
   uint8_t disChar; //Value of font model
	uint16_t Xaddress = x; //horizontal coordinate
	uint16_t  i=0,j=0,m=0;
	
	for(i = 0; i <height; i++)
	{
		for(j = 0; j <(float)width/8; j++)
		{
			disChar = *pImage;

			for(m = 0; m < 8; m++)
			{ 
				if(disChar & 0x01)	
				{		
					LCD_DrawPoint(Xaddress,y,LCD.Color); //When the current modulus value is not 0, use the brush color to draw the point
				}
				else		
				{		
					LCD_DrawPoint(Xaddress,y,LCD.BackColor); //Otherwise use the background color to draw the point
				}
				disChar >>= 1;
				Xaddress++; //Horizontal coordinates are added automatically
				
				if((Xaddress - x)==width) //If the horizontal coordinate reaches the character width, exit the current loop
				{ //Enter the drawing of the next line
					Xaddress = x;
					y++;
					break;
				}
			}	
			pImage++;			
		}
	}	
}


/***************************************************************************************************************************************
* Function name: LCD_DrawLine
*
* Entry parameter: x1 - horizontal coordinate of the starting point, value range 0~479
* y1 - Vertical coordinate of the starting point, value range 0~271
*
* x2 - horizontal coordinate of the end point, value range 0~479
* y2 - Vertical coordinate of the end point, value range 0~271
*
* Function: draw a line between two points
*
* Note: This function is transplanted from the routine of ST official evaluation board
*						 
*****************************************************************************************************************************************/

#define ABS(X)  ((X) > 0 ? (X) : -(X))    

void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0, 
	yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0, 
	curpixel = 0;

	deltax = ABS(x2 - x1);        /* The difference between the x's */
	deltay = ABS(y2 - y1);        /* The difference between the y's */
	x = x1;                       /* Start x off at the first pixel */
	y = y1;                       /* Start y off at the first pixel */

	if (x2 >= x1)                 /* The x-values are increasing */
	{
	 xinc1 = 1;
	 xinc2 = 1;
	}
	else                          /* The x-values are decreasing */
	{
	 xinc1 = -1;
	 xinc2 = -1;
	}

	if (y2 >= y1)                 /* The y-values are increasing */
	{
	 yinc1 = 1;
	 yinc2 = 1;
	}
	else                          /* The y-values are decreasing */
	{
	 yinc1 = -1;
	 yinc2 = -1;
	}

	if (deltax >= deltay)         /* There is at least one x-value for every y-value */
	{
	 xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
	 yinc2 = 0;                  /* Don't change the y for every iteration */
	 den = deltax;
	 num = deltax / 2;
	 numadd = deltay;
	 numpixels = deltax;         /* There are more x-values than y-values */
	}
	else                          /* There is at least one y-value for every x-value */
	{
	 xinc2 = 0;                  /* Don't change the x for every iteration */
	 yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
	 den = deltay;
	 num = deltay / 2;
	 numadd = deltax;
	 numpixels = deltay;         /* There are more y-values than x-values */
	}
	for (curpixel = 0; curpixel <= numpixels; curpixel++)
	{
	 LCD_DrawPoint(x,y,LCD.Color);             /* Draw the current pixel */
	 num += numadd;              /* Increase the numerator by the top of the fraction */
	 if (num >= den)             /* Check if numerator >= denominator */
	 {
		num -= den;               /* Calculate the new numerator value */
		x += xinc1;               /* Change the x as appropriate */
		y += yinc1;               /* Change the y as appropriate */
	 }
	 x += xinc2;                 /* Change the x as appropriate */
	 y += yinc2;                 /* Change the y as appropriate */
	}
}

/***************************************************************************************************************************************
* Function name: LCD_DrawRect
*
* Entry parameter: x - horizontal coordinate, value range 0~479
* y - vertical coordinate, value range 0~271
* width - the horizontal width of the image, the maximum value is 480
* height - the vertical width of the image, the maximum value is 272
*
* Function: Draw a rectangular line of specified length and width at the pointing position
*
* Note: This function is transplanted from the routine of ST official evaluation board
*						 
*****************************************************************************************************************************************/

void LCD_DrawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
	/* draw horizontal lines */
	LCD_DrawLine(x, y, x+width, y);
	LCD_DrawLine(x, y+height, x+width, y+height);

	/* draw vertical lines */
	LCD_DrawLine(x, y, x, y+height);
	LCD_DrawLine(x+width, y, x+width, y+height);
}

/***************************************************************************************************************************************
* Function name: LCD_DrawCircle
*
* Entry parameter: x - horizontal coordinate of the center of the circle, value range 0~479
* y - vertical coordinate of the center of the circle, value range 0~271
* r - radius
*
* Function: Draw a circular line with radius r at coordinates (x, y)
*
* Note: 1. This function is transplanted from the routine of ST official evaluation board
* 2. The area to be drawn cannot exceed the display area of ??the screen
*
*****************************************************************************************************************************************/

void LCD_DrawCircle(uint16_t x, uint16_t y, uint16_t r)
{
	int Xadd = -r, Yadd = 0, err = 2-2*r, e2;
	do {   

		LCD_DrawPoint(x-Xadd,y+Yadd,LCD.Color);
		LCD_DrawPoint(x+Xadd,y+Yadd,LCD.Color);
		LCD_DrawPoint(x+Xadd,y-Yadd,LCD.Color);
		LCD_DrawPoint(x-Xadd,y-Yadd,LCD.Color);
		
		e2 = err;
		if (e2 <= Yadd) {
			err += ++Yadd*2+1;
			if ( -Xadd == Yadd && e2 <= Xadd ) e2 = 0 ;
		}
		if (e2 > Xadd) err += ++Xadd*2+1;
    }
    while (Xadd <= 0);
    
}

/***************************************************************************************************************************************
* Function name: LCD_DrawEllipse
*
* Entry parameter: x - horizontal coordinate of the center of the circle, value range 0~479
* y - vertical coordinate of the center of the circle, value range 0~271
* r1 - the length of the horizontal semi-axis
* r2 - length of vertical semi-axis
*
* Function: Draw an elliptical line with the horizontal semi-axis r1 and the vertical semi-axis r2 at coordinates (x, y)
*
* Note: 1. This function is transplanted from the routine of ST official evaluation board
* 2. The area to be drawn cannot exceed the display area of ??the screen
*
*****************************************************************************************************************************************/

void LCD_DrawEllipse(int x, int y, int r1, int r2)
{
  int Xadd = -r1, Yadd = 0, err = 2-2*r1, e2;
  float K = 0, rad1 = 0, rad2 = 0;
   
  rad1 = r1;
  rad2 = r2;
  
  if (r1 > r2)
  { 
    do {
      K = (float)(rad1/rad2);
		 
		LCD_DrawPoint(x-Xadd,y+(uint16_t)(Yadd/K),LCD.Color);
		LCD_DrawPoint(x+Xadd,y+(uint16_t)(Yadd/K),LCD.Color);
		LCD_DrawPoint(x+Xadd,y-(uint16_t)(Yadd/K),LCD.Color);
		LCD_DrawPoint(x-Xadd,y-(uint16_t)(Yadd/K),LCD.Color);     
		 
      e2 = err;
      if (e2 <= Yadd) {
        err += ++Yadd*2+1;
        if ( -Xadd == Yadd && e2 <= Xadd ) e2 = 0 ;
      }
      if (e2 > Xadd) err += ++Xadd*2+1;
    }
    while (Xadd <= 0);
  }
  else
  {
    Yadd = -r2;
    Xadd = 0;
    do { 
      K = (float)(rad2/rad1);

		LCD_DrawPoint(x-(uint16_t)(Xadd/K),y+Yadd,LCD.Color);
		LCD_DrawPoint(x+(uint16_t)(Xadd/K),y+Yadd,LCD.Color);
		LCD_DrawPoint(x+(uint16_t)(Xadd/K),y-Yadd,LCD.Color);
		LCD_DrawPoint(x-(uint16_t)(Xadd/K),y-Yadd,LCD.Color);  
		 
      e2 = err;
      if (e2 <= Xadd) {
        err += ++Xadd*3+1;
        if (-Yadd == Xadd && e2 <= Yadd) e2 = 0;
      }
      if (e2 > Yadd) err += ++Yadd*3+1;     
    }
    while (Yadd <= 0);
  }
}
/***************************************************************************************************************************************
* Function name: LCD_FillRect
*
* Entry parameter: x - horizontal coordinate, value range 0~479
* y - vertical coordinate, value range 0~271
* width - the horizontal width of the image, the maximum value is 480
* height - the vertical width of the image, the maximum value is 272
*
* Function: Fill a solid rectangle with specified length and width at coordinates (x, y)
*
* Description: 1. Implemented using DMA2D
* 2. The area to be drawn cannot exceed the display area of ??the screen
*						 
*****************************************************************************************************************************************/

void LCD_FillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{

	DMA2D->CR &= ~(DMA2D_CR_START); // Stop DMA2D
	DMA2D->CR = DMA2D_R2M; // Register to SDRAM
	DMA2D->OPFCCR = LCD.ColorMode; // Set color format
	DMA2D->OCOLR = LCD.Color; // Color
	
	if(LCD.Direction == Direction_H) //Horizontal screen filling
	{		
		DMA2D->OOR = LCD_Width - width; // Set line offset
		DMA2D->OMAR		=	LCD.LayerMemoryAdd + LCD.BytesPerPixel*(LCD_Width * y + x);	// ��ַ;
		DMA2D->NLR = (width<<16)|(height); // Set length and width		
	}
	else //Vertical screen filling
	{		
		DMA2D->OOR = LCD_Width - height; // Set row offset
		DMA2D->OMAR		=	LCD.LayerMemoryAdd + LCD.BytesPerPixel*((LCD_Height - x - 1 - width)*LCD_Width + y);	// ��ַ
		DMA2D->NLR = (width)|(height<<16); // Set length and width		
	}		

	DMA2D->CR |= DMA2D_CR_START; // Start DMA2D
		
	while (DMA2D->CR & DMA2D_CR_START); // Wait for the transfer to complete

}

/***************************************************************************************************************************************
* Function name: LCD_FillCircle
*
* Entry parameter: x - horizontal coordinate of the center of the circle, value range 0~479
* y - vertical coordinate of the center of the circle, value range 0~271
* r - radius
*
* Function: fill a circular area with radius r at coordinates (x, y)
*
* Note: 1. This function is transplanted from the routine of ST official evaluation board
* 2. The area to be drawn cannot exceed the display area of ??the screen
*
*****************************************************************************************************************************************/

void LCD_FillCircle(uint16_t x, uint16_t y, uint16_t r)
{
  int32_t  D;    /* Decision Variable */ 
  uint32_t  CurX;/* Current X Value */
  uint32_t  CurY;/* Current Y Value */ 
  
  D = 3 - (r << 1);
  
  CurX = 0;
  CurY = r;
  
  while (CurX <= CurY)
  {
    if(CurY > 0) 
    { 
		LCD_DrawLine(x - CurX, y - CurY,x - CurX,y - CurY + 2*CurY);
		LCD_DrawLine(x + CurX, y - CurY,x + CurX,y - CurY + 2*CurY); 
    }
    
    if(CurX > 0) 
    {
		LCD_DrawLine(x - CurY, y - CurX,x - CurY,y - CurX + 2*CurX);
		LCD_DrawLine(x + CurY, y - CurX,x + CurY,y - CurX + 2*CurX); 		 
    }
    if (D < 0)
    {
      D += (CurX << 2) + 6;
    }
    else
    {
      D += ((CurX - CurY) << 2) + 10;
      CurY--;
    }
    CurX++;
  }
  
  LCD_DrawCircle(x, y, r);  
}
/************************************************ ****************************************************** ****************************************************** ****************************************************** ****************************************************** ******************FAN***/
// Experimental platform: Anti-customer STM32H743 core board
//
