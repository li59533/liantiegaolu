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
#define APP_BATTERY_ALL_TIMES   80000
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


void APP_Battery_Reduce(void)  
{
	
	g_SystemParam_Config.current_boardtime++;
	DEBUG("APP_Battery_Reduce Count :%d\r\n Battery:%d\r\n" , g_SystemParam_Config.current_boardtime,\
															g_SystemParam_Config.battery);

	
	if(g_SystemParam_Config.current_boardtime < APP_BATTERY_ALL_TIMES)
	{
		if((g_SystemParam_Config.current_boardtime % (APP_BATTERY_ALL_TIMES / 100)) == 0)
		{
			if(g_SystemParam_Config.battery >= 1)
			{
				DEBUG("APP_Battery_Reduce Save\r\n");
				g_SystemParam_Config.battery -- ;
				SystemParam_Save();
			}
		}			
	}
	else
	{
		g_SystemParam_Config.current_boardtime = APP_BATTERY_ALL_TIMES;
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

