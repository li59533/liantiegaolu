/**
 **************************************************************************************************
 * @file        bsp_tim.h
 * @author
 * @version
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#ifndef _BSP_TIM_H_
#define _BSP_TIM_H_

/**
 * @addtogroup    XXX 
 * @{ 
 */
#include "self_def.h"
/**
 * @addtogroup    bsp_tim_Modules 
 * @{  
 */

/**
 * @defgroup      bsp_tim_Exported_Macros 
 * @{  
 */
#define		BSP_CLOCK0  0
#define		BSP_CLOCK1  1
#define 	BSP_CLOCK2  2


/**
 * @}
 */

/**
 * @defgroup      bsp_tim_Exported_Constants
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_tim_Exported_Types 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_tim_Exported_Variables 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_tim_Exported_Functions 
 * @{  
 */
 
void BSP_Clock_Init(uint8_t BSP_CLOCKx);
uint32_t BSP_GetTimrCurCount(uint8_t BSP_CLOCKx);
void BSP_Clock_StartOnceTimer(uint8_t BSP_CLOCKx , uint32_t msec);

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
