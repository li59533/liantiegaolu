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
	Board_Init_BSP();
	Board_Init_ClockRUN();
	
	
	while(1)
	{
		DEBUG("Test\r\n");
		BSP_Test_Delay(1000);
		BSP_LED_Toggle(BSP_LED1);
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

