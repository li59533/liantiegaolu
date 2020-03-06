/**
 **************************************************************************************************
 * @file        app_revmessage.h
 * @author
 * @version
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#ifndef _APP_REVMESSAGE_H_
#define _APP_REVMESSAGE_H_

/**
 * @addtogroup    XXX 
 * @{ 
 */
#include "self_def.h"
/**
 * @addtogroup    app_revmessage_Modules 
 * @{  
 */

/**
 * @defgroup      app_revmessage_Exported_Macros 
 * @{  
 */


/**
 * @}
 */

/**
 * @defgroup      app_revmessage_Exported_Constants
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_revmessage_Exported_Types 
 * @{  
 */
typedef enum
{
	APP_RevACK_Wait = 1,
	APP_RevACK_Get ,
	APP_RevACK_Timeout ,
}APP_RevACK_Status_e;
/**
 * @}
 */

/**
 * @defgroup      app_revmessage_Exported_Variables 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_revmessage_Exported_Functions 
 * @{  
 */


void APP_RevMess_Analysis(uint8_t *buf , uint16_t len);
void APP_RevMessage_Process(uint8_t * buf , uint16_t len);

void APP_RevClearAckFlag(void);
uint8_t APP_RevGetAckFlag(void);



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
