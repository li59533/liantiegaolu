/**
 **************************************************************************************************
 * @file        bsp_systick.c
 * @author
 * @version
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */

#include "bsp_systick.h"
#include "bsp_conf.h"

/**
 * @addtogroup    XXX 
 * @{  
 */
#include "clog.h"
#include "bsp_led.h"
/**
 * @addtogroup    bsp_systick_Modules 
 * @{  
 */

/**
 * @defgroup      bsp_systick_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup       bsp_systick_Macros_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_systick_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup       bsp_systick_Private_Types
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_systick_Private_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_systick_Public_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_systick_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_systick_Functions 
 * @brief         
 * @{  
 */
void BSP_SysTick_Init(void)
{
	SysTick_Config(CLOCK_GetFreq(kCLOCK_CoreSysClk) / 1000);
}


void SysTick_Handler(void)
{
	DEBUG("SysTick_Handler\r\n");
	BSP_LED_Toggle(BSP_LED_TEST);
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

