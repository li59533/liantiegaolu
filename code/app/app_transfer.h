/**
 **************************************************************************************************
 * @file        app_transfer.h
 * @author
 * @version
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#ifndef _APP_TRANSFER_H_
#define _APP_TRANSFER_H_

/**
 * @addtogroup    XXX 
 * @{ 
 */
#include "self_def.h"
/**
 * @addtogroup    app_transfer_Modules 
 * @{  
 */

/**
 * @defgroup      app_transfer_Exported_Macros 
 * @{  
 */


/**
 * @}
 */

/**
 * @defgroup      app_transfer_Exported_Constants
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_transfer_Exported_Types 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_transfer_Exported_Variables 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_transfer_Exported_Functions 
 * @{  
 */
void APP_Transfer_CoreLoop(void);  
void APP_Transfer_cmdClear(void);

void APP_LowPower_BeforeFunc(void);
void APP_LowPower_AfterFunc(void);

void APP_Transfer_SendDataSerial_Process(void);

void APP_Transfer_SendData_Serial(void);

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
