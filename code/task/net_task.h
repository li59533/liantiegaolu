/**
 **************************************************************************************************
 * @file        net_task.h
 * @author
 * @version    v0.1.0
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#ifndef _NET_TASK_H_
#define _NET_TASK_H_

/**
 * @addtogroup    XXX 
 * @{ 
 */
#include "osal.h"
/**
 * @addtogroup    net_task_Modules 
 * @{  
 */

/**
 * @defgroup      net_task_Exported_Macros 
 * @{  
 */

#define NET_TASK_LOOP_EVENT            	0x0001
#define NET_TASK_MODULE_INIT_EVENT		0x0002
#define NET_TASK_CORE_LOOP_EVENT		0X0004
#define NET_TASK_REV_EVENT				0X0008


/**
 * @}
 */

/**
 * @defgroup      net_task_Exported_Constants
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      net_task_Exported_Types 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      net_task_Exported_Variables 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      net_task_Exported_Functions 
 * @{  
 */
void NetTask_Init(uint8_t taskId);
osal_event_t NetTask_Process(uint8_t taskid,osal_event_t events);
void NetTask_Send_Event(osal_event_t events);
void NetTask_Timer_Start_Event(osal_event_t event_flag,uint32_t timeout);
void NetTask_Timer_Stop_Event(osal_event_t event_flag);
void NetTask_Clear_Event(osal_event_t events);
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
