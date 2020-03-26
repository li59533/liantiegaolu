/**
 **************************************************************************************************
 * @file        bsp_power.h
 * @author
 * @version
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#ifndef _BSP_POWER_H_
#define _BSP_POWER_H_

/**
 * @addtogroup    XXX 
 * @{ 
 */
#include "self_def.h"
/**
 * @addtogroup    bsp_power_Modules 
 * @{  
 */

/**
 * @defgroup      bsp_power_Exported_Macros 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_power_Exported_Constants
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_power_Exported_Types 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_power_Exported_Variables 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_power_Exported_Functions 
 * @{  
 */
 
void BSP_Power_Init(void);
void BSP_Power_V30_ON(void);
void BSP_Power_V30_OFF(void); 
void BSP_Power_EnterVLPS(void);
void BSP_Power_EnterVLPS_WithCallFunc(void);
void BSP_Power_RegisterCall(void (* beforefunc)(void) , void (* afterfunc)(void));
void BSP_Power_EnterVLPS_WithCall(void (* beforefunc)(void) , void (* afterfunc)(void)) ; 
// -------Test Func------------- 
void BSP_Power_ModeTest(void);


// -----------------------------
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
