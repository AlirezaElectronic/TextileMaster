/***
	***************************************************************************************************************************
	*	@file  	lcd_test.c
	*	@version V1.0
	*  @date    2021-7-20
	* @author anti-customer technology
	* @brief LTDC test function	
   ***************************************************************************************************************************
   *  @description
	*
	* Experimental platform: FK STM32H743IIT6 core board (model: FK743M2-IIT6) + 4.3-inch RGB LCD screen (screen model RGB043M1-480*272)
	* Eshop: https://kavirelectronic.ir/eshop
	* TELEGRAM ID:   @kavirsupport
	*
>>>>> File description:
	*
	* 1. It is only used for testing. This file is not necessary and can be discarded when users migrate it.
	*
	****************************************************************************************************************************
***/


#include "lcd_test.h"

/*************************************************************************************************
* Function name: LCD_Test_DoubleLayer
*
* Function: double-layer display
*
* Note: It only takes effect when double layer is enabled.			
*************************************************************************************************/

void LCD_Test_DoubleLayer(void)
{
	uint16_t time = 100; // Delay time
	uint8_t i = 0; // Count variable
	
// Draw the initial interface, including title, LOGO and progress bar >>>>>
	
	LCD_SetBackColor(0xffB9EDF8); //Set the background color and use a custom color
	LCD_Clear(); // Clear the screen and refresh the background color
	
	LCD_SetTextFont(&CH_Font32); // Set 3232 Chinese font, ASCII font corresponds to 3216
	LCD_SetColor(0xff333333); //Set the brush color and use a custom color
	LCD_DisplayText(176, 87,"Double-layer test"); //Display text
	
	LCD_SetColor(0xfffd7923); //Set the brush color and use a custom color
	LCD_DrawImage(120, 120, 240, 83, Image_FANKE_240x83); // Display LOGO image

	LCD_SetColor(0xff003366); //Set the brush color and use a custom color	
	for(i=0;i<100;i++)
   {
		LCD_FillRect(44,228,4*i,6); // Draw a rectangle to achieve the effect of a simple progress bar
		HAL_Delay(15);	
   }	
	
	
	LCD_SetLayer(1); //Switch to layer 1
	LCD_SetBackColor(LCD_BLACK & 0x00FFFFFF); // Transparency 100%, the smaller the high 8-bit number, the higher the transparency
	LCD_Clear(); // The foreground layer is completely transparent at this time, otherwise the image of layer0 cannot be seen	
	
// layer0 display content >>>>>
		
	LCD_SetLayer(0); //Switch to layer 0	

	LCD_SetBackColor(LCD_BLACK);   
	LCD_Clear();
	
	LCD_SetColor(LCD_WHITE);	
	LCD_SetTextFont(&CH_Font24); //Set 2424 Chinese font, ASCII font corresponding to 2412
	LCD_DisplayText(10,10,"STM32H743 LTDC background layer, layer0");

	HAL_Delay(1000);
	
	LCD_SetColor(LIGHT_CYAN);  	LCD_DrawLine(5, 75,200, 75);	 	HAL_Delay(time);
	LCD_SetColor(LIGHT_MAGENTA);	LCD_DrawLine(5, 90,200, 90);   	HAL_Delay(time);
	LCD_SetColor(LIGHT_YELLOW); 	LCD_DrawLine(5,105,200,105);	 	HAL_Delay(time);
	
	LCD_SetColor(LCD_RED);    		LCD_FillCircle( 60,200,60);		HAL_Delay(time);
	LCD_SetColor(LCD_GREEN);  		LCD_FillCircle(100,200,60); 	   HAL_Delay(time);
	LCD_SetColor(LCD_BLUE);   		LCD_FillCircle(140,200,60);  		HAL_Delay(time);
	
	LCD_SetColor(LIGHT_RED);	  	LCD_FillRect(246, 65,125,120);	HAL_Delay(time);	  
	LCD_SetColor(LIGHT_GREEN);  	LCD_FillRect(293, 96,125,120);  	HAL_Delay(time);  
	LCD_SetColor(LIGHT_BLUE);    	LCD_FillRect(334,130,125,120);  	HAL_Delay(time);
	
	HAL_Delay(1000);
	
// layer1 display content >>>>>

	LCD_SetLayer(1); //Switch to layer 1

	LCD_SetColor(0Xff348498);		
	LCD_FillRect(62, 48,322,192);		  

	LCD_SetBackColor(0Xff348498);  
	LCD_SetColor(LCD_BLACK);		
	LCD_DisplayText(146,69,"foreground layer, layer1");
	HAL_Delay(1000);
	
#if ColorMode_1 == LTDC_PIXEL_FORMAT_ARGB4444 // If layer1 is defined as ARGB4444

	LCD_SetBackColor(LCD_BLACK & 0xB0FFFFFF); //Set the background color transparency. The smaller the high 8-bit number, the higher the transparency. ARGB4444 supports 4-bit transparent color and has 16 transparent states.
	LCD_SetColor(LCD_WHITE); //The brush is opaque
	LCD_DisplayText(28,100,"STM32H743IIT6 LTDC, ARGB4444£¬layer1"); 
	HAL_Delay(1000);
	
	LCD_SetBackColor(LCD_BLACK & 0x80FFFFFF); //Set the background color transparency. The smaller the upper 8-bit number, the higher the transparency. ARGB4444 supports 4-bit transparent color and has 16 transparent states.
	LCD_SetColor(LCD_WHITE);		 
	LCD_DisplayText(28,140,"STM32H743IIT6 LTDC, ARGB4444£¬layer1"); 
	HAL_Delay(1000);
		
	LCD_SetBackColor(LCD_BLACK & 0x30FFFFFF); //Set the background color transparency. The smaller the high 8-bit number, the higher the transparency. ARGB4444 supports 4-bit transparent color and has 16 transparent states.
	LCD_SetColor(LCD_WHITE);
	LCD_DisplayText(28,180,"STM32H743IIT6 LTDC, ARGB4444£¬layer1"); 	
	HAL_Delay(1000);
	
	LCD_SetBackColor(LCD_BLACK ); //Set the background color to opaque
	LCD_SetColor(LCD_WHITE & 0x00FFFFFF); // Set the brush color to completely transparent	 
	LCD_DisplayText(28,220,"STM32H743IIT6 LTDC, ARGB4444£¬layer1"); 	
	HAL_Delay(2000);	
	
#elif ColorMode_1 == LTDC_PIXEL_FORMAT_ARGB1555 // If layer1 is defined as ARGB1555


	LCD_SetBackColor(LCD_BLACK & 0xffFFFFF); //Set the background color transparency. The smaller the high 8-bit number, the higher the transparency. ARGB1555 only supports one-bit transparent color, that is, there are only two states: transparent and opaque.
	LCD_SetColor(LCD_WHITE); //The brush is opaque
	LCD_DisplayText(28,100,"STM32H743IIT6 LTDC, ARGB1555£¬layer1"); 
	HAL_Delay(1000);
	
	LCD_SetBackColor(LCD_BLACK & 0x00FFFFFF); //Set the background color transparency. The smaller the high 8-bit number, the higher the transparency. ARGB1555 only supports one-bit transparent color, that is, there are only two states: transparent and opaque.
	LCD_SetColor(LCD_WHITE); //The brush is opaque
	LCD_DisplayText(28,140,"STM32H743IIT6 LTDC, ARGB1555£¬layer1"); 
	HAL_Delay(1000);
		
	LCD_SetBackColor(LCD_BLACK & 0xffFFFFF); // Set the background color to opaque
	LCD_SetColor(LCD_WHITE & 0x00FFFFFF); // Set the brush color to completely transparent	 
	LCD_DisplayText(28,180,"STM32H743IIT6 LTDC, ARGB1555£¬layer1"); 	
	HAL_Delay(2000);	

#elif ColorMode_1 == LTDC_PIXEL_FORMAT_ARGB8888 // If layer1 is defined as ARGB8888

	LCD_SetBackColor(LCD_BLACK & 0xE0FFFFFF); //Set the background color transparency. The smaller the high 8-bit number, the higher the transparency. ARGB8888 supports 8-bit transparent color and has 255 transparent states.
	LCD_SetColor(LCD_WHITE); //The brush is opaque
	LCD_DisplayText(28,100,"STM32H743IIT6 LTDC, ARGB8888£¬layer1"); 
	HAL_Delay(1000);
	
	LCD_SetBackColor(LCD_BLACK & 0x80FFFFFF); //Set the background color transparency. The smaller the high 8-bit number, the higher the transparency. ARGB8888 supports 8-bit transparent color and has 255 transparent states.
	LCD_SetColor(LCD_WHITE);		 
	LCD_DisplayText(28,140,"STM32H743IIT6 LTDC, ARGB8888£¬layer1"); 
	HAL_Delay(1000);
		
	LCD_SetBackColor(LCD_BLACK & 0x20FFFFFF); //Set the background color transparency. The smaller the upper 8-bit number, the higher the transparency. ARGB8888 supports 8-bit transparent color and has 255 transparent states.
	LCD_SetColor(LCD_WHITE);
	LCD_DisplayText(28,180,"STM32H743IIT6 LTDC, ARGB8888£¬layer1"); 	
	HAL_Delay(1000);
	
	LCD_SetBackColor(LCD_BLACK); //Set the background color to opaque
	LCD_SetColor(LCD_WHITE & 0x00FFFFFF); // Set the brush color to completely transparent	 
	LCD_DisplayText(28,220,"STM32H743IIT6 LTDC, ARGB8888£¬layer1"); 	
	HAL_Delay(2000);	
	
#endif


}

/*************************************************************************************************
* Function name: LCD_Test_Clear
*
* Function: Clear screen test
*
* Description: None	
*************************************************************************************************/

void LCD_Test_Clear(void)
{
	uint16_t time = 1000; // Delay time
	uint8_t i = 0; // Count variable
	
// Draw the initial interface, including title, LOGO and progress bar >>>>>
	
	LCD_SetBackColor(0xffB9EDF8); //Set the background color and use a custom color
	LCD_Clear(); // Clear the screen and refresh the background color
	
	LCD_SetTextFont(&CH_Font32); // Set 3232 Chinese font, ASCII font corresponds to 3216
	LCD_SetColor(0xff333333); //Set the brush color and use a custom color
	LCD_DisplayText(176, 87,"Brush test"); //Display text
	
	LCD_SetColor(0xfffd7923); //Set the brush color and use a custom color
	LCD_DrawImage(120, 120, 240, 83, Image_FANKE_240x83); // Display LOGO image

	LCD_SetColor(0xff003366); //Set the brush color and use a custom color	
	for(i=0;i<100;i++)
   {
		LCD_FillRect(44,228,4*i,6); // Draw a rectangle to achieve the effect of a simple progress bar
		HAL_Delay(15);	
   }	
	
	
	
// Refresh screen test >>>>>
		
	LCD_SetTextFont(&CH_Font24); //Set 2424 Chinese font, ASCII font corresponding to 2412
	LCD_SetColor(LCD_BLACK); //Set the brush color

	for(i=0;i<8;i++)
	{
		switch (i) // switch background color
		{
			case 0: LCD_SetBackColor(LIGHT_RED); 		break;	
			case 1: LCD_SetBackColor(LIGHT_GREEN); 	break;				
			case 2: LCD_SetBackColor(LIGHT_BLUE); 		break;
			case 3: LCD_SetBackColor(LIGHT_YELLOW); 	break;
			case 4: LCD_SetBackColor(LIGHT_CYAN);		break;
			case 5: LCD_SetBackColor(LIGHT_GREY); 		break;
			case 6: LCD_SetBackColor(LIGHT_MAGENTA); 	break;
			case 7: LCD_SetBackColor(LCD_WHITE); 		break;			
			default:	break;			
		}
		LCD_Clear(); // Clear screen
		LCD_DisplayText(68, 68,"STM32H743 LTDC refresh test");
		LCD_DisplayText(68,106,"Board model:NANOFH7433");
		LCD_DisplayText(68,144,"Screen resolution: 480*272");		
		HAL_Delay(time); // Delay
	}
}

/*************************************************************************************************
* Function name: LCD_Test_Text
*
* Function: Text display test
*
* Description: None	
*************************************************************************************************/

void LCD_Test_Text(void)
{
	uint16_t i; // Count variable

// Draw the initial interface, including title, LOGO and progress bar >>>>>
	
	LCD_SetBackColor(0xffB9EDF8); //Set the background color and use a custom color
	LCD_Clear(); // Clear the screen and refresh the background color
	
	LCD_SetTextFont(&CH_Font32); // Set 3232 Chinese font, ASCII font corresponds to 3216
	LCD_SetColor(0xff333333); //Set the brush color and use a custom color
	LCD_DisplayText(176, 87,"Text display"); // Display text
	
	LCD_SetColor(0xfffd7923); //Set the brush color and use a custom color
	LCD_DrawImage(120, 120, 240, 83, Image_FANKE_240x83); // Display LOGO image

	LCD_SetColor(0xff003366); //Set the brush color and use a custom color	
	for(i=0;i<100;i++)
   {
		LCD_FillRect(44,228,4*i,6); // Draw a rectangle to achieve the effect of a simple progress bar
		HAL_Delay(15);	
   }
	
// Display text, including Chinese and ASCII characters in various font sizes >>>>>

	LCD_SetBackColor(LCD_BLACK); //Set the background color
	LCD_Clear(); // Clear screen
	
	LCD_SetColor(LCD_WHITE); //Set the brush to white
	LCD_SetFont(&Font32); LCD_DisplayString(0, 0,"!#$%&'()*+,-.0123456789:;<=>?"); HAL_Delay(80); // Use 3216 font		
	LCD_SetFont(&Font24); LCD_DisplayString(0,32,"!#$%&'()*+,-.0123456789:;<=>?@ABCDEFGH"); HAL_Delay(80); // Use 2412 font
	LCD_SetFont(&Font20); LCD_DisplayString(0,56,"!#$%&'()*+,-.0123456789:;<=>?@ABCDEFGHIJKLMNO"); HAL_Delay(80); // Use 2010 font
	LCD_SetFont(&Font16); LCD_DisplayString(0,76,"!#$%&'()*+,-.0123456789:;<=>?@ABCDEFGHIJKLMNOPQRST"); HAL_Delay(80); // Use 1608 font
	LCD_SetFont(&Font12); LCD_DisplayString(0,92,"!#$%&'()*+,-.0123456789:;<=>?@ABCDEFGHIJKLMNOPQRST"); HAL_Delay(80); // Use 1206 font
																																				
	LCD_SetColor(LCD_CYAN);                                                                               	
	LCD_SetFont(&Font12); LCD_DisplayString(0,104,"!#$%&'()*+,-.0123456789:;<=>?@ABCDEFGHIJKLMNOPQRST"); HAL_Delay(80); // Use 1206 font
	LCD_SetFont(&Font16); LCD_DisplayString(0,116,"!#$%&'()*+,-.0123456789:;<=>?@ABCDEFGHIJKLMNOPQRST"); HAL_Delay(80); // Use 1608 font
	LCD_SetFont(&Font20); LCD_DisplayString(0,132,"!#$%&'()*+,-.0123456789:;<=>?@ABCDEFGHIJKLMNO"); HAL_Delay(80); // Use 2010 font	
	LCD_SetFont(&Font24); LCD_DisplayString(0,152,"!#$%&'()*+,-.0123456789:;<=>?@ABCDEFGH"); HAL_Delay(80); // Use 2412 font	
	LCD_SetFont(&Font32); LCD_DisplayString(0,176,"!#$%&'()*+,-.0123456789:;<=>?"); HAL_Delay(80); // Use 3216 font	

	LCD_SetTextFont(&CH_Font24); //Set 2424 Chinese font, ASCII font corresponding to 2412
	LCD_SetColor(LCD_YELLOW); //Set the brush, yellow
	LCD_DisplayText(0, 210,"Text display, can display Chinese and ASCII character sets");
	LCD_DisplayText(0, 240,"Users can add or delete fonts according to their needs");	
	HAL_Delay(2000); // Delay waiting
	
	LCD_SetBackColor(LCD_BLACK); //Set the background color
	LCD_Clear(); // Clear screen	
	
	LCD_SetTextFont(&CH_Font12); //Set 1212 Chinese font, ASCII font corresponds to 1206
	LCD_SetColor(LCD_WHITE); //Set the brush to white
	LCD_DisplayText(28, 20,"1212 Chinese font: Anti-Ke Technology");	
	
	LCD_SetTextFont(&CH_Font16); //Set 1616 Chinese font, ASCII font corresponds to 1608
	LCD_SetColor(LIGHT_RED); //Set the brush, bright red
	LCD_DisplayText(28, 48,"1616 Chinese font: Anti-Ke Technology");		
	
	LCD_SetTextFont(&CH_Font20); //Set the 2020 Chinese font, and the ASCII font corresponds to 2010
	LCD_SetColor(LIGHT_RED); //Set the brush, bright red
	LCD_DisplayText(28, 82,"2020 Chinese Font: Anti-Ke Technology");		

	LCD_SetTextFont(&CH_Font24); //Set 2424 Chinese font, ASCII font corresponding to 2412
	LCD_SetColor(LIGHT_CYAN); //Set the brush, blue-green
	LCD_DisplayText(28, 121,"2424 Chinese font: Anti-Ke Technology");		

	LCD_SetTextFont(&CH_Font32); // Set 3232 Chinese font, ASCII font corresponds to 3216
	LCD_SetColor(LIGHT_CYAN); //Set the brush, blue-green
	LCD_DisplayText(28, 160,"3232 Chinese font: Anti-Ke Technology");		

	LCD_SetTextFont(&CH_Font24); //Set 2424 Chinese font, ASCII font corresponding to 2412
	LCD_SetColor(LIGHT_YELLOW); //Set the brush, bright yellow
	LCD_DisplayText(28, 217,"Core board model: FK743M3-IIT6");		

	HAL_Delay(2000); // Delay waiting
}

/*************************************************************************************************
* Function name: LCD_Test_Variable
*
* Function: Variable display, including integers and decimals
*
* Description: None	
*************************************************************************************************/

void LCD_Test_Variable (void)
{

	uint16_t i; // Count variable

	int32_t a = 0; // Define integer variables for testing
	int32_t b = 0; // Define integer variables for testing
	int32_t c = 0; // Define integer variables for testing

	double p = 3.1415926; // Define floating point variables for testing
	double f = -1234.1234; // Define floating point variables for testing
	
// Draw the initial interface, including title, LOGO and progress bar >>>>>
	
	LCD_SetBackColor(0xffB9EDF8); //Set the background color and use a custom color
	LCD_Clear(); // Clear the screen and refresh the background color
	
	LCD_SetTextFont(&CH_Font32); // Set 3232 Chinese font, ASCII font corresponds to 3216
	LCD_SetColor(0xff333333); //Set the brush color and use a custom color
	LCD_DisplayText(176, 87,"Variable display"); // Display text
	
	LCD_SetColor(0xfffd7923); //Set the brush color and use a custom color
	LCD_DrawImage(120, 120, 240, 83, Image_FANKE_240x83); // Display LOGO image

	LCD_SetColor(0xff003366); //Set the brush color and use a custom color	
	for(i=0;i<100;i++)
   {
		LCD_FillRect(44,228,4*i,6); // Draw a rectangle to achieve the effect of a simple progress bar
		HAL_Delay(15);	
   }
	
// Display positive integers, negative integers, and decimals >>>>>	
	
	LCD_SetBackColor(LCD_BLACK); //Set the background color
	LCD_Clear(); // Clear screen
	
	LCD_SetTextFont(&CH_Font24); //Set 2424 Chinese font, ASCII font corresponding to 2412
	LCD_SetColor(LCD_WHITE); //Set the brush to white
	
	LCD_DisplayText(28, 20,"Extra bits are filled with spaces"); //Display text	
	LCD_DisplayText(261,20,"Fill excess bits with 0"); //Display text		
	
	LCD_SetColor(LIGHT_CYAN); //Set the brush, blue-green
	LCD_DisplayText(28, 61,"Positive integer:");				
	LCD_DisplayText(28, 92,"Positive integer:");				
	LCD_DisplayText(28,123,"Negative integer: ");					
				
	LCD_SetColor(LIGHT_YELLOW); //Set the brush, bright yellow		
	LCD_DisplayText(261, 61,"Positive integer:");	
	LCD_DisplayText(261, 94,"Positive integer:");	
	LCD_DisplayText(261,127,"Negative integer: ");	
			
	LCD_SetColor(LIGHT_RED); //Set the brush, bright red		
	LCD_DisplayText(28, 171,"Positive decimal:");	
	LCD_DisplayText(28, 202,"Negative decimal: ");			
			
	for(i=0;i<100;i++)
   {
		LCD_SetColor(LIGHT_CYAN); // Set brush, blue-green	
		LCD_ShowNumMode(Fill_Space); // Fill in spaces with extra bits
		LCD_DisplayNumber(120, 61, a+i*125, 8); // Display variable		
		LCD_DisplayNumber(120, 92, b+i, 6); //Display variable			
		LCD_DisplayNumber(120, 123, c-i, 6); // Display variable			
		
		LCD_SetColor(LIGHT_YELLOW); //Set the brush, bright yellow	
		LCD_ShowNumMode(Fill_Zero); //Fill excess bits with 0
		LCD_DisplayNumber(353, 61, a+i*125, 8); // Display variable		
		LCD_DisplayNumber(353, 92, b+i, 6); //Display variable			
		LCD_DisplayNumber(353, 123, c-i, 6); // Display variable				
		
		LCD_SetColor(LIGHT_RED); //Set the brush, bright red			
		LCD_ShowNumMode(Fill_Space); // Fill in spaces with extra bits		
		LCD_DisplayDecimals( 120, 171, p+i*0.1, 6,3); // Display decimals	
		LCD_DisplayDecimals( 120, 202, f+i*0.01, 11,4); // Display decimals		
		
		HAL_Delay(40);				
   }
	
	HAL_Delay(2500);		
	
}



/*************************************************************************************************
* Function name: LCD_Test_FillRect
*
* Function: Rectangular filling test
*
* Description: None	
*************************************************************************************************/

void LCD_Test_FillRect(void)
{
	uint16_t i; // Count variable
	
// Draw the initial interface, including title, LOGO and progress bar >>>>>
	
	LCD_SetBackColor(0xffB9EDF8); //Set the background color and use a custom color
	LCD_Clear(); // Clear the screen and refresh the background color
	
	LCD_SetTextFont(&CH_Font32); // Set 3232 Chinese font, ASCII font corresponds to 3216
	LCD_SetColor(0xff333333); //Set the brush color and use a custom color
	LCD_DisplayText(176, 87,"Rectangle Fill"); //Display text
	
	LCD_SetColor(0xfffd7923); //Set the brush color and use a custom color
	LCD_DrawImage(120, 120, 240, 83, Image_FANKE_240x83); // Display LOGO image

	LCD_SetColor(0xff003366); //Set the brush color and use a custom color	
	for(i=0;i<100;i++)
   {
		LCD_FillRect(44,228,4*i,6); // Draw a rectangle to achieve the effect of a simple progress bar
		HAL_Delay(15);	
   }
	
// Rectangular filling >>>>>	
	
	LCD_SetBackColor(LCD_BLACK); //Set the background color
	LCD_Clear(); //Clear the screen and refresh the background color

	LCD_SetFont(&Font16);
	LCD_SetColor(LCD_BLUE);	  	  LCD_FillRect(130,  3,320,16);	LCD_DisplayString(0,  3,"LCD_BLUE");		  
	LCD_SetColor(LCD_GREEN);  	  LCD_FillRect(130, 25,320,16);  LCD_DisplayString(0, 25,"LCD_GREEN");		  
	LCD_SetColor(LCD_RED);    	  LCD_FillRect(130, 47,320,16);  LCD_DisplayString(0, 47,"LCD_RED");			  
	LCD_SetColor(LCD_CYAN);   	  LCD_FillRect(130, 69,320,16);  LCD_DisplayString(0, 69,"LCD_CYAN");		  
	LCD_SetColor(LCD_MAGENTA);	  LCD_FillRect(130, 91,320,16);  LCD_DisplayString(0, 91,"LCD_MAGENTA");	  
	LCD_SetColor(LCD_YELLOW); 	  LCD_FillRect(130,113,320,16);  LCD_DisplayString(0,113,"LCD_YELLOW");		  
	LCD_SetColor(LCD_GREY);   	  LCD_FillRect(130,135,320,16);	LCD_DisplayString(0,135,"LCD_GREY");		  
																																				
	LCD_SetColor(LIGHT_BLUE);	  LCD_FillRect(130,157,320,16);  LCD_DisplayString(0,157,"LIGHT_BLUE");		  
	LCD_SetColor(LIGHT_GREEN);   LCD_FillRect(130,179,320,16);  LCD_DisplayString(0,179,"LIGHT_GREEN");	  
	LCD_SetColor(LIGHT_RED);     LCD_FillRect(130,201,320,16);  LCD_DisplayString(0,201,"LIGHT_RED");	     
	LCD_SetColor(LIGHT_CYAN);    LCD_FillRect(130,223,320,16);  LCD_DisplayString(0,223,"LIGHT_CYAN");	     
	LCD_SetColor(LIGHT_MAGENTA); LCD_FillRect(130,245,320,16);  LCD_DisplayString(0,245,"LIGHT_MAGENTA");	  

	HAL_Delay(3000);
}


/*************************************************************************************************
* Function name: LCD_Test_Color
*
* Function: color test
*
* Description: None	
*************************************************************************************************/

void LCD_Test_Color(void)
{
	uint16_t i;
	
// Draw the initial interface, including title, LOGO and progress bar >>>>>
	
	LCD_SetBackColor(0xffB9EDF8); //Set the background color and use a custom color
	LCD_Clear(); // Clear the screen and refresh the background color
	
	LCD_SetTextFont(&CH_Font32); // Set 3232 Chinese font, ASCII font corresponds to 3216
	LCD_SetColor(0xff333333); //Set the brush color and use a custom color
	LCD_DisplayText(176, 87,"Color Test"); // Display text
	
	LCD_SetColor(0xfffd7923); //Set the brush color and use a custom color
	LCD_DrawImage(120, 120, 240, 83, Image_FANKE_240x83); // Display LOGO image

	LCD_SetColor(0xff003366); //Set the brush color and use a custom color	
	for(i=0;i<100;i++)
   {
		LCD_FillRect(44,228,4*i,6); // Draw a rectangle to achieve the effect of a simple progress bar
		HAL_Delay(15);	
   }
	
// Color test >>>>>
		
	LCD_SetBackColor(LCD_BLACK); //Set the background color
	LCD_Clear(); //Clear the screen and refresh the background color
	
	LCD_SetTextFont(&CH_Font24); //Set 2424 Chinese font, ASCII font corresponding to 2412
	LCD_SetColor(LCD_WHITE); //Set the brush color
	
	LCD_DisplayText(21,14,"Board model:NANOFH7433");
	LCD_DisplayText(21,54,"Screen resolution: 480*272");		
	LCD_DisplayText(21,94,"RGB three primary color level test");	
	
	//Use the line drawing function to draw the three primary color strips
	for(i=0;i<240;i++)
	{
		LCD_SetColor( LCD_RED-(i<<16) );
		LCD_DrawLine(5+2*i,  140,5+2*i,  170);	
		LCD_DrawLine(5+2*i+1,140,5+2*i+1,170);	
	}
	for(i=0;i<240;i++)
	{
		LCD_SetColor( LCD_GREEN-(i<<8) );
		LCD_DrawLine(5+2*i,  180,5+2*i,  210);	
		LCD_DrawLine(5+2*i+1,180,5+2*i+1,210);
	}
	for(i=0;i<240;i++)
	{
		LCD_SetColor( LCD_BLUE-i );
		LCD_DrawLine(5+2*i,  220,5+2*i,  250);	
		LCD_DrawLine(5+2*i+1,220,5+2*i+1,250);
	}	
	HAL_Delay(3000);	
	
	
}

/*************************************************************************************************
* Function name: LCD_Test_GrahicTest
*
* Function: 2D graphics drawing
*
* Description: None	
*************************************************************************************************/

void LCD_Test_GrahicTest(void)
{
	uint16_t time = 80; // Delay time
	uint16_t i; // Count variable
		
// Draw the initial interface, including title, LOGO and progress bar >>>>>
	
	LCD_SetBackColor(0xffB9EDF8); //Set the background color and use a custom color
	LCD_Clear(); // Clear the screen and refresh the background color
	
	LCD_SetTextFont(&CH_Font32); // Set 3232 Chinese font, ASCII font corresponds to 3216
	LCD_SetColor(0xff333333); //Set the brush color and use a custom color
	LCD_DisplayText(176, 87,"Drawing Test"); //Display text
	
	LCD_SetColor(0xfffd7923); //Set the brush color and use a custom color
	LCD_DrawImage(120, 120, 240, 83, Image_FANKE_240x83); // Display LOGO image

	LCD_SetColor(0xff003366); //Set the brush color and use a custom color	
	for(i=0;i<100;i++)
   {
		LCD_FillRect(44,228,4*i,6); // Draw a rectangle to achieve the effect of a simple progress bar
		HAL_Delay(15);	
   }
	
// 2D graphics drawing >>>>>>>
	
	LCD_SetBackColor(LCD_BLACK); //Set the background color
	LCD_Clear(); //Clear the screen and refresh the background color	

	LCD_SetColor(LCD_RED); LCD_FillCircle(60,200,60); //Fill the circle
	LCD_SetColor(LCD_GREEN);  LCD_FillCircle(100,200,60); 	
	LCD_SetColor(LCD_BLUE);   LCD_FillCircle(140,200,60);  	

	LCD_SetColor(LIGHT_GREY);
	LCD_DrawLine(5,5,200,5); HAL_Delay(time); //Draw a straight line
	LCD_DrawLine(5,10,150,10);	HAL_Delay(time);
	LCD_DrawLine(5,15,100,15); HAL_Delay(time);
	LCD_DrawLine(5,20,50,20);	HAL_Delay(time);	

	LCD_SetColor(LIGHT_CYAN);
	LCD_DrawCircle(350,60,60); HAL_Delay(time); //Draw a circle
	LCD_DrawCircle(350,60,50);   HAL_Delay(time);
	LCD_DrawCircle(350,60,40);   HAL_Delay(time);
	LCD_DrawCircle(350,60,30);   HAL_Delay(time);

	LCD_SetColor(LCD_RED);	
	LCD_DrawRect(5,30,200,80); HAL_Delay(time); //Draw a rectangle
	LCD_DrawRect(30,35,150,70); HAL_Delay(time);
	LCD_DrawRect(55,40,100,60);  HAL_Delay(time);
	LCD_DrawRect(80,45, 50,50);   HAL_Delay(time);

	LCD_SetColor(LIGHT_MAGENTA);	
	LCD_DrawEllipse(350,200,100,50); HAL_Delay(time); //Draw an ellipse
	LCD_DrawEllipse(350,200,80,40);  HAL_Delay(time);
	LCD_DrawEllipse(350,200,60,30);  HAL_Delay(time);
	LCD_DrawEllipse(350,200,40,20);  HAL_Delay(time);

	LCD_SetColor(LCD_WHITE);	
	LCD_DrawLine(0,0,479,0); //Draw a rectangular frame
	LCD_DrawLine(479,0,479,271);  
	LCD_DrawLine(479,271,0,271);  
	LCD_DrawLine(0,271,0,0);  

	HAL_Delay(2000);	
}
/*************************************************************************************************
* Function name: LCD_Test_Image
*
* Function: Image display test
*
* Description: None	
*************************************************************************************************/

void LCD_Test_Image(void)
{
	uint16_t i; // Count variable
		
// Draw the initial interface, including title, LOGO and progress bar >>>>>
	
	LCD_SetBackColor(0xffB9EDF8); //Set the background color and use a custom color
	LCD_Clear(); // Clear the screen and refresh the background color
	
	LCD_SetTextFont(&CH_Font32); // Set 3232 Chinese font, ASCII font corresponds to 3216
	LCD_SetColor(0xff333333); //Set the brush color and use a custom color
	LCD_DisplayText(176, 87,"Picture drawing"); // Display text
	
	LCD_SetColor(0xfffd7923); //Set the brush color and use a custom color
	LCD_DrawImage(120, 120, 240, 83, Image_FANKE_240x83); // Display LOGO image

	LCD_SetColor(0xff003366); //Set the brush color and use a custom color	
	for(i=0;i<100;i++)
   {
		LCD_FillRect(44,228,4*i,6); // Draw a rectangle to achieve the effect of a simple progress bar
		HAL_Delay(15);	
   }
	
// Picture drawing>>>>>>>>	

	LCD_SetBackColor(LCD_BLACK); //Set the background color
	LCD_Clear(); // Clear screen
	
	LCD_SetColor( 0xFFFFAD60);
	LCD_DrawImage( 84, 49, 60, 60, Image_Android_60x60) ;
	
	LCD_SetColor( 0xFFFFB6B9);
	LCD_DrawImage( 210, 55, 60, 60, Image_Cloud_60x60) ;
	
	LCD_SetColor( 0xFF8AC6D1);
	LCD_DrawImage( 336, 55, 60, 60, Image_Folder_60x60) ;

	LCD_SetColor( 0xFFFFF5C3);
	LCD_DrawImage( 84,  162, 60, 60, Image_Message_60x60) ;
	
	LCD_SetColor( 0xFFA696C8);
	LCD_DrawImage( 210, 162, 60, 60, Image_Toys_60x60) ;
	
	LCD_SetColor( 0xFFE59572);
	LCD_DrawImage( 336, 162, 60, 60, Image_Video_60x60) ;	

	HAL_Delay(2000);
	
	LCD_SetBackColor(LCD_WHITE); //Set the background color
	LCD_Clear(); // Clear screen
	LCD_SetColor( LCD_BLACK);
	LCD_DrawImage( 0, 16, 480, 239, Image_FANKE_480x239) ;	
	HAL_Delay(2000);
	
}


/*************************************************************************************************
* Function name: LCD_Test_Vertical
*
* Function: Vertical screen test
*
* Description: None	
*************************************************************************************************/

void  LCD_Test_Vertical(void)
{
	uint16_t i;
	uint16_t time = 100;	
	
	LCD_DisplayDirection(Direction_V); // Switch to vertical screen display
	
// Draw the initial interface, including title, LOGO and progress bar >>>>>
	
	LCD_SetBackColor(0xffB9EDF8); //Set the background color and use a custom color
	LCD_Clear(); // Clear the screen and refresh the background color
	
	LCD_SetTextFont(&CH_Font32); // Set 3232 Chinese font, ASCII font corresponds to 3216
	LCD_SetColor(0xff333333); //Set the brush color and use a custom color
	LCD_DisplayText(72, 100,"vertical screen display"); //Display text
	
	LCD_SetColor(0xfffd7923); //Set the brush color and use a custom color
	LCD_DrawImage(16, 150, 240, 83, Image_FANKE_240x83); // Display LOGO image

	LCD_SetColor(0xff003366); //Set the brush color and use a custom color	
	for(i=0;i<130;i++)
   {
		LCD_FillRect(6,250,2*i,6); // Draw a rectangle to achieve the effect of a simple progress bar
		HAL_Delay(10);	
   }
	
//Draw other content>>>>>>>>		
	
	LCD_SetBackColor(LCD_BLACK); //Set the background color
	LCD_Clear(); //Clear the screen and refresh the background color		

	LCD_SetColor(LCD_WHITE);
	LCD_SetFont(&Font32);LCD_DisplayString(0, 0,"!#$%&'()*+,-.012"); 					HAL_Delay(time);		
	LCD_SetFont(&Font24);LCD_DisplayString(0,32,"!#$%&'()*+,-.0123456");			 	HAL_Delay(time);
	LCD_SetFont(&Font20);LCD_DisplayString(0,56,"!#$%&'()*+,-.012345678"); 			HAL_Delay(time);	
	LCD_SetFont(&Font16);LCD_DisplayString(0,76,"!#$%&'()*+,-.0123456789:;"); 		HAL_Delay(time);	
	LCD_SetFont(&Font12);LCD_DisplayString(0,92,"!#$%&'()*+,-.0123456789:;<=>"); 	HAL_Delay(time);	
                                                                                                      
	LCD_SetColor(LCD_CYAN);                                                                            
	LCD_SetFont(&Font12);LCD_DisplayString(0,104,"!#$%&'()*+,-.0123456789:;<=>");	HAL_Delay(time);	
	LCD_SetFont(&Font16);LCD_DisplayString(0,116,"!#$%&'()*+,-.0123456789:;");		HAL_Delay(time);	
	LCD_SetFont(&Font20);LCD_DisplayString(0,132,"!#$%&'()*+,-.012345678");			HAL_Delay(time);		
	LCD_SetFont(&Font24);LCD_DisplayString(0,152,"!#$%&'()*+,-.0123456");			HAL_Delay(time);		
	LCD_SetFont(&Font32);LCD_DisplayString(0,176,"!#$%&'()*+,-.012");					HAL_Delay(time);	

	LCD_SetFont(&Font32);
	LCD_SetColor(LCD_YELLOW);
	LCD_DisplayNumber(10,220, 294967295,10); HAL_Delay(time);		
	LCD_DisplayNumber(10,260,123456,6);	      HAL_Delay(time);		
	LCD_DisplayNumber(10,300,1234,4);		   HAL_Delay(time);		
	LCD_DisplayNumber(10,340,0,0);			   HAL_Delay(time);		
	 
	LCD_SetFont(&Font24);
	LCD_SetColor(LIGHT_BLUE);
	LCD_DisplayString(10,380,"STM32H743 LCD TEST");HAL_Delay(time);

	HAL_Delay(2000);

	time = 80;
	
	LCD_SetBackColor(LCD_BLACK); //Set the background color
	LCD_Clear(); //Clear the screen and refresh the background color	
	
	LCD_SetColor(LCD_RED);    LCD_FillCircle( 90,240,80);	
	LCD_SetColor(LCD_GREEN);  LCD_FillCircle(135,240,80); 
	LCD_SetColor(LCD_BLUE);   LCD_FillCircle(180,240,80);  	
	
   LCD_SetColor(LCD_RED);	
	LCD_DrawRect( 0,35,270,100);  HAL_Delay(time);
	LCD_DrawRect(20,45,230,80); 	HAL_Delay(time);
	LCD_DrawRect(40,55,190,60);  	HAL_Delay(time);
	LCD_DrawRect(60,65,150,40);  	HAL_Delay(time);
	
	LCD_SetColor(LIGHT_GREY);
	LCD_DrawLine(0,5, 271,5);	HAL_Delay(time);
	LCD_DrawLine(0,10,271,10);	HAL_Delay(time);
	LCD_DrawLine(0,15,271,15); HAL_Delay(time);
	LCD_DrawLine(0,20,271,20);	HAL_Delay(time);		

   LCD_SetColor(LIGHT_MAGENTA);	
	LCD_DrawEllipse(135,400,130,70); HAL_Delay(time);
	LCD_DrawEllipse(135,400,110,55);  HAL_Delay(time);
	LCD_DrawEllipse(135,400,90 ,40);  HAL_Delay(time);
	LCD_DrawEllipse(135,400,70 ,25);  HAL_Delay(time);
                                                        
	HAL_Delay(2000);	
	
	LCD_SetBackColor(LCD_BLACK); //Set the background color
	LCD_Clear(); //Clear the screen and refresh the background color

	LCD_SetFont(&Font16);
	LCD_SetColor(LCD_BLUE);	  	  LCD_FillRect(120,  3,150,16);	LCD_DisplayString(0,  3,"LCD_BLUE"); 			
	LCD_SetColor(LCD_GREEN);  	  LCD_FillRect(120, 25,150,16);  LCD_DisplayString(0, 25,"LCD_GREEN");	
	LCD_SetColor(LCD_RED);    	  LCD_FillRect(120, 47,150,16);  LCD_DisplayString(0, 47,"LCD_RED");	 	
	LCD_SetColor(LCD_CYAN);   	  LCD_FillRect(120, 69,150,16);  LCD_DisplayString(0, 69,"LCD_CYAN");		
	LCD_SetColor(LCD_MAGENTA);	  LCD_FillRect(120, 91,150,16);  LCD_DisplayString(0, 91,"LCD_MAGENTA");	
	LCD_SetColor(LCD_YELLOW); 	  LCD_FillRect(120,113,150,16);  LCD_DisplayString(0,113,"LCD_YELLOW");		
	LCD_SetColor(LCD_GREY);   	  LCD_FillRect(120,135,150,16);	LCD_DisplayString(0,135,"LCD_GREY");		
                                                                            
	LCD_SetColor(LIGHT_BLUE);	  LCD_FillRect(120,157,150,16);  LCD_DisplayString(0,157,"LIGHT_BLUE");		
	LCD_SetColor(LIGHT_GREEN);   LCD_FillRect(120,179,150,16);  LCD_DisplayString(0,179,"LIGHT_GREEN");	
	LCD_SetColor(LIGHT_RED);     LCD_FillRect(120,201,150,16);  LCD_DisplayString(0,201,"LIGHT_RED");	   
	LCD_SetColor(LIGHT_CYAN);    LCD_FillRect(120,223,150,16);  LCD_DisplayString(0,223,"LIGHT_CYAN");	   
	LCD_SetColor(LIGHT_MAGENTA); LCD_FillRect(120,245,150,16);  LCD_DisplayString(0,245,"LIGHT_MAGENTA");	
	LCD_SetColor(LIGHT_YELLOW);  LCD_FillRect(120,267,150,16);  LCD_DisplayString(0,267,"LIGHT_YELLOW");	
	LCD_SetColor(LIGHT_GREY);    LCD_FillRect(120,289,150,16);	LCD_DisplayString(0,289,"LIGHT_GREY");  	
	                                                                             
	LCD_SetColor(DARK_BLUE);	  LCD_FillRect(120,311,150,16);  LCD_DisplayString(0,311,"DARK_BLUE");		
	LCD_SetColor(DARK_GREEN);    LCD_FillRect(120,333,150,16);  LCD_DisplayString(0,333,"DARK_GREEN");		
	LCD_SetColor(DARK_RED);      LCD_FillRect(120,355,150,16);  LCD_DisplayString(0,355,"DARK_RED");		
	LCD_SetColor(DARK_CYAN);     LCD_FillRect(120,377,150,16);  LCD_DisplayString(0,377,"DARK_CYAN");		
	LCD_SetColor(DARK_MAGENTA);  LCD_FillRect(120,399,150,16);  LCD_DisplayString(0,399,"DARK_MAGENTA");	
	LCD_SetColor(DARK_YELLOW);   LCD_FillRect(120,421,150,16);  LCD_DisplayString(0,421,"DARK_YELLOW");	
	LCD_SetColor(DARK_GREY);     LCD_FillRect(120,443,150,16);	LCD_DisplayString(0,443,"DARK_GREY");		
	
	HAL_Delay(2000);	
	
	LCD_SetBackColor(LCD_BLACK); //Set the background color
	LCD_Clear(); //Clear the screen and refresh the background color

	LCD_SetColor(LCD_RED);    	LCD_DisplayString(0,3,"0XFF0000");	
	LCD_SetColor(LCD_GREEN);  	LCD_DisplayString(0,25,"0X00FF00");
	LCD_SetColor(LCD_BLUE);	  	LCD_DisplayString(0,47,"0X0000FF");	
	
	//Brush the color bar
	for(i=70;i<255;i++){LCD_SetColor( LCD_RED-(i<<16) ); 		LCD_DrawLine(80+i-70,  3,80+i-70, 19);}
	for(i=70;i<255;i++){LCD_SetColor( LCD_GREEN-(i<<8) );		LCD_DrawLine(80+i-70, 25,80+i-70, 41);}
	for(i=70;i<255;i++){LCD_SetColor( LCD_BLUE-i );		 		LCD_DrawLine(80+i-70, 47,80+i-70, 63);}
	for(i=70;i<255;i++){LCD_SetColor( LIGHT_RED-(i<<16) );	LCD_DrawLine(80+i-70, 69,80+i-70, 85);}LCD_DisplayString(0, 69,"0xFF8080");
	for(i=70;i<255;i++){LCD_SetColor( LIGHT_GREEN-(i<<8) );	LCD_DrawLine(80+i-70, 91,80+i-70,107);}LCD_DisplayString(0, 91,"0x80FF80");
	for(i=70;i<255;i++){LCD_SetColor( LIGHT_BLUE-i );			LCD_DrawLine(80+i-70,113,80+i-70,129);}LCD_DisplayString(0,113,"0x8080FF");	
	for(i=70;i<255;i++){LCD_SetColor( DARK_BLUE-(i<<16) );	LCD_DrawLine(80+i-70,135,80+i-70,151);}LCD_DisplayString(0,135,"0x000080");	
	for(i=70;i<255;i++){LCD_SetColor( DARK_BLUE-(i<<8) );		LCD_DrawLine(80+i-70,157,80+i-70,173);}LCD_DisplayString(0,157,"0x000080");	
	for(i=70;i<255;i++){LCD_SetColor( DARK_RED-i );				LCD_DrawLine(80+i-70,179,80+i-70,195);}LCD_DisplayString(0,179,"0x800000");		
	for(i=70;i<255;i++){LCD_SetColor( DARK_RED-(i<<8));		LCD_DrawLine(80+i-70,201,80+i-70,217);}LCD_DisplayString(0,201,"0x800000");	
	for(i=70;i<255;i++){LCD_SetColor( DARK_GREEN-(i<<16)); 	LCD_DrawLine(80+i-70,223,80+i-70,239);}LCD_DisplayString(0,223,"0x008000");	
	for(i=70;i<255;i++){LCD_SetColor( DARK_GREEN-i);			LCD_DrawLine(80+i-70,245,80+i-70,261);}LCD_DisplayString(0,245,"0x008000");	
	for(i=70;i<255;i++){LCD_SetColor( DARK_CYAN-(i<<16));		LCD_DrawLine(80+i-70,267,80+i-70,283);}LCD_DisplayString(0,267,"0x008080");	
	for(i=70;i<255;i++){LCD_SetColor( DARK_YELLOW-i);			LCD_DrawLine(80+i-70,289,80+i-70,305);}LCD_DisplayString(0,289,"0x808000");	
	for(i=70;i<255;i++){LCD_SetColor( DARK_MAGENTA-(i<<8));	LCD_DrawLine(80+i-70,311,80+i-70,327);}LCD_DisplayString(0,311,"0x800080");	
                                                                                          
	HAL_Delay(2000);	
	LCD_DisplayDirection(Direction_H); // Switch back to horizontal screen display
	
}






