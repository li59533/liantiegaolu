/**
 **************************************************************************************************
 * @file        main.c
 * @author
 * @version
 * @date        2/21/2020
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */

#include "main.h"
#include "board.h"
#include "clog.h"
#include "bsp_led.h"
/**
 * @addtogroup    XXX 
 * @{  
 */
#include "bsp_mkl16_tim.h"
#include "bsp_mkl16_rtc.h"
#include "bsp_mkl16_adc.h"
/**	
 * @addtogroup    main_Modules 
 * @{  
 */

/**
 * @defgroup      main_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup       main_Macros_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      main_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup       main_Private_Types
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      main_Private_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      main_Public_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      main_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      main_Functions 
 * @brief         
 * @{  
 */
int main(void)
{
	Board_Init_ClockRUN();
	
	Board_Init_BSP();
	
	while(1)
	{
		//DEBUG("Time Count : %d\r\n" , BSP_MKL16_GetTimrCurCount(BSP_CLOCK1));
		BSP_Test_Delay(1000);
		
		//BSP_MKL16_ShowDate();
		//BSP_MKL16_ADC_ShowValue();
		//BSP_LED_Toggle(BSP_LED1);
		//DEBUG("Time Count : %d\r\n" , BSP_MKL16_GetTimrCurCount(BSP_CLOCK1));
	}
}


void Error_Handler(void)
{
	while(1)
	{
		//DEBUG(0,"Error_Handler\r\n");
	};
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

