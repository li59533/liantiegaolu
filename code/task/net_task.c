/**
 **************************************************************************************************
 * @file        net_task.c
 * @author
 * @version   v0.1.0
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#include "self_def.h"
#include "osal.h"
#include "clog.h"
#include "net_task.h"


/**
 * @addtogroup    XXX 
 * @{  
 */
#include "bsp_led.h"
#include "bsp_e32.h"
/**
 * @addtogroup    net_task_Modules 
 * @{  
 */

/**
 * @defgroup      net_task_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      net_task_Macros_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      net_task_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      net_task_Private_Types
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      net_task_Private_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      net_task_Public_Variables 
 * @brief         
 * @{  
 */
uint8_t g_NetTask_Id = 0;
/**
 * @}
 */

/**
 * @defgroup      net_task_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      net_task_Functions 
 * @brief         
 * @{  
 */
void NetTask_Init(uint8_t taskId)
{
    g_NetTask_Id = taskId;
    //NetTask_Send_Event(NET_TASK_LOOP_EVENT);	
	OS_Timer_Start(g_NetTask_Id, NET_TASK_LOOP_EVENT,5000);	
	NetTask_Send_Event(NET_TASK_MODULE_INIT_EVENT);
}

osal_event_t NetTask_Process(uint8_t taskid,osal_event_t events)
{
    if (events & NET_TASK_LOOP_EVENT)
    {
		DEBUG("NET_TASK_LOOP_EVENT\r\n");
		
		uint8_t databuf[3] = {0x7e,0x88,0x11};
		BSP_E32_SendData(0x0B00 , 0x0F, databuf , 3);
		
		OS_Timer_Start(g_NetTask_Id, NET_TASK_LOOP_EVENT,5000);			
        return events ^ NET_TASK_LOOP_EVENT;
    }
	
    if (events & NET_TASK_MODULE_INIT_EVENT)
    {
		DEBUG("NET_TASK_MODULET\r\n");	
		BSP_E32_Init();
        return events ^ NET_TASK_MODULE_INIT_EVENT;
    }

    if (events & NET_TASK_CORE_LOOP_EVENT)
    {
		//DEBUG("NET_TASK_CORE_LOOP_EVENT\r\n");	
		BSP_E32_CoreLoop();
		NetTask_Timer_Start_Event(NET_TASK_CORE_LOOP_EVENT,20);
        return events ^ NET_TASK_CORE_LOOP_EVENT;
    }	
	if (events & NET_TASK_REV_EVENT)
    {
		DEBUG("NET_TASK_REV_EVENT\r\n");	
		BSP_E32_Rev();
        return events ^ NET_TASK_REV_EVENT;
    }	
	
	
    return 0;
}

void NetTask_Send_Event(osal_event_t events)
{
    OS_Events_Set(g_NetTask_Id,events);
}

void NetTask_Timer_Start_Event(osal_event_t event_flag,uint32_t timeout)
{
	OS_Timer_Start(g_NetTask_Id, event_flag,timeout);	
}

void NetTask_Clear_Event(osal_event_t events)
{
	OS_Events_Clear(g_NetTask_Id,events);
}
/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

