/**
 **************************************************************************************************
 * @file        bsp_rtc.c
 * @author
 * @version
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */

#include "bsp_conf.h"

#include "clog.h"

/**
 * @addtogroup    XXX 
 * @{  
 */

/**
 * @addtogroup    bsp_rtc_Modules 
 * @{  
 */

/**
 * @defgroup      bsp_rtc_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup       bsp_rtc_Macros_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_rtc_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup       bsp_rtc_Private_Types
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_rtc_Private_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_rtc_Public_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_rtc_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_rtc_Functions 
 * @brief         
 * @{  
 */
void BSP_RTC_Init(void)
{
	rtc_config_t config = { 0 };
	rtc_datetime_t datetime = { 0 };
	
	CLOCK_EnableClock(kCLOCK_PortC);
	PORT_SetPinMux(PORTC, 1,kPORT_MuxAsGpio);
	gpio_pin_config_t gpio_pin_config = { 0 };
	GPIO_PinInit(GPIOC, 1, &gpio_pin_config);
	
	CLOCK_EnableClock(kCLOCK_Rtc0);
	
	CLOCK_SetEr32kClock(2);
	
	RTC_GetDefaultConfig(&config);
		
	RTC_Init(RTC , &config);
	
	RTC_SetOscCapLoad(RTC, kRTC_Capacitor_8p);
	
	
	datetime.year = 2020;
	datetime.month = 1;
	datetime.day = 2;
	datetime.hour = 3;
	datetime.minute = 4;
	datetime.second = 5;
	
	RTC_SetDatetime(RTC , &datetime);
	
	//RTC_EnableInterrupts(RTC,kRTC_AlarmInterruptEnable);
	//EnableIRQ(RTC_IRQn);

	RTC_StartTimer(RTC);
}


void BSP_ShowDate(void)
{
	rtc_datetime_t datetime = { 0 } ;
	RTC_GetDatetime(RTC , &datetime);
	
	DEBUG("%d . %d . %d  %d:%d:%d\r\n" ,datetime.year,datetime.month , datetime.day ,datetime.hour ,datetime.minute ,datetime.second );
	
}

void RTC_IRQHandler(void)
{
	DEBUG("RTC_IRQHandler\r\n");
}


/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

