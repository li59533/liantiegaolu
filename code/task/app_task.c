/**
 **************************************************************************************************
 * @file        app_task.c
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
#include "app_task.h"


/**
 * @addtogroup    XXX 
 * @{  
 */
#include "bsp_led.h"
#include "bsp_e32.h"
#include "app_transfer.h"
#include "bsp_power.h"
/**
 * @addtogroup    app_task_Modules 
 * @{  
 */

/**
 * @defgroup      app_task_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_task_Macros_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_task_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_task_Private_Types
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_task_Private_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_task_Public_Variables 
 * @brief         
 * @{  
 */
uint8_t g_AppTask_Id = 0;
/**
 * @}
 */

/**
 * @defgroup      app_task_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_task_Functions 
 * @brief         
 * @{  
 */
void AppTask_Init(uint8_t taskId)
{
    g_AppTask_Id = taskId;
	BSP_LED_Blink( BSP_LED_TEST , 0 , 10, 1000);
	BSP_Power_RegisterCall(APP_LowPower_BeforeFunc , APP_LowPower_AfterFunc) ; 
	
	//AppTask_Send_Event(APP_TASK_TRANSFER_CORELOOP_EVENT);
	OS_Timer_Start(g_AppTask_Id, APP_TASK_TRANSFER_CORELOOP_EVENT,1000);		
}

osal_event_t AppTask_Process(uint8_t taskid,osal_event_t events)
{
    if (events & APP_TASK_LOOP_EVENT)
    {		
		OS_Timer_Start(g_AppTask_Id, APP_TASK_LOOP_EVENT,5000);			
        return events ^ APP_TASK_LOOP_EVENT;
    }

    if (events & APP_TASK_TRANSFER_CORELOOP_EVENT)
    {		
		APP_Transfer_CoreLoop();	
        return events ^ APP_TASK_TRANSFER_CORELOOP_EVENT;
    }	
	
    if (events & APP_TASK_REPORTVALUE_INSERIAL_EVENT)
    {		
		APP_Transfer_SendDataSerial_Process();
        return events ^ APP_TASK_REPORTVALUE_INSERIAL_EVENT;
    }	
	
    return 0;
}

void AppTask_Send_Event(osal_event_t events)
{
    OS_Events_Set(g_AppTask_Id,events);
}

void AppTask_Timer_Start_Event(osal_event_t event_flag,uint32_t timeout)
{
	OS_Timer_Start(g_AppTask_Id, event_flag,timeout);	
}

void AppTask_Timer_Stop_Event(osal_event_t event_flag)
{
	OS_Timer_Stop(g_AppTask_Id, event_flag);	
}

void AppTask_Clear_Event(osal_event_t events)
{
	OS_Events_Clear(g_AppTask_Id,events);
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

