/**
 **************************************************************************************************
 * @file        app_task.h
 * @author
 * @version    v0.1.0
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#ifndef _APP_TASK_H_
#define _APP_TASK_H_

/**
 * @addtogroup    XXX 
 * @{ 
 */
#include "osal.h"
/**
 * @addtogroup    app_task_Modules 
 * @{  
 */

/**
 * @defgroup      app_task_Exported_Macros 
 * @{  
 */

#define APP_TASK_LOOP_EVENT            	0x0001
#define APP_TASK_TRANSFER_CORELOOP_EVENT 0X0002


/**
 * @}
 */

/**
 * @defgroup      app_task_Exported_Constants
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_task_Exported_Types 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_task_Exported_Variables 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_task_Exported_Functions 
 * @{  
 */
void AppTask_Init(uint8_t taskId);
osal_event_t AppTask_Process(uint8_t taskid,osal_event_t events);
void AppTask_Send_Event(osal_event_t events);
void AppTask_Timer_Start_Event(osal_event_t event_flag,uint32_t timeout);
void AppTask_Clear_Event(osal_event_t events);
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
