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
#include "osal.h"

#include "bsp_led.h"
/**
 * @addtogroup    XXX 
 * @{  
 */
#include "bsp_systick.h"
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
	
	BSP_SysTick_Init();
	Board_Init_BSP();
	
	OS_Init(); // init osal
    
    OS_Start(); // while(1)
	
	while(1)
	{
		DEBUG("MAIN Some Err\r\n");
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

