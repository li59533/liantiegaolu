/**
 **************************************************************************************************
 * @file        bsp_systick.h
 * @author
 * @version
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#ifndef _BSP_SYSTICK_H_
#define _BSP_SYSTICK_H_

/**
 * @addtogroup    XXX 
 * @{ 
 */
#include "self_def.h"
/**
 * @addtogroup    bsp_systick_Modules 
 * @{  
 */

/**
 * @defgroup      bsp_systick_Exported_Macros 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_systick_Exported_Constants
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_systick_Exported_Types 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_systick_Exported_Variables 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_systick_Exported_Functions 
 * @{  
 */
void BSP_SysTick_Init(void);

void BSP_SysTick_DisableIRQ(void);
void BSP_SYSTICK_IncTick(void);
uint32_t BSP_SYSTICK_GetTick(void);

void BSP_Systick_Delayms(uint32_t ticks); // block delay

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */
#endif
