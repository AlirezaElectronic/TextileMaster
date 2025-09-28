/***
	************************************************************************************************
	*	@file  	lcd_pwm.c
	*	@version V1.0
	*  @date    2021-7-20
	* @author anti-customer technology
	* @brief LCD backlight pwm related functions
   ************************************************************************************************
   *  @description
	*
	* Experimental platform: NANOFH743IIT6
	* Eshop: https://kavirelectronic.ir/eshop
	* TELEGRAM ID:   @kavirsupport
	*
>>>>> File description:
	*
	* 1.PWM frequency is 2KHz
	* 2.HAL_TIM_MspPostInit is used to initialize the IO port, HAL_TIM_Base_MspInit is used to start the clock
	*
	************************************************************************************************
***/


#include "lcd_pwm.h"

//TIM_HandleTypeDef htim12; // TIM_HandleTypeDef structure variable

static uint16_t LCD_PwmPeriod = 500; //Timer reload value

/*************************************************************************************************
* Function name: HAL_TIM_MspPostInit
* Entry parameter: htim - TIM_HandleTypeDef structure variable, which represents the defined TIM (handle)
* Return value: None
* Function: Initialize TIM corresponding PWM port
* Description: Pins used to initialize PWM
*
*************************************************************************************************/
//
//void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim)
//{
//	GPIO_InitTypeDef GPIO_InitStruct = {0};
//	if(htim->Instance == LTDC_PWM_TIM)
//	{
//		GPIO_LDC_Backlight_CLK_ENABLE; // Turn on the backlight pin port clock
//
//		GPIO_InitStruct.Pin = LTDC_PWM_PIN; // Backlight pin
//		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP; // Multiplex push-pull output
//		GPIO_InitStruct.Pull = GPIO_NOPULL; // No pull-up or drop-down
//		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; // low speed mode
//		GPIO_InitStruct.Alternate = LCD_PWM_AF; // Multiplexing
//
//		HAL_GPIO_Init(LTDC_PWM_PORT, &GPIO_InitStruct); // Initialize IO port
//	}
//}


/*************************************************************************************************
* Function name: LCD_PwmSetPulse
* Entry parameters: pulse - PWM duty cycle, range 0~100
* Return value: None
* Function: Set PWM duty cycle
* Description: None
*************************************************************************************************/
	
void  LCD_PwmSetPulse (uint8_t pulse)
{
	uint16_t compareValue ; 
	
	compareValue = pulse * LCD_PwmPeriod / 100; //Set the comparison value according to the duty cycle

	TIM4->CCR4 = compareValue; // Modify the channel 4 comparison value of TIM4
}

/*************************************************************************************************
* Function name: LCD_PWMinit
* Entry parameters: pulse - PWM duty cycle, range 0~100
* Return value: None
* Function: Initialize TIM4, configure PWM frequency to 2KHz
* Description: None
*************************************************************************************************/

void  LCD_PWMinit(uint8_t pulse)
{

//	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
//	TIM_MasterConfigTypeDef sMasterConfig = {0};
//	TIM_OC_InitTypeDef sConfigOC = {0};
//
//	htim12.Instance = LTDC_PWM_TIM; // timer
//	htim12.Init.Prescaler = 240; // Prescaler coefficient, the counting frequency of the timer is 1MKHz at this time
//	htim12.Init.CounterMode = TIM_COUNTERMODE_UP; // Count up mode
//	htim12.Init.Period = LCD_PwmPeriod -1; // Reload value 499, that is, count 500 times
//	htim12.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; // The clock is not divided
//	htim12.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE; // Set the ARPE bit of the control register TIMx_CR1 to 0, which disables preloading of the automatic reload register
//
//	HAL_TIM_Base_Init(&htim12); // Initialize TIM2 according to the above parameters
//
//	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL; //Select internal clock source
//	HAL_TIM_ConfigClockSource(&htim12, &sClockSourceConfig); // Initialize the configuration clock source
//
//	HAL_TIM_PWM_Init(&htim12); // According to the above parameters, perform PWM initialization on TIM
//
//	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET; // Trigger output selection, configure reset mode at this time, that is, the MMS of register TIMx_CR2 is 000
//	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE; // Do not use slave mode
//	HAL_TIMEx_MasterConfigSynchronization(&htim12, &sMasterConfig); // Initialization configuration
//
//	sConfigOC.OCMode = TIM_OCMODE_PWM1; // PWM mode 1
//	sConfigOC.Pulse = pulse*LCD_PwmPeriod/100; // Comparison value is 250, reload is 500, then the duty cycle is 50%
//	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH; // Valid state is high level
//	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE; // Disable fast mode
//	HAL_TIM_PWM_ConfigChannel(&htim12, &sConfigOC, LTDC_PWM_TIM_CHANNEL); // Initialize PWM configuration
//
//	HAL_TIM_MspPostInit(&htim12); //Initialize IO port
//	HAL_TIM_PWM_Start(&htim12,LTDC_PWM_TIM_CHANNEL); // Start PWM
}

/*************************************************************************************************
* Function name: HAL_TIM_Base_MspInit
* Entry parameter: htim_base - TIM_HandleTypeDef structure variable, which represents the defined TIM (handle)
* Return value: None
* Function: Turn on the clock
* Description: None
*
*************************************************************************************************/
//
//void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
//{
//  if(htim_base->Instance==LTDC_PWM_TIM)
//  {
//    LTDC_PWM_TIM_CLK_ENABLE; // Turn on TIM clock
//  }
//
//}

