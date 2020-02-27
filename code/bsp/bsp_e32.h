/**
 **************************************************************************************************
 * @file        bsp_e32.h
 * @author
 * @version
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#ifndef _BSP_E32_H_
#define _BSP_E32_H_

/**
 * @addtogroup    XXX 
 * @{ 
 */
#include "self_def.h"
/**
 * @addtogroup    bsp_e32_Modules 
 * @{  
 */

/**
 * @defgroup      bsp_e32_Exported_Macros 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_e32_Exported_Constants
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_e32_Exported_Types 
 * @{  
 */
typedef enum
{
	E32_MODE_NORMAL = 1,
	E32_MODE_WAKEUP = 2,
	E32_MODE_LOWPOWER = 3,
	E32_MODE_SLEEP	  = 4, // CONF mode
}E32_Mode_e;

/*
speed: 00 011 100B (0x1C) 8N1 , TTL->9600 , Air->9600 ,
channel : = (410 + channel)MHz . default:3CH( 470 MHz) [00~73H]->[410MHz ~ 525MHz]
option: 1 1 000 1 00 (0xC4)  point to point , internal pull up ,open FEC ,max send power.

*/
typedef struct
{
	uint8_t head ;
	uint16_t local_addr ;
	uint8_t speed ;
	uint8_t channel ;
	uint8_t option ;
	
}BSP_E32_ConfCMD_t ;

/**
 * @}
 */

/**
 * @defgroup      bsp_e32_Exported_Variables 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_e32_Exported_Functions 
 * @{  
 */
void BSP_E32_Init(void);
uint8_t BSP_E32_GetAUX_Status(void);
void BSP_E32_SetMode(E32_Mode_e mode);


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
