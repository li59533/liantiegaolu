/**
 **************************************************************************************************
 * @file        app_battery.c
 * @author
 * @version
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */

#include "bsp_conf.h"
/**
 * @addtogroup    XXX 
 * @{  
 */
#include "app_battery.h"
#include "clog.h"
#include "system_param.h"
/**
 * @addtogroup    app_battery_Modules 
 * @{  
 */

/**
 * @defgroup      app_battery_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup       app_battery_Macros_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_battery_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup       app_battery_Private_Types
 * @brief         
 * @{  
 */



/**
 * @}
 */

/**
 * @defgroup      app_battery_Private_Variables 
 * @brief         
 * @{  
 */
static uint32_t  powertime = 0; 
/**
 * @}
 */

/**
 * @defgroup      app_battery_Public_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_battery_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_battery_Functions 
 * @brief         
 * @{  
 */
 

void APP_Battery_Reduce(void)  // 不理解这个函数
{

	g_SystemParam_Config.current_boardtime++;
	powertime=500000*60/g_SystemParam_Config.send_invteral;
	
	if(g_SystemParam_Config.current_boardtime>(powertime-1))  
	{
		g_SystemParam_Config.current_boardtime=powertime;
	}
	
	g_SystemParam_Config.battery = g_SystemParam_Config.send_invteral * (powertime-g_SystemParam_Config.current_boardtime) / 300000;
	
	if((g_SystemParam_Config.current_boardtime % (powertime / 250)) == 1)
	{
		SystemParam_Save();
	}	 
}


// -----------TestFunc----------

void APP_Battery_TestFunc(void)
{
	DEBUG("Battery : %d\r\n" , g_SystemParam_Config.battery);
}


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

