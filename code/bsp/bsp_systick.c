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
#include "osal.h"
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
static uint32_t s_Systick_Ticks = 0;
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

void BSP_SysTick_DisableIRQ(void)
{
	SysTick->CTRL = 0x00010006;
	NVIC_DisableIRQ(SysTick_IRQn);
}



void BSP_SYSTICK_IncTick(void)
{
    s_Systick_Ticks++;
}

uint32_t BSP_SYSTICK_GetTick(void)
{
    return s_Systick_Ticks;
}


void BSP_Systick_Delayms(uint32_t ticks) // block delay
{
    uint32_t tickstart = 0;
    tickstart = BSP_SYSTICK_GetTick();
    while ((BSP_SYSTICK_GetTick() - tickstart) < ticks) 
    {
    }
}


void SysTick_Handler(void)
{
	
	OS_Timer_Update(1);
	OS_Clock_Update(1);
	BSP_SYSTICK_IncTick();
	//DEBUG("SysTick_Handler\r\n");
	//BSP_LED_Toggle(BSP_LED_TEST);
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

