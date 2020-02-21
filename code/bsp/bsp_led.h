/**
 **************************************************************************************************
 * @file        bsp_led.h
 * @author
 * @version
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#ifndef _BSP_LED_H_
#define _BSP_LED_H_

/**
 * @addtogroup    XXX 
 * @{ 
 */
#include "self_def.h"
/**
 * @addtogroup    bsp_led_Modules 
 * @{  
 */

/**
 * @defgroup      bsp_led_Exported_Macros 
 * @{  
 */

#define BSP_LED1   0 
#define BSP_LED2   1

#define BSP_LED_COUNT  1
/**
 * @}
 */

/**
 * @defgroup      bsp_led_Exported_Constants
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_led_Exported_Types 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_led_Exported_Variables 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_led_Exported_Functions 
 * @{  
 */
void BSP_LED_Init(void);

void BSP_LED_Open(uint8_t BSP_LEDx);

void BSP_LED_Close(uint8_t BSP_LEDx);

void BSP_LED_Toggle(uint8_t BSP_LEDx);

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
