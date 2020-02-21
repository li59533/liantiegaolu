/**
 **************************************************************************************************
 * @file        board.h
 * @author
 * @version
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#ifndef _BOARD_H_
#define _BOARD_H_

/**
 * @addtogroup    XXX 
 * @{ 
 */
#include "self_def.h"
/**
 * @addtogroup    board_Modules 
 * @{  
 */

/**
 * @defgroup      board_Exported_Macros 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      board_Exported_Constants
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      board_Exported_Types 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      board_Exported_Variables 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      board_Exported_Functions 
 * @{  
 */
void Board_Init_ClockRUN(void);
void Board_Init_BSP(void);



// ----------Test code -----------
void BSP_Test_Delay(uint32_t count);
// -------------------------------

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
