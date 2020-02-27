/**
 **************************************************************************************************
 * @file        system_param.c
 * @author
 * @version
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *              
 **************************************************************************************************
 */
#include "self_def.h"
#include "system_param.h"
#include "bsp_flash.h"
/**
 * @addtogroup    XXX 
 * @{  
 */
#include "clog.h"
#include "crc.h"
/**
 * @addtogroup    system_param_Modules 
 * @{  
 */

/**
 * @defgroup      system_param_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      system_param_Macros_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      system_param_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      system_param_Private_Types
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      system_param_Private_Variables 
 * @brief         
 * @{  
 */
const SystemParam_Config_t SystemParam_Config_Default = 
{

};
/**
 * @}
 */

/**
 * @defgroup      system_param_Public_Variables 
 * @brief         
 * @{  
 */
SystemParam_Config_t g_SystemParam_Config;
/**
 * @}
 */

/**
 * @defgroup      system_param_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      system_param_Functions 
 * @brief         
 * @{  
 */
 
 
 
void SystemParam_Init(void)
{		
	BSP_Flash_ReadBytes(SYS_PARAM_SAVE_FLASH_FIRSTHEAD, (uint8_t *)&g_SystemParam_Config , sizeof(g_SystemParam_Config) );
	if(CRC16_Modbus((uint8_t *)&g_SystemParam_Config,sizeof(g_SystemParam_Config)) == 0) // Same Save
	{
		DEBUG("SYS Param Read OK\r\n");
	}
	else
	{
		g_SystemParam_Config.Test = 500.0f;

		g_SystemParam_Config.crc = CRC16_Modbus((uint8_t*)&g_SystemParam_Config, sizeof(g_SystemParam_Config) - sizeof(g_SystemParam_Config.crc));
		BSP_Flash_WriteBytes(SYS_PARAM_SAVE_FLASH_FIRSTHEAD,(uint8_t *)&g_SystemParam_Config,sizeof(g_SystemParam_Config));
	}

}

int16_t SystemParam_Read(uint8_t handle)
{
    switch (handle)
    {
        case SYSTEMPARAM_CONFIG:
        {
            if (1)
            {
                
            }
            break;
        }
    }
    return 0;
}

void SystemParam_Save(void)
{
	g_SystemParam_Config.crc = CRC16_Modbus((uint8_t*)&g_SystemParam_Config, sizeof(g_SystemParam_Config) - sizeof(g_SystemParam_Config.crc));
	BSP_Flash_WriteBytes(SYS_PARAM_SAVE_FLASH_FIRSTHEAD,(uint8_t *)&g_SystemParam_Config,sizeof(g_SystemParam_Config));
	DEBUG("Sys save\r\n");
}

void SystemParam_Reset(uint8_t handle)
{
    switch (handle)
    {
        case SYSTEMPARAM_CONFIG:
        {
            g_SystemParam_Config = SystemParam_Config_Default;
            SystemParam_Save();
            break;
        }
    }
}

void SystemParam_Apply(uint8_t handle)
{
    switch (handle)
    {
        case SYSTEMPARAM_CONFIG:
        {
 
            break;
        }
    }
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

