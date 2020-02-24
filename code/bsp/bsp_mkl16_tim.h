/**
 **************************************************************************************************
 * @file        bsp_mkl16_tim.h
 * @author
 * @version
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#ifndef _BSP_MKL16_TIM_H_
#define _BSP_MKL16_TIM_H_

/**
 * @addtogroup    XXX 
 * @{ 
 */
#include "self_def.h"
/**
 * @addtogroup    bsp_mkl16_tim_Modules 
 * @{  
 */

/**
 * @defgroup      bsp_mkl16_tim_Exported_Macros 
 * @{  
 */
#define  BSP_CLOCK0  0
/**
 * @}
 */

/**
 * @defgroup      bsp_mkl16_tim_Exported_Constants
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_mkl16_tim_Exported_Types 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_mkl16_tim_Exported_Variables 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_mkl16_tim_Exported_Functions 
 * @{  
 */
 
void BSP_MKL16_Clock_Init(uint8_t BSP_CLOCKx);
uint32_t BSP_MKL16_GetCurCount(void);
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
